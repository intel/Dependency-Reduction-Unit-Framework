echo "run self test"
echo "test GDBManipulator"
CPPARG=FALSE
GDBMan=FALSE
BISTMODE=FALSE
EUnit=FALSE
EUnitWithGdbMann=FALSE
cd GDBManipulator/build/
	if make test 
	then 	GDBMan=1
	else 	GDBMan=-1
	fi
cd ../lib/CPP-Argument-Parser/build
	if make test
	then		CPPARG=1
	else		CPPARG=-1
	fi
echo "test eUnit on Host"
cd ../../../../eUnit/eUnit/eUnitSelfTest
ldconfig -p | grep cunit
 	if [ $? == 0 ] 
	then
		echo "found cunit"
		make test
		if make test
		then
			echo Unit success
			EUnit=1
		else
			EUnit=-1
		fi
 	else
		EUnit=-2
	fi
echo "test eUnit in BIST Mode"
cd BistMode
echo "run BIST Mode"
bash  "test.sh"
if   [ $? == 1 ]
	then 	BISTMODE=1
	else
		echo "$?"
	fi

echo "test eUnit with gdbMan on Host"
cd ../Host
bash "run.sh"
if   [ $? == 1 ]
	then 	EUnitWithGdbMann=1
	else
		echo "$?"
	fi

echo ""
echo "Testresults"
echo "-------------------------------------------"
echo "Unit Tests:"
if  [ "$GDBMan" == 1 ]
        then 
                echo   "  GDBManipulator 	:: success"
        else 
                echo   "  GDBManipulator 	:: failed"
        fi
 if [ "$CPPARG" == 1 ]
        then 
                echo   "  CPP-Argument-Parser 	:: success"
        else 
                echo   "  CPP-Argument-Parser 	:: failed"
        fi
 if [ "$EUnit" == 1 ]
        then 
                echo   "  eUnit-SelfTest 	:: success"
        elif [ "$EUnit" == -2 ]
	then
                echo   "  eUnit-SelfTest 	:: can not find cunit"
	else
		echo   "  eUnit-SelfTest 	:: failed"
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
                echo   "  EUnit with GdbMann 	:: failed"
        fi
echo "-------------------------------------------"
