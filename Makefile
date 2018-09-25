udp_server: UDPSocket.c
	gcc -Wall -Werror -g -o udp_server UDPSocket.c

clean:
	rm -f udp_server *~
