//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Vernon Watson - Project 1
//	CSCE 4550 - leon@vlw2.com
//	This program implements a simple "Secure" system 
//	following the Bell-LaPadula security rules and the *-property
//	The subjects read and write to objects following the 'no read up' and 'no write down'
//	heiarchy of Bell-LaPadula. The three Security levels are LOW, MEDIUM, and HIGH.
//	The reference monitor checks security levels and executes intructions.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Subjects.cpp"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//prints out bad instruction error
void printBadInstruction(string instruction){
	cout << "Bad Instruction: " + instruction << endl;

}

/*
* 	Tries to execute instruction using the specified reference Monitor
*	Parses instruction and sends it to reference monitor to handle
*	@param string instruction, the instruction passed in
*		ex: 'read tim lobj' or 'write love tobj 20'
*	@param ReferenceMonitor referenceMonitor, the monitor to execute instruction
*	@return bool,
*		true: if the instruction is valid and the subject and objects exists
*		false: if the instruction is invalid 
*/
bool tryInstruction(string instruction, ReferenceMonitor *referenceMonitor){
	string action;
	string subject;
	string object;
	string value;

	stringstream stringStream(instruction);

	//get action
	if(!(stringStream >> action)){
		printBadInstruction(instruction);
		return false;
	} else if(action != "read" and action != "write") {
		printBadInstruction(instruction);
		return false;
	}
	//get subject
	if(!(stringStream >> subject)) {
		printBadInstruction(instruction);
		return false;
	}
	//get object
	if(!(stringStream >> object)){
		printBadInstruction(instruction);
		return false;
	}

	//if action is not write and there is a value
	if(action == "write" and !(stringStream >> value)){
		printBadInstruction(instruction);
		return false; 
	}
	else if(stringStream >> instruction){
		return false;
	}
	if(action == "read"){
		
		if(referenceMonitor -> executeRead(subject, object)){
			return true;
		}else{
			printBadInstruction(instruction);
			return false;
		}
	}


	if(action == "write"){

		if(referenceMonitor -> executeWrite(subject, object, value)){
			return true;
		}else{
			printBadInstruction(instruction);
			return false;
		}
	}
	return false;

}

int main(int argc, char* argv[])

{
	ReferenceMonitor refM;
	
	if(argc > 1)
		cout << "File name: " << argv[1] << endl;
	else 
		cout << "No file name provided: For first argument.";

	string temp;
	ifstream file;
	file.open(argv[1]);
	refM.createSubject("adam", LOW);
	refM.createSubject("james", MEDIUM);
	refM.createSubject("tim", HIGH);	
	refM.createSubject("sara", LOW);
	refM.createSubject("kristy", MEDIUM);
	refM.createSubject("liz", HIGH);

	refM.createObject("aobj", LOW);
	refM.createObject("jobj", MEDIUM);
	refM.createObject("tobj", HIGH);	
	refM.createObject("sobj", LOW);
	refM.createObject("kobj", MEDIUM);
	refM.createObject("lobj", HIGH);

	if(file.is_open()){
		
		while(getline(file, temp)){
			
			tryInstruction(temp, &refM);
				
		}
		file.close();
	}
	else{
		cout << "Failed to open file" << endl;
		return 0;
	}


	
	
return 0;

}
