set(dir src/comChain/decoder/)
set(src_decoder
        ${dir}DecodeState.cpp
        ${dir}StopFrame.h
        ${dir}GdbErrorStream.h
        ${dir}GdbOutputStream.h
        ${dir}GdbClient.cpp
        ${dir}NameLessPipeLinux.cpp
        ${dir}GdbDecoder.cpp
        ${dir}NameLessPipeWindows.cpp
        ${dir}NameLessPipe.h
        ${dir}GdbErrorStream.cpp
        ${dir}GdbDecoder.h
        ${dir}FrameDecoder.cpp
        ${dir}GdbClient.h
        ${dir}GdbStreams.h
        ${dir}GdbInterface.h
        ${dir}ValueDecoder.h
        ${dir}GdbOutputStream.cpp
        ${dir}DecodeState.h
        ${dir}GdbInterface.cpp
        ${dir}FrameDecoder.h
        ${dir}ValueDecoder.cpp
        PARENT_SCOPE )