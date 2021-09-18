
add_executable(zmq_poll_example zmq_poll_example.cpp)
target_link_libraries(zmq_poll_example zmq)

add_subdirectory(./router_dealer)