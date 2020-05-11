/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_COMCHAIN_H
#define GDBMANIPULATOR_COMCHAIN_H


#include "logging.h"
#include "LogFileFilter.h"
#include "tester/MemoryDump.h"
#include "tester/TestAnalyser.h"
#include "tester/TestStatistic.h"
#include "Utils.h"
using namespace logFilter;


/**
 * Abstract communication class.
 * Idea:
 *          To enable different kind of decoders each decoder has to implement this basis class
 *          It is possible to bypass the slower gdb communication by an optional serial interface to speed up test execution
 *
 * Usage:
 *          Is is possible to build a chain of communication interfaces. Each element adds some functionality.
 *          If one specific function is not possible to implement in an interface (like break insert ) the next instance get called.
 *
 * E.G.
 *          The last instance migth be the gdb that implement the howl functionality.
 *          If the GDB can get bypassed by an serial interface. This Interface implement e.g. the "getResultData()" method.
 *          Each call for this function gets bypassed to the Serial interface, but each other call gets redirected to the GDB
 *
 *          Chain : Serial->GDB
 *
 */
class comChain {

protected:
    comChain *nextInstance = nullptr;
    bool tryToFailSilent = false;
    /**
        * Macro EUNIT_TESTMODE.
        */
    bool targetIsInTestMode= false;
    /**
   * Macro EUNIT_BISTMODE.
   */
    bool targetIsInBistMode = false;

    /**
     * Macro targetIsInTestMode
     * use SW breakpoints
     */
    bool targetUsesesSWBreakpoints = false;
    /**
     * Macro EUNIT_LONGJUMP
     */
    bool tartIsInLongJump = false;
    /**
     * Macro eUnit_Version
     */
     uint8_t targetVersion = 0;
public:

    ~comChain(){
    		cerr << "close chain"<<endl;
    	  char toShare[] = "true\0";
    	  utils::sharedMemoryWrite(toShare, 4);
    	  closeComChain();
    }
    /*!
     * comChain contructor.
     * Each chain element has a nextInstance (or an nullpointer).
     * Each chain can add additional or more faster ways to execute the tests.
     * if one chain element fails it must escalate the execution to the next chain instance.
     * If there is no instance (nullpointer) the execution fails.
     * @param nextInstace next instance
     */
    comChain(comChain *nextInstace) : nextInstance(nextInstace) {};

    /*!
     * Change variable value.
     * set the value of an variable to an specific one
     * @param variable variable to change
     * @param newValue new  value to set
     * @return success
     */
    virtual bool setVariableTo(string variable, string newValue);

    /*!
     * Execute an Expression.
     * This method execute an expression on the target like an sub function
     * @param expression to evaluate
     * @return result string
     */
    virtual string evaluateExpression(string expression);

    /*!
     * insert a stop condition for the target.
     * @param when function where the stop condition gets insert
     */
    virtual void insertBreakpoint(string when);

    /*!
     * Delete all Break conditions.
     */
    virtual void deleteAllBreakpoints();

    /*!
     * set the program counter to an address or function.
     * @param function entry point
     */
    virtual void setProgramCounterTo(string function);

    /**
     * leaf the current function instantly.
     * Sometimes it is nesessary to leaf the current function immediately.
     * It is possible to define an defined return value. This could bo also be expresion that gets executet like "add(5,5)"
     * @param returnValue predefined return value or expression
     * @return stop frame of the calling function
     */
    virtual stopFrame *jumpOutOfCurrentFunction(string returnValue);

    /*!
     * initiate the system.
     * This function initiate the target system.
     * It is expected, that this function leafs with the first messageToDebugger(start testing)
     * @return success
     */
    virtual bool initSystem();

    /*!
     * Set the Testlevel.
     * Set the TestLevel to an specific amount. This changes the test execution time
     * @param testLevel new level
     */
    virtual void setTestLevel(amountOfInfo testLevel);

    /**
     * continue the program execution.
     * E.G. after the an communication message to from the target
     * @param waitUntilNextBreakpoint  if true waits until the program execution has stoped agan
     */
    virtual void continueAfterMessage(bool waitUntilNextBreakpoint);

    /**
	* continue the program execution.
	* E.G. after the an communication message to from the target
	* @param waitUntilNextBreakpoint  if true waits until the program execution has stoped agan
	*/
    virtual void continueAfterReturnFromFunction(bool waitUntilNextBreakpoint);

    /**
     * continue the program execution After an breakpoint.
     * @param waitUntilNextBreakpoint  if true waits until the program execution has stoped agan
     */
    virtual void continueAfterMockStub(bool waitUntilNextBreakpoint);

    /**
     * Get test result data.
     * Read the test result data from the targets buffer.
     * @return memory Dump
     */
    virtual memoryDump *getResultData();

    /**
     * Add Stub.
     * Add an expression that gets executed instead of the origin function
     * @param when function to stub
     * @param expression expression to execute instead
     */
    virtual void addStub(string when, string expression);

    /**
     * Add Mock.
     * Add an alternitiv function that gets executed instead of the origin function
     *  *WARNING: Function Must have the same signature
     * @param when function to mock
     * @param expression function to execute instead
     */
    virtual void addMock(string when, string mockFunction);

    /**
     * Reduce singe dependency.
     * Reduce the Dependency for the current program execution.
     * This function checks all registered dependencies
     * @param when current function where the program execution has stopped
     * @return success
     */
    virtual bool reduceDependency(string when);

    /**
     * delete all registered depenencies.
     */
    virtual void deleteAllDependencies();

    /**
	 * renew eUnit breakpoints
	 */
    virtual bool renewEUnitBreakpoints();
    /**
     * Has the target stoped.
     * Possibilities are :
     *      breakpoint
     *      message functions
     *      to reduced dependencies
     * @return success
     */
    virtual bool executionHasStopped();

    /**
     * get the current stop frame.
     * If the program execution has halted an stop frame gets produced where the information about the
     * reason and position gets documented
     * @return stop frame
     */
    virtual stopFrame *getExecutionStopFrame();


    /**
    * close comchain.
     * this closes the chainelement on the clean way
    */
    virtual void closeComChain();

    /**
     * execute a command if this command is known to the current comChain element.
     * Is this not the case the command get passed to the next element until the root chain is reached in this case this method returns false
     */
    virtual bool executeCommand(string command);

    virtual bool startTestMode();

    virtual void interruptTarget();

    /**
     * set break Mode
     * Set the targets breakMode.
     */
    virtual void setTargetBreakMode(bool enable);

    /**
     * detach the target.
     * @param terminateTarget stop the execution if true
     */
    virtual void detachTarget(bool terminateTarget = false);
    
    /**
     * try to fail silent.
     */
    virtual void failSilent(bool shallFailSilent);
    uint8_t getTargetVersion();

};


#endif //GDBMANIPULATOR_COMCHAIN_H
