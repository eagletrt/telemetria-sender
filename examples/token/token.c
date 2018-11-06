/*
 * Authors: Ivan Martini <ivanmartini96@gmail.com>
 *
 * Basic and rude system for a token ring communication over the can bus
 * From the user view of course. The idea is to let all the nodes receive and communicate. 
 * A node can only used in one way so there is a "token" which allows the owner to send data.
 * When the node wants to read the data on the bus he must give the token to another member of the network.
 *
 * Constraints:
 * - The nodes can be 254 (with id going from 1 to 254, 255 is reserved to broadcast)
 * - The network is not aware of the number of nodes not about their id
 *   So when giving the token or when assigning an id, CARE ABOUT WHAT YOU DO.
 * - Payload of a message is 6 bytes.
 * - Message can be sent to a specific id or on broadcast
 * - node with id 1 starts with the token.
 *
 */

#include <stdio.h>
#include "plugin/can_custom_lib.h"

int main(int argc, char const *argv[]) {	
	//set up 
	int node_id;
	
	printf("Enter this node id [1 - 254] :");
	scanf("%d", &node_id);

	if (node_id < 1 || node_id > 254) {
		printf("The entered node id is out of valid range. [0 - 254]\n");
		return -1;
	}

	printf("\n\nInitializing Node %d.\n\n", node_id);

	int s;
	struct sockaddr_can addr;

	char* name = (char*) malloc(sizeof(char)*5);
	strcpy(name,"vcan0");

	s = open_can_socket(name,&addr);

	int token = 0;

	//starting the real program
	if (node_id == 1) {
		token = 1;
	}

	while (token != -1) {
		if (token) {
			//write
			char choice = 'a';
			
			while (choice != 'w' && choice != 'W' && choice != 't' && choice != 'T' && choice != 'b' && choice != 'B') {
				printf("\nWhat do you want to do [ \033[4mW\033[0mhisper | \033[4mB\033[0mroadcast | \033[4mT\033[0moken ] ");
				fflush(0);
				scanf(" %c", &choice);
			}

			int id = 0x7FF;

			char* data = (char*) malloc(8*sizeof(char));
			data[0] = node_id;

			if (choice != 't' || choice != 'T') {
				//whisper or broadcast
				if (choice == 'w' || choice == 'W') {
				int dest = -1;
					while (dest < 1 || dest > 254) {
						printf("To which node do you want to whisper? [ 1 - 254 ] ");
						fflush(0);
						scanf(" %d", &dest);
					}

					data[1] = dest;
				} else {
					data[1] = 0xFF;
				}

				int prio = -1;
				while (prio < 0x001 || prio > 0x7FF) {
					printf("Which priority do you want for your message [ 1 - 7FF ] ");
					fflush(0);
					scanf(" %x", &prio);
				}

				id = prio;

				int byte = -2;
				while (byte < -1 || byte > 6) {
					printf("How many bytes do you want to send [ 0 - 6 ]");
					fflush(0);
					scanf(" %d", &byte);
				}

				printf("Write down your payload ");
				fflush(0);

				for (int i = 2; i < 2+byte; ++i) {
					scanf("%x", (unsigned int *) &data[i]);
				}

			} else {
				//give token

				id = 0x001;

				for (int i = 2; i < 8; ++i)
					data[i] = 0xFF;

				int tok = -1;
				while (tok < 1 || tok > 254) {
					printf("Which node do you want to give the token? [1 - 254]: ");
					fflush(0);
					scanf(" %d", &tok);
				}

				data[1] = tok;
				token = 0;

				printf("\033[96mGiving the token to node %i.\033[0m\n", tok);
			}
			
			send_can(s,id,data);	

		} else {
			//read
			int id = 0;
			char* data;

			receive_can(s,&id,&data);
			if ( (unsigned char) data[1] == node_id || (unsigned char) data[1] == 255) {
				//we read the message

				int check = 1;
				for (int i = 2;i<strlen(data); i++) {
					if ((unsigned char) data[i] != 255)
						check = 0;
				}

				if (check && data[1] == node_id) {
					// we got the token
					printf("\033[96mNode %i gave us the token! Now we can write\033[0m\n", (unsigned char) data[0]);
					token = 1;
				} else {
					// go on reading
					char* style = (char*) malloc(12*sizeof(char));

					if (id < 101) {
						//red
						style = "\033[91m";
					} else if (id < 301) {
						//orange
						style = "\033[93m";
					} else {
						//green
						style = "\033[92m";	
					}

					if ( (unsigned char)data[1] == node_id)
						printf("Node %i whispered to us with %s %d priority\033[0m: ", (unsigned char)data[0], style, id);
					else 
						printf("Node %i broadcasted to everyone with %s %d priority\033[0m: ", (unsigned char)data[0], style, id);

					for (int i = 2;i<strlen(data); i++) {
						printf("%02X ", (data[i] & 0xFF));
					}
					printf("\n");
				}				
			}
			
		}
	}

	return 0;
}