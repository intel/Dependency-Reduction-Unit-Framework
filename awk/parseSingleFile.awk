# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
# Usage: Arguments: filename
# This script parses the source file and replaces the TEST(CLASS,NAME) makro with a function
# signature. For each class it creates a callClass_function (test_callGroupe<Class>) and for each
# file a call_File (call_File_<file-name>) function which can be called by an "invoke all
# functions" (separate script). Example: awk -f parsSingleFile.awk test_AllSuccess.c
# It is possible to filter test Groups on compile time out by setting the variable: testFilter with
# 'awk -v testFilter='<group>'  -f awk/parseSingleFile.awk test.c <group> stands for the
# start of each group to be filtered. To filter several groups it is possible to to separate the
# groups with ',' awk -v testFilter='<group1>,<group2>,<group3>' -f awk/parseSingleFile.awk test.c


BEGIN{
	depth = 0
	isTestCase = 0
	lastDepth = 0
	className = ""
	disabledTest = 0
	disabled = 0
	print "/* This is file generated by eUnit, don't edit */"
	#print "#include \"eUnit.h\""

	gsub("[[:blank:]]","",testFilter)
	split(testFilter,testFilter_,",")
	for(x in testFilter_){
		if(testFilter_[x]){# string not empty
			print "//  current testFilters: {" testFilter_[x] "}" 
		}
		
	}
	print "\n\n"
	print "#include <setjmp.h> "
	print "static jmp_buf env;"
}


{
	# search the end of an test case
	depth += split($0,openX,"{")
	depth -= split($0,openX,"}")
	if(lastdepth >0 && depth == 0 && isTestCase == 1){
		print "   testCaseSuccess(testSuccess);"
		print "   return 0;"


		isTestCase = 0
	}
	lastdepth = depth

	if(/TEST[(]/){ # line contains TEST*(
		isTestCase = 1
		# set depth to 0 beacause cpp does close more breaks that it does open
		depth = 0
		lastdepth = 0
    depth += split($0,openX,"{")
	  depth -= split($0,openX,"}")
		gsub("[[:blank:]]","",$0); 
		split($0,a,"[(),]"); 
		group=a[2]; 
		test =a[3];
		className = group;
		tmpx = "__eUnitTest_" group "_" test "()"

		if(disabled){
			print "*/ \n"
			disabled = 0
		}
		
		if($0 ~ /^DISABLED/){
			printf "/*   Disabled\n"
			disabled = 1
			disabledTest ++;
		}

		if(index(testFunctions[className],tmpx) == 0){
			testFunctions[className] =  testFunctions[className] "    numberOfFailedTests += "tmpx";\n"
		}
		
		print "uint8_t " tmpx "\n{";
		if(index($0,"{") == 0){
			delNextBreak = "true"
		}
		print "\t bool testSuccess = true;"
		print "\t int val = 0;\n\t val = setjmp (env);";
		print "\t if(val) {\n\t\treturn 1; // one assert has failed\n\t}"
		print "#if(WRITE_TEST_NAME)"
		print "    addTestName(\"" test "\",sizeof(\"" test"\"));"
		print "#endif"
		if(disabled == 0){
			arr[className] = group
		}
	}
	else if(/#define __STDC_/){
		# skip this line
	}	
	else{
		if(delNextBreak){
			gsub("{","",$0); 
		}
		print $0
		delNextBreak = ""
	}

}


END{
	if(disabled){
		print "*/"
	}

	print "extern int disabledTest;\n"
	callAll =""
	# check all groups
	for(x in arr){
		# if current group has beend opt out next 
		for (y in testFilter_){
		   if(arr[x] ~ testFilter_[y]){ 
			if(testFilter_[y]){# string not empty
				callAll = callAll "//AWK Filter class starts with:" testFilter_[y] "\n//"
				disabledTest ++;
			}
		   }
		}
		
		callAll = callAll "    if(shallExecuteTestGroup(\""arr[x]"\")){numberOfFailedTests += __eUnitGroup_" arr[x] "();} else {disabledTest ++;}\n"
		
		print "unsigned int __eUnitGroup_" arr[x] "(){"
		print " #if(WRITE_CLASS_NAME)"
		print "    addTestClass(\""arr[x]"\",sizeof(\""arr[x]"\"));"
		print " #endif"
		print "  unsigned int numberOfFailedTests = 0;"
		print  testFunctions[x]
		print "    deleteAllMocks();"
		print " return numberOfFailedTests; \n}\n\n"
	}

	split(ARGV[ARGIND],filename,".");
	print "unsigned int  __eUnitFile_" filename[1] "()\n{"
	print "  unsigned int numberOfFailedTests = 0;"
	print "    disabledTest += "  disabledTest ";"
	printf " #if(WRITE_FILE_NAME)"
	print "\n    addTestFileName(__FILE__,sizeof(__FILE__));"
	print " #endif"
	print callAll
	print "  return numberOfFailedTests;"
	print "}\n\n"
}

#each file with _generated.c contains its own test class witch cann be parsed to an invoke all
