import math
import subprocess
import os
import time
import matplotlib.pyplot as plt

def file_gen(fname, fsize):
  subprocess.call(["dd", "if=/dev/urandom", "of={}".format(fname), "bs={}".format(fsize), "count=1"])

def generate_fset(names, sizes):
  if (len(names) != 3 or len(names) != 3):
    return -1 
  for i in range(3):
    file_gen(names[i], sizes[i])

def compile_buff(buf_size):
  subprocess.call(["make", "BUF_SIZE={}".format(buf_size)])

def run_prog(fname_in, fname_out, sender, reciever):
  print("Current command: " + "time {} {} & {} {}".format(sender, fname_in, reciever, fname_out))
  subprocess.run("{} {} & {} {}".format(sender, fname_in, reciever, fname_out), shell=True)
  #a = subprocess.check_call(["time", "{} {} & {} {}".format(sender, fname_in, reciever, fname_out)])
  #return a

def run_single_test(fnames, sender, reciever):
  times = []
  if (len(fnames) != 3):
    return None
  for name in fnames:
    start = time.time()
    run_prog(name, "out.txt", sender, reciever)
    end = time.time()
    times.append(end-start)
  return times


def run_tests(fnames, buf_szs, send_names, recieve_names):
  times = []
  if (len(fnames) != 3 or len(buf_szs) != 3):
    return -1
  for i in range(3):
    print("---------------------------------------------------------------------------")
    print("Run tests with buffer size {}".format(buf_szs[i]))
    compile_buff(buf_szs[i])
    buf_times = []
    for j in range(3):
      print("Run programs: {} {}".format(send_names[j], recieve_names[j]))
      buf_times.append(run_single_test(fnames, send_names[j], recieve_names[j]))
    times.append(buf_times)
  return times

'''
  returnable val for run_tests
  [fixed_buff_size:[shm, msq, fifo], where shm, maq, fifo = [small, med, huge]]
  '''
def draw(times:list):
  sz = int(input("select buf size:\n1 - small\n2-medium\n3-huge\n")) - 1
  tm = times[sz]
  nums_2 = [1, 2, 3]
  nums_1 = [i - 0.3 for i in nums_2]
  nums_3 = [i + 0.3 for i in nums_2]

  fig, ax = plt.subplots()
  ax.bar(nums_1, tm[0],width=0.3,label="shared memory")
  ax.bar(nums_2, tm[1],width=0.3,label="message queue")
  ax.bar(nums_3, tm[2],width=0.3,label="FIFO channel")
  ax.set_xlabel("File size,")
  ax.set_ylabel("Time, secs")
  ax.legend()
  plt.show()

if __name__ == "__main__":
  '''
  returnable val for run_tests
  [fixed_buff_size:[shm, msq, fifo], where shm, maq, fifo = [small, med, huge]]
  '''
  names = ["small_test.txt", "medium_test.txt", "huge_test.txt"]
  sizes = [100 * 1024, 1 * 1024 * 1024, 100 * 1024 * 1024] # [100kB, 1 MB, 100MB]
  buffs = [1024, 10 * 1024, 100 * 1024]
  send_names = ["./executables/" + name for name in ["shm_sender", "msq_sender", "msq_sender"]]
  recieve_names = ["./executables/" + name for name in ["shm_reciever", "msq_reciever", "msq_reciever"]]
  generate_fset(names, sizes)
  times = run_tests(names, buffs, send_names, recieve_names)
  draw(times)
  print(times)
