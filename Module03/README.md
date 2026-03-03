# Module 3

## Task 1
OpenWRT is a linux based operating systems for routers. It is free and open source and allows for very extensive configuration.
It's optimized to work on low spec systems and to not require many resources.

The IP of the router on the local network is 192.168.1.1. It can be found by just connecting to ethernet and checking the UI of your operating system, or by running nmap -p 80 192.168.1.0/24.

We logged in and changed the IP address to 192.168.14.1.

## Phase 3

Mosquitto is running on TCP. Other services are dropbear and dnsmasq

## Phase 4
We managed to set up [communications](Module03/pictures/mqtt.png) quickly.

Wildcards can be used to subscribe to arbitrary topics, such as subscribing to all subtopics of team15 would mean subscribing to team15/#

## Minitutorial

Set up and connect to your mango router. By default, root does not have a password.
Start the WiFi network. If necessary, add a password and ajust the channel.
Connect your Mango to the internet to install Mosquitto. If no internet connection is available and you have already installed Mosquitto on your device, you may be able to transfer it via the ethernet or WiFi connection.
Start the mosquitto service and connect your clients to the WiFi and to the Mango as MQTT host. Once you agree on topics, you can subscribe and publish under those for communication.