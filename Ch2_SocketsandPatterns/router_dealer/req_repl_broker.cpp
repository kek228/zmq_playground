#include "common.h"

std::atomic_bool stop;

int main ()
{
    //  Prepare our context and sockets
    void *context = zmq_ctx_new ();
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    void *backend  = zmq_socket (context, ZMQ_DEALER);
    zmq_bind (frontend, "tcp://*:10000");
    zmq_bind (backend,  "tcp://*:10001");

    //  Initialize poll set
    zmq_pollitem_t items [] = {
            { frontend, 0, ZMQ_POLLIN, 0 },
            { backend,  0, ZMQ_POLLIN, 0 }
    };
    //  Switch messages between sockets
    while (!stop) {
        zmq_msg_t message;
        zmq_poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            int c = 1;
            while (true) {
//                frontend msg data 1 msg: k�Eg -> routing id
//                frontend msg data 2 msg: -> empty delimiter
//                frontend msg data 3 msg:Hello -> actual data

                //  Process all parts of the message
                zmq_msg_init (&message); // should zmq_msg_init after each send !!!
                zmq_msg_recv (&message, frontend, 0);
                const size_t msg_size = zmq_msg_size(&message);
                std::cout<<"frontend msg data "<<c<<" msg:"<<std::string_view((char*)zmq_msg_data(&message), msg_size)<<std::endl;
                int more = zmq_msg_more (&message);
                zmq_msg_send (&message, backend, more? ZMQ_SNDMORE: 0);
                zmq_msg_close (&message);
                ++c;
                if (!more)
                    break;      //  Last message part
            }
        }
        if (items [1].revents & ZMQ_POLLIN) {
            int c = 1;
            while (true) {
//                backend msg data 1 msg: k�Eg -> routing id
//                backend msg data 2 msg: -> empty delimiter
//                backend msg data 3 msg:resp: 3 -> actual data
                //  Process all parts of the message
                zmq_msg_init (&message);
                zmq_msg_recv (&message, backend, 0);
                const size_t msg_size = zmq_msg_size(&message);
                std::cout<<"backend msg data "<<c<<" msg:"<<std::string_view((char*)zmq_msg_data(&message), msg_size)<<std::endl;
                int more = zmq_msg_more (&message);
                zmq_msg_send (&message, frontend, more? ZMQ_SNDMORE: 0);
                zmq_msg_close (&message);
                ++c;
                if (!more)
                    break;      //  Last message part
            }
        }
    }
    //  We never get here, but clean up anyhow
    zmq_close (frontend);
    zmq_close (backend);
    zmq_ctx_destroy (context);
    return 0;
}
