import os
import subprocess
import time
import sys

script_dir = os.path.abspath(os.path.dirname(__file__))
print("[Runner] Starting IoT Network...")
try:
    print(os.path.abspath(__file__))
    python_cmd = sys.executable 
    ac_process = subprocess.Popen([python_cmd, "ac_sim.py"], cwd=script_dir)
    time.sleep(1)
    
    integrator_process = subprocess.Popen([python_cmd, "integrator.py"], cwd=script_dir)
    time.sleep(1)
    
    temp_process = subprocess.Popen([python_cmd, "temp_sim.py"], cwd=script_dir)

    print("[Runner] All systems running!")
    
    while True:
        time.sleep(1)
        
except KeyboardInterrupt:
    print("\n[Runner] Shutting down IoT Network...")
    temp_process.terminate()
    integrator_process.terminate()
    ac_process.terminate()
    print("[Runner] All processes stopped successfully.")