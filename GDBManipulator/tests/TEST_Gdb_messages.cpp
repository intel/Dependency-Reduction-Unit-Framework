//
// Created by basto on 2/20/18.
//

#include <Utils.h>
#include "gtest/gtest.h"
#include "comChain/decoder/GdbDecoder.h"

using namespace std;

TEST(GDB_Message_Async_out, Message_From_BreakPoint) {
    string s = "*stopped,stopFrame={addr=\"0x08000cb4\",func=\"Reset_Handler\",args=[],file=\"../src/startup_stm32f40xx.s\","
               "fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s\",line=\"76\"},"
               "thread-id=\"1\",stopped-threads=\"all\"";
    gdbDecoder *dec = new gdbDecoder();
    asycnOutput *o = new asycnOutput();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::stopped);
    ASSERT_TRUE(o->hasStoppedThreads);
    ASSERT_FALSE(o->hasReason);
    ASSERT_FALSE(o->hasSignal);
    ASSERT_TRUE(o->hasFrame);
    ASSERT_EQ(o->mode, debuggerHasStopped);
    ASSERT_EQ(o->stoppedFrame->addr, 134220980);
    ASSERT_EQ(o->stoppedFrame->function, "Reset_Handler");
    ASSERT_EQ(o->stoppedFrame->args.size(), 0);
//    ASSERT_EQ(o->stoppedFrame->file,"../src/startup_stm32f40xx.s");
    //   ASSERT_EQ(o->stoppedFrame->fullname,"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s");
    ASSERT_EQ(o->threadID, 1);
    ASSERT_EQ(o->stoppedThreads, "all");
    ASSERT_EQ(dec->getDebuggerState(), debuggerHasStopped);
    delete o;
    delete dec;

}

TEST(GDB_Message_Async_out, Basic_Message) {
    string s = "*running,thread-id=\"all\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    ASSERT_EQ(dec->getDebuggerState(), debuggerHasStopped);

    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::running);

    ASSERT_FALSE(o->hasStoppedThreads);
    ASSERT_FALSE(o->hasReason);
    ASSERT_FALSE(o->hasSignal);
    ASSERT_FALSE(o->hasFrame);
    ASSERT_EQ(o->mode, debuggerIsRunning);
    ASSERT_EQ(dec->getDebuggerState(), debuggerIsRunning);
    delete o;
    delete dec;
}

TEST(GDB_Message_Async_out, Message_With_Unkown_elf) {
    string s = "*stopped,stopFrame={addr=\"0x00000000\",func=\"??\",args=[]},thread-id=\"1\",stopped-threads=\"all\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::stopped);

    ASSERT_TRUE(o->hasStoppedThreads);
    ASSERT_FALSE(o->hasReason);
    ASSERT_FALSE(o->hasSignal);
    ASSERT_TRUE(o->hasFrame);
    ASSERT_EQ(o->mode, debuggerHasStopped);
    ASSERT_EQ(o->threadID, 1);
    ASSERT_EQ(o->stoppedThreads, "all");
    ASSERT_EQ(o->stoppedFrame->addr, 0);
    ASSERT_EQ(o->stoppedFrame->function, "??");
    ASSERT_EQ(o->stoppedFrame->args.size(), 0);
    delete o;
    delete dec;
}

TEST(GDB_Message_Async_out, Message_With_Signal) {
    string s = "*stopped,reason=\"signal-received\",signal-name=\"SIGTRAP\",signal-meaning=\"Trace/breakpoint trap\",stopFrame={addr=\"0x08000a56\","
               "func=\"MessageToDebugger\",args=[{name=\"message\",value=\"bufferIsFull\"}],file=\"../src/TestSuite/testSuite.c\","
               "fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/TestSuite/testSuite.c\",line=\"107\"},thread-id=\"3\",stopped-threads=\"all\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::stopped);
    ASSERT_TRUE(o->hasStoppedThreads);
    ASSERT_TRUE(o->hasReason);
    ASSERT_TRUE(o->hasSignal);
    ASSERT_TRUE(o->hasFrame);
    ASSERT_EQ(o->mode, debuggerHasStopped);
    ASSERT_EQ(o->threadID, 3);
    ASSERT_EQ(o->stoppedThreads, "all");
    ASSERT_EQ(o->stoppedFrame->addr, 0x08000a56);
    ASSERT_EQ(o->stoppedFrame->function, "MessageToDebugger");
    ASSERT_EQ(o->stoppedFrame->args.size(), 1);
    functionArgument *arg = o->stoppedFrame->args.front();
    ASSERT_EQ(arg->name, "message");
    ASSERT_EQ(arg->value, "bufferIsFull");
    // ASSERT_EQ(*o->stoppedFrame->args,"{name=\"message\",value=\"bufferIsFull\"}");
    ASSERT_EQ(o->signal, "SIGTRAP");
    ASSERT_EQ(o->reason, "signal-received");
    ASSERT_EQ(o->signalMeaning, "Trace/breakpoint trap");
    delete o;
    delete dec;
}

