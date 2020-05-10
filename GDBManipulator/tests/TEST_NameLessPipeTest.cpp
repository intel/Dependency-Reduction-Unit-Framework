//
// Created by basto on 4/12/18.
//
#include "gtest/gtest.h"
#include "comChain/decoder/NameLessPipe.h"

using namespace std;
#ifdef __linux__

void fork_and_read_from_STDIN_Write_To_stdOut(int mxsize, nameLessPipe *nlp) {
    char result_buff[mxsize + 7] = "read : ";
    if ((fork()) == 0) {

        nlp->redirectSTD();
        nlp->closeChild();
        int s = read(STDIN_FILENO, result_buff + 7, mxsize);
        write(STDOUT_FILENO, result_buff, s + 7);
    }

}

void fork_and_read_from_STDIN_Write_To_stdOut_EchoTO_STDERR(int mxsize, nameLessPipe *nlp) {
    char result_buff[mxsize + 7] = "read : ";

    if ((fork()) == 0) {

        nlp->redirectSTD();
        nlp->closeChild();
        int s = read(STDIN_FILENO, result_buff + 7, mxsize);
        write(STDOUT_FILENO, result_buff, s + 7);
        write(STDERR_FILENO, result_buff + 7, s);
    }

}


TEST(nameLessPipeWithFork, echoSTDIN) {
    nameLessPipe *nlp = new nameLessPipe();
    char buff[] = "hallo welt\n";
    char exep[] = "read : hallo welt\n";
    char res[20];
    nlp->writeToSTD_IN(buff, sizeof(buff));
    fork_and_read_from_STDIN_Write_To_stdOut(sizeof(buff), nlp);
    nlp->closeParent();
    int s = nlp->readFromSTD_OUT(res, sizeof(exep));
    ASSERT_EQ(s, sizeof(exep)); // + "read : "
    // match both directions
    ASSERT_STREQ(exep, res);
    ASSERT_STREQ(res, exep);


}

TEST(nameLessPipeWithFork, echoSTDERR) {
    nameLessPipe *nlp = new nameLessPipe();
    char buff[] = "hallo welt\n";
    char exep[] = "read : hallo welt\n";
    char res[100];
    nlp->writeToSTD_IN(buff, sizeof(buff));
    fork_and_read_from_STDIN_Write_To_stdOut_EchoTO_STDERR(19, nlp);
    nlp->closeParent();
    int s = nlp->readFromSTD_OUT(res, 19);
    // match both directions
    ASSERT_EQ(s, sizeof(exep)); // + "read : "
    ASSERT_STREQ(exep, res);
    s = nlp->readFromSTD_ERR(res, sizeof(buff));
    ASSERT_EQ(s, sizeof(buff)); // + "read : "
    ASSERT_STREQ(buff, res);

}

#endif
