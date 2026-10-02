# Room Climate Hub Live Dashboard

Build a smart home prototype that uses PIR sensor, relay module, current sensor to display live measurements and status. Include setup instructions, a circuit diagram, tested firmware, and sample output.

## Project details

| Field | Value |
| --- | --- |
| Roadmap ID | 1 |
| Category | Smart Home |
| Platform | ESP32 |
| Difficulty | Beginner |
| Estimated build time | 8 hours |
| Connectivity | Wi-Fi |
| Core components | PIR sensor, relay module, current sensor |
| Control mode | telemetry |

## Repository layout

- `firmware/room-climate-hub-live-dashboard/room-climate-hub-live-dashboard.ino`: runnable firmware or application
- `docs/wiring.md`: suggested low-voltage wiring plan
- `docs/architecture.md`: system data flow
- `docs/test-plan.md`: repeatable verification steps
- `sample-data/example.json`: example telemetry record
- `tools/validate.py`: dependency-free repository validation

## Quick start

1. Open `firmware/room-climate-hub-live-dashboard/room-climate-hub-live-dashboard.ino` in Arduino IDE or Arduino CLI.
2. Select the board matching **ESP32**.
3. Compile and upload, then open the serial monitor at 115200 baud.

## Expected behavior

Live Dashboard demonstration with repeatable test steps. The default implementation supports simulated or generic analog inputs so the control path can be exercised before hardware-specific drivers are added.

## Hardware adaptation

The included code is a safe reference implementation. Update pin assignments and sensor conversions from the exact component datasheets, then repeat the test plan before connecting actuators.

## License

MIT
