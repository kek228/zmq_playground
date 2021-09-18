
################## REQ/RESP example ##################
# SRV
add_executable(hello_world_srv hello_world_srv.cpp)
target_link_libraries(hello_world_srv zmq)

# CLNT
add_executable(hello_world_clnt hello_world_cl.cpp)
target_link_libraries(hello_world_clnt zmq)

################## PUB/SUB example ##################
# PUB
add_executable(weather_pub weather_pub.cpp ../common/common.h)
target_link_libraries(weather_pub zmq)

# SUB
add_executable(weather_sub weather_sub.cpp ../common/common.h)
target_link_libraries(weather_sub zmq)

################## PUB/SUB example ##################
add_executable(task_ventilator ./DivideAndConquer/task_ventilator.cpp)
target_link_libraries(task_ventilator zmq)

add_executable(task_worker ./DivideAndConquer/task_worker.cpp)
target_link_libraries(task_worker zmq)

add_executable(task_sink ./DivideAndConquer/task_sink.cpp)
target_link_libraries(task_sink zmq)
