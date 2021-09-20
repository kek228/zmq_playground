
add_executable(zmq_poll_example zmq_poll_example.cpp)
target_link_libraries(zmq_poll_example zmq)

add_executable(zmq_xpub_example zmq_xpub_example.cpp)
target_link_libraries(zmq_xpub_example zmq)

add_executable(zmq_xsub_example zmq_xsub_example.cpp)
target_link_libraries(zmq_xsub_example zmq)

add_executable(multithread_req_srv multithread_req_srv.cpp)
target_link_libraries(multithread_req_srv zmq pthread)

add_executable(zmq_thread_sync zmq_thread_sync.cpp)
target_link_libraries(zmq_thread_sync zmq pthread)


add_subdirectory(./router_dealer)