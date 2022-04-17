#pragma once

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
//
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <vector>

#pragma pack(1)
struct WeaterReport {
    char zipcode[8];
    size_t temp;
    uint32_t postcode;
    char type;
};

struct Header {
    char name[20];
    uint16_t msg_id;
    size_t data_size;
};

struct Data {
    char *data;
};

#pragma pack()
