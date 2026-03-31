# Reflection 1 in Module 4

don't point at forbidden address space, nothing new here

PWM was already discussed before I thought

I hadn't worked with ISRs since we did RISC-V, so it was neat to touch them again.

For Task 5, we originally compiled for the wrong chip (W1_Mini_Prop instead of W1_Mini), which causes a headache. Serial upload was also unreliable for some reason.

# Reflection 2 in Module 4

Spending the extra time last session to setup OTA for the ESPs was definitively worth it, it saves so much time.

# Reflections 3 in Module 4

We wrote a small library to take care of a lot of boilerplate (WiFi, OTA, MQTT setup, etc.) for us. Today we will try and see how well it works and where we can improve on it.

MQTT is easy to set up, which is why it simplifies IoT integration. We can easily connect many sensors and actuators in a network via MQTT without having to do a lot of work ourselves.
One example for MQTT would be digital room signage. One could install displays on every room containing the current class, timetable, etc. These could easily be connected by and updated through MQTT.

I don't like NodeRed. It does not feel simpler or faster than just implementing these things in code on some machine.

Our library is really useful when it works, but had some quite annoying bugs left, which slowed us down a lot. Now that these are fixed, we can hopefully get stuff done quicker.