# DataRange Library for Arduino

The DataRange library dynamically tracks the minimum and maximum values in a data stream and provides normalized values between 0.0 and 1.0. Perfect for sensor calibration, data normalization, and dynamic range detection.

## Features

- **Dynamic Range Tracking**: Automatically tracks minimum and maximum values
- **Normalization**: Convert raw values to normalized 0.0-1.0 range
- **Simple API**: Easy-to-use interface following Arduino style guidelines
- **Lightweight**: Minimal memory footprint
- **Universal**: Works with all Arduino-compatible boards

## Installation

### Using Arduino Library Manager

1. Open the Arduino IDE
2. Navigate to **Sketch** > **Include Library** > **Manage Libraries**
3. Search for "Bounds"
4. Click **Install**

### Manual Installation

1. Download the library as a ZIP file
2. In Arduino IDE, go to **Sketch** > **Include Library** > **Add .ZIP Library**
3. Select the downloaded ZIP file

## Usage

### Basic Example

```cpp
#include <DataRange.h>

DataRange sensorBounds;

// Optionally set known initial bounds
// sensorBounds.setInitialBounds(0, 1023);

// If your signal can dip below zero and you want the lower bound to track it,
// enable floating lower bounds (default keeps lower bound fixed at 0):
// sensorBounds.setFloatingLower(true);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);

  // Update bounds with new value
  sensorBounds.update(sensorValue);

  // Get normalized value (0.0 to 1.0)
  float normalized = sensorBounds.normalized();

  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print(" Normalized: ");
  Serial.println(normalized);

  delay(100);
}
```

## API Reference

### Constructor

#### `DataRange()`

Creates a new DataRange object with uninitialized bounds.

```cpp
DataRange myBounds;
```

#### `DataRange(float upper)`

Creates a DataRange with a fixed lower bound of 0 and a starting upper bound. The lower bound does not float in this form.

```cpp
DataRange zeroBased(1023.0f);
```

### DataEnvelope

`DataEnvelope` tracks a decaying envelope over streaming data. Upper and lower bounds expand to new extremes and then decay toward recent values using a configurable per-update decay amount, yielding a local-in-time range. Decay is **exponential by default** (shrinks the distance to the latest value by the given fraction each update). You can opt into linear decay via `setLinearDecay()` to subtract/add a fixed amount per update instead. All constructors require a decay amount. Constructors mirror `DataRange`: with a fixed upper bound (lower pinned at 0), or with explicit lower/upper bounds (lower/upper are swapped if provided inverted).

#### `DataEnvelope()`

Creates an envelope with zero decay (pure tracking, no decay).

#### `DataEnvelope(float decayAmount)`

Creates an envelope with a per-update decay amount. Higher values decay faster toward recent samples.

```cpp
#include <DataEnvelope.h>

DataEnvelope env(0.25f); // exponential: shrink distance to the latest sample by 25% per update
env.update(sample);
float lo = env.lower();
float hi = env.upper();
float n  = env.normalized(sample);

// Initialize with an upper bound and decay (lower defaults to 0)
DataEnvelope envUpper(10.0f, 0.1f); // start at [0,10], exponential decay 10%

// Initialize with lower/upper bounds and decay; switch to linear mode
DataEnvelope envBounds(-5.0f, 5.0f, 1.0f); // start at [-5,5], exponential decay
envBounds.setLinearDecay(); // switch to linear decay of 1 unit/update

// Optional: switch to linear decay (useful when working in raw units)
env.setLinearDecay();
env.setDecay(1.0f); // subtract/add 1 unit per update

// Switch back to exponential decay when needed
env.setExponentialDecay();

// Exponential decay works best with values between 0.0f and 1.0f; higher
// values clamp to 0.0f internally (instant convergence to the latest sample).

Additional DataEnvelope controls:
- `setDecay(float amount)`: per-update decay amount (fractional for exponential, units for linear)
- `setLinearDecay()`: switch to linear decay
- `setExponentialDecay()`: switch to exponential decay (default)
- `reset()`, `upper()`, `lower()`, `range()`, `normalized()`: same semantics as DataRange
```

#### `DataRange(float lower, float upper)`

Creates a DataRange with explicitly provided initial bounds. If `lower` is greater than `upper`, the values are swapped.

When you provide a lower bound, the lower bound will float with the data (it can move lower if new data is smaller).

```cpp
DataRange calibrated(0.0f, 1023.0f);
```

