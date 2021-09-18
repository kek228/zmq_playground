//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
//
#include <atomic>
#include <iostream>
#include <stdexcept>

std::atomic_bool stop;

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    while (!stop) {
        char buffer [10];
        zmq_recv (responder, buffer, 10, 0);
        const std::string rec = std::string(buffer);
        std::cout<<"Received: "<<rec<<std::endl;
        sleep (1);          //  Do some 'work'
        const std::string repl = "reply for " + rec;
        zmq_send (responder, repl.data(), repl.size(), 0);
    }
    return 0;
}
