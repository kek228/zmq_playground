//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


#include <atomic>
#include <iostream>

std::atomic_bool stop;

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *client = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect(client, "tcp://localhost:5555");
    if (rc != 0) {
        const int errno_val = zmq_errno();
        std::cerr<<"FAILED TO CONNECT error: "<< zmq_strerror(errno_val)<<std::endl;
        exit(-1);
    }

    while (!stop) {
        char buffer [6];
        zmq_send (client, "Hello", 5, 0);
        sleep (1);          //  Do some 'work'
        zmq_recv (client, buffer, 5, 0);
        printf ("Received Reply\n");
        std::cout<<"SRV REPL=="<<std::string(buffer)<<std::endl;
    }
    zmq_close (client);
    zmq_ctx_destroy (context);

    return 0;
}