TEST(GDB_Message_Async_And_Sync_out, Decode_Several_Messages) {
    string s1 = "*stopped,stopFrame={addr=\"0x08000cb4\",func=\"Reset_Handler\",args=[],file=\"../src/startup_stm32f40xx.s\","
                "fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s\",line=\"76\"},"
                "thread-id=\"1\",stopped-threads=\"all\"";
    string s2 = "*running,thread-id=\"all\"";
    string s3 = "*stopped,reason=\"signal-received\",signal-name=\"SIGTRAP\",signal-meaning=\"Trace/breakpoint trap\",stopFrame={addr=\"0x08000a56\","
                "func=\"MessageToDebugger\",args=[{name=\"message\",value=\"bufferIsFull\"}],file=\"../src/TestSuite/testSuite.c\","
                "fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/TestSuite/testSuite.c\",line=\"107\"},thread-id=\"3\",stopped-threads=\"all\"";

    string s4 = "^done";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    ASSERT_EQ(dec->analyseLine(s1, o).kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(dec->analyseLine(s2, o).kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_FALSE(o->hasStoppedThreads);
    ASSERT_FALSE(o->hasReason);
    ASSERT_FALSE(o->hasSignal);
    ASSERT_FALSE(o->hasFrame);
    ASSERT_EQ(o->mode, debuggerIsRunning);

    ASSERT_EQ(dec->getLastCommand(), waitForResult);

    ASSERT_EQ(dec->analyseLine(s3, o).kind, decodeKindeOfDecodedLine::decodedAsyncOut);

    decodingStatus status = dec->analyseLine(s4, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodeSyncronOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::done);;
    ASSERT_EQ(dec->getDebuggerState(), debuggerState::debuggerHasStopped);
    ASSERT_EQ(dec->getLastCommand(), commandState::debuggerDone);
    //ASSERT_TRUE(o->hasStoppedThreads);
    ASSERT_TRUE(o->hasReason);
    ASSERT_TRUE(o->hasSignal);
    /*/ASSERT_TRUE(o->hasFrame);
    ASSERT_EQ(o->mode,debuggerHasStopped);
    ASSERT_EQ(o->threadID, 3);
    ASSERT_EQ(o->stoppedThreads,"all");
    ASSERT_EQ(o->stoppedFrame->addr,0x08000a56);
    ASSERT_EQ(o->stoppedFrame->function,"MessageToDebugger");
    ASSERT_EQ(o->stoppedFrame->args.at(0)->value,"bufferIsFull");
    ASSERT_EQ(o->stoppedFrame->args.at(0)->name,"message");
    //ASSERT_EQ(*o->stoppedFrame->args,"{name=\"message\",value=\"bufferIsFull\"}");
    ASSERT_EQ(o->signal,"SIGTRAP");
    ASSERT_EQ(o->reason,"signal-received");
    ASSERT_EQ(o->signalMeaning,"Trace/breakpoint trap");
     */
    delete o;
    delete dec;
}

TEST(GDB_Message_Sync_out, Decode_Sync_Message) {
    string s = "^done,bkpt={number=\"1\",type=\"breakpoint\",disp=\"keep\",enabled=\"y\",addr=\"0x08000844\",func=\"main\",file=\"../src/main.c\",fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/main.c\",line=\"67\",thread-groups=[\"i1\"],times=\"0\",original-location=\"main\"}";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodeSyncronOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::done);
    delete o;
    delete dec;
}

TEST(GDB_Message_Async_And_Sync_out, Decode_Frame_With_4_arg) {
    string s = "*stopped,reason=\"signal-received\",signal-name=\"SIGTRAP\",signal-meaning=\"Trace/breakpoint trap\",frame={addr=\"0x0800089c\",func=\"magic\",args=[{name=\"a\",value=\"0x2001ffd0\"},{name=\"sizeA\",value=\"4\"},{name=\"b\",value=\"0x2001ffc8\"},{name=\"sizeB\",value=\"7\"}],file=\"../src/TestSuite/testSuite.c\",fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/TestSuite/testSuite.c\",line=\"49\"},thread-id=\"1\",stopped-threads=\"all\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(o->stoppedFrame->args.size(), 4);
    functionArgument *a1 = o->stoppedFrame->args.at(0);
    functionArgument *a2 = o->stoppedFrame->args.at(1);
    functionArgument *a3 = o->stoppedFrame->args.at(2);
    functionArgument *a4 = o->stoppedFrame->args.at(3);
    ASSERT_EQ(a1->name, "a");
    ASSERT_EQ(a1->value, "0x2001ffd0");
    ASSERT_EQ(a2->name, "sizeA");
    ASSERT_EQ(a2->value, "4");
    ASSERT_EQ(a3->name, "b");
    ASSERT_EQ(a3->value, "0x2001ffc8");
    ASSERT_EQ(a4->name, "sizeB");
    ASSERT_EQ(a4->value, "7");
    delete o;
    delete dec;
}


TEST(GDB_Message_Sync_out, Decode_ERROR_Message) {
    utils::sharedMemoryInit();
    string s = "^error,msg=\"Undefined MI command: xx\",code=\"undefined-command\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodeSyncronOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::error);
    ASSERT_EQ(dec->getLastCommand(), debuggerError);
    delete o;
    delete dec;
}

TEST(GDB_Message_Sync_out, Decode_Two_MemoryDumps) {

    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"0120\"}]";
    string s2 = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"0230\"}]";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodeSyncronOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::doneWithMemory);

    dec->analyseLine(s2, o);
    ASSERT_TRUE(dec->hasNextDump());
    memoryDump *d1 = dec->getNextDump();
    ASSERT_TRUE(dec->hasNextDump());
    memoryDump *d2 = dec->getNextDump();
    ASSERT_FALSE(dec->hasNextDump());
    ASSERT_EQ(d1->getDump()[0], 1);
    ASSERT_EQ(d1->getDump()[1], 32);
    ASSERT_EQ(d2->getDump()[0], 2);
    ASSERT_EQ(d2->getDump()[1], 48);
    delete d1;
    delete d2;
    delete o;
    delete dec;
}


