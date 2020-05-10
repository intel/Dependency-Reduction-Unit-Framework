
cd tests
rm *.c
awk -f ../../../../../awk/parseSingleFile.awk test_asserts.cpp > test_asserts.c 
awk -f ../../../../../awk/parseSingleFile.awk tests.cpp > tests.c 
awk -f ../../../../../awk/parseSingleFile.awk test_crc.cpp > test_crc.c
awk -f ../../../../../awk/parseSingleFile.awk test_testObj.cpp > test_testObj.c
mkdir tmp
ls *.c | awk -f ../../../../../awk/generateInvokeAll.awk > tmp/invokeAll.c
mv tmp/invokeAll.c invokeAll.c
cd ..
make
./bisttest > isOutput

cmp --silent isOutput shouldOutput || echo "failed on BistMode"  || exit -1;
exit 1
