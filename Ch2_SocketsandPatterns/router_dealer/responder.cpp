#include "common.h"
std::atomic_bool stop;

int main (void)
{
    void *context = zmq_ctx_new ();

    //  Socket to talk to clients
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_connect (responder, "tcp://localhost:10001");
    char buffer[256];
    int c = 0;
    const std::string resp_suffix = "resp: ";
    while (!stop) {
        //  Wait for next req client
        const int resp_size = zmq_recv (responder, buffer, sizeof(buffer), 0);
        std::cout<<"Received request:"<<std::string_view(buffer, resp_size);
        //  Do some 'work'
        sleep (1);
        //  Send reply back to client
        const std::string resp_msg = resp_suffix + std::to_string(c);
        zmq_send (responder, resp_msg.data(), resp_msg.size(), 0);
        ++c;
    }
    //  We never get here, but clean up anyhow
    zmq_close (responder);
    zmq_ctx_destroy (context);
    return 0;
}
