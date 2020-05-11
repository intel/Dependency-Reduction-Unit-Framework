set(dir src/comChain/)
add_subdirectory(decoder)
set(src_comChain
        ${src_decoder}
        ${dir}ComArmNoneEabiGdb.cpp
        ${dir}ComGnuDebugger.cpp
        ${dir}ComArmNoneEabiGdb.h
        ${dir}ComChain.cpp
        ${dir}ComGnuDebugger.h
        ${dir}ComChain.h
        PARENT_SCOPE )