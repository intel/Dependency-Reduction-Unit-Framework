echo selfTest gdbMann interaction

cd tests
rm *.c

awk -f ../../../../../awk/parseSingleFile.awk test_asserts.cpp > test_asserts.c 
awk -f ../../../../../awk/parseSingleFile.awk tests.cpp > tests.c 
awk -f ../../../../../awk/parseSingleFile.awk test_crc.cpp > test_crc.c
mkdir tmp -p
ls *.c | awk -f ../../../../../awk/generateInvokeAll.awk > tmp/invokeAll.c
mv tmp/invokeAll.c invokeAll.c
cd ..


gcc -w -o runOnHost1  *.c  tests/*.c ../../eUnit.c  -O0 -ggdb -I ../../ -D EUNIT_TESTMODE=1 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=1 -D EUNIT_BISTMODE=0
gcc -w -o runOnHost2  *.c  tests/*.c ../../eUnit.c  -O0 -ggdb -I ../../ -D EUNIT_TESTMODE=1 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=0 -D EUNIT_BISTMODE=0
gcc -w -o runOnHost3  *.c  tests/*.c ../../eUnit.c  -O0 -ggdb -I ../../ -D EUNIT_TESTMODE=0 -D EUNIT_USE_SOFTWARTE_BREAK_POINTs=0 -D EUNIT_BISTMODE=0

../../../../GDBManipulator/build/gdbMan/gdbMan -host -bin runOnHost1  -c /usr/bin/gdb -nh > resultTestMode
../../../../GDBManipulator/build/gdbMan/gdbMan -host -bin runOnHost2  -c /usr/bin/gdb -nh > resultTestModeSWBreak
../../../../GDBManipulator/build/gdbMan/gdbMan -host -bin runOnHost3  -c /usr/bin/gdb -nh -l CriticError > resultNoTestMode

cmp --silent expectTestMode resultTestMode || echo "failed on Test Mode" || exit -1;
cmp --silent expectTestMode resultTestModeSWBreak || echo "failed on Test Mode with no SW breakpoints" || exit -1;
cmp --silent expectNoTestMode resultNoTestMode || echo "failed on no Test Mode" || exit -1;
exit 1;
