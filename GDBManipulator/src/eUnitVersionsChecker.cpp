/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#include "eUnitVersionsChecker.h"
#include "FeatureCheck.h"
featureCheck *eUnitFeatureChecker;
feature * UseSoftwareBreakpoints = new feature("UseSoftwareBreakpoints",1);
feature * RunTimeTestFiler = new feature("RunTimeTestFiler",0);
feature * printOverGDB = new feature("printOverGDB",0);


bool defineEunitVersions(){
    eUnitFeatureChecker = new featureCheck(1);
    if(eUnitFeatureChecker->addFeature(UseSoftwareBreakpoints) == FeatureAlreadyExists)
        return false;
    if(eUnitFeatureChecker->addFeature(RunTimeTestFiler) == FeatureAlreadyExists)
        return false;
    if(eUnitFeatureChecker->addFeature(printOverGDB) == FeatureAlreadyExists)
        return false;
}