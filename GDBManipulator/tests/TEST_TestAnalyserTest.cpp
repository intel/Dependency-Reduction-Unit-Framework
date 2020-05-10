//
// Created by basto on 3/12/18.
//

#include "gtest/gtest.h"
#include "tester/TestAnalyser.h"

#define private public


class StubDump : public memoryDump {
    uint8_t *data;
    int size;
public:
    StubDump(uint8_t *data_, int size_) : memoryDump(new string()) {
        data = data_;
        size = size_;
    }

    uint8_t *getDump() {
        return data;
    }

    bool decodeDump() {
        return true;
    }


    int getSize() {
        return size;
    }

};

TEST(testAnalyser_JustSuccess, emptyData) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[0] = {};
    StubDump *d = new StubDump(dump, 0);
    t->analyse(d);

    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 0);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_JustSuccess, FourSuccessFullTests) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[1] = {0b1010101};
    StubDump *d = new StubDump(dump, 1);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 4);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_JustSuccess, ThreeSuccessFullTests) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[1] = {0b10101};
    StubDump *d = new StubDump(dump, 1);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_JustSuccess, ThreeSuccessOneFailFullTests) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[1] = {0b10010101};
    StubDump *d = new StubDump(dump, 1);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 1);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_JustSuccess, Two_SuccessAndTheEnd_IgnoreRest) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[1] = {0b10000101};
    StubDump *d = new StubDump(dump, 1);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}


TEST(testAnalyser_JustSuccess, severallyBytesOfData) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[4] = {0b1100101, 0b10010101, 0b01001001};
    StubDump *d = new StubDump(dump, 4);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 3);
    ASSERT_EQ(t->getSucceedTests(), 7);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_JustSuccess, Data_With_Name) {
    testAnalyser *t = new testAnalyser(amountOf_justSuccess, 0, false);
    uint8_t dump[4] = {0b1100111, 0b10010101, 0b01001001};
    StubDump *d = new StubDump(dump, 4);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 3);
    ASSERT_EQ(t->getSucceedTests(), 6);
    ASSERT_EQ(t->getErrors(), 1);
    delete t;
    delete d;
}

TEST(testAnalyser_WithTestNames, EmptyData) {
    testAnalyser *t = new testAnalyser(amountOf_testNames, 0, false);
    uint8_t dump[] = {};
    StubDump *d = new StubDump(dump, 0);
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 0);
    ASSERT_EQ(t->getErrors(), 0);
    delete t;
    delete d;
}

TEST(testAnalyser_WithTestNames, TestNameAndOneSuccessFullTest) {
    testAnalyser *t = new testAnalyser(amountOf_testNames, 0, false);
    uint8_t dump[] = {0b11, 0, 4, 0, 0, 0, 'a', 'b', 'c', 0, 0b1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 0);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    string res = (t->generateStringResult());
    //ASSERT_TRUE(utils::startsWith(&res,
    //                              "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::abc\n\ntest run : 1  \t test failed : 0\t errors : 0"));
    delete t;
    delete d;
}

TEST(testAnalyser_WithTestNames, TestNameAndOneFailedTest) {
    testAnalyser *t = new testAnalyser(amountOf_testNames, 0, false);
    uint8_t dump[] = {0b1101, 0, 4, 0, 0, 0, 'a', 'b', 'c', 0, 0b10};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 1);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    string res = (t->generateStringResult());
   // ASSERT_TRUE(utils::startsWith(&res,
  //                               "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::abc\n\ntest run : 2  \t test failed : 1\t errors : 0"));
    delete t;
    delete d;
}

TEST(testAnalyser_WithTestNames, TestNameAndError) {
    testAnalyser *t = new testAnalyser(amountOf_testNames, 0, false);
    uint8_t dump[] = {0b11, 0, 4, 0, 0, 0, 'a', 'b', 'c', 0, 0b110};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getFailedTests(), 1);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    string res = (t->generateStringResult());

   // ASSERT_TRUE(
    //        utils::startsWith(&res,
    //                         "[==========] \n[----------] \n[----------] [ RUN      ]\n[  FAILED  ] not defined class::abc\n[ RUN      ]\n[       OK ] not defined class::abc\n\ntest run : 2  \t test failed : 1\t errors : 0"));
    delete t;
    delete d;
}
/**
 * typedef enum typeOfData{
	testNotRun = 0,
	testSucceed,			// size data is
	testFailed,		// size dataShould  size dataIS
	testName			// size data
}typeOfData;
 */

TEST(testAnalyser_ResultSize, justSuccess) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b1, 0, 0b10, 0, 0, 0, 'a', 'b', 'c', 0, 0b110};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 0);

}

TEST(testAnalyser_ResultSize, justSuccess2) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b101, 0, 0b10, 0, 0, 0, 'a', 'b', 'c', 0, 0b110};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 0);

}

