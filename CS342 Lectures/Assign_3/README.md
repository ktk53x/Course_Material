Group ID: 40
Member 1: Anirudh Varanasi
Roll Number: 180101084

Member 2: Kartikeya Saxena
Roll Number: 180101034

ID 5: Client-Server Programming using both TCP and UDP sockets
The following application uses sockets and fork to implement concurrent server.

Usage:
make // To create executables for server and client
./server <PORT_NO> // Select a port Number for server to listen to
./client 0.0.0.0 <PORT_NO> // To connect to the server

./prequest 0.0.0 <PORT_NO> // For making 10 concurrent requests to the server

make clean // To clean the executables when done
// Make sure that the PORT numbers are valid and are available
// We have also created a thread pool based implementation but there is a significant packet loss in using just 10 threads to do the work
