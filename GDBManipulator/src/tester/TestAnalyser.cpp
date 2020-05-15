/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#include "TestAnalyser.h"

#include <sstream>
#include <iomanip>
#include "LogFileFilter.h"
#include "TestStatistic.h"
#include "Utils.h"

using namespace std;


bool testAnalyser::analyse(memoryDump *dump) {

    switch (amountOfResultData) {
        case amountOf_testResults :
            return analyseResults(dump);
        case amountOf_testNames:
            return analyseSuccess(dump);
        case amountOf_justSuccess :
            return analyseSuccess(dump);
        case amountOf_lineNumbers:
            return analyseResults(dump);

    }
    return false;
}

string testAnalyser::generateStringResult() {
    string s = "";
    s += "[----------] "+to_string(testPerGroup)+" tests from "+currentClassName+"\n[==========]\n\n";
    if (!printLiveToCli)
        s += resultString + "\n";
    return s += "Test run : " + to_string(failed + succeed) + "  \t Test failed : " + to_string(failed) +
                "\t Errors : " + to_string(errors);
}

bool testAnalyser::analyseResults(memoryDump *dump) {
    uint8_t *data;
    int failedInLine;
    string errorMessage = "";
    string messageFromTarget;
    typeOfString messageIsNewTestName;
    if (dump->decodeDump()) {
        data = dump->getDump();
        for (int i = 0; i < dump->getSize(); i++) {
            string s;
            for (int shift = 0; shift < 8; shift += 2) {
                switch ((typeOfData) ((data[i]) >> shift) & 0b11) {
                    case typeOfData::testSucceed    :
                        s = TESTOK + " " + currentClassName + "::" + currentTestName;
                        succeed++;
                        if (xml != nullptr)
                            xml->addTestCase(currentTestName, "");
                        break;
                    case typeOfData::testName       :
                        shift = 8;
                        i += 1; // type of string
                        messageIsNewTestName = (typeOfString) data[i];
                        messageFromTarget = string((char *) data + i + 5);

                        if (messageIsNewTestName == typeOfString_testName){
                            currentTestName = messageFromTarget;
                            Log::log("got new Test : " + currentTestName, Message, logFilter::Test_Analyser);
                            s+=RUNTEST;
                            testPerGroup++;
                        }

                        else if (messageIsNewTestName == typeOfString_testFile) {
                            Log::log("got new file name : " + messageFromTarget, Message, logFilter::Test_Analyser);
                            currentFileName = messageFromTarget;
                        } else if (messageIsNewTestName == typeOfString_testClass) {
                            Log::log("got new class : " + messageFromTarget, Message, logFilter::Test_Analyser);
                            if(currentClassName != "" && testPerGroup> 0){
                                s += "[----------] "+to_string(testPerGroup)+" tests from "+currentClassName;
                            	testPerGroup = 0;
                            }
                            s+= "\n\n[==========] " + messageFromTarget;
                            currentClassName = messageFromTarget;
                            testStatistic::setTestGroupName(currentClassName);
                            if (xml != nullptr)
                                xml->addTestClass(currentClassName);
                        } else if(messageIsNewTestName == typeOfString_gdbMessage)
						{
                        	Log::log("got new user defines message : " + messageFromTarget, Message, logFilter::Test_Analyser);
                        	cout << messageFromTarget;
						}else
						{
                            Log::log("got undefined type of string " + to_string(messageIsNewTestName), Error,
                                     logFilter::Test_Analyser);
                            errors++;
                        }
                        i += (unsigned int) data[i + 1] + 4;
                        break;
                    case typeOfData::testFailed     :
                        shift = 8;
                        s = TESTFAIL + " " + currentClassName + "::" + currentTestName;

                        if (amountOfResultData >= amountOfInfo::amountOf_testResults) {
                            if (amountOfResultData == amountOf_lineNumbers) {
                                failedInLine = *(int *) (data + i + 1);
                                i += 4;
                                s += " \t\tfailed at : " + currentFileName + ":" + to_string(failedInLine);
                            }
                            errorMessage = analyseFailedResult(data, &i);
                            s += errorMessage;
                            // i += 1; // type
                            //i += (unsigned int) data[i + 1] + 4; // is
                            //i += (unsigned int) data[i + 1] + 4; // should
                        }
                        failed++;
                        if (xml != nullptr) {
                            xml->addTestCase(currentClassName, errorMessage);
                        }
                        break;
                    case typeOfData::testNotRun :
                        if (shift > 0 && i < dump->getSize() &&
                            data[i + 1] != 0) { // check if the next byte is also empty
                            shift = 8;
                            s = "";
                            break;
                        }
                        s = "test end";
                        return true;
                    default:
                        s = "error";
                        Log::log("Failed to decode target buffer", Error, logFilter::Test_Analyser);
                        errors++;
                        return false;
                }

                if (s != "") {
                    if (printLiveToCli) {
                        cout << s << endl;
                    }
                    // Log::log_(s,Debug,33);
                    resultString += s + "\n";
                }
            }
        }
    }
    return false;
}

