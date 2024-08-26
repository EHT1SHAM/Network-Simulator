#pragma once
#include <iostream>
#include "tree.h"
#include <string>
#include "Graph.h"
#include "Globals.h"
#include "Router.h"
#include "Parser.h"
#include <fstream>


#define CLRBUFFER {cin.ignore(1000,'\n');; cin.clear();}

void printRT(Router** myR, string command) {

	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	int index = getRouterIndex(tokens.getAtIndex(2));

	if (index < 0) {
		return;
	}

	myR[index]->printRoutingTable();
}

void printPathsTo(Graph& topology, string command) {


	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());

	if (deviceOne == -1 || deviceOne >= topology.getVerticesCount()) {
		exit(1);
	}
	cout << "Shortest Path Between Two Machines-> " << endl;
	for (int i = 0; i < topology.getMachinesCount(); i++) {
		NodeDetails* info = topology.getDij(i);
		cout << info->pathList[deviceOne] << endl;
		//printArray(info->pathList, topology.getMachinesCount());
		delete info;
	}
	
}

void printPathsFrom(Graph& topology, string command) {


	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());

	if (deviceOne == -1 || deviceOne >= topology.getVerticesCount()) {
		exit(1);
	}

	NodeDetails* info = topology.getDij(deviceOne);
	cout << "Shortest Path Between Two Machines-> " << endl;
	printArray(info->pathList, topology.getMachinesCount());
}

void writeMessageToFile(Message& msg, string filename = "path.txt") {

	fstream file;
	file.open(filename, ios::in | ios::out | ios::app);

	if (file.fail()) {
		cout << "Failed To Open File, Message Wont Be Written." << endl;
		return;
	}

	file << msg << endl << endl;
	file.close();
}
int getConnectedRouter(string src, Graph& tp) {

	int rIndex = tp.getRouterConnectedToMachine(extractInteger(src) - 1);
	// cout << rIndex - tp.getMachinesCount() << endl;
	return rIndex - tp.getMachinesCount();
}

Message* generateMessage(int id, int priority, Graph& tp, string source, string destination, string msg) {

	Message* newMsg = new Message;
	newMsg->id = id;
	newMsg->priority = priority;
	newMsg->destination = destination;
	newMsg->source = source;
	newMsg->payload = msg;

	int src = getRouterIndex(source);
	int dst = getRouterIndex(destination);
	/*
	//newMsg->trace = tp.getDij(src)->pathList[dst];


	LinkedList<string> tokens;
	stringstream pathstream(newMsg->trace);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	for (int i = 0; i < tokens.getListSize(); i++) {
		newMsg->path.insert(tokens.getAtIndex(i));
	}
	*/
	return newMsg;
}

RouterType* getRouters(Graph& topology, string choice = "?") {
	Router** myR = new Router * [topology.getRoutersCount()];
	RouterType* myI = new RouterType;
	//myR = new TreeRouter;
	//

	int modeChoice = 0;
	if (choice == "?") {
		cout << "Creating New Routers..." << endl;
		cout << "1- LinkedList Routing Tables.\n2- SplayTree Routing Tables.\n>>> ";
		cin >> modeChoice;
		CLRBUFFER;
	}
	else if (choice == "list") {
		modeChoice = 1;
	}
	else if (choice == "tree") {
		modeChoice = 2;
	}


	if (modeChoice == 1) {
		if (choice == "?") {
			myI->type = "list";
		}

		for (int i = 0; i < topology.getRoutersCount(); i++) {
			myR[i] = new ListRouter;
		}
	}
	else if (modeChoice == 2) {
		if (choice == "?") {
			myI->type = "tree";
		}
		for (int i = 0; i < topology.getRoutersCount(); i++) {
			myR[i] = new TreeRouter;
		}
	}
	else {
		cout << "Wrong Input" << endl;
		exit(1);
	}


	for (int i = 0; i < topology.getRoutersCount(); i++) {
		myR[i]->setID(i);
		myR[i]->setTopology(topology);
		myR[i]->generateRoutingTable();

	}
	myI->list = myR;
	return myI;
}

