/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "FeatureCheck.h"
#include "main.h"
featureReturn featureCheck::addFeature(feature *newFeature) {
    if(findFeature(newFeature->feature_name) != nullptr){
        return FeatureAlreadyExists;
    }
    featureList->push_back(newFeature);
    return FeatureAdded;
}

feature * featureCheck::findFeature(string name) {
    for(int i = 0; i< featureList->size();i++){
        if(featureList->at(i)->feature_name == name){
            return featureList->at(i);
        }
    }
    return nullptr;
}

featureReturn featureCheck::checkFeature(string featureName, int version) {
    feature *f = findFeature(featureName);
    if(f == nullptr){
        return NoSuchFeature;
    }
    featureReturn returnCode;
    if(f->version_start > version)
        returnCode =  FeatureNotJetSupported;
    else if(f->version_last < version && f->version_last >0)
        returnCode =   FeatureNotAnyMoreSupported;
    else if (knownVersion == -1 || version <= knownVersion)
        returnCode =   FeatureSupported;
    else
        returnCode = FeatureMaybeNotAnymoreSupported;
    return returnCode;

}

featureCheck::featureCheck(int knownVersion) : knownVersion(knownVersion){
    featureList = new vector<feature*>();
}


std::ostream &operator<<(ostream &os, const featureReturn &dt) {
    switch(dt){
        case NoSuchFeature: os <<"No such Feature Defined";break;
        case FeatureNotAnyMoreSupported: os <<"This Feature is not any more supported" ;break;
        case FeatureNotJetSupported: os <<"Feature not jet supported :: update version";break;
        case FeatureMaybeNotAnymoreSupported: os <<"gdbMan does not know this eUnit version --> feature maybe not supported.";break;
        case  FeatureSupported: os <<" Feature is supported" ;break;
        case FeatureAdded: os <<"Feature has been added" ;break;
        case FeatureAlreadyExists: os <<"Feature has already been added";break;
        default: os << "unknown eunum Type";
    }
    return os;
}