/**
 * s = test Success = 0b01
 * f = test failed  = 0b10
 * x = test not run = 0b00
 * n = test name    = 0b11
 *                                                  7-6     5-4     3-2     1-0
 * Single result without test name : byte :: b = [(s|f|x),(s|f|x),(s|f|x),(s|f|x)]
 * Single result with test name : b = [ ..., ... n,(s|f)]
 *
 * does the byte contain a testName flag the rest of the byte gets ignored the name frame starts with the next byte
 *
 * result buffer = b,b,b,b... until first byte contains an x
 * @param dump
 * @return
 */
bool testAnalyser::analyseSuccess(memoryDump *dump) {
    uint8_t *data;
    if (dump->decodeDump()) {
        data = dump->getDump();

        for (int i = 0; i < dump->getSize(); i++) {
            string s;
            for (int shift = 0; shift < 8; shift += 2) {
                uint8_t res = (data[i] >> shift) & 0b11;
                if (res == typeOfData::testSucceed) {
                    if (amountOfResultData > amountOfInfo::amountOf_justSuccess)
                        s = TESTOK + " " + currentClassName + "::" + currentTestName;
                    else
                        s = TESTOK;
                    succeed++;
                    if (xml != nullptr)
                        xml->addTestCase(currentTestName, "");
                } else if (res == typeOfData::testName) {
                	testPerGroup++;
                    if (amountOfResultData == amountOf_justSuccess) {
                        Log::log("failed there should be no testName ", Error, logFilter::Test_Analyser);
                        s = "failed there should be no testName ";
                        errors++;
                    } else {
                        string name = string((char *) data + i + 6);
                        typeOfString stringContent = (typeOfString) *(data + i + 1);
                        if (stringContent == typeOfString_testName) {
                            Log::log("got new test name : " + name, Message, logFilter::Test_Analyser);
                            currentTestName = name;
                            s+= RUNTEST;
                        } else if (stringContent == typeOfString_testClass) {
                            Log::log("got new class : " + name, Message, logFilter::Test_Analyser);
                            if(currentClassName != "" && testPerGroup> 0){
                                s += "[----------] "+to_string(testPerGroup)+" tests from "+currentClassName;
                                testPerGroup = 0;
                            }
                            s+= "\n\n[==========] " + stringContent;
                            currentClassName = name;
                            testStatistic::setTestGroupName(currentClassName);
                            if (xml != nullptr)
                                xml->addTestClass(name);
                        }else if(stringContent == typeOfString_gdbMessage)
						{
                        	Log::log("got new user defines message : " + stringContent, Message, logFilter::Test_Analyser);
                        	s+=stringContent+'\0';
						} else {
                            Log::log("got undefined type of string " + to_string(data[i + 1]), Error,
                                     logFilter::Test_Analyser);
                            errors++;
                        }
                        i += (unsigned int) data[i + 2] + 5;

                    }
                } else if (res == typeOfData::testFailed) {
                    s = TESTFAIL;
                    if (currentClassName != "")
                        s += " " + currentClassName + "::";
                    s += currentTestName;
                    failed++;
                    if (xml != nullptr)
                        xml->addTestCase(currentTestName, "to enable fail message set a testLevel > justSuccess");
                } else if (res == typeOfData::testNotRun) {
                    if (shift > 0 && i < dump->getSize() && data[i + 1] != 0) { // check if the next byte is also empty
                        shift = 8;
                        s = "";
                        break;
                    }
                    s = "test end";
                    return true;
                } else {
                    s = "error";
                    errors++;
                    return false;
                }

                if (printLiveToCli) {
                    cout << s << endl;
                } else {
                    // avoid duplicate line on the cli
                    Log::log("Analyzes result : " + s, Message, logFilter::Test_Analyser);
                }
                resultString += s + "\n";

            }

        }


    }
return false;
}

unsigned int testAnalyser::getSucceedTests() const {
    return succeed;
}

unsigned int testAnalyser::getFailedTests() const {
    return failed;
}

unsigned int testAnalyser::getErrors() const {
    return errors;
}

amountOfInfo testAnalyser::getAmountOfResultData() const {
    return amountOfResultData;
}


string testAnalyser::arrayAnalyse(uint8_t *startIs, int sizeIs, uint8_t *startShould, int sizeShould) {
    std::stringstream hexIs, hexShould;
    string asciShould = "";
    string asciIs = "";
    for (int i = 0; i < sizeIs || i < sizeShould; i++) {

        if (i < sizeIs && i < sizeShould) {
            hexIs << std::setfill('0') << std::setw(2) << std::hex << (int) (startIs[i]) << " ";
            if (startIs[i] == startShould[i]) {
                hexShould << "** ";
            } else {
                hexShould << std::setfill('0') << std::setw(2) << std::hex << (int) (startShould[i]) << " ";
            }
        } else {
            if (i < sizeIs)
                hexIs << std::setfill('0') << std::setw(2) << std::hex << (int) (startIs[i]) << " ";
            else
                hexIs << "   ";
            if (i < sizeShould)
                hexShould << std::setfill('0') << std::setw(2) << std::hex << (int) (startShould[i]) << " ";
            else
                hexShould << "   ";
        }


        if (i < sizeIs) {
            if (startIs[i] >= 33 && startIs[i] <= 124)
                asciIs += (char) (int) (startIs[i]);
            else
                asciIs += ".";
        } else
            asciIs += "";

        if (i < sizeShould) {
            if (startShould[i] >= 33 && startShould[i] <= 124)
                asciShould += (char) (int) (startShould[i]);
            else if (i < sizeShould)
                asciShould += ".";
        } else
            asciShould += " ";

    }
    string s = hexIs.str();

    string is = "\n\t\t\t\t     is<" + to_string(sizeIs) + "> :\t " + hexIs.str() + "  |  " + asciIs;
    string should = "\n\t\t\t\t should<" + to_string(sizeShould) + "> :\t " + hexShould.str() + "  |  " + asciShould;
    return is + should;
}

