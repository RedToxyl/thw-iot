# Module 2

We are going to show here notes for Task 1, 2, 3, 4, and 5 to prove

## Task 1

An electric circuit consists of a power source (e.g. a battery), a wire connection between its poles and resistive elements inbetween.

A breadboard allows for easy reconfiguration of circuits by having connected power lanes that can be plugged into without requiring soldering.

Blue/Black are ground, red is "hot", white and other colours are signals

The longer pin is the anode, the shorter pin the cathode. Thus we connect the longer pin to the + of the power source. To prevent it from frying we should add a resistor.

An LED is a light emitting diodode; current can only flow from the anode to the cathode, emitting light in the process. They are usually very energy efficient and, if used correctly, can last a long time. 

Important: check your voltages and include resistors to protect sensitive and expensive elements.


## Task 2

[Hardware](/Hardware/README.md)

## Task 3

A D1 Mini has a 3.3V and 5V power pins, a ground pin, 11 GPIOs and special RST and ADC pins.
GPIO stands for general purpose input output, these are the most commonly used microchip components. They can be set to emit voltage to control actuators or to instead read sensor input and data streams. For the first task, we didn't have any issues getting the LED to [work](/Module02/pictures/task03.png). We experimented a bit with the last task and added an additional [LED](/Module02/pictures/task03-2.png)

## Task 4

[Here](/Module02/pictures/circuit.png) is a simplified circuit for the button-controlled LED. The power supply subsitutes for the D1 Mini.

## Task 5

We used VSCode and PlatformIO to write and build our sourcecode instead of the ArduinoIDE, as thats the workflow we're more familier with. The basic circuit setup is simple, two LEDs connected to two separate GPIOs, both connected to Ground through a 270 Ohm resistor.

Our [code](/src/module02/task05/main.cpp) is based on [this](/Module02/pictures/pinout.png) pinout. The results can be seen [here](/Module02/pictures/blink.gif)

## Task 6
Without a Pull-Up resistor, opening a switch (not pressing our button) would leave the circuit floating, i.e. with an undefined charge, purely influenced by random, external factors. In order to prevent this, a PU resistor can pull-up (or down) the remaining signal, leaving us with a clearly defined voltage in all cases. Thi behaviour can be seen [here](/Module02/pictures/pullup.gif).

Our [code](/src/module02/task06/main.cpp) was written with [this](/Module02/pictures/pullup.png) setup in mind. The button is read at D0 and the LED controlled via D6.

Using INPUT_PULLUP simplifies the circuit design and means we don't have to worry about choosing the right resistor ourselves. It removes overhead. However, we give up control over the exact resistance and trust in the chip. In some usecases, we might want to wire this ourselves to ensure correct behaviour. In class however, INPUT_PULLUP should mostly be sufficient.

## Task 7

[This](/Module02/pictures/solenoid.gif) shows our setup and working example.

Had we switched NO and COM, we would have connected our solenoid directly to the 12V power source, skipping the relay. It would have been constantly on and overheated quickly.

## Task 8
[GIF](/Module02/pictures/pwm.gif)

## Task 9

[Table of Connection Methods](/Hardware/README.md)

As the shield already includes the correct wiring, using the temperature sensor is uncomplicated. We merely had to [slot it onto the W1](/Module02/pictures/dallas.png) and read out the temperature.

Connecting the LED was similarly trivial, as it can just be stacked on top of the W1. [GIF](/Module02/pictures/rainbow.gif)

The touch sensor was troublesome. Baseline was constantly registred at 0. Turns out the newest version of the Adafruit library has some issues, and downgrading to version 1.3.1 fixed things. The [wiring](/Module02/pictures/touch.png) for the I2C was unproblematic, and in the end we managed to detect touch on the electrodes. With more time we would have added an LED controlled by the W1 to indicate touch.
## Reflection 2
[Reflection 2](/Reflections/ref02.md)


