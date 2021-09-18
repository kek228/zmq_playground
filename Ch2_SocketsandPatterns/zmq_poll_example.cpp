#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
//
#include <atomic>
#include <iostream>
#include <stdexcept>
#include "../common/weater_struct.h"

std::atomic_bool stop;

int main ()
{
    //  Connect to task ventilator
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_PULL);
    zmq_connect (receiver, "tcp://localhost:5557");

    //  Connect to weather server
    void *weather_sub = zmq_socket (context, ZMQ_SUB);
    zmq_connect (weather_sub, "tcp://localhost:5556");
    zmq_setsockopt (weather_sub, ZMQ_SUBSCRIBE, "zipcode1 ", 6);

    zmq_pollitem_t items [] = {
            { receiver,   0, ZMQ_POLLIN, 0 },
            {weather_sub, 0, ZMQ_POLLIN, 0 }
    };
    //  Process messages from both sockets
    WeaterReport report;

    while (!stop) {
        char msg [256];

        zmq_poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            int size = zmq_recv (receiver, msg, 255, 0);
            if (size != -1) {
                zmq_recv (receiver, &msg, sizeof(msg), 0);
                std::cout<<"From receiver: "<<std::string_view(msg)<<std::endl;
            }
        }
        if (items [1].revents & ZMQ_POLLIN) {
            int size = zmq_recv (weather_sub, &report, sizeof(report), 0);
            if (size != -1) {
                std::string_view zipcode(report.zipcode, sizeof(report.zipcode));
                std::cout<<"zipcode: "<<zipcode<<std::endl; // uses string_view.size() not a null-terminator
                std::cout<<"temp: "<<report.temp <<" postcode: "<<report.postcode<<" type: "<<report.type<<std::endl;
            }
        }
    }
    zmq_close (weather_sub);
    zmq_ctx_destroy (context);
    return 0;
}