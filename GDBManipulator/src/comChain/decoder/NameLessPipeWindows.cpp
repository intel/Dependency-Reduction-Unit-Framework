/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#ifdef  _WIN32

#include <windows.h>

#define BUFSIZE 4096

#include <iostream>
#include "NameLessPipe.h"
#include "Utils.h"


using namespace std;


void nameLessPipe::writeToSTD_IN(char *buff, int size) {
    DWORD dwWritten;
    BOOL bSuccess = FALSE;

    bSuccess = WriteFile(g_hChildStd_IN_Wr, buff, size, &dwWritten, NULL);
    if (!bSuccess)
        Log::log("Failt to write to subprocess", CriticError);

    // Close the pipe handle so the child process stops reading.

}

int nameLessPipe::readFromSTD_OUT(char *buff, int maxSize) {
    DWORD dwRead;
    BOOL bSuccess = FALSE;
    bSuccess = ReadFile(g_hChildStd_OUT_Rd, buff, maxSize, &dwRead, NULL);
    if (!bSuccess || dwRead == 0) {
        return -1;
    }
    return (int) dwRead; // max BUFSIZE == 4k

}

int nameLessPipe::readFromSTD_ERR(char *buff, int maxSize) {
    DWORD dwRead;
    BOOL bSuccess = FALSE;
    bSuccess = ReadFile(g_hChildStd_ERR_Rd, buff, maxSize, &dwRead, NULL);
    if (!bSuccess || dwRead == 0) {
        return -1;
    }
    return (int) dwRead; // max BUFSIZE == 4k
}

void nameLessPipe::redirectSTD() {

}


void nameLessPipe::closeChild() {
    // noting to do her
}

void nameLessPipe::closeParent() {
    // noting to do her
}

processHander nameLessPipe::createSubProcess(string newProcess) {
    Log::log("Create subProcess " + newProcess, Info);
    PROCESS_INFORMATION ProcInfo;
    STARTUPINFO siStartInfo;
    BOOL forkSuccess = FALSE;
    // Set up members of the PROCESS_INFORMATION structure.
    ZeroMemory(&ProcInfo, sizeof(PROCESS_INFORMATION));
    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.hStdError = g_hChildStd_ERR_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
    Log::log("Create new windows sub process with: " + newProcess,Message);
    // Create the child process.
    forkSuccess = CreateProcess(NULL,
                                (char *) newProcess.c_str(),     // command line
                                NULL,          // process security attributes
                                NULL,          // primary thread security attributes
                                TRUE,          // handles are inherited
                                0,             // creation flags
                                NULL,          // use parent's environment
                                NULL,          // use parent's current directory
                                &siStartInfo,  // STARTUPINFO pointer
                                &ProcInfo);  // receives PROCESS_INFORMATION

    // If an error occurs, exit the application.
    if (!forkSuccess) {
        utils::cleanUpAndExit(-6);
    } else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.
        CloseHandle(ProcInfo.hProcess);
        CloseHandle(ProcInfo.hThread);
    }
    return ProcInfo.hProcess;
}


nameLessPipe::nameLessPipe() {
    SECURITY_ATTRIBUTES saAttr;
    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    // Create a pipe for the child process's STDOUT.

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        utils::cleanUpAndExit(-6);
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        utils::cleanUpAndExit(-6);
    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        utils::cleanUpAndExit(-6);
    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        utils::cleanUpAndExit(-6);
    if (!CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &saAttr, 0))
        utils::cleanUpAndExit(-6);
    if (!SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
        utils::cleanUpAndExit(-6);
}


#endif
