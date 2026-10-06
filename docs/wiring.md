# Wiring authority

Use the exact [README pin table](../README.md) and editable [circuit SVG](circuit-diagram.svg).
PIR OUT is digital GPIO27, not an analog input. The active-high, 3.3V-compatible
relay module IN is GPIO26. INA219 SDA/SCL are GPIO21/GPIO22, VCC is 3.3V and
I2C pull-ups must terminate at 3.3V. Ground INA219 A0/A1 for address 0x40.

HC-SR501 VCC and relay VCC use fused external 5V; both grounds join ESP32 GND.
The ESP32 is powered from USB; do not join external +5V to its USB 5V rail.
Connect fused +5V → relay COM → NO → INA219 VIN+ → VIN− → 5V lamp+.
Lamp− returns to external GND. NC is unconnected. INA219 has a 0.1Ω shunt.
The relay module contains its coil driver and flyback diode; never drive a bare coil.

Before powering, verify labels, polarity, ground continuity, no 5V on GPIO/I2C,
PIR OUT maximum 3.3V, and relay input compatible with 3.3V active HIGH.
Use a ≤100mA 5V LED lamp and 0.5A fuse. No mains or hazardous loads.
