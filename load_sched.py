import psutil
import time


def load_sched(path):
    vals = {"CPU times": [], "RSS memory": [], "VMS memory": [], "Shared memory": [], "Text segment": [],
            "Data segment": [], "Memory Percent": []}

    proc = psutil.Popen([path])

    def check_running_processes():
        try:
            with proc.oneshot():
                cpu_times = proc.cpu_times()
                burst_time = cpu_times.system + cpu_times.user
                memory_info = proc.memory_full_info()
                mem_percent = proc.memory_percent()
                rss = memory_info.rss
                vms = memory_info.vms
                shared = memory_info.shared
                text = memory_info.text
                data = memory_info.data
                lib = memory_info.lib

                vals["CPU times"].append(burst_time)
                vals["RSS memory"].append(rss)
                vals["VMS memory"].append(vms)
                vals["Shared memory"].append(shared)
                vals["Text segment"].append(text)
                vals["Data segment"].append(data)
                vals["Memory Percent"].append(mem_percent)

        except Exception as e:
            pass

    while True:
        time.sleep(1e-2)
        check_running_processes()
        retCode = proc.poll()
        if retCode is not None:
            break

    vals["CPU times"] = [vals["CPU times"][-1]]
    vals["RSS memory"] = [sum(vals["RSS memory"])/len(vals["RSS memory"])]
    vals["VMS memory"] = [sum(vals["VMS memory"])/len(vals["VMS memory"])]
    vals["Shared memory"] = [
        sum(vals["Shared memory"])/len(vals["Shared memory"])]
    vals["Text segment"] = [
        sum(vals["Text segment"])/len(vals["Text segment"])]
    vals["Data segment"] = [
        sum(vals["Data segment"])/len(vals["Data segment"])]
    vals["Memory Percent"] = [
        sum(vals["Memory Percent"])/len(vals["Memory Percent"])]

    return vals
