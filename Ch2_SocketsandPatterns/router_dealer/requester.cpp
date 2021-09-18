#include "common.h"

int main (void)
{
    void *context = zmq_ctx_new ();

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:10000");

    int request_nbr;
    std::string msg = "Hello";
    char buffer[256];
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        zmq_send (requester, msg.data(), msg.size(), 0);
        const int resp_size = zmq_recv (requester, buffer, sizeof(buffer), 0);
        std::cout<<"Got resp:"<<std::string_view(buffer, resp_size)<<std::endl;
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
