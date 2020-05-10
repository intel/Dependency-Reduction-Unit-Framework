/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */



#include "ArgumentCallBackFunctions.h"

#include <sstream>
#include "logging.h"
#include "Utils.h"
#include "Version.h"
using namespace std;

string elfFile = "";
string callServer = "";
string callClient = "";
string xmlFile = "";
int gdbPort = 61234;
bool flashTar = false;
amountOfInfo testLevel = amountOfInfo::amountOf_testResults;
string targetResetFunction = "Reset_Handler";
bool runOnTarget = true;
bool createStatistic = false;
bool cliHighlighting = true;
runModeType executionMode = testModeSetting;
string gdbScriptAfterExecution = "";
string disabedTestGroups = "";
string enabledTestGroups = "";
int targetTimeout = -1; // timeout when a failed assert gets added to the target buffer
bool failByFailedTests = false;

int callBackRunMode(int i, char *buff[]) {
    i++;
    string nxt = string(buff[i]);
    if(nxt == "flashMode"){
        Log::log("Set runMode to flashMode",Message);
        executionMode = flashModeSetting;
        flashTar = true;
    }else if(nxt == "testMode") {
        Log::log("Set runModet to testMode",Message);
        executionMode = testModeSetting;
    }else if(nxt == "runMode"){
    	Log::log("Set runMode to runMode",Message);
    	executionMode = runModeSetting;
    }
    else{
        Log::log("Undefined run Mode " + nxt,Error);
    }
    return i;
}
int callBackResetFunction(int i, char **buff) {
    i++;
    targetResetFunction = buff[i];
    return i;
}

int callBackElfFile(int i, char *buff[]) {
    i++;
    elfFile = string(buff[i]);
    Log::log("Got argument : elf " + elfFile, Message);
    return i;
}

int callBackServer(int i, char *buff[]) {
    i++;
    callServer = string(buff[i]);
    Log::log("Parsed argument : server " + callServer, Message);
    return i;
}

int callBackClient(int i, char *buff[]) {
    i++;
    callClient = string(buff[i]);
    Log::log("Parsed argument : client " + callClient, Message);
    return i;
}

int callBackPort(int i, char **buff) {
    i++;
    gdbPort = atoi(buff[i]);
    Log::log("Parsed argument : port " + to_string(gdbPort), Message);
    return i;
}

int callBackXML(int i, char *buff[]) {
    i++;
    xmlFile = string(buff[i]);
    return i;
}

int callBackLogFileName(int i, char *buff[]) {
    i++;
    Log::setLogFileName(buff[i]);
    return i;
}

int callBackLogLogLevel(int i, char *buff[]) {
    i++;
    Log::setLogLevel(buff[i]);
    return i;
}

int callBackLogLogLevelSeperat(int i, char *buff[]) {
    i++;
    Log::setLogLevel(buff[i], buff[i + 1]);
    return ++i;
}


int callBackLogTestLevel(int i, char *buff[]) {
    i++;
    if (utils::startsWith(new string(buff[i]), "testResults")) {
        Log::log("Set amount of test info to : amountOf_testResults", Info);
        testLevel = amountOfInfo::amountOf_testResults;
    } else if (utils::startsWith(new string(buff[i]), "lineNumber")) {
        Log::log("Set amount of test info to : amountOf_lineNumbers", Info);
        testLevel = amountOfInfo::amountOf_lineNumbers;
    } else if (utils::startsWith(new string(buff[i]), "testName")) {
        Log::log("Set amount of test info to : amountOf_testNames", Info);
        testLevel = amountOfInfo::amountOf_testNames;
    } else if (utils::startsWith(new string(buff[i]), "justSuccess")) {
        Log::log("Set amount of test info to : amountOf_justSuccess", Info);
        testLevel = amountOfInfo::amountOf_justSuccess;
    }

    return i;
}

int callBackScript(int i,char ** buff){
    i++;
    gdbScriptAfterExecution = string(buff[i]);
    return i;
}

