server:
	g++ src/util.cpp src/InetAddress.cpp src/Socket.cpp client.cpp -g -o client && \
	g++ src/util.cpp server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp -g -o server
clean:
	rm server && rm client