# KNN Classifier Server/Client

This project contains a TCP server and client that implement a KNN classifier. The server runs the classifier and listens for incoming connections from clients, while the client sends unclassified vectors to the server for classification and receives the classification back from the server and then prints out the clasification

## Building and Running

To build and run the server and client executables, run the make file by executing the following command:

make

This will create the server.out and client.out executables using the Makefile provided in the project, start the server listening on the specified port, and run the client to connect to the server and send unclassified vectors for classification.

## Input Format

The client gets the input from the user in the following format:

vector distance k

- vector: vector for classification, the format is 
- distance metric: one of the following strings: AUC, MAN, CHB, CAN, MIN,
- k: number of neighbors (any positive int value).

Notes

The server and client must be run on separate machines or on different ports on the same machine.

## stopping the program