### Methods

#### `void update(float value)`

Updates the bounds with a new value. If this is the first value, both min and max are set to this value. Otherwise, min and max are updated if necessary.

**Parameters:**

- `value` - The new value to track

```cpp
sensorBounds.update(analogRead(A0));
```

#### `float min()`

Returns the minimum value seen since initialization or last reset.

**Returns:** Minimum value as a float

```cpp
float minValue = sensorBounds.min();
```

#### `float max()`

Returns the maximum value seen since initialization or last reset.

**Returns:** Maximum value as a float

```cpp
float maxValue = sensorBounds.max();
```

#### `float range()`

Returns the range between maximum and minimum values (max - min).

**Returns:** Range as a float

```cpp
float range = sensorBounds.range();
```

#### `float normalized()`

Returns the most recently updated value normalized to a 0.0-1.0 range based on the current bounds.

**Returns:** Normalized value between 0.0 and 1.0

```cpp
float normalized = sensorBounds.normalized();
```

#### `float normalized(float value)`

Returns any arbitrary value normalized to a 0.0-1.0 range based on the current bounds.

**Parameters:**

- `value` - The value to normalize

**Returns:** Normalized value between 0.0 and 1.0

```cpp
float normalized = sensorBounds.normalized(someValue);
```

#### `void setInitialBounds(float lower, float upper)`

Explicitly sets the initial bounds and marks the range as initialized. If `lower` is greater than `upper`, the values are swapped. Useful when you already know the expected sensor range and want normalization to work before the first update.

```cpp
DataRange bounds;
bounds.setInitialBounds(0.0f, 1023.0f);
```

#### `void setFloatingLower(bool enable)`

Controls whether the lower bound should stay fixed (default) or float with the data. When disabled, the initial lower bound is assumed to be 0. When enabled, the lower bound will track the lowest observed value.

```cpp
DataRange bounds;
bounds.setFloatingLower(true); // allow lower bound to move
```

#### `void reset()`

Resets the bounds to uninitialized state. The next call to `update()` will set both min and max to the provided value.

```cpp
sensorBounds.reset();
```

## Examples

The library includes two example sketches:

### BasicDataRange

Demonstrates basic usage of the library by reading an analog sensor and displaying raw values, bounds, and normalized output.

### SensorNormalization

Shows how to use the library for sensor calibration and normalization, controlling LED brightness based on normalized sensor readings.

## How It Works

The DataRange library maintains internal state tracking:

1. **Minimum Value**: The lowest value seen since initialization or reset
2. **Maximum Value**: The highest value seen since initialization or reset
3. **Last Value**: The most recently updated value
4. **Initialization State**: Whether any values have been received

When you call `normalized()`, the library:

1. Calculates the range (max - min)
2. Computes: `(value - min) / range`
3. Constrains the result to 0.0-1.0

This is particularly useful for:

- Sensor calibration
- Dynamic range adaptation
- Auto-scaling visualizations
- Input normalization for control systems

## Use Cases

### Sensor Calibration

Automatically calibrate sensors by moving them through their full range during a calibration period.

### Adaptive Thresholds

Create dynamic thresholds that adapt to changing environmental conditions.

### Data Visualization

Normalize sensor data for consistent graphing and display regardless of absolute values.

### Control Systems

Convert sensor readings to normalized control signals (0-255 for PWM, 0-180 for servos, etc.).

## Compatibility

This library is compatible with all Arduino boards and architectures. It uses only standard Arduino functions and has no architecture-specific code.

## Testing

The library includes a comprehensive test suite using PlatformIO and the Unity test framework. The test suite contains 22 tests covering:

- Initialization and updates
- Min/max/range tracking
- Normalization functions
- Edge cases (negative values, large/small values)
- State management (reset)
- Typical sensor scenarios (ADC ranges)

### Running Tests

With PlatformIO installed:

```bash
# Run tests natively (fast, no hardware required)
pio test -e native

# Run on Arduino Uno
pio test -e uno --upload-port /dev/cu.usbmodem*

# Run on ESP32
pio test -e esp32 --upload-port /dev/cu.usbserial*

# Run on all environments
pio test
```

See `test/README.md` for detailed testing documentation.

## License

This library is released under the MIT License. See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Author

Your Name - <your.email@example.com>

## Version History

- **1.0.0** (2025-12-09): Initial release
  - Basic bounds tracking
  - Normalization functions
  - Example sketches
