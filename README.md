# KNN Classifier Server/Client

This project contains a TCP server and client that implements a KNN classifier. The server runs the classifier and listens for incoming connections from clients, while the client sends unclassified vectors to the server for classification and receives the classification back from the server and then prints out the clasification. 

server will be activated as follows:
server.out file port

Where: 
- file is the name of the tagged vector file (can be a relative or full address)
- port is the port number on which the server should listen

The client will be activated as follows:
client.out ip port

where: 
- port is the port number on which the server listens 
- ip is the ip address of the server.


## Client:
The client runs in an infinite loop, and receives input from the user: vector distance k
Where: 
- vector is the vector for classification. 
- distance is the distance metric. 
- k is the number of neighbors.

### Client Input Format
- vector: Vectors can contain integers and float values only. Ensure that when inputting the vector there is a single space between each number.
- distance metric: one of the following strings: AUC, MAN, CHB, CAN, MIN. If MIN (Minkowski) distance was chosen, you will be prompted to enter a p value
- k: any positive int value.

The client sends the information received from the user to the server. It then receives the classification back from the server and prints the received answer to the screen on a separate line and receives the next input from the user.
- If the user enters the input -1 the client closes the connection to the server and closes the program. 
- In any case of invalid input, to the client, the client must print invalid input only and continue to receive the next input.

## Server:
Listens for incoming connections. Once it gets a connection, it gets the information from the client, deciphers it and sends the vector, distance and k it got to the KNN classifier in order to classify the vector. It then sends the classification back to client and continues waiting for more messages. If the client enters -1, the connection with that client and the server will be closed and the server will continue to the next connection (the server does not close).
- In any case that the server receives invalid input from the client, the server must return invalid input to the client and continue receiving information from that client.

## Building and Running

To build and run the server and client executables, run the make file by executing the following command:

Step-by-step compiling and running instructions:

compiling:

- open Terminal

- type 'git init'

- type 'git clone https://github.com/itaialcalai/part3.git'

- type 'cd knn'

- type 'make'

running:

- type './a.out k file distance'

Where:

k: number of nearest neighbors to use. Must be a positive integer
file: name of the tagged vector file, file containing the training dataset. Can be a relative or full address
distance: distance metric to use.
Euclidean - AUC,
Manhattan - MAN,
Chebyshev - CHB,
Canberra - CAN,
Minkowski - MIN
if MIN (Minkowski) distance was chosen, you will be prompted to enter a p value
Once the code is running, you can enter a vector of data points and then press enter to get the predicted class. The input must be a vector of real numbers, separated by spaces. When you are done entering data vectors to be classified, type '-1' to exit the program.

This will create the server.out and client.out executables using the Makefile provided in the project, start the server listening on the specified port, and run the client to connect to the server and send unclassified vectors for classification. The server will keep listening until you type -1 to exit the program.
