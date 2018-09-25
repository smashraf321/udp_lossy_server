#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> 
#include <inttypes.h>
#include <time.h> 

int main(int argc, char *argv[]) 
{ 
	srand(time(NULL)); // setting the seed of the random number generator

    //variables
    struct sockaddr_in src_address, server_address, dest_address; // address struct for the sourc, server and destination sockets
	int s_socket_fd, d_socket_fd; // file descriptors for the sockets for server and destination .
	int opt = 1; // options for the socket
	int s_port, d_port; // for storing source/server and client port numbers
	int loss_rate; // for storing loss rate
	int src_addr_len, dest_addr_len; // for storing size of source and destination structure
	int n; // for number of bytes of received data
	char buffer[4086] = " "; // buffer to store intermediate incoming streaming data
	
	if(argc!=6) // if the correct number of arguments are not found
	{
		fprintf(stderr,"\n Please enter the Input arguments in the following order \n");
		fprintf(stderr," Source IP, Source Port, Destination IP, Destination Port, Loss rate\n");
		return -1;
	}
	
	s_port = atoi(argv[2]); // converting input source port number string to integer
	d_port = atoi(argv[4]); // converting input destination port number string to integer
	loss_rate = atoi(argv[5]); // converting input loss rate number string to integer
	
    // Creating socket file descriptor 
    if ((s_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
	
	printf("\n Opened server socket for video stream reception \n");
	
    // Creating socket file descriptor 
    if ((d_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
	
	printf("\n Opened destination socket for transmission \n");
    
    // Attaching options to the port
    setsockopt(s_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
	// making both sockets for source and destination as internet sockets
    server_address.sin_family = AF_INET; 
	dest_address.sin_family = AF_INET;
	
	// assigning the port values for both source and destination 
    server_address.sin_port = htons( s_port ); 
	dest_address.sin_port = htons( d_port );
	
	// assigning the IP addresses for both source and destination
    inet_aton(argv[1], &server_address.sin_addr);
	inet_aton(argv[3], &dest_address.sin_addr);
	
    // Forcefully attaching socket to the source port  
    if (bind(s_socket_fd, (struct sockaddr *)&server_address,sizeof(server_address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    printf("Successfully bound the source socket for receiving from source on port %d\n",s_port);

	dest_addr_len = sizeof(dest_address); // length of destination structure
	
	while (1)
	{
		n = recvfrom(s_socket_fd,buffer,4086,0,((struct sockaddr *) &src_address),((socklen_t *) &src_addr_len)); // receiving from source
		
		if( ((rand()%100)+1) > loss_rate ) // simple probability for making sure you transmit keeping loss rate 
		sendto(d_socket_fd,buffer,n,0,((struct sockaddr *) &dest_address),(*((socklen_t *) &dest_addr_len))); // send to a destination on a different socket
		//printf("Sent packet");
	}
	
	// close the sockets
	
	close(d_socket_fd);
	close(s_socket_fd);
	
    return 0;
}
 
