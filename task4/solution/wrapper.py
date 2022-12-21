import subprocess
import numpy as np
import matplotlib.pyplot as plt


def compile(threads_num: int):
  subprocess.call(["make", "T_NUM={}".format(threads_num)])

def run(max_threads:int):
  f = open("res.txt", "w")
  f.close()
  for i in range(1, max_threads +1):
    compile(i)
    subprocess.call(["./integrate"])

def plot(fname:str):
  fig, ax = plt.subplots()
  x_threads = []
  y_times = []
  f = open(fname, "r")
  for line in f:
    [x, y] = line.split()
    x_threads.append(int(x))
    y_times.append(float(y))
  ax.bar(x_threads, y_times)
  ax.minorticks_on()
  ax.set_xlabel("Количество потоков", fontsize=15)
  ax.set_ylabel("Время рассчета, сек.", fontsize=15)
  plt.show()
  

if __name__ == "__main__":
  mov = int(input("Type:\n 1 - for compile and run codes \n 2 - to plot elapsed time\n"))
  if mov == 1:
    n_threads = int(input("Введите максимальное желаемое количество потоков:"))
    run(n_threads)
  elif mov == 2:
    plot("res.txt")
