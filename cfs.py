#!/usr/bin/env python3

"""
Program to simulate Completely Fair Scheduler (CFS).
"""

import random
import sys
from math import floor
from typing import Callable, List
import time

import numpy
import pandas as pd
from sortedcontainers import SortedKeyList
from tabulate import tabulate

priority_to_weight = [
    88761, 71755, 56483, 46273, 36291,
    29154, 23254, 18705, 14949, 11916,
    9548, 7620, 6100, 4904, 3906,
    3121, 2501, 1991, 1586, 1277,
    1024, 820, 655, 526, 423,
    335, 272, 215, 172, 137,
    110, 87, 70, 56, 45,
    36, 29, 23, 18, 15,
]

Sum_Of_Weights = 445163


def modified_cfs_schedule(tasks: List[dict], sched_latency: int, min_granularity=6):
    """
    Schedule tasks according to CFS algorithm and set waiting and turnaround times.
    """

    get_vruntime: Callable[[dict], int] = lambda task: task["vruntime"]
    get_nice: Callable[[dict], int] = lambda task: task["nice"]
    global_timer = 0
    tasks_sorted = SortedKeyList(key=get_vruntime)
    for i in tasks:
        tasks_sorted.add(i)
    min_vruntime = 0
    while (num := len(tasks_sorted)) > 0:
        min_task = tasks_sorted[0]
        min_vruntime = get_vruntime(min_task)
        min_nice = get_nice(min_task)
        # print(min_task)

        timeslice = (priority_to_weight[min_nice]
                     * sched_latency) / Sum_Of_Weights
        if (timeslice < min_granularity):
            timeslice = min_granularity

        t_rem = min_task["burst_time"] - min_task["exec_time"]
        # Time of execution of smallest task
        time_sched = min([timeslice, t_rem])
        if (min_task["pred_burst_time"] < min_task["burst_time"]):
            if (min_task["pred_burst_time"] - min_task["exec_time"] > 0):
                if (min_task["exec_time"] + time_sched < min_task["pred_burst_time"]):
                    if (min_task["pred_burst_time"] - min_task["exec_time"] - time_sched <= timeslice):
                        time_sched = min_task["pred_burst_time"] - \
                            min_task["exec_time"]

        global_timer += time_sched

        # Execute process
        vruntime = min_vruntime + \
            ((time_sched * 1024) / priority_to_weight[min_nice])
        min_task["exec_time"] += time_sched
        min_task["turnaround_time"] += time_sched

        # Increment waiting and turnaround time of all other processes
        for i in range(1, num):
            if (global_timer >= tasks_sorted[i]["arrival_time"]):
                tasks_sorted[i]["waiting_time"] += time_sched
                tasks_sorted[i]["turnaround_time"] += time_sched

        # Remove from sorted list and update vruntime
        task = tasks_sorted.pop(0)
        task["vruntime"] = vruntime

        # Insert only if execution time is left
        if min_task["exec_time"] < min_task["burst_time"]:
            tasks_sorted.add(task)

        # Adding Context Switch Delay Time Period
        for i in range(0, len(tasks_sorted)):
            tasks_sorted[i]["waiting_time"] += 100
            tasks_sorted[i]["turnaround_time"] += 100


def normal_cfs_schedule(tasks: List[dict], sched_latency: int, min_granularity=6):
    """
    Schedule tasks according to CFS algorithm and set waiting and turnaround times.
    """

    get_vruntime: Callable[[dict], int] = lambda task: task["vruntime"]
    get_nice: Callable[[dict], int] = lambda task: task["nice"]
    global_timer = 0
    tasks_sorted = SortedKeyList(key=get_vruntime)
    for i in tasks:
        tasks_sorted.add(i)
    min_vruntime = 0

    while (num := len(tasks_sorted)) > 0:
        min_task = tasks_sorted[0]
        min_vruntime = get_vruntime(min_task)
        min_nice = get_nice(min_task)
        # print(min_task)

        timeslice = (priority_to_weight[min_nice]
                     * sched_latency) / Sum_Of_Weights
        if (timeslice < min_granularity):
            timeslice = min_granularity

        t_rem = min_task["burst_time"] - min_task["exec_time"]

        # Time of execution of smallest task
        time_sched = min([timeslice, t_rem])
        global_timer += time_sched

        # Execute process
        vruntime = min_vruntime + \
            ((time_sched * 1024) / priority_to_weight[min_nice])
        min_task["exec_time"] += time_sched
        min_task["turnaround_time"] += time_sched

        # Increment waiting and turnaround time of all other processes
        for i in range(1, num):
            if (global_timer >= tasks_sorted[i]["arrival_time"]):
                tasks_sorted[i]["waiting_time"] += time_sched
                tasks_sorted[i]["turnaround_time"] += time_sched

        # Remove from sorted list and update vruntime
        task = tasks_sorted.pop(0)
        task["vruntime"] = vruntime

        # Insert only if execution time is left
        if min_task["exec_time"] < min_task["burst_time"]:
            tasks_sorted.add(task)

        # Adding Context Switch Delay Time Period
        for i in range(0, len(tasks_sorted)):
            tasks_sorted[i]["waiting_time"] += 100
            tasks_sorted[i]["turnaround_time"] += 100


