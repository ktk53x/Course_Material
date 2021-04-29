Assignment II: Advance Java Concurrent Programming

Name: Kartikeya Saxena
Roll Number: 180101034

To run:
	javac answer.java
	java Main

	or for a descriptive output

	java answer.java
	java Main verbose > output.txt

Output:
	the program will output
	Duration: {time in seconds}
	The Duration was 120-130 seconds on my machine

Node Class:
This class has accountNumber, amount and a lock

list Class:
The linked list implements the lazy synchronization.For more information refer Chapter 9, Section 9.7 of "Art Of Multiprogramming"

{Deposit, Withdraw, Transfer, AddCustomer, DeleteCustomer, TransferCustomer} Classes:
These are the requests which can be submitted to the updaters in the branches.

Bank Class:
This Class contains HashMap of the ten branch lists with repective Updators and the respective methods to make changes to the accounts.

Main Class:
This Class Simulates the requests with probability distribution as given in the problem statement and prints the time taken to complete these requests.
