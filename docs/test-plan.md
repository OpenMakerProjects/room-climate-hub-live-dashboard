# Test plan

## Executed cloud checks

Run `python tools/validate.py` and `python tools/validate_completion.py --allow-pending-image`.
The full validator is required before completion. See validation-results.md for actual results.

## Pending board and physical checks

1. Build `pio run -e esp32dev` with pinned platform and capture output.
2. With power OFF verify net connections, 3.3V logic and fused 5V load path.
3. Flash, power up, verify relay OFF before INA219 becomes ready.
4. Connect AP and confirm GET schema and digital motion behavior after PIR warm-up.
5. POST invalid `on=2`: expect 400. Disconnect INA219 SDA: expect null current,
   sensor_ok false, relay OFF at next one-second sample and ON command 409.
6. Restore sensor: relay stays OFF until explicit ON. Compare measured lamp current
   to an external ammeter and document calibration uncertainty.
7. Verify common GND, relay active-high behavior and no 5V on GPIO or I2C.

These physical steps were not performed by the cloud agent. No mains/load-risk tests.
