/* CSci 4061 F2014 Assignment 1
 * Login: jiang481
 * Date: 10/01/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <ctype.h>

#include "util.h"

target_t depTree[10];
int fd;

/*
 *  isTarget check if certain name is a target
 */
int isTarget(char* name) {
	for (int i = 0; i < 10; i++) {
		if ((depTree[i].name != NULL) && (strcmp(name, depTree[i].name) == 0))
			return 1;
	}
	return 0;
}

/*
 *  parse will parse makefile input from user or default makeFile. 
 *  lpszFileName is the name of the file that needs to be parsed.
 *  Successful parsing will return 0, otherwise -1.
 */
int parse(char * lpszFileName)
{

	int nLine = 0;
	int lkupIndex = -1;
	char szLine[1024];
	char * lpszLine;
	FILE * fp = file_open(lpszFileName);
	char* tempStr;
	char* targetLkup[10];

	for (int i = 0; i < 10; i++) {
		targetLkup[i] = NULL;
	}

	

	if(fp == NULL)
	{
		return -1;
	}

	while(file_getline(szLine, fp) != NULL) 
	{
		
		nLine++;
		printf(szLine);
		// this loop will go through the given file, one line at a time
		// this is where you need to do the work of interpreting
		// each line of the file to be able to deal with it later

		//Remove newline character at end if there is one
		lpszLine = strtok(szLine, "\n"); 
		
		//You need to check below for parsing. 
		//Skip if blank or comment.
		//Remove leading whitespace.
		//Skip if whitespace-only.
		//Only single command is allowed.
		//If you found any syntax error, stop parsing. 
		//If lpszLine starts with '\t' it will be command else it will be target.
		//It is possbile that target may not have a command as you can see from the example on project write-up. (target:all)
		//You can use any data structure (array, linked list ...) as you want to build a graph

		if ((lpszLine == NULL) || (*lpszLine == '#')) continue;		//skip blank lines or comments
		//printf("%s\n", lpszLine);
		if (*lpszLine != '\t') {	//target line parsing

			lkupIndex++;

			while (*lpszLine == ' ') lpszLine++;
			if (*lpszLine == 0) continue;
			else if (!isalpha(*lpszLine)) return -1;

			if  (strstr(lpszLine, ":") == NULL) {
				fprintf(stderr, "Syntax error.\n" );
				return -1;
			}
			depTree[lkupIndex].name = strdup(strtok(lpszLine, ":"));
			
			int i = 0;
			while ((tempStr = strtok(NULL, " ")) != NULL) {
				depTree[lkupIndex].dep[i].name = strdup(tempStr);
				i++;
			}
			targetLkup[lkupIndex] = depTree[lkupIndex].name;

		} else {	//command line parsing


			lpszLine++;
			while (*lpszLine == ' ') lpszLine++;
			if (*lpszLine == 0) continue;
			else if (!isalpha(*lpszLine)) return -1;
			depTree[lkupIndex].cmd = strdup(lpszLine);
			depTree[lkupIndex].argv[0]= strdup(strtok(lpszLine, " "));
			int i = 1;
			while ((tempStr = strtok(NULL, " ")) != NULL) {
				depTree[lkupIndex].argv[i] = strdup(tempStr);
				i++;
			}
			
			
		}		
	}


	//set up dependency relation
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 10; k++) {
				if ((depTree[i].dep[j].name != NULL) && (targetLkup[k] != NULL) && (strcmp(depTree[i].dep[j].name, targetLkup[k]) == 0))
					depTree[i].dep[j].index = k;
			}
		}
	}


	//Close the makefile. 
	fclose(fp);

	return 0;
}

void show_error_message(char * lpszFileName)
{
	fprintf(stderr, "Usage: %s [options] [target] : only single target is allowed.\n", lpszFileName);
	fprintf(stderr, "-f FILE\t\tRead FILE as a maumfile.\n");
	fprintf(stderr, "-h\t\tPrint this message and exit.\n");
	fprintf(stderr, "-n\t\tDon't actually execute commands, just print them.\n");
	fprintf(stderr, "-B\t\tDon't check files timestamps.\n");
	fprintf(stderr, "-m FILE\t\tRedirect the output to the file specified .\n");
	exit(0);
}

/*
 *  refreshes status of all targets
 *  timed = 1 when timestamp should be checked, otherwise zero
 */
void refreshStatus(int timed) {		
	for (int i = 0; i < 10; i++) {


		//if target is ready or finished don't change anything
		if (depTree[i].status == READY || depTree[i].status == FINISHED) continue;

		//if target doesn't exist target is INELIGIBLE
		if (depTree[i].name == NULL) {
			depTree[i].status = INELIGIBLE;
			continue;
		}

		//if target has dependency but no command target is INELIGIBLE
		if ((depTree[i].dep[0].name != NULL) && (depTree[i].cmd == NULL)){
			depTree[i].status = INELIGIBLE;
			continue;
		}

		//if target has no dependency target is READY
		if (depTree[i].dep[0].name == NULL) {
			depTree[i].status = READY;
			continue;
		}

		//if target mod_time > any dependency then target is FINISHED
		if (timed && depTree[i].status != FINISHED) {
			int finished = 1;	
			for (int j = 0; j < 10; j++) {
				if ((depTree[i].dep[j].name != NULL) && (compare_modification_time(depTree[i].name, depTree[i].dep[j].name) != 1))
					finished = 0;
			}
			if (finished){
				depTree[i].status = FINISHED;
				continue;
			}
		}


		//if all dependencies exist then target is READY
		int allExist = 1;
		if (timed) {
			for (int j = 0; j < 10; j++) {
				if ((depTree[i].dep[j].name != NULL) && (isTarget(depTree[i].dep[j].name)) && (is_file_exist(depTree[i].dep[j].name) == -1) && (depTree[depTree[i].dep[j].index].status != FINISHED))
					allExist = 0;
			}
		}
		else {
			for (int j = 0; j < 10; j++) {
				if ((depTree[i].dep[j].name != NULL) && (isTarget(depTree[i].dep[j].name)) && (depTree[depTree[i].dep[j].index].status != FINISHED))
					allExist = 0;
			}
		}
		if (allExist) {
			depTree[i].status = READY;
			continue;
		}



		//otherwise INELIGIBLE
		depTree[i].status = INELIGIBLE;

	}
}

