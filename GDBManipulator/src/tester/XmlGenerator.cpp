/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#include "XmlGenerator.h"

void xmlGenerator::addTestCase(string name, string failedMessage) {
    testRunClass++;
    testCases += "    <testcase name=\"" + name + "\" status=\"run\" time=\"0.000\" classname=\"" + className + "\">\n";
    if (failedMessage != "") {
        testFailedClass++;
        testCases += "      <failure message=\"" + failedMessage + "\"></failure>\n";
    }
    testCases += "    </testcase>\n";
}

void xmlGenerator::addTestClass(string testClass) {
    closeClass();
    className = testClass;
    testCases = "";
}

void xmlGenerator::closeClass() {
    if (testRunClass != 0) {
        testFailedTotal += testFailedClass;
        testRunTotal += testRunClass;
        output += "  <testsuite name=\"" + className + "\" tests=\"" + to_string(testRunClass) + "\" failures=\"" +
                  to_string(testFailedClass) + "\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n";
        output += testCases;
        output += " </testsuite>\n";
        testRunClass = 0;
        testFailedClass = 0;
    }
}

string xmlGenerator::buildXml(int errors, string timeStamp, float runtime) {
    string o = "";
    if (errors > 0) {
        addTestClass("test evaluation");
        addTestCase("target communication result",
                    "failed with " + to_string(errors) + " failure. For further information's see logfile(if enabled)");
    }
    closeClass();

    o += xmlHeader;
    o += "<testsuites tests=\"" + to_string(testRunTotal) + "\" failures=\"" + to_string(testFailedTotal) +
         "\" disabled=\""
         "0\" errors=\"" + to_string(errors) + "\" timestamp=\"" + timeStamp + "\" time=\"" + to_string(runtime) + "\" "
                                                                                                                   "name=\"AllTests\">\n";
    o += output;
    o += "</testsuites>";
    if (outFile != "") {
        ofstream *outf = new ofstream();
        outf->open(outFile);
        outf->write(o.c_str(), o.length());
        outf->flush();
    }
    return o;
}
