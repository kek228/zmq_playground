//  Task worker
//  Connects PULL socket to tcp://localhost:5557
//  Collects workloads from ventilator via that socket
//  Connects PUSH socket to tcp://localhost:5558
//  Sends results to sink via that socket
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
std::atomic_bool stop;

int main (void)
{
    //  Socket to receive messages on
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_PULL);
    zmq_connect (receiver, "tcp://localhost:5557");

    //  Socket to send messages to
    // ZMQ_PUSH -> sends msgs to subscribers (ZMQ_PULLs) in round-robin fashion
    // in contrast with a ZMQ_PUB/SUB
    void *sender = zmq_socket (context, ZMQ_PUSH);
    zmq_connect (sender, "tcp://localhost:5558");

    //  Process tasks forever
    char string [256];
    while (!stop) {
        zmq_recv (receiver, &string, sizeof(string), 0);
        printf ("SLEEP FOR %s \n", string);     //  Show progress
        fflush (stdout);
        usleep(std::strtol(string, nullptr, 10));    //  Do the work
        zmq_send (sender, string, strlen(string), 0);
    }
    zmq_close (receiver);
    zmq_close (sender);
    zmq_ctx_destroy (context);
    return 0;
}