/*
 *  execute runs the tree built in parse
 *  target is the root of the tree that needs to run
 *  sverbose = 1 when commands are displayed but not executed, otherwise 0
 *  timed = 1 when timestamp should be checked, otherwise 0
 */

void execute(target_t* target, int sverbose, int timed){

	//if target is ineligible recursively resolve its dependencies
	for (int i = 0; i < 10; i++) {
		if ((target->dep[i].name != NULL) && (target->status == INELIGIBLE))
			execute(depTree+(target->dep[i].index), sverbose, timed);
	}
	
	for (int j = 0; j < 10; j++) {
			if ((target->dep[j].name != NULL) && (target->dep[j].index == -1) && (is_file_exist(target->dep[j].name) == -1)) {
				fprintf(stderr, "make4061: Can't resolve dependency %s: file doesn't exist.\n", target->dep[j].name);
				exit (EXIT_FAILURE);
			}
	}
	
	refreshStatus(timed);

	if (target->status == READY) {		//only execute command when target is READY
		int pid = fork();
		int status = 0;
		if (pid == 0) { 	
			//child process
			if (target->cmd != NULL && !sverbose) {
				fprintf(stderr, "make4061: %s\n", target->cmd);
				execvp(target->argv[0], transformargv(target->argv));

				//If child gets to this point then something went wrong.
				fprintf(stderr, "make4061: Command failed to execute.\n" );
				exit(-1);
			}
			else if (target->cmd != NULL){
				fprintf(stderr, "%s\n", target->cmd);
				exit(0);
			}
			else exit(0);	
		}
		//parent process
		else if (pid > 0) {	
			target->status = RUNNING;
			waitpid(pid, &status, 0);
			if (status == -1) exit(EXIT_FAILURE);
			target->status = FINISHED;
			refreshStatus(timed);			
		}
		else {
			fprintf(stderr, "make4061: Fork failed.\n");
			exit(EXIT_FAILURE);
		}
	} else if (target->status == FINISHED){
			fprintf(stderr, "make4061: %s is up to date.\n", target->name);
	}

}


int main(int argc, char **argv) 
{

	//initialize depTree
	for (int i = 0; i < 10; i++) {	
		depTree[i].name = NULL;
		depTree[i].cmd = NULL;
		depTree[i].status = INELIGIBLE;
		for (int j = 0; j < 10; j++){
			depTree[i].argv[j] = NULL;
			depTree[i].dep[j].name = NULL;
			depTree[i].dep[j].index = -1;
		}
	}

	// Declarations for getopt
	extern int optind;
	extern char * optarg;
	int ch;
	char * format = "f:hnBm:";
	
	// Default makefile name will be Makefile
	char szMakefile[64] = "Makefile";
	char szTarget[64];
	char szLog[64];
	int fflag = 0, nflag = 0, bflag = 0, mflag = 0;

	while((ch = getopt(argc, argv, format)) != -1) 
	{
		switch(ch) 
		{
			case 'f':
				fflag = 1;
				strcpy(szMakefile, strdup(optarg));
				break;
			case 'n':
				nflag = 1;
				break;
			case 'B':
				bflag = 1;
				break;
			case 'm':
				mflag = 1;
				strcpy(szLog, strdup(optarg));
				break;
			case 'h':
			default:
				show_error_message(argv[0]);
				exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	// at this point, what is left in argv is the targets that were 
	// specified on the command line. argc has the number of them.
	// If getopt is still really confusing,
	// try printing out what's in argv right here, then just running 
	// with various command-line arguments.

	//if -m switch is on redirect stderr
	if (mflag){
		fd = open(szLog, O_WRONLY|O_CREAT, S_IRWXU|S_IRGRP|S_IROTH);
		if (fd > 0){
			dup2(fd, 2);
		}
		else {
			fprintf(stderr, "make4061: Failed to open log file.\n" );
			return EXIT_FAILURE;
		}
	}

	if(argc > 1)
	{
		show_error_message(argv[0]);
		return EXIT_FAILURE;
	}

	/* Parse graph file or die */
	if((parse(szMakefile)) == -1) 
	{
		return EXIT_FAILURE;
	}




	//You may start your program by setting the target that make4061 should build.
	//if target is not set, set it to default (first target from makefile)
	if(argc == 1)
	{				
		strcpy(szTarget, strdup(*argv));

		//find index of specified target
		int index = -1;	
		for (int i = 0; i < 10; i++) {
			if ((depTree[i].name != NULL) && (strcmp(depTree[i].name, szTarget) == 0))
				index = i;
		}
		if (index == -1) {
			fprintf(stderr, "make4061: Target not found.\n" );
			exit(EXIT_FAILURE);
		}
		else {
			refreshStatus(!bflag);
			execute(&depTree[index], nflag, !bflag);
		}
	}
	else
	{
		refreshStatus(!bflag);
		execute(&depTree[0], nflag, !bflag);
	}

	sleep(1);

	//after parsing the file, you'll want to check all dependencies (whether they are available targets or files)
	//then execute all of the targets that were specified on the command line, along with their dependencies, etc.
	return EXIT_SUCCESS;
}
