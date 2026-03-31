# Module 4

## Task 1
Understandably, the arduino does not enjoy us trying access this undefined pointer. Broken code can be found [here](/src/module04/task01-nullptr/main.cpp).

## Task 2
Similarly, a division by 0 [isn't great](/Module04/pictures/crash1.png) either.

## Task 3

## Task 4
The delay of 1000ms was significantly too long for an ISR. Furthermore, using delay inside an interrupt will lead to unexpected behaviour in general, as most timers don't work during interrupts. [Removing the delay](/src/module04/task04-interrupt/main.cpp) fixed things.

## Task 5
Our code can be found [here](/src/module04/task05-ota/main.cpp). Only changes are credentials and the hostname-setting.

## Task 6
Mainly a combination of 5 and 2. Code can be found [here](/src/module04/task06-alarm/main.cpp).

## Task 7
1. D1 Mini: Two interfaces, Serial (GPIO1 & GPIO3 or GPIO15 & GPIO13) and Serial1 (GPIO2, transmit only). ESP32: Three full interfaces, Serial, Serial1, Serial2. Pins can be remapped freely.
2. Software serial refers to the dynamic emulation of serial connections via any data pins. Hardware Serial refers to the dedicated serial setup. Hardware serial is more stable but less flexible, whereas software serial can be adapted more easily at the cost of dependability.

[Node A](/src/module04/task07-node_a/main.cpp) connects to WiFi establishes an MQTT connection and listens. When a message arrives, it transmits it via the second serial to [Node B](/src/module04/task07-node_b/main.cpp). This node just listens to serial input and posts arriving messages.
To enable OTA ability, B also connects to the WiFi, but communication between the two nodes is strictly over serial.

The wiring is simple, both nodes are directly connected from GPIO16 to GPIO16 and GPIO17 to GPIO17.

## Task 8
We just slightly modifed the code from the previous task and kept the ESP to ESP connection for simplicity. It worked [well](/Module04/pictures/oled.gif).

## Task 9

We did most things in code and had little need for NodeRed. Connecting the LED-strip also didn't need any special consideration. We were able to reuse some old button code from a previous task, and [things worked well](/Module04/pictures/led.gif).

# Module 4 Part 2

## Task 1
- [Node Red Integrator](/Module04/pictures/flow1.png)
- [Node Red Simulator](/Module04/pictures/flow2.png)
- [Node Red Dashboard](/Module04/pictures/dashboard.png)