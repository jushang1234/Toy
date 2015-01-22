/* CSci4061 F2014 Assignment 2
 * Date: 10/27/14
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */ 

#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <gtk/gtk.h>

#define MAX_TAB 100

/*
 * Name:		uri_entered_cb
 * Input arguments:'entry'-address bar where the url was entered
 *			 'data'-auxiliary data sent along with the event
 * Output arguments:void
 * Function:	When the user hits the enter after entering the url
 *			in the address bar, 'activate' event is generated
 *			for the Widget Entry, for which 'uri_entered_cb'
 *			callback is called. Controller-tab captures this event
 *			and sends the browsing request to the router(/parent)
 *			process.
 */
void uri_entered_cb(GtkWidget* entry, gpointer data) {
	
	if(data == NULL) {	
		return;
	}

	browser_window* b_window = (browser_window*)data;
	comm_channel channel = b_window->channel;
	
	// Get the tab index where the URL is to be rendered
	int tab_index = query_tab_id_for_request(entry, data);

	if(tab_index < 0) {
		fprintf(stderr, "ERROR DURING CREATING TAB.\n");
        return;
	}

	// Get the URL.
	char* uri = get_entered_uri(entry);
	
	// Create a new request of type NEW_URI_ENTERED
	child_req_to_parent new_req;
	new_req.type = NEW_URI_ENTERED;
	strcpy(new_req.req.uri_req.uri, uri);
	new_req.req.uri_req.render_in_tab = tab_index;
	
	// Write request to pipe
	write(channel.child_to_parent_fd[1], &new_req, sizeof(child_req_to_parent));

}

/*
 * Name:		new_tab_created_cb
 * Input arguments:	'button' - whose click generated this callback
 *			'data' - auxillary data passed along for handling
 *			this event.
 * Output arguments:    void
 * Function:		This is the callback function for the 'create_new_tab'
 *			event which is generated when the user clicks the '+'
 *			button in the controller-tab. The controller-tab
 *			redirects the request to the parent (/router) process
 *			which then creates a new child process for creating
 *			and managing this new tab.
 */ 
void new_tab_created_cb(GtkButton *button, gpointer data) {
	
	if(data == NULL) {
		return;
	}

 	int tab_index = ((browser_window*)data)->tab_index;
	
	// This channel have pipes to communicate with router. 
	comm_channel channel = ((browser_window*)data)->channel;

	// Create a new request of type CREATE_TAB
	child_req_to_parent new_req;
	new_req.type = CREATE_TAB;
	//new_req.req.new_tab_req.tab_index = tab_index;
	
	// Write request to pipe
	write(channel.child_to_parent_fd[1], &new_req, sizeof(child_req_to_parent));
	

	// Users press + button on the control window. 
	// What is next?
	// Insert code here!!
}

/*
 * Name:                run_control
 * Input arguments:     'comm_channel': Includes pipes to communctaion with Router process
 * Output arguments:    void
 * Function:            This function will make a CONTROLLER window and be blocked until the program terminate.
 */
int run_control(comm_channel comm) {
	browser_window * b_window = NULL;

	//Create controler process
	create_browser(CONTROLLER_TAB, 0, G_CALLBACK(new_tab_created_cb), G_CALLBACK(uri_entered_cb), &b_window, comm);

	//go into infinite loop.
	show_browser();
	return 0;
}

/*
* Name:                 run_url_browser
* Input arguments:      'nTabIndex': URL-RENDERING tab index
                        'comm_channel': Includes pipes to communctaion with Router process
* Output arguments:     void
* Function:             This function will make a URL-RENDRERING tab Note.
*                       You need to use below functions to handle tab event. 
*                       1. process_all_gtk_events();
*                       2. process_single_gtk_event();
*                       3. render_web_page_in_tab(uri, b_window);
*                       For more details please Appendix B.
*/
int run_url_browser(int nTabIndex, comm_channel comm) {
	browser_window * b_window = NULL;
	
	//Create controler window
	create_browser(URL_RENDERING_TAB, nTabIndex, G_CALLBACK(new_tab_created_cb), G_CALLBACK(uri_entered_cb), &b_window, comm);


	child_req_to_parent rcvd_req;
	
	while (1) {

		process_single_gtk_event();
		usleep(1000);
		
		
		read(comm.parent_to_child_fd[0], &rcvd_req, sizeof(rcvd_req));
		
		if (rcvd_req.type == CREATE_TAB) {
			continue;
		}
		else if (rcvd_req.type == NEW_URI_ENTERED) {
			render_web_page_in_tab(rcvd_req.req.uri_req.uri, b_window);
		}
		else if (rcvd_req.type == TAB_KILLED) {
			close(comm.parent_to_child_fd[0]);
			close(comm.parent_to_child_fd[1]);
			process_all_gtk_events();
			exit(0);
		}
		else {
			fprintf(stderr, "INCORRECT REQUEST TYPE.\n");
			continue;
		}
	}
	
	return 0;
}

