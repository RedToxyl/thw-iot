from iotknit import *
import time

init("192.168.14.1")

# prefix("sensor")
temp_sensor = publisher("sensor")

current_temp = 20.0
step = 1.0
direction = 1

print("[TempSim] Started...")

while True:
    current_temp += (step * direction)
    
    if current_temp >= 28.0:
        direction = -1
    elif current_temp <= 20.0:
        direction = 1

    temp_sensor.publish("living_room_temp", str(current_temp))
    print(f"[TempSim] Current room temperature is {current_temp}°C")
    
    process()
    time.sleep(2)