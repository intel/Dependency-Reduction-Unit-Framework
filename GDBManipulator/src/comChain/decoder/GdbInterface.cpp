/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "GdbInterface.h"
#include <thread>
#include "runModes/TestMode.h"
#include <cstring>
#include <algorithm>

//#define MAKE_SYSTEM_TEST

extern bool createStatistic;

static string callServer = "";
static string callClient = "";

/**
 * coppy an array
 */
char * coppyString(const char * toCoppy)
{
    int size = strlen(toCoppy)+1;
    char * out = static_cast<char *>(malloc(size));
    memset(out,0,size);
    memcpy(out,toCoppy,size);
    return out;
}

/**
 * build argument array for execv
 * @param applicationCallString user defined string to call the application
 * @param amountOfAdditionalArguments  amount of additional arguments
 * @param additionalArguments additional arguments
 * @return null terminatig array of argumetns
 */
char ** buildArgv(string applicationCallString, int amountOfAdditionalArguments, char *additionalArguments[] )
{
    int outSize = 1; // \0 at the end
    outSize += count(applicationCallString.begin() ,applicationCallString.end(),' ' );
    outSize += amountOfAdditionalArguments;
    char ** out = static_cast<char**> (malloc(outSize * sizeof(char*)));
    memset(out,0,outSize * sizeof(char*));
    int indexInOut = 0;
    int subIndex = 0;
    while(applicationCallString.length() >0 && (subIndex = applicationCallString.find(' ')) != string::npos)
    {
        if(applicationCallString.at(0) != ' ')
        {
            string subString = applicationCallString.substr(0,subIndex);
            out[indexInOut] = coppyString(subString.c_str());
            indexInOut++;
            //cout << subString <<endl;
        }
        applicationCallString.erase(0,subIndex+1);

    }
    if(applicationCallString.length() >0){
		out[indexInOut] = coppyString(applicationCallString.c_str());
		indexInOut++;
    }
    for(int i = 0; i<amountOfAdditionalArguments && additionalArguments[i] != nullptr;i++)
    {
        out[indexInOut] = coppyString(additionalArguments[i]);
        indexInOut++;
    }
    return out;

}

/**
 * thread function gdbStdERR.
 * this function handles the gdb STDERR of the gdb client
 * @param gdbErr PipeReader
 */
static void handleError(gdbErrorStream *gdbErr) {
    gdbErr->run();
    Log::log("\n\nThe GDB-Client(handleSTDOUT) has returend unexpectet. Please check if its work properly! \n ", Error, logFilter::GDB_Interface);
    utils::cleanUpAndExit(-13);

}

/**
 * thread function gdbStdOut.
 * this function handles the gdb STDOUT of the gdb client
 * @param gdbOut PipeReader
 */
static void handleOut(gdbOutputStream *gdbOut) {

    gdbOut->run();
    Log::log("\n\nThe GDB-Client(handleSTDOUT) has returend unexpectet. Please check if its work properly! \n", Error);
    usleep(10000);
    utils::cleanUpAndExit(-2);
}

/**
 * thread function.
 * start the gdb cleint in mi2 mode
 * @param command command to start the gdb client
 */
/**
 * thread function.
 * start the gdb cleint in mi2 mode
 * @param command command to start the gdb client
 */
static void handleGDB(string command, nameLessPipe *nlp, string elfLoc, int port) {
    processHander pid;
    char mi2[] = "--interpreter=mi2";
    char qiet[] = "-q";
    char port_v[20];
    sprintf(port_v, "%d", port);

    char elf_[elfLoc.length() + 1] = {0,};

    for (unsigned int i = 0; i < elfLoc.length(); i++) {
        elf_[i] = elfLoc.at(i);
    };
    char *argsUserDef[] = {qiet, mi2, elf_, NULL};
    char ** argv = buildArgv(command,4,argsUserDef);
    string call = "";
    int i = 0;
    while(argv[i] != NULL) {
        call += string(argv[i]) + " ";
        i++;
    }
    Log::log(("Call client with : \t\"" +call+"\""),Message,logFilter::GDB_Client);
    callServer = command;
#ifdef __linux__
    if ((pid = nlp->createSubProcess()) == 0) {
        nlp->redirectSTD();
        if(execv(argv[0], argv) == -1) {
            Log::log("GDB-Client exit with errno: \t\"" + string(strerror(errno))+ "\" \nI my be caused by a incorrect client. Please try again the entire application path  ", CriticError);
        }
        //TODO
        //sleep(1);
        char toShare[] = "true\0";
        utils::sharedMemoryWrite(toShare, 4);
        sleep(1);
        Log::log("Gdb client has been terminated unexpected",CriticError); // terminate exeuction
        utils::deleteChildProcess(getpid());
        utils::cleanUpAndExit(-7);
    } else {
        utils::addChildProzess(pid);
    }
#elif _WIN32
    pid = nlp->createSubProcess(call);
    utils::addChildProzess(pid);
#endif


}


