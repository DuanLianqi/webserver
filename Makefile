server:
	g++ util.cpp Epoll.cpp InetAddress.cpp Socket.cpp client.cpp -o client && \
	g++ util.cpp Epoll.cpp InetAddress.cpp Socket.cpp server.cpp -o server
clean:
	rm server && rm client