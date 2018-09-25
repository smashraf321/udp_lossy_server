# udp_lossy_server

This is an implementation of a UDP streaming server that takes in an input as a VLC stream and streams it to another PC with some loss introduced.
For running the Program, clone the repository and run make.
You should now start streaming the video on VLC by UDP protocol and allow the client VLC to wait for a stream.

You may now run the program as:
./udp_server source IP source port destination IP destination port loss rate percentage

example:
./udp_server 111.111.111.111 1234 222.222.222.222 4321 25
