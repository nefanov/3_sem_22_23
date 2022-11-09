import os 

prefixProgram = '/mnt/c/Users/olegb/YandexDisk-berezin.op@phystech.edu/Учёба/CT/3_sem_22_23/task3/solution/'
prefixText = '~/text/'

bufs = ['SMALL ', 'MEDIUM ', 'BIG ']
files = ['small.txt', 'medium.txt','big.txt']

outputFile = prefixText + '2.txt'

# sender = 'time ' + prefixProgram + 'SharedMemory/./shm_server ' + buf + inputFile
# receiver = 'time ' + prefixProgram + 'SharedMemory/./shm_client ' + buf + inputFile + ' ' + outputFile

# print(sender)
# print(receiver)

for file in files:
    print('---file: ' + file + ' ---')

    for buf in bufs:
        print('~~~buf: ' + buf + ' ~~~')

        sender = 'time ' + prefixProgram + 'FIFO/./fifo_server ' + buf + prefixText + file
        receiver = 'time ' + prefixProgram + 'FIFO/./fifo_client ' + buf + prefixText + file + ' ' + outputFile
        cmd = sender + '&' + receiver
        os.system(cmd)
        # os.system(receiver)