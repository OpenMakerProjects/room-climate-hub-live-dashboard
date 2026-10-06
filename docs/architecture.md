# Architecture

The firmware polls HC-SR501 digital motion and INA219 signed current over I2C once
per second. `Control` accepts finite current −20…500mA only with valid I2C readings.
Invalid sensor data drops the relay request and de-energizes GPIO26. Sensor recovery
never restores a previous request. A new explicit HTTP POST is required.

ESP32 Wi-Fi AP and WebServer provide `/` and `/api/status`; `/api/relay` is POST-only.
USB serial emits the same synthetic-schema JSON used by the browser. The relay is
manually controlled, not motion-controlled. The measurement path uses a 0.1Ω shunt,
4096 calibration and 0.1mA current-register scaling. The browser only visualizes;
it is not an independent safety system. Pin mappings are in README and circuit SVG.
