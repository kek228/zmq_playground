## The Socket API #
Create and destroy:  zmq_socket(), zmq_close()
Configuring sockets: zmq_setsockopt(), zmq_getsockopt()
Starting sockets: zmq_bind(), zmq_connect()
Using the sockets: zmq_msg_send(), zmq_msg_recv()

Difference between zmq and normal sockets:
1. zmq_inproc(), zmq_ipc(), zmq_tcp(), zmq_pgm(), and zmq_epgm() Avaliable transports
2. One socket may have many outgoing and many incoming connections
3. No zmq_accept()
4. ZeroMQ will automatically reconnect if the network connection is broken
5. You cannot work directly with connection

Difference between zmq and TCP:
1. ZeroMQ message is length-specified binary data
2. ZeroMQ sockets do their I/O in a background thread. Also both input and output msgs are QUEUED.
3. One-to-N routing behavior built-in for zmq.

I/O Threads:
In the debugger I can see 3 threads:
1. Main one of my app.
2. IO thread.
3. And the "Reaper"
For the IO thread ZMQ docs say: "The general rule of thumb is to allow one I/O thread per gigabyte of data in or out per second"
Io thread is created in zmq_ctx:
   int io_threads = 4;
   void *context = zmq_ctx_new ();
   zmq_ctx_set (context, ZMQ_IO_THREADS, io_threads);
   assert (zmq_ctx_get (context, ZMQ_IO_THREADS) == io_threads);

## Messaging Patterns
In the core of ZMQ ideology are patterns:
1. Request-reply
2. Pub-sub. Pubs to subs many-to-many each msg will be sent to ALL subs for a concrite pub
3. Pipeline. Like Pub-Sub BUT in fan-out manner.
4. Exclusive pair. Mostly for inter-threaded connection.

Valid connect-bind pairs:
PUB and SUB
REQ and REP
REQ and ROUTER (take care, REQ inserts an extra null frame)
DEALER and REP (take care, REP assumes a null frame)
DEALER and ROUTER
DEALER and DEALER
ROUTER and ROUTER
PUSH and PULL
PAIR and PAIR

### Working with messages
Send with zmq_send() and read with zmq_recv()
zmq_recv has a downside of cutting msg, instead zmq_msg_t might be used
Messages and Frames:
- A message can be one or more parts.
- These parts are also called “frames”.
- Each part is a zmq_msg_t object.
- You send and receive each part separately, in the low-level API.
- Higher-level APIs provide wrappers to send entire multipart messages.
Good to know:
- Zmq can send 0-length msgs. 
- Guarantees delivering all parts
- Zmq does not send msg right away
- You must call zmq_msg_close() when finished with a received message

### Handling Multiple Sockets
use zmq_poll()

DEALER and ROUTER
Needed only to create proxies between req/rep sockets

### Handling Errors and ETERM
The error code is provided in errno or zmq_errno().
A descriptive error text for logging is provided by zmq_strerror().

