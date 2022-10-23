import os
import sys
import time 


## hate makefiles....
class ANSI():
    def background(code):
        return "\33[{code}m".format(code=code)
  
    def style_text(code):
        return "\33[{code}m".format(code=code)
  
    def color_text(code):
        return "\33[{code}m".format(code=code)


argc = len(sys.argv)

if (argc > 1):
    if (sys.argv[1] == "-h"):
        print("Write 'start.py -small'  for testing with SMALL_SIZE")
        print("Write 'start.py -medium' for testing with MEDIUM_SIZE")
        print("Write 'start.py -large'  for testing with LARGE_SIZE")
        exit()


    if (sys.argv[1] == '-small'):
        cmd = "gcc -DSMALL_SIZE ./mqSYSV/mqSYSVSender.c -o ./mqSYSV/mqSYSVSender.out -lrt"
        os.system(cmd)
        cmd = "gcc -DSMALL_SIZE ./mqSYSV/mqSYSVReceiver.c -o ./mqSYSV/mqSYSVReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DSMALL_SIZE ./shmPOSIX/shmClientPOSIX.c -o ./shmPOSIX/shmClientPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DSMALL_SIZE ./shmPOSIX/shmServerPOSIX.c -o ./shmPOSIX/shmServerPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DSMALL_SIZE ./FIFO/FIFOReceiver.c -o ./FIFO/FIFOReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DSMALL_SIZE ./FIFO/FIFOSender.c   -o ./FIFO/FIFOSender.out -lrt"
        os.system(cmd)
        print("")
        cmd = "dd if=/dev/urandom of=input.txt bs=4096 count=1"
        os.system(cmd)
        print("")

        cmd = "./FIFO/FIFOReceiver.out & ./FIFO/FIFOSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        outputFormat = ANSI.background(49) + ANSI.color_text(31) + ANSI.style_text(7)
        print(outputFormat + "FIFO TIME:", passedTime)
        print("")

        cmd = "./mqSYSV/mqSYSVReceiver.out & ./mqSYSV/mqSYSVSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        print("MESSAGE QUEUE SYSV TIME:", passedTime)
        print("")

        cmd = "./shmPOSIX/shmClientPOSIX.out & ./shmPOSIX/shmServerPOSIX.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()
        
        passedTime = end - start
        print("SHARED MEMORY POSIX TIME:", passedTime)
        print("")
    
    if (sys.argv[1] == '-medium'):
        cmd = "gcc -DMEDIUM_SIZE ./mqSYSV/mqSYSVSender.c -o ./mqSYSV/mqSYSVSender.out -lrt"
        os.system(cmd)
        cmd = "gcc -DMEDIUM_SIZE ./mqSYSV/mqSYSVReceiver.c -o ./mqSYSV/mqSYSVReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DMEDIUM_SIZE ./shmPOSIX/shmClientPOSIX.c -o ./shmPOSIX/shmClientPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DMEDIUM_SIZE ./shmPOSIX/shmServerPOSIX.c -o ./shmPOSIX/shmServerPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DMEDIUM_SIZE ./FIFO/FIFOReceiver.c -o ./FIFO/FIFOReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DMEDIUM_SIZE ./FIFO/FIFOSender.c   -o ./FIFO/FIFOSender.out -lrt"
        os.system(cmd)
        cmd = "dd if=/dev/urandom of=input.txt bs=25600 count=4096"
        os.system(cmd)
        print("")

        cmd = "./FIFO/FIFOReceiver.out & ./FIFO/FIFOSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        outputFormat = ANSI.background(49) + ANSI.color_text(31) + ANSI.style_text(7)
        print(outputFormat + "FIFO TIME:", passedTime)
        print("")

        cmd = "./mqSYSV/mqSYSVReceiver.out & ./mqSYSV/mqSYSVSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        print("MESSAGE QUEUE SYSV TIME:", passedTime)
        print("")

        cmd = "./shmPOSIX/shmClientPOSIX.out & ./shmPOSIX/shmServerPOSIX.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()
        
        passedTime = end - start
        print("SHARED MEMORY POSIX TIME:", passedTime)
        print("")
    
    if (sys.argv[1] == '-large'):
        cmd = "gcc -DLARGE_SIZE ./mqSYSV/mqSYSVSender.c -o ./mqSYSV/mqSYSVSender.out -lrt"
        os.system(cmd)
        cmd = "gcc -DLARGE_SIZE ./mqSYSV/mqSYSVReceiver.c -o ./mqSYSV/mqSYSVReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DLARGE_SIZE ./shmPOSIX/shmClientPOSIX.c -o ./shmPOSIX/shmClientPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DLARGE_SIZE ./shmPOSIX/shmServerPOSIX.c -o ./shmPOSIX/shmServerPOSIX.out -lrt"
        os.system(cmd)
        cmd = "gcc -DLARGE_SIZE ./FIFO/FIFOReceiver.c -o ./FIFO/FIFOReceiver.out -lrt"
        os.system(cmd)
        cmd = "gcc -DLARGE_SIZE ./FIFO/FIFOSender.c   -o ./FIFO/FIFOSender.out -lrt"
        os.system(cmd)
        cmd = "dd if=/dev/urandom of=input.txt bs=1048576 count=2048"
        os.system(cmd)
        print("")

        cmd = "./FIFO/FIFOReceiver.out & ./FIFO/FIFOSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        outputFormat = ANSI.background(49) + ANSI.color_text(31) + ANSI.style_text(7)
        print(outputFormat + "FIFO TIME:", passedTime)
        print("")

        cmd = "./mqSYSV/mqSYSVReceiver.out & ./mqSYSV/mqSYSVSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        print("MESSAGE QUEUE SYSV TIME:", passedTime)
        print("")

        cmd = "./shmPOSIX/shmClientPOSIX.out & ./shmPOSIX/shmServerPOSIX.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()
        
        passedTime = end - start
        print("SHARED MEMORY POSIX TIME:", passedTime)
        print("")

else:
    print("Write 'start.py -h' for information.")


# -small  BUF SIZE 512   FIFO 0.00245  MSQ 0.00228  SHM 0.00584
# -medium BUF SIZE 512   FIFO 0.39691  MSQ 1.19831  SHM 0.00657
# -large  BUF SIZE 512   FIFO 16.18651 MSQ 15.76633 SHM 0.00860
# -small  BUF SIZE 4096  FIFO 0.00481  MSQ 0.00231  SHM 0.00280
# -medium BUF SIZE 4096  FIFO 0.25731  MSQ 0.23929  SHM 0.00410
# -large  BUF SIZE 4096  FIFO 4.94263  MSQ 12.67133 SHM 0.00595
# -small  BUF SIZE 65536 FIFO 0.00284  ------------ SHM 0.00234
# -medium BUF SIZE 65536 FIFO 0.25731  ------------ SHM 0.00351
# -large  BUF SIZE 65536 FIFO 3.57521  ------------ SHM 0.00337