argvParser *initProgramArguments() {
    Log::advancedConf()->pintLogSrc(true);
    string desc = "This application helps the eUnit framework to execute test and enables the reduction of dependency’s by using an Debugger.\n It is possible to use the eUnit framework without this application the setting the “SELF_TEST_MODE” flag (see eUnit selftest mode).\n"
                  " This application requires the binary file (\"-elf\") and a command to invoke the debugger (\"-s <command\">). \n Is the target a remote target (like an microcontroller) it is necessary to start a gdb-server manual or by using the (\"—server <command>\") command. "
                  "\nThis every argument can stored in a defauld config file named <gdbMannDefaultConfig>. This file gets pared without further interaction. ";
    argvParser *p = new argvParser("gdbMann",desc,true,"%#");
    p->checkForDefaulConfigFilesIn("gdbMannDefaultConfig","~/ ./ ../");


    p->addSection("Testing");
    // This command defines a binary file that gets executed on the host or on the target. If the execution is on a remote target it might be necessary to flash it with "-f"
    p->addArg("-bin", "-elf", "elf file which gets loaded by the testMode-Client to decode target names",
              callBackElfFile)->asFile()->required()->numberOfParameter(1)
            ->addAdditionalHelp("This command defines a binary file that gets executed on the host or on the target. If the execution is on a remote target it might be necessary to flash it with \"-f\"");
    // Is this command set the defined gbd-server gets started. This is necessary if the target is a remote target. Alternative it is possible to start a gdbserver manual.
    p->addArg("-s", "--server", "command to call and configure the testMode-Server", callBackServer)->asFile()->numberOfParameter(1)
            ->addAdditionalHelp("Is this command set the defined gbd-server gets started. This is necessary if the target is a remote target. Alternative it is possible to start a gdbserver manual. \n"
            		"If e.g. the clients port has changed it must be adjusted in this call too! \n\nIt may be necessary to add the entire application path ");
    // This command defines the location/command for the gdb-client.
    p->addArg("-c", "--client", "command to call testMode-Client", callBackClient)->asFile()->required()->numberOfParameter(1)
            ->addAdditionalHelp("This command defines the location/command for the gdb-client. \n\nIt may be necessary to add the entire application path");
    // This command defines the port for the gdb-client and server (if defined).
       p->addArg("-p", "--port", "port for the GDB-Client  (Not Server!)", callBackPort)->numberOfParameter(1)
               ->addAdditionalHelp("This command defines the port for the gdb-client and server (if defined).");
       // This command does flash the target. It only has effect if the target is a remote one.
	   p->addArg("-f", "--flash", "flash the target with the given elf file", [] { flashTar = true; })
			   ->addAdditionalHelp("This command does flash the target. It only has effect if the target is a remote one.");

       // This command defines the test level. The test level it the amount of information about the test execution that gets shared between the target and the host. The more information gets shared the longer does the test execution take.
     p->addArg("-t", "--testLevel", "set the testLevel to <string>", callBackLogTestLevel)
             ->allowedParameter(4, "testResults", "lineNumber", "testName", "justSuccess")->numberOfParameter(1)
             ->addAdditionalHelp("This command defines the test level. The test level it the amount of information about the test execution that gets shared between the target and the host. The more information gets shared the longer does the test execution take.  \n Levels are: \n\t"
                 "\"justSuccess\" : boolean information about the test-status\n\t"
                 "\"testName\" : (default) + name and group of the executed test\n\t"
                  "\"testResults\" + result and expected value of an failed test\n\t"
                  "\"lineNumber\" + line number of the failed tests");
     p->addArg("-mode","--runMode","define the execution mode of this application",callBackRunMode)->numberOfParameter(1)
                  ->allowedParameter(3,"testMode","runMode","flashMode")
                  ->addAdditionalHelp("This command defines the run Mode. Each Mode defines a different gdnManipulator behavior. \n  The mode \"--runMode\" does does connect to the target  and continue its execution to execute gdb commands by using –runGdbScriptAfterExec. \n  The mode <testMode> connects to the target and wait for the first message and try’s to execute all test and mocks"
                		  "\n  The mode <flashMode>");
     function<int(int,char**)> getTimeout = [](int i, char ** buff){
    	 i++;
		 targetTimeout = atoi(buff[i]);
		 if(targetTimeout<500){
		     Log::log("timeouts <500 ms might result in a communication errors",UserInfo);
		 }
    	 return i;
     };
    p->addArg("","--timeout","When the target has not send in <timeout> ms a signal the target returns out of the current function",getTimeout)
    		->numberOfParameter(1)->addAdditionalHelp("Communication Timeout.\n\t This command enables in TestMode a timeout. On each communication with the gdbMan a timer gets reseted(wachdog).\n\t Does this timer expire a Timeout Assert gets added and the target continues after the current function. \n\t The timeout is given in milliseconds. \r\n It is recommendet to have at least 500ms to timeout");

     //This command defines the run Mode. Each Mode defines a different gdnManipulator behaviour. \n  The mode "--runMode" does flash the target (if defined by -f) and allows to execute gdb commands by using –runGdbScriptAfterExec. \n  The mode testMode connects to the target and wait for the first message and try’s to execute all test and mocks
    function<int(int,char**)> disableG = [](int i, char ** buff){i++;disabedTestGroups = string(buff[i]); return i;};
    p->addArg("-d","--disableGroups","Start string of test groups that shall not get executed multible string are seperated by <:>", disableG)
            ->addAdditionalHelp("Disable on runtime test groups. \n \t -d <group>\nEach group gets checked whether the group starts with the argument <group>. \n "
                    "If multiple arguments shall get opt out it is possible to separate each group with <:> \n\t -d TEST_TIMER*:TEST_HAL:TEST_LCD");
    function<int(int,char**)> enableGroup = [](int i, char ** buff){i++;enabledTestGroups = string(buff[i]); return i;};
    p->addArg("-e","--enableGroups","Start string of test groups that shall get executed multible string are seperated by <:>", enableGroup)
               ->addAdditionalHelp("Enables on runtime test groups. \n \t -d <group>\nEach group gets checked whether the group starts with the argument <group>. \n "
                       "If multiple arguments shall get opt out it is possible to separate each group with <:> \n\t -d TEST_IIC*:TEST_LCD");

    // This command executes the gdb command defined in the parameter. Each command gets executed after the runMode has finished.
    p->addArg("","--runGdbScriptAfterExec","executes a gdb-script after executen the curretn runMode  --runGdbScriptAfterExec <file>",callBackScript)->numberOfParameter(1)->asFile()->addAdditionalHelp("This command executes the gdb command defined in the parameter. Each command gets executed after the runMode has finished. ");

    // Is this command set the the elf file gets executed local.
    p->addArg("-host", "--runOnHost", "run test on host and not on target", [] { runOnTarget = false; })
            ->addAdditionalHelp("Is this command set the the elf file gets executed local. ");
    // This command does change the default startup function where the target stops after flashing. By default this is “Reset_Handler”.
    p->addArg("-r", "--resetFunction", "expect to stop in this function after startup",
              callBackResetFunction)->numberOfParameter(1) -> addAdditionalHelp("This command does change the default startup function where the target stops after flashing. By default this is \"Reset_Handler\".");
    p->addArg("","--failgdbManIfOneTestWasNotSuccessfull","gdbMan returns -1 if at least on test failed",[]{failByFailedTests = true;});



    p->addSection("additional output");
    // To create a xml output file use this command.
    p->addArg("-x", "--xml", "generate xml output file <file>", callBackXML)->asFile()->numberOfParameter(1)
            ->addAdditionalHelp("To create a xml output file use this command.");
    // This command enables to local statics about the test execution like time and amount of gdb commands.
    p->addArg("-stat", "--statistic", "create test statistic", [] { createStatistic = true; })
            ->addAdditionalHelp("This command enables to local statics about the test execution like time and amount of gdb commands.");




    p->addSection("logging");
    // This command defines to logfile name.
    p->addArg("-lfn", "--logFileName", "change the fog file name ", callBackLogFileName)->numberOfParameter(1)
            ->addAdditionalHelp("This command defines to logfile name.");
    // This command does adjust he amount of information that gets printed on the cli an and into the logfile.
    p->addArg("-l", "--logLevel", "change the cli loglevel enum name <string> e.g -l Error ", callBackLogLogLevel)
            ->allowedParameter(9, "None", "UserInfo", "CriticError", "Error", "Message", "Info", "Debug", "DebugL2","DebugL3")->numberOfParameter(1)
            ->addAdditionalHelp("This command does adjust he amount of information that gets printed on the cli an and into the logfile.");
    p->addArg("-lls", "--logLevelSeparate",
              "change the log level for cli and logfile separate to <string cli> <string file> e.g -lls Error DebugL3", callBackLogLogLevelSeperat)->numberOfParameter(2)
              ->addAdditionalHelp("This command does adjust he amount of information that gets printed on the cli an and into the logfile sperate.");
    p->addArg("-nh", "--noHighlighting", "disables the cli highlighting ", [] {
        Log::advancedConf()->setCliHighLight(false);
        cliHighlighting = false;
    });

    p->addArg("-v","--version","print build version",[]{cout <<"gdbManipulator version: "<< VERSION_MAJOR<<"."<<VERSION_MINOR<<"."<<VERSION_REVISION<<endl; });
    return p;

}
string getOsName()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __unix || __unix__
    return "Unix";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#else
    return "Other OS ";
