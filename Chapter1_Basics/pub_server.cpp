#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
//
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <random>

//
#include "weater_struct.h"


std::atomic_bool stop;


int main(void) {
    //  Prepare our context and publisher
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, "tcp://*:5556");
    assert (rc == 0);

    //  Initialize random number generator
    srandom((unsigned) time(NULL));
    const std::string record_types = "ABCDEFGH";
    const std::vector<std::string> zipcodes = {"zipcode1", "zipcode1", "zipcode3", "zipcode4", "zipcode5", "zipcode6", "zipcode7", "zipcode8"};
    size_t counter = 0;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100500);
    //
    WeaterReport report;
    while (!stop) {
        ++counter;
        counter &= (record_types.size() - 1);

        memcpy(report.zipcode, zipcodes[counter].data(), zipcodes[counter].size());
        report.type = record_types[counter];
        report.postcode = dist(rng);
        report.temp = dist(rng);
        //  Send message to all subscribers
        zmq_send(publisher, &report, sizeof(WeaterReport), 0);
    }
    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}
