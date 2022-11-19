#include "message_queue.h"
#define MSG_TYPE 1
#define LAST_MSG_TYPE 2

const char* key_name = "/home/ilya/Документы/GitHub/3_sem_22_23/task3/solution/times";

int get_queue(const char* name, int flags) {
  int msq_id;
	key_t key;

	if ((key = ftok(name, 2)) < 0) {
		perror("[constructor]ftok");
		exit(1);
	}
  //printf("Current key: %d\n", key);

	if ((msq_id = msgget(key, flags)) < 0){
		perror("[constructor]msgget");
		exit(1);
	}

	return msq_id;
}


int msq_snd(const char* fname) {
  int msgid = get_queue(key_name, IPC_CREAT | 0666);
    // Get file descriptor
  int fin = open(fname, O_RDONLY);
  if (fin == -1) {
    perror("[sender]open fd error");
    exit(1);
  }

  // Initialization
  message s_msg;
  s_msg.mtype = MSG_TYPE;
  char *buf = malloc(BUF_SIZE);

  // Msg creation
  size_t read_sz = BUF_SIZE;
  while (1) {
    // Read data from file and write in msg
    read_sz = read(fin, buf, BUF_SIZE);
    strcpy(s_msg.buffer.mdata, buf);

    if (read_sz != BUF_SIZE) {
      if (read_sz != 0) {
        // Send last not full  msg
        msgsnd(msgid, &s_msg, read_sz, 0);
      }

      s_msg.mtype = LAST_MSG_TYPE;
      msgsnd(msgid, &s_msg, 0, 0);
      //printf("Sender: last msg was sent.\n");

      //printf("Sender: EOF!\n");
      break;
    } else {
      msgsnd(msgid, &s_msg, sizeof(buffer_t), 0);
    }
  }

  free(buf);
  close(fin);
  return 0;
}

int msq_rcv(const char* fname) {
  int msgid = get_queue(key_name, IPC_CREAT | 0666);
  int fout = open(fname, O_WRONLY | O_CREAT);
  if (fout == -1) {
    perror("[reciever]open fd error");
    exit(1);
  }

  message r_msg;
  size_t written_sz;
  int len;

  // Msg receiving
  while (1) {
    // Receive msg from queue and write in file
    len = (int)msgrcv(msgid, &r_msg, sizeof(buffer_t),0,0);
    if (len < 0) {
      printf("Receiver: len < 0\n");
      exit(1);
    }

    if (r_msg.mtype == LAST_MSG_TYPE) {
      printf("Receiver: last msg was received.\n");

      printf("Receiver: deleting msg queue..\n");
      msgctl(msgid, IPC_RMID, (struct msqid_ds *) NULL);

      break;
    } else {

      written_sz = write(fout, r_msg.buffer.mdata, len);
      if (written_sz == -1) {
        printf("Bad write of file\n");
        exit(1);
      }
    }
  }

  close(fout);
  return 0;
}