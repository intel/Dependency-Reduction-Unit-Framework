set(dir src/comChain/)
add_subdirectory(decoder)
set(src_comChain
        ${src_decoder}
        ${dir}ComArmNoneEabiGdb.cpp
        ${dir}ComGnuDebugger.cpp
        ${dir}ComArmNoneEabiGdb.h
        ${dir}ComChain.cpp
        ${dir}ComSerial.cpp
        ${dir}ComGnuDebugger.h
        ${dir}ComChain.h
        ${dir}ComSerial.h
        PARENT_SCOPE )