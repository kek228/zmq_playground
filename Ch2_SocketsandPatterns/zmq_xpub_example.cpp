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
#include <thread>
#include <chrono>

#include "common.h"


std::atomic_bool stop;
using namespace std::chrono_literals;


int main(void) {
    //  Prepare our context and publisher
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_XPUB);
    int rc = zmq_bind(publisher, "tcp://*:10000");
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
    std::vector<zmq_pollitem_t> items = {
            { publisher,   0, ZMQ_POLLIN, 0 },
            { publisher, 0, ZMQ_POLLOUT, 0 }
    };

    WeaterReport report;
    char msg [256];
    while (!stop) {
        ++counter;
        counter &= (record_types.size() - 1);
        memcpy(report.zipcode, zipcodes[counter].data(), zipcodes[counter].size());
        report.type = record_types[counter];
        report.postcode = dist(rng);
        report.temp = dist(rng);
        zmq_poll (items.data(), 2, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            int size = zmq_recv (publisher, msg, 255, 0);
            if (size != -1) {
                // got subscription message
                zmq_recv (publisher, &msg, sizeof(msg), 0);
                std::cout<<"From receiver: "<<std::string_view(msg)<<std::endl;
                // From receiver: zipcode1
            }
        }
        if (items [1].revents & ZMQ_POLLOUT) {
            zmq_send(publisher, &report, sizeof(WeaterReport), 0);
            std::this_thread::sleep_for(1s);
            std::cout<<"msg " << counter << " sent"<<std::endl;
        }
    }
    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}
