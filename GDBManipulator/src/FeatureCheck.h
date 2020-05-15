/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_FEATURECHECK_H
#define GDBMANIPULATOR_FEATURECHECK_H

#include <string>
#include <vector>

using namespace std;

typedef struct feature{
public:
    string feature_name;
    int version_start;
    int version_last;

    feature(string featureName,int versionStart=0,int versionLast=-1):feature_name(featureName),
                          version_start(versionStart),version_last(versionLast){};
}feature;

enum featureReturn{
    NoSuchFeature, // No such Feature Defined
    FeatureNotAnyMoreSupported, // This Feature is not any more supported
    FeatureNotJetSupported, // Feature not jet supported :: update version
    FeatureMaybeNotAnymoreSupported,  // gdbMan does not know this eUnit version --> feature maybe not supported.
    FeatureSupported,
    // just for adding new features
    FeatureAdded,
    FeatureAlreadyExists
};

class featureCheck{
vector<feature*>  *featureList;

feature * findFeature(string name);
int knownVersion;
public:
    /**
     * init feature checker
     * @param knownVersion current version number of known features
     */
    featureCheck(int knownVersion = -1);
    featureReturn addFeature(feature* newFeature);
    featureReturn checkFeature(string featureName,int version);
};

ostream& operator<<(ostream& os, const featureReturn& dt);

#endif //GDBMANIPULATOR_FEATURECHECK_H
