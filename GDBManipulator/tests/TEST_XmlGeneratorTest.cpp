//
// Created by basto on 4/22/18.
//

#include "gtest/gtest.h"
#include "tester/XmlGenerator.h"

TEST(xmlGeneartor, emptyXmlWithNoErrors) {
    xmlGenerator *xml = new xmlGenerator("");
    string s = xml->buildXml(0, "abc", 0.0);
    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"0\" failures=\"0\" disabled=\"0\" errors=\"0\" timestamp=\"abc\" time=\"0.000000\" name=\"AllTests\">\n</testsuites>");
}

TEST(xmlGeneartor, emptyXmlWithErrors) {
    xmlGenerator *xml = new xmlGenerator("");
    string s = xml->buildXml(1, "xxx", 1.0);
    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"1\" timestamp=\"xxx\" time=\"1.000000\" name=\"AllTests\">\n  <testsuite name=\"test evaluation\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"target communication result\" status=\"run\" time=\"0.000\" classname=\"test evaluation\">\n      <failure message=\"failed with 1 failure. For further information's see logfile(if enabled)\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTestCaseToDefautlClass) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestCase("t1");
    string s = xml->buildXml(0);
    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" timestamp=\"noTimeStamp\" time=\"0.000000\" name=\"AllTests\">\n  <testsuite name=\"class_name_not_defined\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"class_name_not_defined\">\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTwoTestCaseToDefautlClass) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestCase("t1");
    xml->addTestCase("t2");
    string s = xml->buildXml(0);
    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"2\" failures=\"0\" disabled=\"0\" errors=\"0\" timestamp=\"noTimeStamp\" time=\"0.000000\" name=\"AllTests\">\n  <testsuite name=\"class_name_not_defined\" tests=\"2\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"class_name_not_defined\">\n    </testcase>\n    <testcase name=\"t2\" status=\"run\" time=\"0.000\" classname=\"class_name_not_defined\">\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTwoTestCaseToDefautlClassWithOneError) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestCase("t1");
    xml->addTestCase("t2", "errorMessage");
    string s = xml->buildXml(0);

    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"2\" failures=\"1\" disabled=\"0\" errors=\"0\" timestamp=\"noTimeStamp\" time=\"0.000000\" name=\"AllTests\">\n  <testsuite name=\"class_name_not_defined\" tests=\"2\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"class_name_not_defined\">\n    </testcase>\n    <testcase name=\"t2\" status=\"run\" time=\"0.000\" classname=\"class_name_not_defined\">\n      <failure message=\"errorMessage\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTestClassWithNoTest) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestClass("testC");
    string s = xml->buildXml(1, "xxx", 1.0);

    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"1\" timestamp=\"xxx\" time=\"1.000000\" name=\"AllTests\">\n  <testsuite name=\"test evaluation\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"target communication result\" status=\"run\" time=\"0.000\" classname=\"test evaluation\">\n      <failure message=\"failed with 1 failure. For further information's see logfile(if enabled)\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTestClassWithTestCase) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestClass("testA");
    xml->addTestCase("t1");
    string s = xml->buildXml(1, "xxx", 1.0);

    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"2\" failures=\"1\" disabled=\"0\" errors=\"1\" timestamp=\"xxx\" time=\"1.000000\" name=\"AllTests\">\n  <testsuite name=\"testA\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"testA\">\n    </testcase>\n </testsuite>\n  <testsuite name=\"test evaluation\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"target communication result\" status=\"run\" time=\"0.000\" classname=\"test evaluation\">\n      <failure message=\"failed with 1 failure. For further information's see logfile(if enabled)\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTestClassWithTwoTestCase) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestClass("testA");
    xml->addTestCase("t1");
    xml->addTestClass("testB");
    xml->addTestCase("t2");
    string s = xml->buildXml(1, "xxx", 1.0);

    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"3\" failures=\"1\" disabled=\"0\" errors=\"1\" timestamp=\"xxx\" time=\"1.000000\" name=\"AllTests\">\n  <testsuite name=\"testA\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"testA\">\n    </testcase>\n </testsuite>\n  <testsuite name=\"testB\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t2\" status=\"run\" time=\"0.000\" classname=\"testB\">\n    </testcase>\n </testsuite>\n  <testsuite name=\"test evaluation\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"target communication result\" status=\"run\" time=\"0.000\" classname=\"test evaluation\">\n      <failure message=\"failed with 1 failure. For further information's see logfile(if enabled)\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}

TEST(xmlGeneartor, addTestClassWithTwoTestCaseAndFailedTests) {
    xmlGenerator *xml = new xmlGenerator("");
    xml->addTestClass("testA");
    xml->addTestCase("t1");
    xml->addTestClass("testB");
    xml->addTestCase("t2", "failed");
    string s = xml->buildXml(1, "xxx", 1.0);
    ASSERT_EQ(s,
              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"3\" failures=\"2\" disabled=\"0\" errors=\"1\" timestamp=\"xxx\" time=\"1.000000\" name=\"AllTests\">\n  <testsuite name=\"testA\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t1\" status=\"run\" time=\"0.000\" classname=\"testA\">\n    </testcase>\n </testsuite>\n  <testsuite name=\"testB\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"t2\" status=\"run\" time=\"0.000\" classname=\"testB\">\n      <failure message=\"failed\"></failure>\n    </testcase>\n </testsuite>\n  <testsuite name=\"test evaluation\" tests=\"1\" failures=\"1\" disabled=\"0\" errors=\"0\" time=\"0.000\">\n    <testcase name=\"target communication result\" status=\"run\" time=\"0.000\" classname=\"test evaluation\">\n      <failure message=\"failed with 1 failure. For further information's see logfile(if enabled)\"></failure>\n    </testcase>\n </testsuite>\n</testsuites>");
}