void sendSimpleMessage(Graph& topology, Router** myR) {
	int modeChoice = 1;
	for (int i = 0; modeChoice != 0; i++) {
		system("CLS");
		string source;
		string destination;
		string message;
		int priority = -1;



		cout << "Enter Source Machine.\n>>> ";
		getline(cin, source);

		cout << "Enter Destination Machine.\n>>> ";
		getline(cin, destination);

		cout << "Enter Message.\n>>> ";
		getline(cin, message);

		cout << "Enter Priority.\n>>> ";
		cin >> priority;

		CLRBUFFER;

		Message* msg = generateMessage(i, priority, topology, source, destination, message);
		myR[getConnectedRouter(source, topology)]->receiveMessage(*msg, myR);
		msg->trace = msg->trace.substr(0, msg->trace.length() - 1);
		cout << "Trace Of The Message -> " << msg->trace << endl;
		writeMessageToFile(*msg);
		// message ban gaya hai
		// implement a max heap to take message
		// find the router to take messages and send it to machines

		cout << "Do You Want To Send Another Message? Press 1 For Another Message Or 0 To End Message Mode.\n>>> ";
		cin >> modeChoice;
		
	}
	CLRBUFFER;
}

void deleteRouters(Router** myR, Graph& topology) {
	for (int i = 0; i < topology.getRoutersCount(); i++) {

		delete myR[i];
	}
	//cout << "here" << endl;
	delete[] myR;
	myR = nullptr;
}

Router** changeEdgeWeight(Router** myR, string type, Graph& topology, string command) {

	deleteRouters(myR, topology);
	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());
	int deviceTwo = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());
	//cout << deviceOne << "-" << deviceTwo << endl;

	if (deviceOne == -1 || deviceTwo == -1 || deviceOne >= topology.getVerticesCount() || deviceTwo >= topology.getVerticesCount()) {
		exit(1);
	}
	topology.getAdjMatrix()[deviceOne][deviceTwo] = stoi(tokens.getAtIndex(4));
	topology.getAdjMatrix()[deviceTwo][deviceOne] = stoi(tokens.getAtIndex(4));
	myR = getRouters(topology, type)->list;
	cout << "\nEdge Weight Updated." << endl;
	return myR;
}


void printPath(Graph& topology, string command) {

	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());
	int deviceTwo = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());

	if (deviceOne == -1 || deviceTwo == -1 || deviceOne >= topology.getVerticesCount() || deviceTwo >= topology.getVerticesCount()) {
		exit(1);
	}

	NodeDetails* info = topology.getDij(deviceOne);
	cout << "Shortest Path Between Two Machines-> " << info->pathList[deviceTwo] << endl;
}

void sendMessagesByFile(Router** myR, Graph& topology, string command) {

	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	string filename = tokens.getAtIndex(2);
	fstream file;
	file.open(filename, ios::in | ios::out);
	if (file.fail()) {
		cerr << "Failed To Open Messages File" << endl;
		exit(1);
	}
	LinkedList<string> messageList;
	string fileData;
	while (!file.eof()) {

		string temp;
		getline(file, temp);

		messageList.insert(temp);

	}
	file.close();
	Message* msgs = new Message[messageList.getListSize()];

	for (int i = 0; i < messageList.getListSize(); i++) {

		LinkedList<string> messageToken;
		stringstream messageStream(messageList.getAtIndex(i));
		string tempTokens;
		while (getline(messageStream, tempTokens, ':')) {
			messageToken.insert(tempTokens);
		}
		msgs[i].id = stoi(messageToken.getAtIndex(0));
		msgs[i].priority = stoi(messageToken.getAtIndex(1));
		msgs[i].source = messageToken.getAtIndex(2);
		msgs[i].destination = messageToken.getAtIndex(3);
		msgs[i].payload = messageToken.getAtIndex(4);

	}
	int modeChoice = 0;
	cout << "\n1- Send All Messages From File.\n2- Send A Particular Message.\n>>> ";
	cin >> modeChoice;

	if (modeChoice == 1) {
		MaxHeap<Message> priorityMsgs;

		for (int i = 0; i < messageList.getListSize(); i++) {

			priorityMsgs.insert(msgs[i]);
		}
		// fileData = fileData.substr(0, fileData.length() - 1);
		// cout << fileData;
		//cout << priorityMsgs.getArray()[0] << endl;
		for (int i = 0; i < messageList.getListSize(); i++) {
			Message toSend = priorityMsgs.getMax();
			static_cast<void>(priorityMsgs.popMax());
			cout << "Sending The Following Message\n";
			cout << toSend << endl;
			myR[getConnectedRouter(toSend.source, topology)]->receiveMessage(toSend, myR);
			toSend.trace = toSend.trace.substr(0, toSend.trace.length() - 1);
			cout << "Trace->" << toSend.trace << endl << endl;
			writeMessageToFile(toSend);
		}
	}
	else if (modeChoice == 2) {
		for (int i = 0; i < messageList.getListSize(); i++) {
			cout << msgs[i] << endl << endl;
		}
		cout << "\nSelect Message Id To Send\n>>> ";
		cin >> modeChoice;
		if (modeChoice > messageList.getListSize() || modeChoice <= 0) {
			cout << "ID Does Not Exit" << endl;
			return;
		}
		cout << "\nSending The Following Message\n";
		cout << msgs[modeChoice - 1] << endl;
		myR[getConnectedRouter(msgs[modeChoice - 1].source, topology)]->receiveMessage(msgs[modeChoice - 1], myR);
		msgs[modeChoice - 1].trace = msgs[modeChoice - 1].trace.substr(0, msgs[modeChoice - 1].trace.length() - 1);
		cout << "Trace->" << msgs[modeChoice - 1].trace << endl << endl;
		writeMessageToFile(msgs[modeChoice - 1]);
	}
	else {
		cout << "Wrong Input" << endl;
		exit(1);
	}
	CLRBUFFER;
}