#endif
}

void logArgumentStatus(){
	ostringstream s;
	s<< "\ngdbManipulator version: "<< VERSION_MAJOR<<"."<<VERSION_MINOR<<"."<<VERSION_REVISION<<endl;
	s<< "\telf file: "<< elfFile<<endl;
	s<< "\tcall server command    : "<< callServer<<endl;
	s<< "\tcall client command    : "<< callClient<<endl;
	s<< "\ttest level             : "<< testLevel<<endl;
	s<< "\texecution Mode         : "<< executionMode   <<endl;
	s<< "\trun on Target          : "<<runOnTarget   <<endl;
	s<< "\tdisabled test groups   : "<< disabedTestGroups  <<endl;
	s<< "\tenabled test groups    : "<<enabledTestGroups<<endl;
	s<< "\ttime out               : "<< targetTimeout<<endl;
	s<< "\tflash target           : "<< flashTar<<endl;
	s<< "\tgdb Port               : "<< gdbPort<<endl;
	s<< "\tXML file               : "<< xmlFile<<endl;
	s<< "\trun skript after execution : "<< gdbScriptAfterExecution  <<endl;
	s<< "\ttarget reset handler   : "<< targetResetFunction<<endl;
	s<< "\tcreate test Statistic  : "<<createStatistic   <<endl;
	s<< "\tcli highlighting       : "<< cliHighlighting  <<endl;
    s<< "\tcli highlighting       : "<< cliHighlighting  <<endl;
    s<< "\tgdbMann is running on  : " << getOsName() <<endl<<endl;

	Log::log("\n"+s.str(),Message);
}


std::ostream &operator<<(std::ostream &strm, const runModeType &a){
	string s;
	switch(a){
		case testModeSetting: s = "testMode";break;
		case runModeSetting:s = "runMode";break;
		case flashModeSetting:s = "flashModeS";break;
	}
	return strm << s;
}
