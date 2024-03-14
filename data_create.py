import os
import numpy as np
import signal

def handle_interrupt(signal_number, stack_frame):
    print("\nInterrupt received! Stopping execution...")
    exit(0)  # Exit gracefully

signal.signal(signal.SIGINT, handle_interrupt)  # Register the handler for Ctrl+C

for i in range(100):
    if np.random.uniform() < 0.1:
        os.system("python null_load.py")
    else:
        os.system("python load.py")
