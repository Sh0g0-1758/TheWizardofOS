import os
import numpy as np
from Load.load import load
for i in range(200):
    load("./Benchmarks/CPU/ml.exe")
    load("./Benchmarks/CPU/matmul.exe")
    load("./Benchmarks/CPU/sort.exe")
    load("./Benchmarks/IO/file.exe")
