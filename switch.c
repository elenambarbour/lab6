// filename: switch.c
// primary author: Galen Sasaki
// co-author: Jessica Grazziotin
// EE 367 lab 6
// Description: This file is a type of host that acts as a switch 
//              to redirect messages to the proper destination
//		similar to a router


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "net.h"
#include "man.h"
#include "host.h"
#include "packet.h"

#define MAX_FILE_BUFFER 1000
#define MAX_MSG_LENGTH 100
#define MAX_DIR_NAME 100
#define MAX_FILE_NAME 100
#define PKT_PAYLOAD_MAX 100
#define TENMILLISEC 10000   /* 10 millisecond sleep */
#define MAX_TABLE_SIZE 100 //size of the forwarding table -jess

//start off with the same main as host (no need anything before) -jess
// just add the struct for the forwarding table

struct switch_table_format { // struct setup for switch table-jess
	int valid_status;
	char dest;
	int port_num;
};

/*
 *  Main 
 */

void switch_main(int switch_id)// change host_id to switch_id -jess
{

struct switch_table_format forwardTable[MAX_TABLE_SIZE];
//^^create a array of the struct "switch_table_format" -jess

for (int j=0; j<MAX_TABLE_SIZE; j++){
	forwardTable[j].valid_status = 0; 
}

/* State */ //keep all this stuff? -jess
char dir[MAX_DIR_NAME];
int dir_valid = 0;

char man_msg[MAN_MSG_LENGTH];
char man_reply_msg[MAN_MSG_LENGTH];
char man_cmd;
//struct man_port_at_host *man_port;  // Port to the manager
//^^don't need that anymore -jess

struct net_port *node_port_list;
struct net_port **node_port;  // Array of pointers to node ports
int node_port_num;            // Number of node ports

int ping_reply_received;

int i, k, n;
int dst;
char name[MAX_FILE_NAME];
char string[PKT_PAYLOAD_MAX+1]; 

FILE *fp;

struct packet *in_packet; /* Incoming packet */
struct packet *new_packet;

struct net_port *p;
struct host_job *new_job;
struct host_job *new_job2;

struct job_queue job_q;

//take out all of the file buffer stuff cuz it's for the host - jess
/*
struct file_buf f_buf_upload;  
struct file_buf f_buf_download; 

file_buf_init(&f_buf_upload);
file_buf_init(&f_buf_download);
*/
/* //get ride of this too I think cuz it will just use once it gets to the right host? - jess
 * Initialize pipes 
 * Get link port to the manager
 */

//man_port = net_get_host_port(host_id);

/*
 * Create an array node_port[ ] to store the network link ports
 * at the host.  The number of ports is node_port_num
 */

//hold on to the rest of this host stuff for info about the nodes and ports -jess
node_port_list = net_get_port_list(switch_id); 

	/*  Count the number of network link ports */
node_port_num = 0;
for (p=node_port_list; p!=NULL; p=p->next) {
	node_port_num++;
}
	/* Create memory space for the array */
node_port = (struct net_port **) 
	malloc(node_port_num*sizeof(struct net_port *));

	/* Load ports into the array */
p = node_port_list;
for (k = 0; k < node_port_num; k++) {
	node_port[k] = p;
	p = p->next;
}	

/* Initialize the job queue */
job_q_init(&job_q);

while(1) {
	// take out the switch case for the manager's commands
	// now we're just dealing with sending the packets to the right host
	// which will then parse out the command -jess

	/*
	 * Get packets from incoming links and translate to jobs
  	 * Put jobs in job queue
 	 */

	for (k = 0; k < node_port_num; k++) { /* Scan all ports */

		in_packet = (struct packet *) malloc(sizeof(struct packet));//make room for a new packet -jess
		n = packet_recv(node_port[k], in_packet); //check out the received packet -jess

		//if ((n > 0) && ((int) in_packet->dst == host_id)) {
		//just check if it exists -jess	
		if (n>0){
			//same as host... create a new job, set port index, and the packet -jess
			new_job = (struct host_job *) 
				malloc(sizeof(struct host_job));
			new_job->in_port_index = k;
			new_job->packet = in_packet;
		
			job_q_add(&job_q, new_job);
			//take out the switch and just add it because we're not perfoming the job, just transfering it -jess
			
		}
		
		else {
			free(in_packet);
		}
	}

	/*
 	 * Execute one job in the job queue
 	 */

	if (job_q_num(&job_q) > 0) {//from host - checks if there are any jobs in the queue -jess

		/* Get a new job from the job queue */
		new_job = job_q_remove(&job_q);

		
		//check if the source is in the table (else, add it to the table as a possible destination) -jess
	
		//check if the destination matches a table destination -jess
			//if true -- send it 
			// else -- send it on blast




	}//end of if job_q_num

	/* The host goes to sleep for 10 ms */
	usleep(TENMILLISEC);

} /* End of while loop */

}




