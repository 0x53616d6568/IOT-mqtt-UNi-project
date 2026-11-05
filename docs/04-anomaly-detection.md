# Anomaly Detection

## Target behavior

Identify **sustained deviations from a measured normal vibration baseline** on this prototype. An anomaly is an alert-worthy change in the measured pattern; it is not proof of a fault or diagnosis of a specific component.

## Recommended two-stage approach

### Stage A — interpretable baseline (required)

1. Select and document the acceleration axis/axes and sensor range.
2. Acquire fixed-duration windows with timestamps. Measure actual sample rate and jitter rather than assuming the requested rate was achieved.
3. For each axis in each valid window, remove the window mean.
4. Calculate RMS and maximum absolute amplitude after mean removal, matching the project brief. Optionally add peak-to-peak or cross-axis magnitude only if the definition is explicit.
5. Record normal reference statistics from multiple stable sessions.
6. Compare new features against a baseline/threshold, then require persistence for a configurable duration or number of windows before confirming an anomaly.
7. Require stable normal readings for a configurable clear period before returning to normal.

Start with a transparent baseline; it is easier to validate and provides a reference for judging whether XGBoost adds value.

## Data collection protocol

For every session, save raw data or reproducible window features with metadata:

- session ID, date/time, operator, device/firmware version;
- sensor board and mounting position/orientation;
- machine/fixture and operating condition (or a safe simulated condition);
- measured sample interval, window size/stride, axis/range, units;
- label and exact event/procedure: normal, ordinary handling, or controlled perturbation;
- sensor validity, HR202 state, and any interruptions.

Capture multiple independent runs, not just many overlapping windows from one run. Split training and testing by entire session/run to reduce leakage from adjacent windows. Preserve a final held-out set for evaluation.

## Optional XGBoost edge classifier

- Use a small, fixed feature vector (for example, per-axis RMS and max-absolute amplitude) with a documented order and units.
- Train only on project-collected labeled examples for the intended test setup. Public bearing data can help explore signal processing, but its sensors, machine, mounting, and acquisition conditions differ and may not transfer to the MPU6050 demonstrator.
- Keep HR202 threshold logic separate initially; it is an independent binary alert condition, not a vibration feature unless there is a justified, tested reason to combine it.
- Use run/session-separated train, validation, and test sets. Report class counts and confusion matrix, precision/recall, false alerts per test duration, and missed anomalies. Accuracy alone is not enough when classes are imbalanced.
- Compare against the threshold baseline. If XGBoost does not improve meaningful held-out metrics, retain the simpler baseline.
- Constrain tree count and depth for generated code size and prediction time. Record model and feature-preprocessing versions.

## Export and firmware equivalence

`m2cgen` documents C code generation and support for `XGBClassifier`/`XGBRegressor`, but library compatibility and generated results must be checked with the exact installed versions. The exported C is model inference only; windowing, feature calculation, input validation, event persistence, and output policy remain application code.

1. Generate C from the final trained estimator.
2. Store training code, dependency versions, feature schema, class order, and model checksum.
3. Run a frozen suite of feature vectors through both Python and generated C.
4. Compare class scores/probabilities with a documented numeric tolerance and check behavior near decision boundaries.
5. Measure flash/code size, inference time, and RAM on the actual ESP32.
6. If conversion or resource use is problematic, reduce trees/depth or choose an easier model; do not silently replace a model with different behavior.

## Data quality / evaluation cautions

- Do not split adjacent windows from one recording randomly between train and test.
- Do not label a vibration event as a specific mechanical fault unless the fault was independently verified.
- Avoid test fixtures with unsafe rotating parts or intentional damaging impacts.
- Treat invalid sensor reads, missing reference, and out-of-range features as `unknown/invalid`, not as normal.
- Document limitations of the sensor's bandwidth, mounting, and chosen sampling rate.