TEST(GDB_Message_Sync_out, Decode_Memory_Failed) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"00000\"}]";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodeSyncronOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::failedToDecodeMem);
    delete o;
    delete dec;
}

TEST(GDB_Message_Sync_out, Decode_Value) {
    string s = "^done,value=\"94 '^'\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_TRUE(dec->hasNextValue());
    ASSERT_EQ(dec->getLatestValue(), "94");

    delete o;
    delete dec;
}

TEST(GDB_Message_Async_out, Decode_Stopp_Frame) {
    string s = "*stopped,frame={addr=\"0x08000ce8\",func=\"Reset_Handler\",args=[],file=\"../src/startup_stm32f40xx.s\",fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s\",line=\"76\"},thread-id=\"1\",stopped-threads=\"all\"";
    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);
    ASSERT_EQ(status.kind, decodeKindeOfDecodedLine::decodedAsyncOut);
    ASSERT_EQ(status.info, decodeAdditionalInfo::stopped);

    ASSERT_EQ(dec->getDebuggerState(), debuggerState::debuggerHasStopped);
    ASSERT_EQ(o->stoppedFrame->function, "Reset_Handler");
    ASSERT_EQ(o->stoppedFrame->args.size(), 0);
    ASSERT_EQ(o->stoppedFrame->addr, 134221032);

    delete o;
    delete dec;
}

