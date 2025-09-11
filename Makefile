server:
	g++ util.cpp Epoll.cpp InetAddress.cpp Socket.cpp Channel.cpp client.cpp -o client && \
	g++ util.cpp Epoll.cpp InetAddress.cpp Socket.cpp Channel.cpp server.cpp -o server
clean:
	rm server && rm client