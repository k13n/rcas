import os
import time


path = os.path.dirname(__file__)
os.chdir(path + "../release")
print("Current working directory: {0}".format(os.getcwd()))

def current_milli_time():
    return round(time.time() * 1000)

def run(size):
    cmd = "./bm_scalability2 %d > ../experiments/bm_scalability2.01.%d.txt"%(size,size)
    print(cmd)
    t_start = current_milli_time()
    os.system(cmd)
    t_end = current_milli_time()
    print("runtime (ms): %d"%(t_end - t_start))


gb = 1000000000
dataset_sizes = [
        25*gb,
        50*gb,
        75*gb,
       100*gb,
       125*gb,
       150*gb,
       175*gb,
       200*gb,
]

for dataset_size in dataset_sizes:
    run(dataset_size)