string testAnalyser::analyseFailedResult(uint8_t *data, int *i) {
    *i += 1;
    string out = "\n               (";
    int sizeIs = 0;
    int sizeShould = 0;
    string testKind;
    if ((data[*i] & testTypeFlags::typeCompEqual) > 0)
        testKind = "\tto be equals to : ";
    else
        testKind = "\tnot to be equals to : ";

    testTypeFlags dataType = (testTypeFlags) (data[*i] & 0b111); // Test Type
    bool unSig = (data[*i] & testTypeFlags::typeCompUnsigned) > 0;
    bool array = (testTypeFlags) (data[*i] & testTypeFlags::typeCompArray) > 0;
    if((testTypeFlags) (data[*i] & testTypeFlags::typeTimeOut) > 0){
    	*i+=2;
    	return "\n               failed because of timeout";
    }
    if (unSig)
        out += "unsigned ";
    if (array)
        out += "<array>";

    switch (dataType) {
        case testTypeFlags::typeNotDef :
            out += "undef dataType) : ";
            // Byte und aski left/right
            sizeIs = *(int *) (data + *i + 1);
            sizeShould = *(int *) (data + *i + 1 + 4 + sizeIs);
            out += arrayAnalyse(data + *i + 5, sizeIs, data + *i + 5 + sizeIs + 4, sizeShould);
            break;
        case testTypeFlags::typeCompInt8 :
            out += "int8) "; // t s s s s d s s s s d
            sizeIs = 1;
            sizeShould = 1;
            out += "Expected: " + to_string(*((int8_t *) data + 1 + *i));
            out += testKind + to_string(*((int8_t *) data + sizeIs + 1 + *i));
            break;
        case testTypeFlags::typeCompInt16 :
            out += "int16) ";
            sizeIs = 2;
            sizeShould = 2;
            out += "Expected: " + to_string(*((int16_t *) (data + 1 + *i)));
            out += testKind + to_string(*((int16_t *) (data + sizeIs + 1 + *i)));
            break;
        case testTypeFlags::typeCompInt32 :
            out += "int32) ";// t s s s s d d d d s s s s d d d d
            sizeIs = 4;
            sizeShould = 4;
            out += "Expected: " + to_string(*((int32_t *) (data + 1 + *i)));
            out += testKind + to_string(*((int32_t *) (data + sizeIs + 1 + *i)));
            break;
        case testTypeFlags::typeCompByte :
            out += "<byte)";
            sizeIs = *(uint32_t *) (data + *i + 1);
            sizeShould = *(uint32_t *) (data + *i + 1 + 4 + sizeIs);
            out += arrayAnalyse(data + *i + 5, sizeIs, data + *i + 5 + sizeIs + 4, sizeShould);
            sizeIs += 4;
            sizeShould += 4;
            break;
        default:
            errors++;
            Log::log("Not defined test type flag \"" + to_string(dataType)+"\"", Error, logFilter::Test_Analyser);
    }
    *i += sizeIs + sizeShould;
    return out;


}

xmlGenerator *testAnalyser::getXmlGenerator() {
    return xml;
}

bool testAnalyser::analyseDirect(string directData) {
    if (utils::startsWith(&directData, "{testStatus = testFailed")) {
        failed++;
        string m = RUNTEST + "\n" + TESTFAIL + "\t" + "read data direct from Target : " + directData;
        resultString += m + "\n";
        if (printLiveToCli)
            cout << m << endl;
        if (xml != nullptr) {
            xml->addTestCase(currentClassName, "read data direct from Target : " + directData);
        }
    } else if (utils::startsWith(&directData, "{testStatus = testSucceed")) {
        succeed++;

        resultString += RUNTEST + "\n" + TESTOK + "\n";
        if (printLiveToCli)
            cout << RUNTEST + "\n" + TESTOK << endl;
    } else if (utils::startsWith(&directData, "buffer to sma")) {
        Log::log("The targets buffer is to small. To add this to the test data increase the targets buffer size \t : " + directData, Message);
    } else if (directData != "") {
        Log::log("Unknown direct buffer data: \"" + directData+"\"", CriticError);
    }

    return true;
}

void testAnalyser::addError() {
    errors++;
}

