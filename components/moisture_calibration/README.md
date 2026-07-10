# Moisture sensor calibration tool

This sub-project is designed to calibrate the **Capacitive Soil Moisture Sensor v1.2** using the **ESP-IDF v5.x** framework on an ESP32 DevKitC.

## Hardware connection

Connect the capacitive sensor to the ESP32 pins as described below:

| Sensor Pin | ESP32 Pin | Description |
| :--- | :--- | :--- |
| **3.3V / VCC** | 3V3 | Power supply |
| **GND** | GND | Ground reference |
| **Aout** | GPIO34 | Analog output connected to ADC1_CH4 |

> ⚠️ **Note:** GPIO34 is an input-only pin on the ESP32, which makes it perfect for reading analog sensors without risk of accidental output configuration.

## Build and run
in PlatformIO CLI
```sh
pio run -e calibration -t upload

```

## Calibration procedure

1. Flash the firmware to your ESP32.
2. Open your serial monitor (`idf.py monitor`).
3. **Dry Measurement:** Leave the sensor exposed to the open air. Note the stable raw value displayed. This represents your **100% Dry** value ($V_{\text{dry}}$).
4. **Wet Measurement:** Submerge the sensor in a glass of water up to the maximum immersion line (do not submerge the electronics at the top). Note the stable raw value. This represents your **100% Wet** value ($V_{\text{wet}}$).

## Interpretation

Capacitive sensors output a **lower** voltage when the medium is wet and a **higher** voltage when dry. 

When implementing the final application logic, you will map the values using these calibrated bounds:
* `Raw >= Dry_Value` $\rightarrow$ 0% Humidity
* `Raw <= Wet_Value` $\rightarrow$ 100% Humidity