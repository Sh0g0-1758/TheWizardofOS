from Sched_Load.load_sched import load_sched
import pickle
import pandas as pd
import random
import os


def get_scheduler_data(num_proc=10):
    if os.path.exists("sched_data.csv"):
        os.remove("sched_data.csv")
    df = {"actual_cpu_time": [], "predicted_cpu_time": []}
    for i in range(2*num_proc):
        if len(df["actual_cpu_time"]) == num_proc:
            break
        benchmark_dir = "./Benchmarks/Executables/CPU"
        bench_file = random.choice(os.listdir(benchmark_dir))
        proc_attr = pd.DataFrame(load_sched(f"{benchmark_dir}/{bench_file}"))
        cpu_time = proc_attr["CPU times"][0]*1000
        if cpu_time < 0.1:
            continue
        df["actual_cpu_time"].append(cpu_time)
        proc_attr.drop(columns=["CPU times"], inplace=True)
        model = pickle.load(open('model.pkl', 'rb'))
        df["predicted_cpu_time"].append(model.predict(proc_attr)[0]*1000)

    pd.DataFrame(df).to_csv("sched_data.csv")


get_scheduler_data()
