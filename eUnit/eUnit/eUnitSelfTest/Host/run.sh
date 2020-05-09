echo selfTest gdbMann interaction
gcc -o runOnHost1  *.c  tests/*.c ../../*.c  -O0 -ggdb -D EUNIT_TESTMODE=1 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=1 -D EUNIT_BISTMODE=0
gcc -o runOnHost2  *.c  tests/*.c ../../*.c  -O0 -ggdb -D EUNIT_TESTMODE=1 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=0 -D EUNIT_BISTMODE=0
gcc -o runOnHost3  *.c  tests/*.c ../../*.c  -O0 -ggdb -D EUNIT_TESTMODE=0 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=0 -D EUNIT_BISTMODE=0

../../../../GDBManipulator/build/gdbMann/gdbMann -host -bin runOnHost1  -c /usr/bin/gdb -nh > resultTestMode
../../../../GDBManipulator/build/gdbMann/gdbMann -host -bin runOnHost2  -c /usr/bin/gdb -nh > resultTestModeSWBreak
../../../../GDBManipulator/build/gdbMann/gdbMann -host -bin runOnHost3  -c /usr/bin/gdb -nh -l CriticError > resultNoTestMode

cmp --silent expectTestMode resultTestMode || echo "failed on Test Mode" || exit -1;
cmp --silent expectTestMode resultTestModeSWBreak || echo "failed on Test Mode with no SW breakpoints" || exit -1;
cmp --silent expectNoTestMode resultNoTestMode || echo "failed on no Test Mode" || exit -1;
exit 1;