def display_tasks(tasks: List[dict]):
    """
    Print all tasks' information in a table.
    """

    headers = [
        "ID",
        "Arrival Time",
        "Burst Time",
        "Nice",
        "Waiting Time",
        "Turnaround Time",
    ]
    tasks_mat = []

    for task in tasks:
        tasks_mat.append(
            [
                task["pid"],
                f"{task['arrival_time'] / 1000}",
                f"{task['burst_time'] / 1000}",
                task["nice"] - 20,
                f"{task['waiting_time'] / 1000}",
                f"{task['turnaround_time'] / 1000}",
            ]
        )
    print(
        "\n"
        + tabulate(tasks_mat, headers=headers,
                   tablefmt="fancy_grid", floatfmt=".3f")
    )
    # print('\n' + tabulate(tasks, headers='keys', tablefmt='fancy_grid'))


def find_avg_time(tasks: List[dict]):
    """
    Find average waiting and turnaround time.
    """

    waiting_times = []
    total_wt = 0
    total_tat = 0
    num = len(tasks)

    for task in tasks:
        waiting_times.append(task["waiting_time"])
        total_wt += task["waiting_time"]
        total_tat += task["turnaround_time"]

    print(f"\nAverage waiting time: {total_wt / (num * 1000): .3f} seconds")
    print(f"Average turnaround time: {total_tat / (num * 1000): .3f} seconds")


if __name__ == "__main__":
    MIN_VERSION = (3, 8)
    if not sys.version_info >= MIN_VERSION:
        raise EnvironmentError(
            "Python version too low, required at least "
            f'{".".join(str(n) for n in MIN_VERSION)}'
        )

    SCHED_LATENCY = 5000
    MAX_NICE_VALUE = 39  # proxy value for 20
    MIN_NICE_VALUE = 0  # proxy value for -20
    min_granularity = 6

    NORMAL_TASKS = []
    MODIFIED_TASKS = []
    file_path = "output.txt"

    # Initialize an empty list to store the values
    sched_data = pd.read_csv('sched_data.csv')
    arrival_time = numpy.random.randint(0, 1000, len(sched_data))

    for i in range(len(sched_data)):
        pid, at, nice = (
            random.randint(1, 1000),
            arrival_time[i],
            random.randint(MIN_NICE_VALUE, MAX_NICE_VALUE),
        )
        NORMAL_TASKS.append(
            {
                "pid": pid,
                "arrival_time": at,
                "burst_time": sched_data["actual_cpu_time"][i],
                "nice": nice,
                "vruntime": 0,
                "exec_time": 0,
                "waiting_time": 0,
                "turnaround_time": 0,
            }
        )
        MODIFIED_TASKS.append(
            {
                "pid": pid,
                "arrival_time": at,
                "burst_time": sched_data["actual_cpu_time"][i],
                "pred_burst_time": sched_data["predicted_cpu_time"][i],
                "nice": nice,
                "vruntime": 0,
                "exec_time": 0,
                "waiting_time": 0,
                "turnaround_time": 0,
            }
        )

    # Sort tasks by arrival time
    NORMAL_TASKS_SORTED = SortedKeyList(
        NORMAL_TASKS, key=lambda task: task["arrival_time"])

    MODIFIED_TASKS_SORTED = SortedKeyList(
        MODIFIED_TASKS, key=lambda task: task["arrival_time"])

    # Schedule tasks according to CFS algorithm and print average times
    # reset_tasks(TASKS_SORTED) # might be removable
    normal_cfs_schedule(NORMAL_TASKS_SORTED, SCHED_LATENCY, min_granularity)
    print("\n**************** NORMAL CFS SCHEDULING ****************")
    display_tasks(NORMAL_TASKS_SORTED)
    find_avg_time(NORMAL_TASKS_SORTED)

    modified_cfs_schedule(MODIFIED_TASKS_SORTED,
                          SCHED_LATENCY, min_granularity)
    print("\n**************** MODIFIED CFS SCHEDULING ****************")
    display_tasks(MODIFIED_TASKS_SORTED)
    find_avg_time(MODIFIED_TASKS_SORTED)
