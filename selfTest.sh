echo "Self test"
echo "-------------------------------------------"
CPPARG=FALSE
GDBMan=FALSE
BISTMODE=FALSE
EUnit=FALSE
EUnitWithGdbMann=FALSE
gdbMannBuild=FALSE
FOUND_CTEST=FALSE
FOUND_GTEST=FALSE
GDB_INSTALLED=FALSE
echo check requirements
gcc -v &>/dev/null
if [ $? != 0 ] 
then echo no gcc installed. Exit selfTest
exit -1
fi 
g++ -v &>/dev/null
if [ $? != 0 ] 
then echo no g++ installed. Exit selfTest
exit -1
fi 
make -v &>/dev/null
if [ $? != 0 ] 
then echo no make installed. Exit selfTest
exit -1
fi 
if [[ "$OSTYPE" != "linux-gnu" ]]; then
echo selftest is only on Linux suported. Runinning on $OSTYPE
exit -1
fi
CURRENT_DIR=$PWD
FOLDER_eUnitSelfTest=$CURRENT_DIR/eUnit/eUnit/eUnitSelfTest
FOLDER_GDBMAN=$CURRENT_DIR/GDBManipulator/build/
FOLDER_ARGParser=$CURRENT_DIR/GDBManipulator/lib/CPP-Argument-Parser/build
FOLDER_EUNIT_SELFTEST=$CURRENT_DIR/eUnit/eUnit/eUnitSelfTest
FOLDER_BISTMODE=$FOLDER_EUNIT_SELFTEST/BistMode
FOLDER_HostMode=$FOLDER_EUNIT_SELFTEST/Host
{
	cd $FOLDER_eUnitSelfTest/findCtest
		if make 
		then FOUND_CTEST=1
		else FOUND_CTEST=-1
		fi
		make clean
	cd $FOLDER_eUnitSelfTest/findGtest
		if make 
		then FOUND_GTEST=1
		else FOUND_GTEST=-1
		fi
		make clean
		
	echo find gdb client
	gdb -v 
	if [ $? == 0 ] 
	then GDB_INSTALLED=1
	else GDB_INSTALLED=-1
	fi
		
} &> /dev/null

echo build GDBManipulator
{
	cd $FOLDER_GDBMAN
	echo 
		if make 
		then 	gdbMannBuild=1
		else 	gdbMannBuild=-1
		fi
} &> /dev/null
if [ $FOUND_GTEST == 1 ]
then
	echo "test GDBManipulator"	
	{
			if make test 
			then 	GDBMan=1
			else 	GDBMan=-1
			fi
		cd $FOLDER_ARGParser
			if make  test >/dev/null
			then		CPPARG=1
			else		CPPARG=-1
			fi
	} &> /dev/null
fi
if [ $FOUND_CTEST == 1 ]
then
	echo "run deUnit self test"
	{
		cd $FOLDER_EUNIT_SELFTEST
		ldconfig -p | grep cunit
		 	if [ $? == 0 ] 
			then
				echo "found cunit"
				make  test
				if make test -s -l 
				then
					echo Unit success
					EUnit=1
				else
					EUnit=-1
				fi
		 	else
				EUnit=-2
			fi
	} &> /dev/null
fi
echo "run deUnit system tests"
{
	cd $FOLDER_BISTMODE
	echo "run BIST Mode"
	bash  "test.sh"
	if   [ $? == 1 ]
		then 	BISTMODE=1
		else
			echo "$?"
		fi

	if [ $GDB_INSTALLED == 1 ]
	then
		echo "test eUnit with gdbMan on Host"
		cd $FOLDER_HostMode
		bash "run.sh"
		if   [ $? == 1 ]
			then 	EUnitWithGdbMann=1
			else
				echo "$?"
		fi
	fi
} &> /dev/null


echo ""
echo "-------------------------------------------"
echo "Build results:"
echo "-------------------------------------------"
if  [ "$gdbMannBuild" == 1 ]
        then 
                echo   "  Build 		:: success"
        else 
                echo   "  Build 		:: failed"
        fi
echo "Testresults"
echo "-------------------------------------------"
echo "Unit Tests:"
if  [ "$GDBMan" == 1 ]
        then echo   "  GDBManipulator 	:: success"
else 
	if [ $FOUND_GTEST == 1 ]
	then echo   "  GDBManipulator 	:: failed"
	else echo   "  GDBManipulator 	:: found not gTest"
	fi
 fi
if [ "$CPPARG" == 1 ]
        then 
                echo   "  CPP-Argument-Parser :: success"
        else 
        if [ $FOUND_GTEST == 1 ]
	then echo   "  GDBManipulator 	:: failed"
	else echo   "  GDBManipulator 	:: found not gTest"
	fi
 fi
if [ "$EUnit" == 1 ]
        then 
                echo   "  deUnit-SelfTest 	:: success"
        elif [ "$FOUND_CTEST" == 1 ]
	then echo   "  deUnit-SelfTest 	:: failed"
	else echo   "  deUnit-SelfTest 	:: found no cunit"
        fi

echo "-------------------------------------------"
echo "System Tests:"
if [ "$BISTMODE" == 1 ]
        then 
                echo   "  Bist-Mode 		:: success"
        else 
                echo   "  Bist-Mode 		:: failed"
        fi
if [ "$EUnitWithGdbMann" == 1 ]
        then 
                echo   "  EUnit with GdbMann 	:: success"
        else 
        if [ $GDB_INSTALLED == 1 ]
        then echo   	"  DEUnit with GdbMann :: failed"
        else echo	"  DEUnit with GdbMann :: found no gdb client"
        fi
 fi
echo "-------------------------------------------"
