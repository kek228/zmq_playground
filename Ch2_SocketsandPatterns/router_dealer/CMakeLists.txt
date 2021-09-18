add_executable(requester requester.cpp)
target_link_libraries(requester zmq)
#
add_executable(responder responder.cpp)
target_link_libraries(responder zmq)
#
add_executable(req_repl_broker req_repl_broker.cpp)
target_link_libraries(req_repl_broker zmq)
#
add_executable(req_repl_proxy req_repl_proxy.cpp)
target_link_libraries(req_repl_proxy zmq)
