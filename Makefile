server:
	g++ client.cpp src/util.cpp src/Buffer.cpp src/InetAddress.cpp src/Socket.cpp -g -o client && \
	g++ server.cpp \
	src/util.cpp src/Buffer.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp \
	src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp src/Connection.cpp \
	-g -o server
clean:
	rm server && rm client
