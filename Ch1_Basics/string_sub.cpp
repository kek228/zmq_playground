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
    void *context = zmq_ctx_new ();
    zmq_ctx_set (context, ZMQ_IO_THREADS, 4);
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:5556");
    assert (rc == 0);

    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, nullptr, 0); // subscribe to all
    assert (rc == 0);

    int more;
    size_t more_size = sizeof (more);
    do {
        /* Create an empty ØMQ message to hold the message part */
        zmq_msg_t part;
        int rc = zmq_msg_init (&part);
        assert (rc == 0);
        /* Block until a message is available to be received from socket */
        rc = zmq_msg_recv (&part, subscriber, 0);
        size_t size = zmq_msg_size(&part);
        std::cout<<"RECV: "<<size<<std::endl;
        assert (rc != -1);
        char* b = (char*)zmq_msg_data(&part);
        auto header = reinterpret_cast<Header*>(b);
        std::cout<<"name: "<<header->name<<std::endl;
        std::cout<<"msg_id: "<<header->msg_id<<std::endl;
        std::cout<<"data_size: "<<header->data_size<<std::endl;
        std::cout<<"GOT BODY:"<<std::endl;
        char* body = b + sizeof(Header);
        std::cout<<std::string_view(body, header->data_size)<<std::endl;
        std::cout<<"more: "<<rc<<std::endl;
        zmq_msg_close (&part);
    } while (true);

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