int main() {
	//This is Router process

	comm_channel comm[MAX_TAB];
	int controller = 1;
	
	int i, j;
	for (i = 0; i < MAX_TAB; i++) {
		comm[i].child_to_parent_fd[0] = -1;
		comm[i].child_to_parent_fd[1] = -1;
		comm[i].parent_to_child_fd[0] = -1;
		comm[i].parent_to_child_fd[1] = -1;
	}
	
	// Create a bi-directional pipe to controller.
	if (pipe(comm[0].parent_to_child_fd) == -1 || pipe(comm[0].child_to_parent_fd) == -1) {
		perror("ERROR DURING CREATING PIPE.");
		exit(-1);
	}
	
	int flags = fcntl(comm[0].child_to_parent_fd[0], F_GETFL, 0);
<<<<<<< HEAD
	fcntl(comm[0].child_to_parent_fd[0], F_SETFL, flags | O_NONBLOCK);
=======
	if(fcntl(comm[0].child_to_parent_fd[0], F_SETFL, flags | O_NONBLOCK) == -1){
		perror("ERROR DURING SETTING NON_BLOCK.\n");
		exit(-1);
	}
	flags = fcntl(comm[0].parent_to_child_fd[0], F_GETFL, 0);
	if(fcntl(comm[0].parent_to_child_fd[0], F_SETFL, flags | O_NONBLOCK) == -1){
		perror("ERROR DURING SETTING NON_BLOCK.\n");
		exit(-1);
	}
>>>>>>> e44317425a8c736e0f88fa00ba0de630cbe6a858
	
	// Fork controller process
	int control_pid = fork();
	
	if (control_pid < 0) {
		perror("ERROR DURING CREATING PROCESS.");
		exit(-1);
	}
	
	else if (control_pid > 0) {
		
		// Router process
		
		child_req_to_parent rcvd_req;
		
		while (controller) {  
			
<<<<<<< HEAD
			for (i = 0; i <= opened_tabs; i++) {
				
				if (comm[i].parent_to_child_fd[0] == -1 &&
					comm[i].parent_to_child_fd[1] == -1 &&
					comm[i].child_to_parent_fd[0] == -1 &&
					comm[i].child_to_parent_fd[1] == -1)
						
						continue;
<<<<<<< HEAD
				fprintf(stderr, "I got here\n");
=======
				//fprintf(stderr, "I got here\n");
			
>>>>>>> 427d126eed2aa8329a064978d968112f3f0e6892
				if (read(comm[i].child_to_parent_fd[0], &rcvd_req, sizeof(rcvd_req)) == -1 && errno == EAGAIN) {
=======
			for (i = 0; i < MAX_TAB; i++) {
			
				// If there's nothing to read, continue the loop
				if (read(comm[i].child_to_parent_fd[0], &rcvd_req, sizeof(rcvd_req)) == -1) { 
>>>>>>> e44317425a8c736e0f88fa00ba0de630cbe6a858
					usleep(1000);
					continue;
				}
				else {
					int tab_index = -1;
					
					switch (rcvd_req.type) {
						
						case CREATE_TAB:
<<<<<<< HEAD
<<<<<<< HEAD
							fprintf(stderr, "A new tab is created.");
							tab_index = rcvd_req.req.new_tab_req.tab_index;
=======
=======
						
							// Find the first unused tab index
							for (j = 1; j < MAX_TAB; j++) {
								if (comm[j].child_to_parent_fd[0] == -1 && comm[j].child_to_parent_fd[1] == -1) {
									tab_index = j;
									break;
								}
							}
>>>>>>> e44317425a8c736e0f88fa00ba0de630cbe6a858

							if (tab_index == -1) {
								perror("EXCEED MAX NUMBER OF TABS.\n");
								break;
							}
						
<<<<<<< HEAD
							fprintf(stderr, "A new tab is created.\n");
						
							tab_index = opened_tabs;
											
>>>>>>> 427d126eed2aa8329a064978d968112f3f0e6892
=======
							fprintf(stderr, "A NEW TAB IS CREATED.\n");
							
							// Create pipes for the tab	and set them no non-block
>>>>>>> e44317425a8c736e0f88fa00ba0de630cbe6a858
							if (pipe(comm[tab_index].parent_to_child_fd) == -1 || pipe(comm[tab_index].child_to_parent_fd) == -1) {
								perror("ERROR DURING CREATING PIPE.");
								exit(-1);
							}
<<<<<<< HEAD
=======
							flags = fcntl(comm[tab_index].child_to_parent_fd[0], F_GETFL, 0);
							if(fcntl(comm[tab_index].child_to_parent_fd[0], F_SETFL, flags | O_NONBLOCK) == -1) {
								perror("ERROR DURING SETTING NON_BLOCK.\n");
								exit(-1);
							}
							flags = fcntl(comm[tab_index].parent_to_child_fd[0], F_GETFL, 0);
							if(fcntl(comm[tab_index].parent_to_child_fd[0], F_SETFL, flags | O_NONBLOCK) == -1) {
								perror("ERROR DURING SETTING NON_BLOCK.\n");
								exit(-1);
							}
							
>>>>>>> 427d126eed2aa8329a064978d968112f3f0e6892
							int urltab_pid;
							if ((urltab_pid = fork()) < 0) {
								perror("ERROR DURING CREATING PROCESS.");
								exit(-1);
							}
							
							if (urltab_pid == 0) {
								
								//URL_RENDERING process							
								run_url_browser(tab_index, comm[tab_index]);
								exit(-1);
							}
							
							break;
							
						case NEW_URI_ENTERED:
						
							tab_index = rcvd_req.req.uri_req.render_in_tab;
							write(comm[tab_index].parent_to_child_fd[1], &rcvd_req, sizeof(rcvd_req));
							break;
							
						case TAB_KILLED:
							
							tab_index = rcvd_req.req.killed_req.tab_index;
							if (tab_index == 0) {
								
								// If controller is closed then close all tabs
								for (j = 1; j <= MAX_TAB; j++){
									if (comm[j].child_to_parent_fd[0] == -1 && comm[j].child_to_parent_fd[1] == -1)
										continue;
									write(comm[j].parent_to_child_fd[1], &rcvd_req, sizeof(rcvd_req));
									close(comm[j].child_to_parent_fd[0]);
									close(comm[j].child_to_parent_fd[1]);
								}
								controller = 0;
							}
<<<<<<< HEAD
							close(comm[tab_index].child_to_parent_fd[0]);
							close(comm[tab_index].child_to_parent_fd[1]);
=======
							else {
								write(comm[tab_index].parent_to_child_fd[1], &rcvd_req, sizeof(rcvd_req));
								close(comm[tab_index].child_to_parent_fd[0]);
								close(comm[tab_index].child_to_parent_fd[1]);
								comm[tab_index].child_to_parent_fd[0] = -1;
								comm[tab_index].child_to_parent_fd[1] = -1;
							}
							
>>>>>>> 427d126eed2aa8329a064978d968112f3f0e6892
							break;
							
						default:
							perror("INCORRECT REQUEST TYPE.\n");
							continue;
				
					}
				}
				waitpid(-1, NULL, WNOHANG);
			}
			
			
		}
		while(wait(NULL) > 0);
		
		exit(0);
	}
	
	else {
		
		// Controller process
		
		run_control(comm[0]);
		exit(0);
	}
	
	
	//This is Router process
	//Make a controller and URL-RENDERING tab when user request it. 
	//With pipes, this process should communicate with controller and tabs.

	//Remove this printf when submitting solution
	//printf("Please read the instruction and comments on source code provided for the project 2\n");
	//Insert code here!!
	//create pipe for communication with controller
	//Fork controller
	//poll for requests from child on one to many pipes
	//Use non-blocking read call to read data, identify the type of message and act accordingly
	//  CREATE_TAB:
	//	Create two pipes for bi-directional communication
	//	Fork URL_RENDERING process
	//  NEW_URI_ENTERED:
	//	Write this message on the pipe connecting to ROUTER and URL_RENDERING process.
	//  TAB_KILLED:
	//	Close file descriptors of corresponding tab's pipes.
	//When all child processes exit including controller, exit a success!
	//For more accurate details see section 4.1 in writeup.

	return 0;
}
