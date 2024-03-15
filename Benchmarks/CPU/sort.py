import numpy as np

def sorting_load(maxsize=10000):

    size = np.random.randint(1, maxsize)
    data = np.random.randint(low=1,high=10000,size=size)
    data.sort()
    return data
