//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


#include <atomic>
#include <iostream>
#include <stdexcept>

std::atomic_bool stop;

void print_errno(const int rc, std::string_view error_prefix = "", bool throw_ex = true) {
    if(rc >= 0) {
        return;
    }
    std::string error_msg;
    const int errno_val = zmq_errno();
    error_msg = std::string(error_prefix) + " " + std::string(zmq_strerror(errno_val));
    std::cerr<<error_msg<<std::endl;
    if (throw_ex) {
        throw std::runtime_error(error_msg);
    }
}

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *client = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect(client, "tcp://localhost:5555");
    print_errno(rc, "zmq_connect failed");

    int c = 0;
    char buffer [100];
    while (!stop) {
        std::string msg = "Hello " + std::to_string(c);
        rc = zmq_send (client, msg.data(), msg.size(), 0);
        ++c;
        print_errno(rc, "zmq_send failed", false);

        // zmq_send failed Operation cannot be accomplished in current state for the second zmq_send
        msg = "Hello " + std::to_string(c);
        rc = zmq_send (client, msg.data(), msg.size(), 0);
        ++c;
        print_errno(rc, "zmq_send failed", false);
        //
        sleep (1);          //  Do some 'work'
        zmq_recv (client, buffer, 100, 0);
        std::cout<<"SRV REPL=="<<std::string(buffer)<<std::endl;
        // only repls for Hello 0, 2, 4 ... msgs recv
    }
    zmq_close (client);
    zmq_ctx_destroy (context);

    return 0;
}
