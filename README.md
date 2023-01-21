In this exercise our server will have to handle several clients at the same time. That is, at a given moment, the server can talk to several different clients at the same time.
When in the exercise it says client - it means the client talking to the server, that is, it is about the code you are writing. When it says user - it means a "human being" who communicates with the help of the client in front of the server.
The client (not the user) in this exercise is often "dumb" - that is, sends information to the server that the user entered and prints to the user information that the server sent to him.
When a client connects to the server, the server will send the client a text detailing the server's function. The client will print to the screen the text received from the server.
Specifically, the server must send the following text to the client:

## insert pic

# If the user typed 1 and enter, the user will be given the option to type a path to a local csv file on his computer, and after pressing enter the client will send the contents of the file to the server. At the end of sending, the server will send back to the client a ".Upload complete" message. If the path is invalid, "invalid input" will be printed (and you return to the main menu). It should look like this:


## insert pic


If the user presses enter, the parameters must be left unchanged (make sure the values are initialized to the values in the example above). Otherwise, the user can enter new values separated by a space, where K can have a range of integer values and the distance meter can have the values we saw in the previous exercise) 5 metrics). If everything is correct - return to the main menu. If the user entered an invalid value - the error message will be returned: invalid value for K or invalid value for metric (if necessary - print both). Then - return to the main menu (without Change - even if one of the parameters was valid.

# If the user chose 3, the server will run the algorithm on the previously uploaded CSV files. At the end of the run, the server will write "classifying data complete" and we will return to the main menu. If no files have been uploaded yet, print "please upload data".

# If the user selected 4, the server will return the list of classifications. For each classification, the printout will be as follows: the number of the line in the test file, a tab, the classification, and then a line break. Finally, ".Done" will be printed after the user's enter to return to the main menu.
where A, B, C are the classification classes. If no files have been uploaded yet, print "please upload data". If the files to be printed have not yet been classified "please classify the data".

## insert pic

# If the user chose 5, the behavior will be similar to that of option 4, only that instead of printing the results, the user will enter a path to create the file locally and there the client will save the results, in exactly the same format (without Done).
 - Please note: unlike the other commands, in this command after the user enters a path (who entered a path to the file) the main menu will immediately appear again and the user will be able to send additional commands immediately, even if the download of the file has not yet finished (that is, this process will happen cn a separate thread).

# If the user chose 8, the interaction between the server and the client will end (don't forget to close and release the connection and resources).


This process will be repeated twice, when the first time you receive a file for training the classifier and the second time a file for his exam. Accordingly, for the first time will appear:
Please upload your local train CSV file. Please upload your local test CSV file.
If the user selected 2, the server will send the current classifier parameter values which are the K parameter value and the current distance metric.
And in the second: ￼￼￼￼ after finishing uploading the second file, the main menu will be displayed again.




# If the user selected 2, the server will send the current classifier parameter values which are the K parameter value and the current distance metric.

 ## pic








# Exercise 4 - Multi-Client Server

## Overview

This project is an exercise in implementing a server-client system where the server can handle multiple clients simultaneously. The server is responsible for handling requests from the clients, which are "dumb" in the sense that they simply send information entered by the user and display information received from the server. We implemented the project using the Command design pattern


![image](https://user-images.githubusercontent.com/118285361/213496798-c8a85ca2-e37d-4f36-b3f9-e6bce53ca888.png)


The server can handle the following requests from the clients:
- Uploading a local CSV file for training or testing the classifier
- Retrieving the current classifier parameter values (K and distance metric)
- Running the algorithm on the previously uploaded CSV files
- Retrieving the list of classifications
- Saving the list of classifications to a local file
- Ending the interaction between the server and client

### note: 
* The code handles invalid input and displays appropriate error messages
* Saving the list of classifications to a local file happens in a separate thread
* Connection and resources are closed and released when the interaction between the server and client ends.

## How to Run
write "make" in terminal 
