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

char** transformargv(char** original) {
	int count = 0;
	while (* (original + count) != NULL) {
		count++;
	}

	char** newargv = (char**) malloc ((count + 1) * sizeof(char*));
	for (int i = 0; i < count; i++) {
		*(newargv + i) = strdup(original[i]);
	}
	*(newargv + count) = NULL;
	return newargv;
}

void old_refreshStatus() {
	for (int i = 0; i < 10; i++) {	
		if (depTree[i].status == READY || depTree[i].status == FINISHED) continue;
		depTree[i].status = READY;			
		if (depTree[i].dep[0].name == NULL) {					
			continue;
		}
		for (int j = 0; j < 10; j++) {
			if ((depTree[i].dep[j].name != NULL) && (depTree[depTree[i].dep[j].index].status != FINISHED)){
				depTree[i].status = INELIGIBLE;
			}
		}
	}
}

void refreshStatus(int timed) {
	for (int i = 0; i < 10; i++) {

		//if target is ready or finished don't change anything
		if (depTree[i].status == READY || depTree[i].status == FINISHED) continue;

		if (depTree[i].name == NULL) {
			depTree[i].status = INELIGIBLE;
			continue;
		}

		//if target mod_time > any dependency then target is FINISHED
		if (timed && depTree[i].status != FINISHED) {
			int finished = 1;	
			for (int j = 0; j < 10; j++) {
				if ((depTree[i].dep[j].name != NULL) && !(compare_modification_time(depTree[i].name, depTree[i].dep[j].name) == 1))
					finished = 0;
			}
			if (finished){
				depTree[i].status = FINISHED;
				continue;
			}
		}

		
		if (depTree[i].dep[0].name == NULL) {
			depTree[i].status = READY;
			continue;
		}

		//if all dependencies exist then target is READY
		int allExist = 1;
		for (int j = 0; j < 10; j++) {
			if ((depTree[i].dep[j].name != NULL) && (is_file_exist(depTree[i].dep[j].name) == -1) && (depTree[depTree[i].dep[j].index].status != FINISHED))
				allExist = 0;
		}
		if (allExist) {
			depTree[i].status = READY;
			continue;
		}

		//otherwise INELIGIBLE
		depTree[i].status = INELIGIBLE;

	}
}

void execute(target_t* target){

	for (int i = 0; i < 10; i++) {
		if ((target->dep[i].name != NULL) && (target->status == INELIGIBLE))
			execute(depTree+(target->dep[i].index));
	}
	refreshStatus(1);

	if (target->status == READY) {
		int pid = fork();
		int status = 0;
		if (pid == 0) { 	//child process
			if (target->cmd != NULL) {
				fprintf(stderr, "%s\n", target->cmd);
				execvp(target->argv[0], transformargv(target->argv));

			//If child gets to this point it must have failed.
			fprintf(stderr, "Command failed to execute.\n" );
			exit(-1);}
			else exit(0);
		}
		else if (pid > 0) {		//parent process
			target->status = RUNNING;
			waitpid(pid, &status, WNOHANG);
			if (status == -1) exit(EXIT_FAILURE);
			target->status = FINISHED;
			refreshStatus(1);			
		}
		else {
			fprintf(stderr, "Fork failed.\n");
			exit(EXIT_FAILURE);
		}
	}

}


//This function will parse makefile input from user or default makeFile. 
int parse(char * lpszFileName)
{

	int nLine = 0;
	int lkupIndex = -1;
	char szLine[1024];
	char * lpszLine;
	FILE * fp = file_open(lpszFileName);
	char* tempStr;
	char* targetLkup[10];

	

	if(fp == NULL)
	{
		return -1;
	}

	while(file_getline(szLine, fp) != NULL) 
	{
		
		nLine++;
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
			else if (!isalpha(*lpszLine)) exit(-1);

			if  (strstr(lpszLine, ":") == NULL) exit(-1);
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
			else if (!isalpha(*lpszLine)) exit(-1);
			depTree[lkupIndex].cmd = strdup(lpszLine);
			depTree[lkupIndex].argv[0]= strdup(strtok(lpszLine, " "));
			int i = 1;
			while ((tempStr = strtok(NULL, " ")) != NULL) {
				depTree[lkupIndex].argv[i] = strdup(tempStr);
				i++;
			}
			
			
		}
		
		
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 10; k++) {
				if ((depTree[i].dep[j].name != NULL) && (targetLkup[k] != NULL) && (strcmp(depTree[i].dep[j].name, targetLkup[k]) == 0))
					depTree[i].dep[j].index = k;
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((depTree[i].dep[j].name != NULL) && (depTree[i].dep[j].index == -1) && (is_file_exist(depTree[i].dep[j].name) == -1)) {
				fprintf(stderr, "bogus dependency\n" );
				fprintf(stderr, "i = %d, j = %d\n",i,j);
				exit(-1);
			}

		}
	}



	//Close the makefile. 
	fclose(fp);

	return 0;
}



int main(int argc, char** argv){

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

	parse("tc6");



	refreshStatus(1);
	

	for (int i = 0; i < 10; i++) {
		if (depTree[i].name != NULL) printf("depTree[%d].name: %s\n", i, depTree[i].name);
		if (depTree[i].cmd != NULL) printf("depTree[%d].cmd: %s\n", i, depTree[i].cmd);
		printf("depTree[%d].status: %d\n", i, depTree[i].status);
		for (int j = 0; j < 10; j++) {
			if (depTree[i].argv[j] != NULL) printf("depTree[%d].argv[%d]: %s\n", i, j, depTree[i].argv[j]);
			if (depTree[i].dep[j].name != NULL) printf("depTree[%d].dep[%d].name: %s\n", i, j, depTree[i].dep[j].name);
			if (depTree[i].dep[j].index != -1) printf("depTree[%d].dep[%d].index: %d\n", i ,j, depTree[i].dep[j].index);
		}
	}

	//char* args[] = {"-a", "bc", "def", NULL, NULL, NULL};

	//char** newargv = transformargv(args);

	execute(&depTree[0]);
	sleep(1);	

	return 0;
}