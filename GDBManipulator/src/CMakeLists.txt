set(dir src/)
add_subdirectory(comChain)
add_subdirectory(runModes)
add_subdirectory(tester)
set(src_files
        ${src_comChain}
        ${src_runMode}
        ${src_tester}
        ${dir}ArgumentCallBackFunctions.cpp
        ${dir}ArgumentCallBackFunctions.h
        ${dir}LogFileFilter.h
        ${dir}Utils.cpp
        ${dir}Utils.h
        ${dir}Version.h
        ${dir}FeatureCheck.cpp
        ${dir}FeatureCheck.h
        ${dir}eUnitVersionsChecker.h
        ${dir}eUnitVersionsChecker.cpp
        PARENT_SCOPE )