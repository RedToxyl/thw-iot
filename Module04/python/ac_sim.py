from iotknit import *
import time

init("192.168.14.1")

hardware_ac_state = "off"

def acCommandCallback(msg):
    global hardware_ac_state
    if msg in ["on", "off"]:
        hardware_ac_state = msg

prefix("ac")
ac_unit = subscriber("living_room")
ac_unit.subscribe_change(callback=acCommandCallback)

print("[AcSim] Started...")

while True:
    print(f"[AcSim] Is currently: {hardware_ac_state.upper()}")
    process()
    time.sleep(1)