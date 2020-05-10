# eUnit-Framework
This is the eUnit-Framework. It intent to enable dependency-reduction and test execution on the target.
This document descripts how to do the first steps with eUnit. It is divided into tree Parts:

* using the examples
* use in your own projects 
* test syntax 

## Using the provided examples
### Requirements
* In order to use this system it is necessary to flash and debug the target by the GDB. 
These examples gets explained by using the “Atollic TrueSTUDIO for STM32”.  Once installed its provide an working flash and update mechanism. But feel free to use your own build and debug environment. 
In ‘/examples’ are some examples listed: 
* Example tests in ‘testexamples’ 
The subdirectory ‘withoutParser’ provides test which can be compiled directly. The other folder contains Test who has first to gets preprocess by an parser to create c-Code. Those test are more comfortable to create because you do not have to care about creating in callGroup or an InvokeAll functions. For further information see the wiki entry : “Test-Syntax”.
* Example implementations for the STMF3 and STMF4 Discovery-Boards
Those Examples are Based on the Atollic default example for the corresponding platform, apart from that the ‘eUNit’ folder with some tests had been copied into the Project and the main functiosn does after the system init call the ‘invokeAllTest()’ function to start all tests.

### Using an Example
1. build the wholeproject
2. Switch in to the folder /GDBManipulator/build
3. Type: ‘make’
4. Now you need the commands to: 
+ -s command to call the GDB-Server (alternative if the gdb-Server is already runing in background skip this command )
+ -c command to call the GDB-Client
+ -elf Elf-File location
+ -f to flash the elf file on the target

If you are using the Atollic true studio those applications are per default at : ‘/opt/Atollic_TrueSTUDIO_for_STM32_x86…’

5. for example you can use:

       -s "/opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.0.0/Servers/ST-LINK_gdbserver/ST-LINK_gdbserver -d"
       –c /opt/Atollic_TrueSTUDIO_for_STM32_x86_64_9.0.0/ARMTools/bin/arm-atollic-eabi-gdb 
       –elf /home/testUser/git/EmbeddedTesting/examples/Stm32F4/Debug/EmbeddedTesting.elf 
       -f 
    
5. If the Target is not jet flashed you need additional to add ‘-f’ to flash the target with the elf-file
For Further information’s see the Wiki entry: “Program Arguments”
6. Run the Application in ‘GDBManipulator’ with those arguments. 
7) Now you should see the test output like: 

         [ RUN      ]        
         [       OK ] EQ_SUCCESS::NEQ_SUCCESS
        
        ….
        
         [  FAILED  ] MockSucces::NEQ_FAIL        
                       (int32) Expected: 0	not to be equals to : 0        
        …        
         [ RUN      ]        
         [  FAILED  ] MockSucces::NEQ_FAIL        
        				     is<3> :	 01 02 03   |  ...        
        				 should<3> :	 02 ** **   |  ...        
        ….        
        test result : test run : 23  	 test failed : 9	 errors : 0         
        buffer reads : 4

8) Different Test-Levels
The speedup the testcases there are some Test-Levels. Each additional level provides more information about the failed test but decrease the test execution. It is possible the change the testlevel with the program argument ‘-t <>’ or –testLevel<>
arguments are: 
* "justSuccess" : boolean information about the test-status
* "testName" : (default) + name and group of the executed test 
* "testResults" + result and expected value of an failed test 
* "lineNumber" + number of the failed test

***

## Add eUnit into your Projects
To add ‘eUnit ’ to you own projects it is just necessary to copy the folder ‘/eUnit’ into you project and call the ‘invokeAllTest()’ from the main. 
To create your own tests each test must get called in the ‘invokeAllTest()’ Function. 
It is highly recommended to generate those functions with the provided ‘awk’-script. 
Further information’s are provided in the wiki entry: “Test Syntax and Test creation”


*** 

## Test-Syntax
### Without Code-Generator
To write the test-cases direct you have to create an InvoceAll function which calls all test-cases. 
It is advisable to divide the test-cases into groups. So that the invoceAll function do the setup and calls each test-group. 
#### Test-Setup
Before any test could be executed the function `MessageToDebugger(startTesting)` must be called.
This function establishes the connection to the GDB-Server and blocks until an test-mode gets configured by the GDB-Manipulator. 
#### Test-Execution
Program execution plan : 
![Alt text](../doc/pic/testExecution.png?raw=true "structure") 
##### Asserts
Supported Asserts are defined in assets.h : 
* `ASSERT_EQ(should, is)`

Check if “should” is equals to “is”
For : uint8_t , int, uint32_t 
* `ASSERT_NEQ(should, is)`

Check if “should” is not equals to “is”
For : uint8_t , int, uint32_t 
* `Assert_EQ_Mem(is*, sizeIs, should* , sizeShould)`

Check of the “is” memory is equals to the “should” mempry. 

#### Tests-Teardown
the GDB-Manipulator has to be informed  about the executing of  all test-cases. 
This is done by the calling the Function “MessageToDebugger(finishAllTest)”

### Mocks and Stubs

**Please see the corresponding wiki entry**

Mocks and Stubs are activ until function `deleteAllMocks()` gets called. 

### Automatically Test-code Generation
To simplify the test generation (test setup, invokeAll test cases, test teardown) an testfile parser has been built with “awk”
with this parser it is possible to generate googleTest like tests. 

#### Test creation
An test-case has following setup : 

`TEST(<testGroup>,<testName>){ … <Asserts etc.> }`

e.g. test.c


    #include "testSuite/asserts.h"
    #include "testSuite/testSuite.h"
    TEST(classSuccess,NEQ_SUCCESS){
   	  	uint8_t u1 = 1;
		uint32_t u2 = -1;
 		ASSERT_NEQ(0,1);
 		ASSERT_NEQ(1,0);
 		ASSERT_NEQ(-1,0);
		ASSERT_NEQ(-1,1);
 		ASSERT_NEQ(u1,u2);
     }

To generate “c”-compatible Code call the awk script: “parseSingleFile.awk”
e.g.

`awk -f awk/parseSingleFile.awk  targetExamples/testexamples/withParser/test.c`

This prints the generated test-code to the cli. 
#### Invoke All Tests 
With the script “generateInvokeAll.awk” it possible to generate an function that calls all test groups. 
e.g.

`ls targetExamples/testexamples/withParser/*.c  | awk -f awk/generateInvokeAll.awk`



