# Module 5

## IoTempower as Device Management Framework

Successfully initialized the IoTempower environment and implemented following [nodes](../iot-systems/module05/): rfid, display, distance, led, buzzer. 

Reused the previous Node-Red flow, only slightly adding logic nodes (mostly with js which does similar processing as befor in .cpp files) after reciving/sending mqtt requests/responses, to match the automated IoTempower structure.

Final results are exactly the same as described in [Module4.2](../Module04/README.md).

## Hostel HostmeWell Automation

We implemented a customized hybrid (RFID / Morse based) authentication system.

*Background Story*: Andres and Liis recently inherited HostmeWell, a traditional hostel in Tartu. To improve guest experience and reduce manual labor for staff, we are prototyping an IoTempower-based smart hostel system to solve following chalenges:

### Challenge 1: The Late-Night Check-in
Characters: Mike (a tired backpacker arriving at 2 AM) and Liis (the owner, who wants to sleep).

Problem: Mike needs to safely enter the hostel and find his bed in the middle of the night without waking up the staff or other guests.

Solution: An automated check-in system. Mike uses a pre-issued RFID card to scan at the door, which automatically unlocks the entrance and logs his arrival.

Hardware/Software: RFID sensor, Relay (for the door lock), IoTempower, Node-RED.

Potential Difficulties: Ensuring the system reliably processes the scan quickly so Mike isn't left waiting outside, and making sure the door locks securely behind him.

### Challenge 2: Personalized Safe Access
Characters: Andres (Hostel owner) and Mike (the guest).

Problem: Guests need a secure place to store valuables, but managing physical keys is tedious, and Andres wants a secondary authentication system that is personalized for each user.

Solution: A dual-authentication setup. At reception, Mike scans his RFID card to link his profile and inputs a personalized password. He can then use this custom password to access his personal safe in his room.

Hardware/Software: RFID sensor, Button shield, Database, IoTempower, Node-RED.

Potential Difficulties: Reliably querying the database to match the correct password to the specific RFID user, having passwords managed in a secure way.

### Challenge 3: The Ultra-Efficient Morse Code Lock (Reducing Expenses)
Characters: Andres and Liis (budget-conscious owners) and Mike (the guest).

Problem: Andres and Liis realize that installing full keypad panels and RFID scanners on every single room door and safe is far too expensive for their current budget.

Solution: To achieve ultimate hardware efficiency, they replace the expensive keypads with a single, simple button. The custom-programmed node parses the user's button presses, decodes the timing into Morse code, converts it to a string, and sends it via MQTT to authenticate the user.

Hardware/Software: Single physical button (running custom C++ code), Display, MQTT broker, Node-RED, Database.

Potential Difficulties: Guests might struggle to input Morse code accurately.

This is the challenge we actually implemented. The result can be seen [here](/Module05/pictures/morse.gif)

## Prototyped Implementation

*RFID Scanner*: Wemos D1 Mini configured via IoTempower. It scans the RFID tag and sends an MQTT message, which is then handled by Node-RED.

*Reception Node*: An ESP32 programmed in C++ ([cpp](node.cpp)) using a custom [ESP library](../lib/ecl/). Upon receiving the MQTT message from the scanner, it updates its display and begins listening for Morse code input via a button. The input is shown on the screen in both Morse and string formats. Once the password entry is complete (detected by a long pause), the node transmits the parsed string via MQTT.

*Password node*: This node was not fully implemented since we only had a 2-person team. However, it could easily be built by reusing the Morse code parsing logic from the Reception Node to authenticate users via their personalized Morse passwords at their individual room doors or safes.

*Server side*: An MQTT broker and Node-RED are used to orchestrate communication between the nodes. Node-RED authenticates users by verifying the incoming RFID data and parsed Morse code strings against the user profiles saved and updated in the database.