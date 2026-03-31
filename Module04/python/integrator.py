from iotknit import *

ac_status = False
TARGET_TEMP = 24.0 

init("192.168.14.1")

ac_unit = publisher("ac")

def temperatureCallback(msg):
    global ac_status
    
    print("[Integrator] received: [sensor]", msg)
    
    try:
        current_temp = float(msg)
    except ValueError:
        print("Error: Received invalid temperature reading")
        return

    if current_temp > TARGET_TEMP and not ac_status:
        ac_status = True
        ac_unit.publish("living_room", "on")
        print(f"[Integrator] Temp is {current_temp}. Sending -> [AC ON]")  

    elif current_temp <= TARGET_TEMP and ac_status:
        ac_status = False
        ac_unit.publish("living_room", "off")
        print("sending: [ac_unit]", "off")
        print(f"[Integrator] Temp is {current_temp}. Sending -> [AC OFF]")

prefix("sensor")
temp_sensor = subscriber("living_room_temp")

temp_sensor.subscribe_change(callback=temperatureCallback)
run()