/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */



#ifndef GDBMANIPULATOR_NAMELESSPIPE_H
#define GDBMANIPULATOR_NAMELESSPIPE_H


#include <unistd.h>
#include <iostream>

#include "logging.h"

using namespace std;

#ifdef __linux__
typedef int processHander;
#elif _WIN32

#include <windows.h>

typedef HANDLE processHander;
#endif

/**
 * Child to Parent Communication.
 * this class redirect the STD_OUT/IN/ERR to an nameless Pipe.
 * The parent process can read or write to the stdOut/In/Err of the child-process.
 *
 * Usage:
 *      share this obj with the parent and the chiled process and call redirectSTD() from the child to redirect
 *      std_out/in/err to a nameless pipe which can be readed by the parent process by:
 *          - writeToSTD_IN
 *          - readFromSTD_OUT
 *          - readFromSTD_ERR
 *
 *     e.g.
 *
 *  nameLessPipe *nlp = new nameLessPipe();
 *  if(fork() == 0){
 *      nlp->redirectSTD();
 *      nlp->closeChild();
 *      cout << "gets written to namelessPipe"<<endl;
 *      char buff[20]
 *      int s= read(STDIN_FILENO,buff,20); // read from stdIn
 *      write(STDOUT_FILENO,buff,s); // echo
 *  }else{
 *      nlp->closeParent();
 *      char buff1[20];
 *      nlp->readFromSTD_OUT(buff1,20);
 *      cout << "read : "<< buff1<<endl;
 *      char buff2[20];
 *      nlp->writeToSTD_IN("hallo child\n",12); // write to child's stdIN
 *      nlp->readFromSTD_OUT(buff1,20);
 *      cout << nlp->readFromSTD_ERR(err,20)<<endl;
 *      cout << "read : "<< buff1<<endl;
 *  }
 *
 */
class nameLessPipe {

#ifdef __linux__
    /**
     * [stdeOut,In,Err][read/write]
     */
    int pipes[3][2];
#elif _WIN32
    HANDLE g_hChildStd_IN_Rd = NULL;
    HANDLE g_hChildStd_IN_Wr = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;
    HANDLE g_hChildStd_ERR_Rd = NULL;
    HANDLE g_hChildStd_ERR_Wr = NULL;
#endif

public:
    /**
     * create pipes shared with the chiled process.
     */
    nameLessPipe();;

    /**
     * write to the childs STDIN.
     * Call from parent.
     */
    void writeToSTD_IN(char *buff, int size);

    /**
    * read from childs STDERR.
    * Call from parent.
    */
    int readFromSTD_ERR(char *buff, int maxSize);

    /**
    * read line from childs STDERR.
    * Call from parent.
    */
    string readFromSTD_ERR_Line() {

        string s = "";
        char c;
        do {
            readFromSTD_ERR(&c, 1);
            if (c != '\n')
                s += c;
        } while (c != '\n');

        return s;
    }

    /**
    * read from childs STDOUT.
    * Call from parent.
    */
    int readFromSTD_OUT(char *buff, int maxSize);

    /**
    * read line from childs STDOUT.
    * Call from parent.
    */
    string readFromSTD_OUT_Line() {
        string s = "";
        char c;
        do {
            int l;
            if ((l = readFromSTD_OUT(&c, 1)) > 0 && c != '\n')
                s += c;
            if (l < 0)
                Log::log("failed to read from STDOUT", Error);
        } while (c != '\n');

        return s;
    }

    /**
     * redirect stdout/err/in from the current process.
     * called by the child process.
     */
    void redirectSTD();


    /**
     * close not needed file descriptors by the chiled.
     */
    void closeChild();

    /**
     * close not needed file descriptors by the parent.
     */
    void closeParent();

    /**
     * OS independent fork function
     */
    processHander createSubProcess() {
        return createSubProcess("");
    }

    /**
    * OS independent fork function
     * add param to the arg
    */
    processHander createSubProcess(string newProcess);
};


#endif //GDBMANIPULATOR_NAMELESSPIPE_H
