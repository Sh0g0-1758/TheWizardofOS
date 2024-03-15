import os
import numpy as np
for i in range(200):
    if np.random.uniform() < 0.1:
        os.system("python null_load.py")
    else:
        os.system("python load.py")