TEST(GDB_Message_MemoryDec, decode_With_Wronge_Next_Size) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"000\"}]";
    memoryDump *m = new memoryDump(&s);
    ASSERT_FALSE(m->decodeDump());
    delete m;
}

TEST(GDB_Message_MemoryDec, decode_With_Wronge_DumpSize) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"000\"}]";
    memoryDump *m = new memoryDump(&s);
    ASSERT_FALSE(m->decodeDump());
    delete m;
    s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"000\"}]";
    m = new memoryDump(&s);
    ASSERT_FALSE(m->decodeDump());
    delete m;

}

TEST(GDB_Message_MemoryDec, decode_With_Wronge_Addr) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000105\",contents=\"000000\"}]";
    memoryDump *m = new memoryDump(&s);
    ASSERT_FALSE(m->decodeDump());
    delete m;


}

TEST(GDB_Message_MemoryDec, decode_With_Zeros) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000106\",contents=\"0000\"}]";
    memoryDump *m = new memoryDump(&s);
    ASSERT_TRUE(m->decodeDump());
    ASSERT_EQ(m->getDump()[0], 0);
    ASSERT_EQ(m->getDump()[1], 0);
    delete m;

}

TEST(GDB_Message_MemoryDec, decode_With_Data) {
    string s = "^done,memory=[{begin=\"0x20000104\",offset=\"0x00000000\",end=\"0x20000108\",contents=\"0102Ab00\"}]";
    memoryDump *m = new memoryDump(&s);
    ASSERT_TRUE(m->decodeDump());
    ASSERT_EQ(m->getDump()[0], 1);
    ASSERT_EQ(m->getDump()[1], 2);
    ASSERT_EQ(m->getDump()[2], 171);
    ASSERT_EQ(m->getDump()[3], 0);
    delete m;

}
//"^done,frame={level=\"0\",addr=\"0x08000d60\",func=\"when\",args=[{name=\"ifCalled\",value=\"0x8001350 \\\"add5\\\"\"},{name=\"thenDo\",value=\"0x8001344 \\\"return55()\\\"\"}],file=\"..\\\\src\\\\Test\\\\testSuite/testSuite.c\",fullname=\"C:\\\\Users\\\\sbalz\\\\Documents\\\\git\\\\embeddedtesting\\\\EmbeddedTesting\\\\Debug/..\\\\src\\\\Test\\\\testSuite/testSuite.c\",line=\"266\"}"
TEST(GDB_Message_Async_out, meeningOfArgument) {
    string s = "^done,frame={level=\"0\",addr=\"0x08000d60\",func=\"when\",args=[{name=\"ifCalled\",value=\"0x8001350 "
               "\\\"add5\\\"\"},{name=\"thenDo\",value=\"0x8001344 \\\"return55()\\\"\"}],file=\"..\\\\src\\\\Test\\\\"
               "testSuite/testSuite.c\",fullname=\"C:\\\\Users\\\\sbalz\\\\Documents\\\\git\\\\embeddedtesting\\\\"
               "EmbeddedTesting\\\\Debug/..\\\\src\\\\Test\\\\testSuite/testSuite.c\",line=\"266\"}";

    asycnOutput *o = new asycnOutput();
    gdbDecoder *dec = new gdbDecoder();
    decodingStatus status = dec->analyseLine(s, o);

    ASSERT_EQ(o->stoppedFrame->args.size(), 2);
    functionArgument *arg = o->stoppedFrame->args.at(0);
    ASSERT_EQ(arg->name, "ifCalled");
    ASSERT_EQ(arg->value, "0x8001350");
    ASSERT_EQ(arg->meaning, "add5");

    arg = o->stoppedFrame->args.at(1);
    ASSERT_EQ(arg->name, "thenDo");
    ASSERT_EQ(arg->value, "0x8001344");
    ASSERT_EQ(arg->meaning, "return55()");

    delete o;
    delete dec;
}


// "*stopped,frame={addr=\"0x08000ce8\",func=\"Reset_Handler\",args=[],file=\"../src/startup_stm32f40xx.s\",fullname=\"/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/startup_stm32f40xx.s\",line=\"76\"},thread-id=\"1\",stopped-threads=\"all\""

