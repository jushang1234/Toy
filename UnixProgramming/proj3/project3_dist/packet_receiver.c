/* CSci 4061 F2014 Assignment 3
 * Section 3
 * Date: 10/10/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "packet.h"

int msqid = -1;

static message_t message;   /* current message structure */
static mm_t mm;             /* memory manager will allocate memory for packets */
static int pkt_cnt = 0;     /* how many packets have arrived for current message */
static int pkt_total = 1;   /* how many packets to be received for the message */

/*
   Handles the incoming packet. 
   Store the packet in a chunk from memory manager.
   The packets for given message will come out of order. 
   Hence you need to take care to store and assemble it correctly.
   Example, message "aaabbb" can come as bbb -> aaa, hence, you need to assemble it
   as aaabbb.
   Hint: "which" field in the packet will be useful.
 */
static void packet_handler(int sig) {

  if (sig == SIGINT) {
    if ((msgctl(msqid, IPC_RMID, NULL)) < 0) {
      fprintf(stderr, "Failed to remove message queue. %s\n", strerror(errno));
      exit(-1);
    }
    fprintf(stderr, "Removed message queue %d\n", msqid);
    exit(-1);
  }

  if (sig == SIGTERM) {
    fprintf(stderr, "Sender disconnected. Abort.\n");
    exit(-1);
  }

  packet_t pkt;
  void *chunk;
  packet_queue_msg pkt_msg;

  if (msqid == -1) {
    fprintf(stderr, "Failed to get message queue identifier.\n");
    exit(-1);
  }
    
  // TODO get the "packet_queue_msg" from the queue.

  if ((msgrcv(msqid, (void*)&pkt_msg, sizeof(packet_queue_msg), QUEUE_MSG_TYPE, 0)) == -1) {
    fprintf(stderr, "Failed to receive packet. %s\n", strerror(errno));
    exit(-1);
  }

  pkt = pkt_msg.pkt;
  pkt_total = pkt.how_many;
  pkt_cnt++;

  // TODO extract the packet from "packet_queue_msg" and store it in the memory from memory manager

  if ((chunk = mm_get(&mm)) == NULL) {
    fprintf(stderr, "There is no available memory.\n");
    exit(-1);
  }
  memcpy(chunk, (void*)&pkt, sizeof(pkt));
  message.num_packets = pkt.how_many;
  message.data[pkt.which] = chunk;
  fprintf(stderr, "GOT PACKET %d of %d.\n", pkt.which, pkt.how_many);

  
}

/*
 * TODO - Create message from packets ... deallocate packets.
 * Return a pointer to the message on success, or NULL
 */
static char *assemble_message() {

  char *msg;
  int i;
  int msg_len = message.num_packets * sizeof(data_t);
  packet_t* pkt;

  /* TODO - Allocate msg and assemble packets into it */
  msg = (char*) malloc(sizeof(char) * (msg_len + 1));
  for (i = 0; i < message.num_packets; i++) {
    pkt = (packet_t*) message.data[i];
    memcpy(msg + i * sizeof(data_t), &(pkt->data), sizeof(pkt->data));
  }
  msg[msg_len] = '\0';


  /* reset these for next message */
  pkt_total = 1;
  pkt_cnt = 0;
  message.num_packets = 0;

  return msg;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: packet_sender <num of messages to receive>\n");
    exit(-1);
  }

  int k = atoi(argv[1]); /* no of messages you will get from the sender */
  int i;
  char *msg;

  struct sigaction act; 

  act.sa_handler = packet_handler;
  act.sa_flags = 0;
  sigfillset(&act.sa_mask);

  if ((sigaction(SIGIO, &act, NULL)) < 0 || (sigaction(SIGINT, &act, NULL)) < 0 || (sigaction(SIGTERM, &act, NULL)) < 0) {
    fprintf(stderr, "Failed to turn on signal handler. %s\n", strerror(errno));
    return -1;
  }


  mm_init(&mm, NUM_CHUNKS, CHUNK_SIZE);

  /* TODO - init memory manager for NUM_CHUNKS chunks of size CHUNK_SIZE each */

  message.num_packets = 0;

  /* TODO initialize msqid to send pid and receive messages from the message queue. Use the key in packet.h */
  if ((msqid = msgget(key, 0666|IPC_CREAT)) < 0) {
    fprintf(stderr, "Failed to get message queue identifier. %s\n", strerror(errno));
    exit(-1);
  }

  fprintf(stderr, "The msqid is %d\n", msqid);

  /* TODO send process pid to the sender on the queue */

  int pid = getpid();
  pid_queue_msg pid_msg;

  pid_msg.pid = pid;
  pid_msg.mtype = QUEUE_MSG_TYPE;

  if ((msgsnd(msqid, (void*)&pid_msg, sizeof(pid_queue_msg), 0)) < 0) {
    fprintf(stderr, "Failed to send pid. %s\n", strerror(errno));
    if ((msgctl(msqid, IPC_RMID, NULL)) < 0) {
      fprintf(stderr, "Failed to remove message queue. %s\n", strerror(errno));
    }
    exit(-1);
  }
  
  /* TODO set up SIGIO handler to read incoming packets from the queue. Check packet_handler()*/



  for (i = 1; i <= k; i++) {
    while (pkt_cnt < pkt_total) {
      pause(); /* block until next packet */
    }
  
    msg = assemble_message();
    if (msg == NULL) {
      perror("Failed to assemble message");
    }
    else {
      fprintf(stderr, "GOT IT: message=%s\n", msg);
      free(msg);
    }
  }

  // TODO deallocate memory manager
  mm_release(&mm);

  // TODO remove the queue once done
  if ((msgctl(msqid, IPC_RMID, NULL)) < 0) {
    fprintf(stderr, "Failed to remove message queue. %s\n", strerror(errno));
    return -1;
  }
  
  return EXIT_SUCCESS;
}
