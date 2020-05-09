/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#include "GdbOutputStream.h"

#include <sstream>
#include "LogFileFilter.h"
#include "Utils.h"
#include <fstream>

using namespace std;

std::mutex mutex1;

void gdbOutputStream::run() {
    usleep(100);
    //nameLessPipe1->closeParent()
#ifdef MAKE_SYSTEM_TEST
    ofstream createSystemTest("systemTest.client");
#endif

    while (true) {
    	asycnOutput *o = new asycnOutput;
        string s = nameLessPipe1->readFromSTD_OUT_Line();
#ifdef MAKE_SYSTEM_TEST
        createSystemTest <<s.c_str()<<endl;
#endif
        Log::log("Gdb output stream : got :\"" + s+"\"", DebugL2, logFilter::GDB_Output_Streams);
        if (s != "") {
            decodingStatus status = decoder->analyseLine(s, o);

            ///////////////////////////////////////
            //cout << "  -->> "<< s <<endl;
            //////////////////////////////////////
            if (status.info == decodeAdditionalInfo::failedToEvaluate) {
                utils::cleanUpAndExit(-2);
            }
            if (status.kind == decodeKindeOfDecodedLine::decodedAsyncOut ||
                status.kind == decodeKindeOfDecodedLine::decodeSyncronOut) {
                stringstream ss;
                ss << " handle GDB out :  state :" << decoder->getDebuggerState() << "  last command :"
                   << decoder->getLastCommand();
                if (o->mode != debuggerState::debuggerStateResetet)
                    ss << *o;
                Log::log(ss.str(), Debug, logFilter::GDB_Output_Streams);
                //cout << "  -->> "<< ss.str()<<endl;

                if (o->hasFrame && o->stoppedFrame->function != "") {
                    mutex1.lock();
                    stopFrames.push_back(o->stoppedFrame);
                    mutex1.unlock();
                } else if (o->hasFrame) {
                    cout << "got empty frame" << endl;
                }
            }
            if(s.find("+download,{section=") != string::npos){
            	cout << "." << flush;
            }
        }
    }

}

bool gdbOutputStream::hasStopFrames() {
    mutex1.lock();
    bool tmp = stopFrames.size() != 0;
    mutex1.unlock();
    return tmp;
}

stopFrame *gdbOutputStream::getLatestStopFrame() {
    mutex1.lock();
    stopFrame *tmp = stopFrames.back();
    stopFrames.pop_back();
    mutex1.unlock();
    return tmp;
}

bool gdbOutputStream::hasNextValue() {
    return decoder->hasNextValue();
}

string gdbOutputStream::getLastValue() {
    return decoder->getLatestValue();
}

gdbDecoder *gdbOutputStream::getdecoder() {
    return decoder;
}
