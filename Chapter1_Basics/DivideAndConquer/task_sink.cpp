//  Task sink
//  Binds PULL socket to tcp://localhost:5558
//  Collects results from workers via that socket

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
    //  Prepare our context and socket
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_PULL);
    zmq_bind (receiver, "tcp://*:5558");

    //  Wait for start of batch
    char string [256];
    zmq_recv (receiver, &string, sizeof(string), 0);

    //  Process 100 confirmations
    int total_time = 0;
     for (int task_nbr = 0; task_nbr < 100; task_nbr++) {
        zmq_recv (receiver, &string, sizeof(string), 0);
        total_time += std::strtol(string, nullptr, 10);
    }
    std::cout<<"TOTAL TIME:" <<total_time<<std::endl;
    zmq_close (receiver);
    zmq_ctx_destroy (context);
    return 0;
}
