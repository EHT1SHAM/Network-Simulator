#include <iostream>
#include "CLI.h"
using namespace std;



int main() {
	
	
	Graph topology;
	topology.readFromCSV();
	RouterType* rInfo = getRouters(topology);
	Router** myR = rInfo->list;
	string myT = rInfo->type;
	//CLRBUFFER;
	system("CLS");
	cout << "Commands are case sensitive (type \"help\" for commands list)" << endl;
	//cout << myT << endl;
	while (1) {
		
		cout << "\nNetwork Emulator >>> ";
		string command;
		getline(cin, command);//read command
		
		string parseCmd =  parseCommand(command); //sending to parse function file
		if (parseCmd == "sendMessage") {
			sendSimpleMessage(topology, myR); //function called from command
		}
		if (parseCmd == "printRT") {
			printRT(myR, command);
		}
		else if (parseCmd[0] == '?') {
			cout << endl << parseCmd.substr(1, parseCmd.length()) << endl;
		}
		else if (parseCmd == "clearScreen") {
			system("CLS");
		}
		else if (parseCmd == "exit") {
			exit(0);
		}
		else if (parseCmd == "changeEdge") {
			myR = changeEdgeWeight(myR, myT, topology, command);
		}
		else if (parseCmd == "printPath") {
			printPath(topology, command);
		}
		else if (parseCmd == "printPathTo") {
			printPathsTo(topology, command);
		}
		else if (parseCmd == "printPathFrom") {
			printPathsFrom(topology, command);
		}
		else if (parseCmd == "sendMessageFile") {
			sendMessagesByFile(myR, topology, command);
		}
		else if (parseCmd == "help") {
			printHelpCommands();
		}
		else if (parseCmd == "addTableCLI") {
			addToTableUsingCLI(myR, command);
		}
		else if (parseCmd == "addTableFile") {
			addToTableUsingFile(myR, command);
		}
		else if (parseCmd == "removeTableCLI") {
			removeFromTableUsingCLI(myR, command);
		}
		else if (parseCmd == "removeTableFile") {
			removeFromTableUsingFile(myR, command);
		}
		else {
			cout << "\nInvalid command (type \"help\" for commands list)" << endl;//base case for inputvalidation
		}
	}

	return 0;
}
