# Module 4

## Task 1
Understandably, the arduino does not enjoy us trying access this undefined pointer. Broken code can be found [here](/src/module04/task01-nullptr/).

## Task 2
Similarly, a division by 0 [isn't great](/Module04/pictures/crash1.png) either.

## Task 3

## Task 4
The delay of 1000ms was significantly too long for an ISR. Furthermore, using delay inside an interrupt will lead to unexpected behaviour in general, as most timers don't work during interrupts. [Removing the delay](/src/module04/task04-interrupt/) fixed things.

## Task 5
Our code can be found [here](/src/module04/task05-ota/). Only changes are credentials and the hostname-setting.

## Task 6
Mainly a combination of 5 and 2. Code can be found [here](/src/module04/task06-alarm/).

## Task 7
1. D1 Mini: Two interfaces, Serial (GPIO1 & GPIO3 or GPIO15 & GPIO13) and Serial1 (GPIO2, transmit only). ESP32: Three full interfaces, Serial, Serial1, Serial2. Pins can be remapped freely.
2. Software serial refers to the dynamic emulation of serial connections via any data pins. Hardware Serial refers to the dedicated serial setup. Hardware serial is more stable but less flexible, whereas software serial can be adapted more easily at the cost of dependability.