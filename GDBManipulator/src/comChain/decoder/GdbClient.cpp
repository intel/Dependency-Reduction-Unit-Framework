/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "GdbClient.h"

gdbClient::gdbClient(gdbDecoder *decoder_) {
    decoder = decoder_;
    nlp = new nameLessPipe();

}

gdbClient::~gdbClient() {
    delete decode;
    decode = nullptr;
    delete errorStream;
     errorStream = nullptr;
    delete outStream;
     outStream = nullptr;

}

bool gdbClient::open() {

    errorStream = new gdbErrorStream(decoder, nlp);
    outStream = new gdbOutputStream(decoder, nlp);

    return true;
}

gdbErrorStream *gdbClient::getErrorStream() const {
    return errorStream;
}

gdbOutputStream *gdbClient::getOutStream() const {
    return outStream;
}


nameLessPipe *gdbClient::getNameLessPipe() {
    return nlp;
}

