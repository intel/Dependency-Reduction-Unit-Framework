/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_DECODEFRAMES_H
#define GDBMANIPULATOR_DECODEFRAMES_H


#include <vector>
#include <string>

#include "StopFrame.h"

using namespace std;


/**
 * Frame Decoder.
 * This class decoded the frame part of the stop message.
 */
class frameDecoder {
    /**
    * decode the frame argument and store them in the frame
    * @param line line to decode
    * @param frame frame where the args get stored
    */
    void decodeFunctionArgs(string *line, stopFrame *frame);

protected:


    /**
     * decode the frame.
     * expect the the line starts with <frame> or <stopFrame> otherwise it return an nullpointer
     * @param line line which stats with the frame part
     * @return on usccess : line without the frame part on failior : nullpointer
     */
    stopFrame *decodeFrame(string *line);


};

std::ostream &operator<<(std::ostream &strm, const stopFrame &a);

#endif //GDBMANIPULATOR_DECODEFRAMES_H