void printHelpCommands() {

	cout << "-> send msg (to send a custom message)" << endl;
	cout << "-> send msg filename (to send messages from file)" << endl;
	cout << "-> print path M1 M2 (to print path between two machines)" << endl;
	cout << "-> print path M1 * (to print path between machine to all others machines)" << endl;
	cout << "-> print path * M1 (to print path between from all others machines to a specific machine)" << endl;	
	cout << "-> change edge R1 M2 13 (to change an edge weight dynamically between two devices)" << endl;
	cout << "-> print RT R1 (to print a routing table)" << endl;
	cout << "-> change RT R1 add M9 R4 (to add a routing field to router, M9 = destination, R4 = next device)" << endl;
	cout << "-> change RT R1 add filename (to add to routing tables using filename)" << endl;
	cout << "-> change RT R1 remove M9 R4 (to remove(if exists) a routing field from router, M9 = destination, R4 = next device)" << endl;
	cout << "-> change RT R1 remove filename (to remove from routing tables using filename)" << endl;
	cout << "-> cls (to clear the screen)" << endl;
	cout << "-> exit (to exit program)" << endl;


}

void addToTableUsingCLI(Router** myR, string command) {

	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int routerIndex = getRouterIndex(tokens.getAtIndex(2));

	myR[routerIndex]->addToRouterTable(tokens.getAtIndex(4), tokens.getAtIndex(5));
	cout << "\nUpdated Routing Table: " << endl;
	myR[routerIndex]->printRoutingTable();
}

void addToTableUsingFile(Router** myR, string command) {
	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	string filename = tokens.getAtIndex(4);
	fstream file;
	file.open(filename, ios::in | ios::out);
	if (file.fail()) {
		cerr << "Failed To Open Messages File" << endl;
		exit(1);
	}
	LinkedList<string> fieldList;
	while (!file.eof()) {

		string temp;
		getline(file, temp);

		fieldList.insert(temp);

	}
	file.close();
	int routerIndex = getRouterIndex(tokens.getAtIndex(2));
	for (int i = 0; i < fieldList.getListSize(); i++) {
		myR[routerIndex]->addToRouterTable(fieldList.getAtIndex(i).substr(0, fieldList.getAtIndex(i).find(":")), fieldList.getAtIndex(i).substr(fieldList.getAtIndex(i).find(":") + 1, fieldList.getAtIndex(i).length()));
	}
	cout << "\nUpdated Routing Table: " << endl;
	myR[routerIndex]->printRoutingTable();
}


void removeFromTableUsingCLI(Router** myR, string command) {

	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int routerIndex = getRouterIndex(tokens.getAtIndex(2));

	myR[routerIndex]->deleteFromRouterTable(tokens.getAtIndex(4), tokens.getAtIndex(5));
	cout << "\nUpdated Routing Table: " << endl;
	myR[routerIndex]->printRoutingTable();
}

void removeFromTableUsingFile(Router** myR, string command) {
	LinkedList<string> tokens;

	stringstream pathstream(command);
	string temp;

	while (getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	string filename = tokens.getAtIndex(4);
	fstream file;
	file.open(filename, ios::in | ios::out);
	if (file.fail()) {
		cerr << "Failed To Open Messages File" << endl;
		exit(1);
	}
	LinkedList<string> fieldList;
	while (!file.eof()) {

		string temp;
		getline(file, temp);

		fieldList.insert(temp);

	}
	file.close();
	int routerIndex = getRouterIndex(tokens.getAtIndex(2));
	for (int i = 0; i < fieldList.getListSize(); i++) {
		myR[routerIndex]->deleteFromRouterTable(fieldList.getAtIndex(i).substr(0, fieldList.getAtIndex(i).find(":")), fieldList.getAtIndex(i).substr(fieldList.getAtIndex(i).find(":") + 1, fieldList.getAtIndex(i).length()));
	}
	cout << "\nUpdated Routing Table: " << endl;
	myR[routerIndex]->printRoutingTable();
}