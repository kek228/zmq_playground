//  Task ventilator
//  Binds PUSH socket to tcp://localhost:5557
//  Sends batch of tasks to workers via that socket

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

int main (void)
{
    void *context = zmq_ctx_new ();

    //  Socket to send messages on
    void *sender = zmq_socket (context, ZMQ_PUSH);
    zmq_bind (sender, "tcp://*:5557");

    //  Socket to send start of batch message on
    void *sink = zmq_socket (context, ZMQ_PUSH);
    zmq_connect (sink, "tcp://localhost:5558");

    printf ("Press Enter when the workers are ready: ");
    getchar ();
    printf ("Sending tasks to workers...\n");

    //  The first message is "0" and signals start of batch
    zmq_send (sink, "0", 1, 0);
    //  Initialize random number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> time_dst(1, 100000);


    //  Send 100 tasks
    int task_nbr;
    uint32_t total_microsec = 0;     //  Total expected cost
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {
        uint32_t workload = time_dst(rng);
        //  Random workload from 1 to 100msecs
        total_microsec += workload;
        char string [10];
        sprintf (string, "%d", workload);
        zmq_send (sender, string, strlen(string), 0);
    }
    printf ("Total expected cost: %d msec\n", total_microsec);

    zmq_close (sink);
    zmq_close (sender);
    zmq_ctx_destroy (context);
    return 0;
}