static void handleServer(nameLessPipe *nlp) {
    bool notJetFailed = true;
#ifdef MAKE_SYSTEM_TEST
    ofstream createSystemTest("systemTest.Server");
#endif
    while (notJetFailed) {
        string l = nlp->readFromSTD_OUT_Line();
#ifdef MAKE_SYSTEM_TEST
        createSystemTest <<l.c_str()<<endl;
#endif
        if (l != "") {
            if (utils::startsWith(&l, "Waiting for connection")) {
                Log::log("GDB-Server : handle server wait for connection .. ", Message, logFilter::GDB_Server);
            } else if (utils::startsWith(&l, "Accepted connection")) {
                Log::log("GDB-Server : gDB server has accepted connection", Message, logFilter::GDB_Server);
            } else if (utils::startsWith(&l,
                                         "Reason: USB communication error. Please reconnect the ST-Link USB cable and try again")) {
                Log::log("GDB-Server : USB communication error. Please reconnect the ST-Link USB cable and try again",
                		Error, logFilter::GDB_Server);
                notJetFailed = false;
            } else if (l.find("error while loading shared libraries:") != string::npos) {
                Log::log("GDB-Server : failed to load some libraries. Please check the installed GDB-Servern",
                         CriticError);
                notJetFailed = false;
            } else if (l.find("The GDB-Server(callGdbServer) has returned unexpected") != string::npos) {
                Log::log("\n\nThe GDB-Server(callGdbServer) has returned unexpected."
                         "\n\tPlease check if the gdb client and server are work properly!"
                         "\n\tTry to start the sever and the client local with : "
                         "\n\t\t \"" + callServer + "\"\n\t\t\"" + callClient + "\"\n\t and check the connection", Error, logFilter::GDB_Server);
                notJetFailed = false;
            } else if (l.find("Error in initializing") != string::npos) {
                string reasone = nlp->readFromSTD_OUT_Line();
                if (reasone.find("Error in initializing ST-Link device")) {
                    reasone += "\n\t possible solution: \n\t - re/connect target"
                               "\n\t - check if target has been mounted properly\n";
                }
                Log::log("\nGDB failed cause of : " + reasone + "\n", Error, logFilter::GDB_Server);
                notJetFailed = false;
            } else if (l.find("ST_LINK device status: Failed to get status, Target not responding, ret") !=
                       string::npos) {
                Log::log("Lost connection to remote target", Error, logFilter::GDB_Server);

                notJetFailed = false;
            }
            else if (l.find("Failure starting GDB server: TCP port ") != string::npos &&
                       l.find("not available.") != string::npos) {
                Log::log("Failed to bind GDB-Server to port. Maybe the port is already in use ", Error, logFilter::GDB_Server);
                notJetFailed = false;
            } else if (l.find("Shutting down...") != string::npos) {
            	char buff[50] = {0,};
            	sprintf(buff, "%s", utils::sharedMemoryRead());
            	buff[4] = 0;  // set end string if match
            	if(strcmp(buff,"true") != 0){
					Log::log("GDB-Server has been shutting down unexpected", Error, logFilter::GDB_Server);
					notJetFailed = false;
            	}
            }
            Log::log("GDB-Server : output : " + l, DebugL3, logFilter::GDB_Server);
        }
    }
    usleep(100000); // give the process some time the make a clean exit
    utils::cleanUpAndExit(-3);
}

static void callGdbServer(string command, nameLessPipe *nlp) {
    processHander pid;
    char ** argv = NULL;

	char *tmp[] = {NULL};;
	argv = buildArgv(command,1,tmp);

    callServer = command;

    string call = "call:{"+command+"}  args:{";
    call += "}";
    Log::log(("Call server with:: \t\"" +call+"\""),Message, logFilter::GDB_Server);
#ifdef __linux__
    if ((pid = nlp->createSubProcess()) == 0) {
        nlp->redirectSTD();
        nlp->closeChild();
        long index = command.find_last_of("/");
        if (index < 0)
            index = 0;
        string location = command.substr(0, index);
        Log::log("GDB-Server : set location of child process to " + location, Info, logFilter::GDB_Server);
        if(chdir(location.c_str())<0){
        	return ;
        }
        if(execv(argv[0], argv) == -1) {
            Log::log("GDB-Server exit with errno: \t\"" + string(strerror(errno))+ "\"  ", CriticError, logFilter::GDB_Server);
        }
        char buff[50] = {0,};
		sprintf(buff, "%s", utils::sharedMemoryRead());
		buff[4] = 0;  // set end string if match
		if(strcmp(buff,"true") != 0){
            //cout << buff<<endl;
            Log::log("\n\nThe GDB-Server(callGdbServer) has returend unexpected. Please check if its work properly! \n", Error, logFilter::GDB_Server);
        }
        utils::cleanUpAndExit(-3);
    } else {
        utils::addChildProzess(pid);
        nlp->closeParent();
    }
#elif _WIN32
    string s = command;
    pid = nlp->createSubProcess(s);
    utils::addChildProzess(pid);
#endif

}



bool gdbInterface::open() {

    Log::log("Open GDBClient streams", Message, logFilter::GDB_Interface);
    new thread(handleError, client->getErrorStream());
    new thread(handleOut, client->getOutStream());
    new thread(handleGDB, gdbLoc, client->getNameLessPipe(), elfFileLoc, gdbPort);
    usleep(10000);
    return true;
}

gdbInterface::gdbInterface(string elfFileLoc_, string gdbLoc_, string gdbServer, int gdb_Port_, gdbDecoder *decoder) {
    nameLessPipe *nlp = new nameLessPipe();
    gdbPort = gdb_Port_;
    if (gdbServer != "") {
        new thread(handleServer, nlp);
        usleep(100);
        new thread(callGdbServer, gdbServer, nlp);
    }
    client = new gdbClient(decoder);
    client->open();
    elfFileLoc = elfFileLoc_;
    gdbLoc = gdbLoc_;


}

gdbClient *gdbInterface::getClient() { return client; }
