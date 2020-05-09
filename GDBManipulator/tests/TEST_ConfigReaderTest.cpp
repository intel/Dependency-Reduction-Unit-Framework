//
// Created by basto on 10/16/18.
//

#include <gtest/gtest.h>
#include "../lib/CPP-Argument-Parser/src/configFileReader.h"


TEST(configReader, readEmptyText) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readEmptyTextSpace) {
    configFileReader *reader = new configFileReader(" ");
    reader->inputSourceIsString(" ");
    ASSERT_TRUE(!reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readEmptyTextSpaceOften) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString(" ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readEmptyTextManySpace) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("    ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSingeChar) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("a");
    ASSERT_EQ(reader->readUntilNextSeparator(), "a");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSingeWord) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSingeWordSpace) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readS2Word2Space) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc def ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSomeCharsSomeSpace) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc   def ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}


TEST(configReader, readSomeWordSomeSpace) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc   def ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSomeWordSomeSpace2) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc   def hallo welt  i am a reader");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(), "welt");
    ASSERT_EQ(reader->readUntilNextSeparator(), "i");
    ASSERT_EQ(reader->readUntilNextSeparator(), "am");
    ASSERT_EQ(reader->readUntilNextSeparator(), "a");
    ASSERT_EQ(reader->readUntilNextSeparator(), "reader");

    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}

TEST(configReader, readSeperator) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("\"abc   def\"");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc   def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(configReader, justSeperator) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("\"\"");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_TRUE(reader->isEOF());

}

TEST(configReader, justSeperatorWithbefor) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc \"\"");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_TRUE(!reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_TRUE(reader->isEOF());

}

TEST(configReader, seperatorWithSomeText) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("\"abc \" def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(), "a b c");

}

TEST(configReader, seperatorWithSomeText2) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("\"abc\"def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(), "a b c");

}

TEST(configReader, justSeperatorWithAfter) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString(" \"\" abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}


TEST(confReaderFile, file1) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/config1");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "xxx");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(confReaderFile, file2) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/config2");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(), "world");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(confReaderFile, file3) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/config3");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo world");
    ASSERT_EQ(reader->readUntilNextSeparator(), "yyy");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(confReaderFile, file4) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/config4");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo world");
    ASSERT_EQ(reader->readUntilNextSeparator(), "yyy");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(confReaderFile, file5) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/config5");
    ASSERT_FALSE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(), "hallo world");
    ASSERT_EQ(reader->readUntilNextSeparator(), "yyy");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}

TEST(confReaderFile, noSuchFile) {
    configFileReader *reader = new configFileReader("../tests/TEST_configFiles/configxx");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
    ASSERT_TRUE(reader->isEOF());
    ASSERT_EQ(reader->readUntilNextSeparator(), "");

}


TEST(configReader, unfinishedJoin) {
    configFileReader *reader = new configFileReader("");
    reader->inputSourceIsString("abc \"def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "abc");
    ASSERT_EQ(reader->readUntilNextSeparator(), "def");
    ASSERT_EQ(reader->readUntilNextSeparator(), "");
}
