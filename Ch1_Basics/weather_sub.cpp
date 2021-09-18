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
#include "common.h"


std::atomic_bool stop;

int main (int argc, char *argv [])
{
    //  Socket to talk to server
    printf ("Collecting updates from weather server...\n");
    void *context = zmq_ctx_new ();
    zmq_ctx_set (context, ZMQ_IO_THREADS, 4);
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:5556");
    assert (rc == 0);

    const char *filter = "zipcode1";

    // MUST set a subscription, otherwise no msgs received
    // filtering works based on a first message bytes, seems there is no didicated "topic"
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
    // rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, nullptr, 0); // subscribe to all
    assert (rc == 0);

    //  Process 100 updates
    int update_nbr;
    long total_temp = 0;

    WeaterReport report;

    for (update_nbr = 0; update_nbr < 100; update_nbr++) {
        zmq_recv (subscriber, &report, sizeof(report), 0);
        std::string_view zipcode(report.zipcode, sizeof(report.zipcode));
        std::cout<<"zipcode: "<<zipcode<<std::endl; // uses string_view.size() not a null-terminator
        std::cout<<"temp: "<<report.temp <<" postcode: "<<report.postcode<<" type: "<<report.type<<std::endl;
        total_temp += report.temp;
    }
    printf ("Average temperature for zipcode '%s' was %dF\n",
            filter, (int) (total_temp / update_nbr));

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
