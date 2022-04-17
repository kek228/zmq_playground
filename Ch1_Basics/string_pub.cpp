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
#include <thread>
#include <chrono>

#include "../common/common.h"


std::atomic_bool stop;
using namespace std::chrono_literals;


void* allocate_msg(size_t s) {
    void *ptr = malloc(sizeof(Header) + s);
    return ptr;
}

int main(void) {
    //  Prepare our context and publisher
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, "tcp://*:5556");
    assert (rc == 0);

    //  Initialize random number generator
    srandom((unsigned) time(NULL));
    const std::vector<std::string> data = {"data1111111111111111"
                                           , "data22222"
                                           , "data3"
                                           , "data44444444"
                                           , "data5555"};

    const std::vector<std::string> names = {"name1"
                                            , "name2"
                                            , "name3"
                                            , "name4"
                                            , "name5"};

    size_t counter = 0;

    //
    char data_buf[1024];
    while (!stop) {
        const std::string& data_to_send = data[counter];
        const std::string& name = names[counter];
        // header
        auto h_ptr = reinterpret_cast<Header*>(data_buf);
        memcpy(h_ptr->name, name.data(), name.size());
        h_ptr->msg_id = counter;
        h_ptr->data_size = data_to_send.size();
        // body
        char* body_ptr = data_buf + sizeof(Header);
        memcpy(body_ptr, data_to_send.data(), data_to_send.size());
        // send
        zmq_send(publisher, &data_buf, sizeof(Header) + data_to_send.size(), 0);
        std::this_thread::sleep_for(1s);
        std::cout<<"msg " << counter << " sent"<<std::endl;
        ++counter;
        counter %= data.size();
    }
    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}
