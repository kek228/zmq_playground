#include "common.h"
std::atomic_bool stop;

static void *
worker_routine (void *context) {
    //  Socket to talk to dispatcher. ZMQ_REP connects to the ZMQ_DEALER through the inproc
    // ZMQ_DEALER: Each message sent is round-robined among all connected peers, and each message received is fair-queued from all connected peers.
    void *receiver = zmq_socket (context, ZMQ_REP);
    zmq_connect (receiver, "inproc://workers");
    char msg [256];
    int c = 0;
    while (!stop) {
        const int recv_size = zmq_recv (receiver, &msg, sizeof(msg), 0);
        std::cout<<"recv: "<<std::string_view(msg, recv_size)<<std::endl;
        //  Do some 'work'
        sleep (1);
        //  Send reply back to client
        std::string resp = "resp " + std::to_string(c);
        zmq_send(receiver, resp.data(), resp.size(), 0);
        ++c;
    }
    zmq_close (receiver);
    return NULL;
}

int main (void)
{
    void *context = zmq_ctx_new ();

    //  Socket to talk to clients
    void *clients = zmq_socket (context, ZMQ_ROUTER);
    zmq_bind (clients, "tcp://*:5555");

    //  Socket to talk to workers
    void *workers = zmq_socket (context, ZMQ_DEALER);
    zmq_bind (workers, "inproc://workers");

    //  Launch pool of worker threads
    int thread_nbr;
    for (thread_nbr = 0; thread_nbr < 5; thread_nbr++) {
        pthread_t worker;
        pthread_create (&worker, NULL, worker_routine, context);
    }
    //  Connect work threads to client threads via a queue proxy
    zmq_proxy (clients, workers, NULL);

    //  We never get here, but clean up anyhow
    zmq_close (clients);
    zmq_close (workers);
    zmq_ctx_destroy (context);
    return 0;
}
