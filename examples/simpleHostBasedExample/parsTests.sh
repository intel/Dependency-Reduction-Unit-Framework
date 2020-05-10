cd src/tests
rm -rf generated/*
mkdir -p generated
awk -f ../../../../awk/parseSingleFile.awk test_asserts.cpp 	> generated/test_asserts.c
awk -f ../../../../awk/parseSingleFile.awk test_crc.cpp 	> generated/test_crc.c
awk -f ../../../../awk/parseSingleFile.awk test_testObj.cpp 	> generated/test_testObj.c
awk -f ../../../../awk/parseSingleFile.awk disabled_test.cpp	> generated/disabled_test.c
cd generated
mkdir -p tmp
ls *.c | awk -f ../../../../../awk/generateInvokeAll.awk > tmp/invokeAll.c
mv tmp/invokeAll.c invokeAll.c

cd ../../..
