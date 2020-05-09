#Test-Generator

## about
This awk file is able to parse from a gtest like file to function. 
It create for each group an callGroupe funcktion and for each Test-File an callFile
One Testfile can have one or many testGroups. 
But an testGroup must have one testFile (not more) 
##usage

### Test-file-parser
Each test-file could have several TestClasses(or groups).
--> Each file needs an callGroup and an Call File function
usage :  'awk -f parsSingleFile.awk <src-file.c>'

to parse a single file

#### Filter Test Groups out
It is possible to filter test Groups on compile time out by setting the variable: `testFilter` with

     awk -v testFilter="<group>"  -f awk/parseSingleFile.awk test.c
`<group>` stands for the start of each group to be filtered. 

To filter several groups it is possible to to separate the groups with `,` 

    awk -v testFilter="<group1>,<group2>,<group3>"  -f awk/parseSingleFile.awk  test.c

### in invokeAll-generator
each generated test-file has an function depending to its filename : ”void  call_File_<fileName>".
This invokeAll-generator parse all filenames and call all those functions. 

call : 'ls generated/* | awk -f generateInvokeAll.awk'
to generate invokeAll function

### limitations
* a test case has to start with 'TEST(Dummy,1)\n{'  or with 'TEST(Dummy,1){'
* the last closing brake i an testcase must stay in a singe line
	
