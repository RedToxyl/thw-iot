# Hardware

This is the hardware, I/we took out.

## 2026-02-18
- 1x large breadboard
- 2 D1 Minis
- 2 USB-Cables
- 3 Buttons
- 9 LEDs
- Dht22
- DS1820B Temperature Sensor
- Jumper Cables (9FF, 22MF many MM)
- Multimeter
- Mpr121 Touch Sensor
- Power Supply
- Relay
- Resistors (1x 10k, 1x 1k, 4x 270)
- RFID Reader (+ Tags)
- RGB LED
- Solenoid

[A picture showing all parts taken out this day.](Hardware/pictures/img01.png "A picture showing all parts taken out this day.]")

## Connectivity Classification Table

| Component                          | Communication Method | Example Wiring |
|------------------------------------|----------------------|---------|
| LED | Direct via GPIO | supply power via GPIO (and connect to ground, + resistor), use PWM to change brightness |
| MPR121 Touch Sensor | I2C | SCL (clock) to D1, SDA (data) to D2 |
| RFID Reader | SPI | M-CLK (clock) to D5, MISO (uplink) to D6, MOSI (downlink) to D7 |
| BNO055 (IMU) | UART | TX to TXD0 (GPIO1), RX to RXD0 (GPIO3)|
| DS1820B | OneWire | data to any gpio, but with a pullup resistor to 3.3V |
