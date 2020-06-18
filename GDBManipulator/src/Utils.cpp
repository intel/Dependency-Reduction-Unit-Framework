/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include <vector>
#include <signal.h>
#include <cstring>
#include "main.h"
#include <thread>
#ifdef __linux__
#include <sys/mman.h>
#endif
#include "runModes/AbstractRunMode.h"
#ifdef _WIN32
#include <windows.h>


TCHAR szName[]=TEXT("gdbMann_IPC_SharedMem");

char * pBuf;
#endif

extern abstractRunMode *mode;
bool utils::startsWith(string *start, string with) {
    if (start->length() >= with.length()) {
        for (unsigned long i = 0; i < with.length(); i++) {
            if (start->at(i) != with.at(i)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

vector<processHander> childProzesses;


string generateFurtherInformationAboutExit(int code, int moreInfo = 0) {
	string s = " Terminate with error code :";
	if(code >0){
#ifdef __linux__
		s+=strsignal(code);
#else
        s+=to_string(code);
#endif
	}else{
		s+=to_string(code);
	}
	s +=  "\n\t";
    switch (code) {

        case -2:
            s += "Error while analysing gdb-client:Output stream";
            break;
        case -3:
            s += "Error while handling gdb-server\n\tOr the gdb server has not started properly, please try to add the entire server path.";
            break;
        case -4:
            s += "Error with the file or directory";
            break;
        case -5:
            s += "Missing at least one required argument. Those ar --client and -elf";
            break;
        case -6:
            s += "Failed to create Windows processes";
            break;
        case -7:
            s += "Error while handling gdb-client";
            break;
        case -8:
            s += "Failed to send a command via GDB. Check connection or communication";
            break;
        case -9:
            s += "Error with the file or directory. Possibel reason is an not existing elf file";
            break;
        case -10:
            s += "Failed in test execution. Possible reason might be a target reset";
            break;
        case -11:
        	s+= "The target has send the command to terminate the gdb connection. \n\tOr the gdb client has not started properly, please try to add the entire client path.";
        	break;
        case -12:
			s+= "Failed to evaluate the Target_Mode. It is possible that the mode has been opt out by the compiler. This is a linker bug!"
					"\n\tPlease check \"https://gcc.gnu.org/bugzilla/show_bug.cgi?id=43038\""
					"\n\tTo bypass this bug it is possible to use this variable:"
					"\n\t\t\t extern volatile const uint8_t EUNIT_TARGET_MODE;"
					"\n\t\t\t volatile uint8_t EUNIT_TARGET_MODE_LTO = EUNIT_TARGET_MODE;"
					"\n\n\t It also could be possible the functions: \"detachTarget\" and \"printOverGdb\" as been opt out. "
					"\n\t Its suffice to refer the functions in volatile pointers or to disable the LTO.\n\n";
        	break;
        case -13:
		   s += "Error while analysing gdb-client:Error stream";
		   break;
        case -14:
            s+= "eUnit Version mismatch" + to_string((featureReturn)moreInfo);
            break;

    }
    if(code <-1)
    s += "\n\t\tFor further information's see:\t \"https://github.com/intel/Dependency-Reduction-Unit-Framework/wiki\"\n";
    return s;
}

void utils::killProcess(processHander pid) {
#ifdef __linux__
    Log::log("Close child  process: " + to_string(pid), Info, logFilter::GDB_Interface);
    if (kill(pid, SIGKILL) != 0) {
        Log::log("Failed to send \"SIGKILL\"command to pid: \"" + to_string(pid) +"\"", CriticError);
    }
#elif _WIN32
    Log::log("kill child ", Info, logFilter::GDB_Interface);
    for (unsigned int i = 0; i < childProzesses.size(); i++) {
        TerminateProcess(childProzesses.at(i), 9);
    }

#endif
}

#ifdef __linux__
#elif __WIN32
sig_atomic_t firstExit = false;
#endif
void utils::alarmThread(int code){
    Log::log("start alarm timer",Info);
    sleep(3);
	char toShare[] = "true";
	utils::sharedMemoryWrite(toShare, 4); // close com chains
	Log::log("failed to evaluate last buffer",Message);
	 for (unsigned int i = 0; i < childProzesses.size(); i++) {
	        killProcess(childProzesses.at(i));
	}
	exit(-12);
}

void utils::cleanUpAndExit(int code, int additionalInfo) {
    Log::log("caught signal "+to_string(code),Info );
	if(code > 0){
        new thread(alarmThread,0);
		mode->modeFailed();
		Log::log("mode failed save done",Message);
	}
    char toShare[] = "true";
    utils::sharedMemoryWrite(toShare, 5); // close com chains
    usleep(100000);
    for (unsigned int i = 0; i < childProzesses.size(); i++) {
        killProcess(childProzesses.at(i));
    }
    if (code != 0) {
        cout << generateFurtherInformationAboutExit(code,additionalInfo) << endl;
    }
    fflush(stdout);
    exit(code);
}

void utils::addChildProzess(processHander pid) {
    childProzesses.push_back(pid);
}

void utils::deleteChildProcess(processHander pid) {
    int index = -1;
    for (unsigned int i = 0; i < childProzesses.size(); i++) {
        if (childProzesses.at(i) == pid) {
            index = i;
        }
    }
    if (index > 0) {
        childProzesses.erase(childProzesses.begin() + index);
    }
}

void *shmem;

void utils::sharedMemoryInit() {
#ifdef __linux__
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_ANONYMOUS | MAP_SHARED;

    shmem = mmap(NULL, 128, protection, visibility, 0, 0);
#elif __WIN32
    HANDLE hMapFile;
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            128,                // maximum object size (low-order DWORD)
            szName);                 // name of mapping object

    pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
              FILE_MAP_ALL_ACCESS, // read/write permission
              0,
              0,
              128);
    if (pBuf == NULL)
    {
        Log::log("failed to init shared memory",CriticError);
        CloseHandle(hMapFile);
        cleanUpAndExit(-1);
    }
#else
    Log::log("IPC for thos system is not supported. This may result in some Error messages",Error);
#endif

}

void utils::sharedMemoryWrite(char *buff, int size) {
#ifdef __linux__
    memcpy_s(shmem,128, buff, size);
#elif __WIN32
    HANDLE hMapFile;
    LPCTSTR pBuf;
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            128,                // maximum object size (low-order DWORD)
            szName);                 // name of mapping object
    if (hMapFile == NULL)
    {
        Log::log("failed to open shared memory",CriticError);
        cleanUpAndExit(-1);
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
      FILE_MAP_ALL_ACCESS,  // read/write permission
      0,
      0,
      128);
    if (pBuf == NULL){
        Log::log("failed to open shared memory",CriticError);
        cleanUpAndExit(-1);
    }
    CopyMemory((PVOID)pBuf, buff, size);
    CloseHandle(hMapFile);

#else
    Log::log("IPC for thos system is not supported. This may result in some Error messages",Error);
#endif
}

char *utils::sharedMemoryRead() {
#ifdef __linux__
    return (char *) shmem;
#elif __WIN32
    HANDLE hMapFile;
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            128,                // maximum object size (low-order DWORD)
            szName);                 // name of mapping object
    if (hMapFile == NULL)
    {
        Log::log("failed to open shared memory",CriticError);
        cleanUpAndExit(-1);
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0,
                                  0,
                                  128);
    if (pBuf == NULL){
        Log::log("failed to open shared memory",CriticError);
        cleanUpAndExit(-1);
    }
    CloseHandle(hMapFile);
    return pBuf;

#endif
    Log::log("IPC for thos system is not supported. This may result in some Error messages",Error);
   return nullptr;
}

void utils::cleanUpAndExit(int code) {
    cleanUpAndExit(code,0);
}
