//
// Created by basto on 11/20/19.
//

#include <FeatureCheck.h>
#include <gtest/gtest.h>

feature * UseSoftwareBreakpointsTest = new feature("UseSoftwareBreakpoints", 1);
feature * RunTimeTestFilerTest = new feature("RunTimeTestFiler",2,2);
feature * printOverGDBTest = new feature("printOverGDB",3,6);


TEST(FeatureChecker,addFeatures){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);
}
TEST(FeatureChecker,addFeaturesDouble){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAlreadyExists);
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAlreadyExists);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAlreadyExists);
}

TEST(FeatureChecker,checkVersionMatch){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);

    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",1),FeatureSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",2),FeatureSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",9),FeatureSupported);

    ASSERT_EQ(FeatureChecker->checkFeature("RunTimeTestFiler",2),FeatureSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",3),FeatureSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",6),FeatureSupported);
}

TEST(FeatureChecker,checkVersionNotJet){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);

    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",0),FeatureNotJetSupported);

    ASSERT_EQ(FeatureChecker->checkFeature("RunTimeTestFiler",1),FeatureNotJetSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",2),FeatureNotJetSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",0),FeatureNotJetSupported);
}
TEST(FeatureChecker,checkVersionNotAnymore){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);

    ASSERT_EQ(FeatureChecker->checkFeature("RunTimeTestFiler",3),FeatureNotAnyMoreSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",7),FeatureNotAnyMoreSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGDB",8),FeatureNotAnyMoreSupported);
}

TEST(FeatureChecker,noSucheFeature){
    featureCheck * FeatureChecker = new featureCheck();
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);

    ASSERT_EQ(FeatureChecker->checkFeature("RunTimeTestFiler ",3),NoSuchFeature);
    ASSERT_EQ(FeatureChecker->checkFeature("printOverGD",7),NoSuchFeature);
}

TEST(FeatureChecker,versionMigthNotBeAnymoreBeSupported){
    featureCheck * FeatureChecker = new featureCheck(1);
    ASSERT_EQ(FeatureChecker->addFeature(UseSoftwareBreakpointsTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(RunTimeTestFilerTest),FeatureAdded);
    ASSERT_EQ(FeatureChecker->addFeature(printOverGDBTest),FeatureAdded);

    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",1),FeatureSupported);
    ASSERT_EQ(FeatureChecker->checkFeature("UseSoftwareBreakpoints",2),FeatureMaybeNotAnymoreSupported);
}
