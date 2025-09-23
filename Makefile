server:
	g++ client.cpp src/util.cpp src/Buffer.cpp src/InetAddress.cpp src/Socket.cpp -g -o client && \
	g++ server.cpp -pthread \
	src/util.cpp src/Buffer.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp \
	src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp src/Connection.cpp \
	src/ThreadPool.cpp \
	-g -o server
clean:
	rm server && rm client && rm threadTest

threadTest:
	g++ -pthread ThreadPoolTest.cpp src/ThreadPool.cpp -g -o threadTest

