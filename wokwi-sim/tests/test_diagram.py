import json
import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]


class WokwiDiagramTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.diagram = json.loads((ROOT / "diagram.json").read_text(encoding="utf-8"))
        cls.parts = {part["id"]: part for part in cls.diagram["parts"]}
        cls.connections = cls.diagram["connections"]

    def test_required_components_exist(self):
        expected = {
            "esp": "board-esp32-devkit-c-v4",
            "mpu": "wokwi-mpu6050",
            "lcd": "wokwi-lcd2004",
            "hr202": "wokwi-pushbutton",
            "green-led": "wokwi-led",
            "red-led": "wokwi-led",
            "green-r": "wokwi-resistor",
            "red-r": "wokwi-resistor",
        }
        for part_id, part_type in expected.items():
            with self.subTest(part=part_id):
                self.assertIn(part_id, self.parts)
                self.assertEqual(self.parts[part_id]["type"], part_type)

    def test_every_connection_references_a_known_part_and_pin(self):
        pin_map = {
            "esp": {"21", "22", "25", "26", "27", "3V3", "GND.1"},
            "mpu": {"SDA", "SCL", "VCC", "GND"},
            "lcd": {"SDA", "SCL", "VCC", "GND"},
            "hr202": {"1.l", "2.l"},
            "green-led": {"A", "C"},
            "red-led": {"A", "C"},
            "green-r": {"1", "2"},
            "red-r": {"1", "2"},
        }
        for connection in self.connections:
            self.assertEqual(len(connection), 4)
            for endpoint in connection[:2]:
                part_id, pin = endpoint.split(":", 1)
                self.assertIn(part_id, self.parts, f"Unknown part in {endpoint}")
                self.assertIn(pin, pin_map[part_id], f"Unknown pin in {endpoint}")

    def test_expected_gpio_and_shared_i2c_wiring(self):
        pairs = {(wire[0], wire[1]) for wire in self.connections}
        expected = (
            ("esp:21", "mpu:SDA"), ("esp:22", "mpu:SCL"),
            ("esp:21", "lcd:SDA"), ("esp:22", "lcd:SCL"),
            ("esp:27", "hr202:2.l"), ("esp:25", "green-r:1"),
            ("esp:26", "red-r:1"),
        )
        for pair in expected:
            with self.subTest(pair=pair):
                self.assertIn(pair, pairs)

    def test_lcd_and_button_simulation_attributes(self):
        self.assertEqual(self.parts["lcd"]["attrs"]["pins"], "i2c")
        self.assertEqual(self.parts["hr202"]["attrs"]["bounce"], "0")
        self.assertEqual(self.parts["green-r"]["attrs"]["value"], "330")
        self.assertEqual(self.parts["red-r"]["attrs"]["value"], "330")

    def test_firmware_configuration_matches_diagram(self):
        config = (ROOT / "include/app_config.h").read_text(encoding="utf-8")
        expected = (
            "I2C_SDA_PIN = 21", "I2C_SCL_PIN = 22",
            "HR202_DIGITAL_PIN = 27", "GREEN_LED_PIN = 25",
            "RED_LED_PIN = 26",
            'MQTT_TOPIC_PREFIX[] = "iot/groupe6/equipement"',
        )
        for value in expected:
            with self.subTest(value=value):
                self.assertIn(value, config)

    def test_acquisition_scenario_targets_existing_parts(self):
        scenario = (ROOT / "scenarios/acquisition-smoke-test.yaml").read_text(encoding="utf-8")
        self.assertIn("part-id: mpu", scenario)
        self.assertIn("part-id: hr202", scenario)
        self.assertIn("name: accelX", scenario)
        self.assertIn("name: pressed", scenario)

    def test_modular_firmware_files_exist(self):
        paths = (
            "src/main.cpp", "src/sensor_service.cpp",
            "src/display_service.cpp", "src/mqtt_service.cpp",
            "include/sensor_service.h", "include/display_service.h",
            "include/mqtt_service.h", "platformio.ini", "wokwi.toml",
            "scenarios/acquisition-smoke-test.yaml", ".gitignore",
        )
        for path in paths:
            with self.subTest(path=path):
                self.assertTrue((ROOT / path).is_file())


if __name__ == "__main__":
    unittest.main()
