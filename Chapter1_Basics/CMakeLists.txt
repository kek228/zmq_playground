
################## REQ/RESP example ##################
# SRV
add_executable(hello_world_srv hello_world_srv.cpp)
target_link_libraries(hello_world_srv zmq)

# CLNT
add_executable(hello_world_clnt hello_world_cl.cpp)
target_link_libraries(hello_world_clnt zmq)

################## PUB/SUB example ##################

# PUB
add_executable(pub_server pub_server.cpp weater_struct.h)
target_link_libraries(pub_server zmq)

# SUB
add_executable(sub_client sub_client.cpp weater_struct.h)
target_link_libraries(sub_client zmq)