TEST(testAnalyser_ResultSize, failedTest) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b10, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 0);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
  //  ASSERT_EQ(is,
  // //           "[==========] \n[----------] \n[----------] [ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 1  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccess) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b1001, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
   // ASSERT_EQ(is,
   //           "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 2  \t test failed : 1\t errors : 0);
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccessAndByteShift) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b01, 0b10, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
  //  ASSERT_EQ(is,
  //            "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 2  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccess2) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b1001, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 1);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
  //  ASSERT_EQ(is,
 //             "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 2  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccess3) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
  //  ASSERT_EQ(is,
  //            "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 3  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccess4) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
//  ASSERT_EQ(t->generateStringResult(),
  //            "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n\ntest run : 3  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, failedTestandbeginwithSuccess5) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100011, 1, 0, 0, 0, 1, 0, 0, 0, 0b1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
   // ASSERT_EQ(is,
    //          "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int32) Expected: 1\tto be equals to : 1\n[ RUN      ]\n[       OK ] not defined class::not defined test \n\ntest run : 4  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, sizeOf8) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100001, 1, 1, 0b1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
 //   ASSERT_EQ(is,
  //            "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int8) Expected: 1\tto be equals to : 1\n[ RUN      ]\n[       OK ] not defined class::not defined test \n\ntest run : 4  \t test failed : 1\t errors : 0");

}

TEST(testAnalyser_ResultSize, sizeOf8_2) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100001, 1, 1, 0, 0b1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
   // ASSERT_EQ(t->generateStringResult(),
   //           "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int8) Expected: 1\tto be equals to : 1\n\ntest run : 3  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, sizeOf16) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100010, 1, 0, 1, 0, 0b1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
  //  ASSERT_EQ(is,
   //           "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int16) Expected: 1\tto be equals to : 1\n[ RUN      ]\n[       OK ] not defined class::not defined test \n\ntest run : 4  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_ResultSize, sizeOf16_2) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 0b100010, 1, 0, 1, 0, 0, 0b1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    //ASSERT_EQ(t->generateStringResult(),
    //           "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (int16) Expected: 1\tto be equals to : 1\n\ntest run : 3  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_Byte, size_1) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 5, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);

}

TEST(testAnalyser_Byte, size_2) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 5, 2, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
}

TEST(testAnalyser_Byte, size_6) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 5, 6, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 6, 0, 0, 0, '1', '2', '3', '4', '5', '6',
                      0, 1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
    ASSERT_EQ(is,
              "[----------] 0 tests from not defined class\n[==========]\n\n[==========] \n[----------] \n[----------] [       OK ] not defined class::not defined test \n[       OK ] not defined class::not defined test \n[  FAILED  ] not defined class::not defined test \n               (<byte)\n\t\t\t\t     is<6> :\t 61 62 63 64 65 66   |  abcdef\n\t\t\t\t should<6> :\t 31 32 33 34 35 36   |  123456\n\nTest run : 3  \t Test failed : 1\t Errors : 0"
    );
    }

TEST(testAnalyser_Byte, size_63) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 5, 6, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 3, 0, 0, 0, 'a', 'b', 'c', 0, 1};
    StubDump *d = new StubDump(dump, sizeof(dump));
    t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 2);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
    //   ASSERT_EQ(is,
    //             "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (<byte)\n\t\t\t\t     is<6> :\t 61 62 63 64 65 66   |  abcdef\n\t\t\t\t should<3> :\t ** ** **            |  abc   \n\ntest run : 3  \t test failed : 1\t errors : 0");
}

TEST(testAnalyser_Byte, size_6andMore) {
    testAnalyser *t = new testAnalyser(amountOf_testResults, 0, false);
    uint8_t dump[] = {0b100101, 5, 6, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 6, 0, 0, 0, '1', '2', '3', '4', '5', '6',
                      1, 0};
    StubDump *d = new StubDump(dump, sizeof(dump));
       t->analyse(d);
    ASSERT_EQ(t->getSucceedTests(), 3);
    ASSERT_EQ(t->getErrors(), 0);
    ASSERT_EQ(t->getFailedTests(), 1);
    string is = t->generateStringResult();
    //  ASSERT_EQ(is,
    		//             "[==========] \n[----------] \n[----------] [ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[       OK ] not defined class::not defined test \n[ RUN      ]\n[  FAILED  ] not defined class::not defined test \n               (<byte)\n\t\t\t\t     is<6> :\t 61 62 63 64 65 66   |  abcdef\n\t\t\t\t should<6> :\t 31 32 33 34 35 36   |  123456\n[ RUN      ]\n[       OK ] not defined class::not defined test \n\ntest run : 4  \t test failed : 1\t errors : 0");
}
//TODO testWith LineNumber
