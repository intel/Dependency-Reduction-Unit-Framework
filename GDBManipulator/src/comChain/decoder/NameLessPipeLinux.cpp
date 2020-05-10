/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifdef __linux__

#include "NameLessPipe.h"


#define READ_FD  0
#define WRITE_FD 1


#define PARENT_WRITE_PIPE   0
#define PARENT_READ_PIPE    1
#define PARENT_ERR_PIPE     2

#define PARENT_READ_FD      ( pipes[PARENT_READ_PIPE][READ_FD]   )
#define PARENT_WRITE_FD     ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )
#define PARENT_READ_ERR_FD  ( pipes[PARENT_ERR_PIPE][READ_FD]   )

#define CHILD_READ_FD       ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
#define CHILD_WRITE_FD      ( pipes[PARENT_READ_PIPE][WRITE_FD]  )
#define CHILD_WRITE_ERR_FD  ( pipes[PARENT_ERR_PIPE][WRITE_FD]  )

using namespace std;


void nameLessPipe::writeToSTD_IN(char *buff, int size) {
    if(write(PARENT_WRITE_FD, buff, size)<size){
    	Log::log("Failed to write pipe",Info);
    }
}

int nameLessPipe::readFromSTD_OUT(char *buff, int maxSize) {
    return read(PARENT_READ_FD, buff, maxSize);

}

int nameLessPipe::readFromSTD_ERR(char *buff, int maxSize) {
    int s = read(PARENT_READ_ERR_FD, buff, maxSize);
    return s;
}

void nameLessPipe::redirectSTD() {
    if (dup2(CHILD_WRITE_FD, STDOUT_FILENO) < 0) // redirect stdout to the pipe
    {
    	;
    }
    if (dup2(CHILD_READ_FD, STDIN_FILENO) < 0) // redirect stdout to the pipe
    {
    	;
    }
    if (dup2(CHILD_WRITE_ERR_FD, STDERR_FILENO) < 0) // redirect stdout to the pipe
    {
    	;
    }
}


void nameLessPipe::closeChild() {
    close(PARENT_READ_FD);
    close(PARENT_WRITE_FD);
}

void nameLessPipe::closeParent() {
    /* close fds not required by parent */
    close(CHILD_READ_FD);
    close(CHILD_WRITE_FD);
}


nameLessPipe::nameLessPipe() {
    if( (pipe((pipes[2])) |  (pipe(pipes[0]))   | (pipe(pipes[1]))) == 1){
    	Log::log("Failed to create pipes",Info);
    }

}

processHander nameLessPipe::createSubProcess(string newProcess) {
	Log::log("Create process " +newProcess,Info);
	int pid = 0;
    if((pid=fork()) == 0){
    	if(setsid()<0){
    		Log::log("failed to detach child process",Error);
    	}
    }
    return pid;
}

#endif
