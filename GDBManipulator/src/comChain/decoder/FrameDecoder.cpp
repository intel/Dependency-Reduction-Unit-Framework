/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "FrameDecoder.h"

#include "LogFileFilter.h"
#include "Utils.h"

/*
 * normal
 * "frame={addr=\"0x08001014\",func=\"Reset_Handler\",args=[],file=\"../src/startup_stm32f40xx.s\",fullname=\"/home/xxx/git/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s\",line=\"76\"},thread-id=\"1\",stopped-threads=\"all\""
 * -exec-return frame
 * "frame={level=\"0\",addr=\"0x08000c12\",func=\"addStringToBuffer\",args=[{name=\"name\",value=\"0x80012a8 \\\"EQ_SUCCESS\\\"\"},{name=\"size\",value=\"11\"},{name=\"isTestName\",value=\"typeOfString_testClass\"}],file=\"../src/Test/testSuite/testSuite.c\",fullname=\"/home/xx/git/EmbeddedTesting/EmbeddedTesting/src/Test/testSuite/testSuite.c\",line=\"219\"}"
 *
 */
stopFrame *frameDecoder::decodeFrame(string *line) {
    if (!(utils::startsWith(line, "frame") || utils::startsWith(line, "stopFrame"))) {
        Log::log("DecodeFram : expected frame but was \"" + *line+"\"", Error, logFilter::gdbMI2decoder);

        return nullptr;
    }
    stopFrame *f = new stopFrame();

    if (utils::startsWith(line, "frame={level")) {
        line->erase(0, line->find('"') + 1);
        line->erase(0, line->find('"') + 1);
    }


    line->erase(0, line->find('"') + 1);
    string strAddr = line->substr(0, line->find('"'));
    f->addr = strtoul(strAddr.c_str(), 0, 16);//atoi(strAddr.c_str());

    line->erase(0, line->find('"') + 1);
    line->erase(0, line->find('"') + 1);
    f->function = line->substr(0, line->find('"'));
    line->erase(0, line->find('"') + 1);
    line->erase(0, line->find('=') + 2);
    decodeFunctionArgs(line, f);
    //f->args = new string(line->substr(0,line->find(']')));
    line->erase(0, line->find('}') + 2);

    return f;
}

/*
 * normal
 *     {name=\"message\",value=\"startTesting\"}]
 * -exe-c return
 */
void frameDecoder::decodeFunctionArgs(string *line, stopFrame *frame) {

    while (line->at(0) != ']') {
        functionArgument *arg = new functionArgument;
        line->erase(0, line->find("name=\"") + 6);
        arg->name = line->substr(0, line->find('"'));
        line->erase(0, line->find("value=") + 7);
        arg->value = line->substr(0, line->find('"'));
        line->erase(0, line->find('"') + 1);
        if (line->at(0) == '}')
            line->erase(0, 1);
        else {
            arg->meaning = line->substr(0, line->find('"') - 1);
            line->erase(0, line->find('}') + 1);
            arg->value = arg->value.substr(0, arg->value.size() - 2);
        }

        frame->args.push_back(arg);
    }
    line->erase(0, 1);

}


std::ostream &operator<<(std::ostream &strm, const stopFrame &a) {
    strm << "stopFrame : { ";
    strm << "function : " << a.function << " ";
    strm << "stopped_at : " << a.addr << " ";
    strm << "arguments :[";
    string s = "";
    // arguments
    bool first = true;
    for (unsigned int i = 0; i < a.args.size(); i++) {

        if (!first) {
            s += " , ";
        } else
            first = false;
        s += "{ name: " + a.args.at(i)->name + " , value : " + a.args.at(i)->value + "}";
    }
    strm << s<<"] ";

    return strm << " } ";
}
