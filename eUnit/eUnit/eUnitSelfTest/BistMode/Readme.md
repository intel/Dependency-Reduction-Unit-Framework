compile with : 
    gcc -o exampleSelfTest  *.c  tests/*.c eUnit/*.c  

to execute the Test call:
     ./exampleSelfTest

This executes an minimal example where some test are successful and some should fail. 

Configuration:
This example has just enabled the BIST_Mode. 
See https://github.com/diridari/EmbeddedTesting/wiki/eUnit_Configuration 

In this mode(Bist_Mode (build in self test mode)) it is not possible to use Mock and Stubs

