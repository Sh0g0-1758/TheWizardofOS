import atexit
import psutil
import os
import csv
import socket
import time

proc = psutil.Popen("./Benchmarks/CPU/sort")

def check_running_processes():
    try:
      with proc.oneshot():
        cpu_times = proc.cpu_times()
        burst_time = cpu_times.system + cpu_times.user
        memory_info =proc.memory_full_info()
        mem_percent = proc.memory_percent() 
        open_files = proc.open_files()

        rss = memory_info.rss  
        vms =memory_info.vms  
        shared = memory_info.shared  
        text = memory_info.text  
        data = memory_info.data  
        lib = memory_info.lib
        fields = ["Process Name", "CPU times", "RSS memory",
                    "VMS memory", "Shared memory", "Text segment", "Data segment", "Library code", "Memory Percent", "IO Connections", "Open Files"]
        values = [proc.name(), burst_time, rss, vms, shared, text, data, lib, mem_percent, open_files]

        with open("Data/matmul.csv", "a", newline="") as csvfile:  # Open in append mode
            writer = csv.writer(csvfile)

            # Write header only if the file is empty
            if os.stat("Data/matmul.csv").st_size == 0:
                writer.writerow(fields)

            writer.writerow(values)

            print("Process information written to CSV.")
        
    except psutil.NoSuchProcess:
        pass

while True:
    time.sleep(1e-2)
    check_running_processes()
    retCode = proc.poll()
    if retCode is not None:
        break