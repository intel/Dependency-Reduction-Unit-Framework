//
// Created by basto on 2/1/19.
//

#include <gtest/gtest.h>

using namespace std;

TEST(utilsTest, determineLocationWithPath) {
    string command = "abc/def/xx";
    int index = command.find_last_of("/");
    string location = command.substr(0, index);
    ASSERT_EQ("abc/def", location);
}

TEST(utilsTest, determineLocationWithOutPath) {
    string command = "xx";
    int index = command.find_last_of("/");
    if (index < 0)
        index = 0;
    string location = command.substr(0, index);
    ASSERT_EQ("", location);
}

TEST(utilsTest, determineLocationWithPath2) {
    string command = "/xx";
    int index = command.find_last_of("/");
    if (index < 0)
        index = 0;
    string location = command.substr(0, index);
    ASSERT_EQ("", location);
}


