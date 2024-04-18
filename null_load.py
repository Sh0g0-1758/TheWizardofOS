import atexit
import psutil
import os
import csv


def load():
    return


def check_running_processes():
    """
    This function will be called when the main program exits.
    It iterates through child processes and attempts to get their CPU times.
    """
    pid = os.getpid()
    proc = psutil.Process(pid)

    try:
        # Attempt to get CPU times. May fail if process has exited.
        with proc.oneshot():  # Efficiently retrieve multiple fields at once
            cpu_times = proc.cpu_times()
            burst_time = cpu_times.system + cpu_times.user
            memory_info = proc.memory_full_info()

            # Collect available memory information
            rss = memory_info.rss  # Resident Set Size
            vms = memory_info.vms  # Virtual Memory Size
            shared = memory_info.shared  # Shared memory
            text = memory_info.text  # Text segment size
            data = memory_info.data  # Data segment size
            lib = memory_info.lib  # Library code size
            # Write process information to CSV
            fields = ["Process ID", "Process Name", "CPU times", "RSS memory",
                      "VMS memory", "Shared memory", "Text segment", "Data segment", "Library code"]
            values = [pid, proc.name(), burst_time, rss, vms,
                      shared, text, data, lib]

            with open("null_data.csv", "a", newline="") as csvfile:  # Open in append mode
                writer = csv.writer(csvfile)

                # Write header only if the file is empty
                if os.stat("null_data.csv").st_size == 0:
                    writer.writerow(fields)

                writer.writerow(values)

                print("Process information written to CSV.")

    except psutil.NoSuchProcess:
        pass  # Ignore processes that have already exited


atexit.register(check_running_processes)
load()
