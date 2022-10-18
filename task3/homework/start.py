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
        print(outputFormat + "\nFIFO TIME:", passedTime)
        print("")
        cmd = "./mqSYSV/mqSYSVReceiver.out & ./mqSYSV/mqSYSVSender.out"
        start = time.time()
        os.system(cmd)
        end   = time.time()

        passedTime = end - start
        print("MQSYSV TIME:", passedTime)
    
    
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
        print("MQSYSV TIME:", passedTime)

else:
    print("Write 'start.py -h' for information.")

