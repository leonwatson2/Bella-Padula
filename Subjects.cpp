//Vernon Watson - Program 1 - Subjects.cpp
//CSCE 4550,
// This is a file that is used for SecureClass.cpp
// This contains three classes: Subjects, Objects, and Reference Monitor
// Reference monitor holds all the 

#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
using namespace std;

enum SecurityLevel{LOW=4000, MEDIUM, HIGH}; // used for setting and referencing the security level of subjects and objects

/*
* Subject: Class that holds that holds a tempory value
* 	attributes: 
*		string name
*		int value
*		int access_level_index, holds the index of where its access level is defined
*	
*
*/
class Subject{

	public:
	int value;
	Subject(string name, int access_level_index):name(name), accessLevelIndex(access_level_index){}

	string getName(){
		return this->name;
	}
	int getAccessLevelIndex(){
		return this->accessLevelIndex;
	}
	

	string getData(){
		return to_string(this->value);
	}
	private:
	int accessLevelIndex;
	string name;

};

class Object : public Subject{
	
	public:
	Object(string name, int accessLevelIndex):Subject(name, accessLevelIndex){}

	private:
	string name;
	
};

/*
* 	Reference Monitor:
*	attributes:
*		int total_number_of_objects_and_subjects, the total number of object and subject in monitor
*		Subject *subject[], pointers to subjects in monitor
*		Subject *object[], pointers to objects in monitor
*		int number_of_objects, number of object in the monitor
*		int number_of_subjects, number of subjects in the monitor
*	methods:
*		bool executeRead() execute read instruction
*		bool executeWrite() executes write instruction
*		void printAccessResponse() prints access response
*		bool canRead() tests if subject can read object
*		bool canWrite() tests if subject can write to object
*		void createObject() adds object to reference monitor
*		void createSubject() add subject to reference monitor
*		int getAccessLevel() gets the access level of subjec/object
*		bool areValid() checks if the subject and object are valid
*		Object * getObject() get the pointer the object with the name
*		Subject * getSubject() get the pointer the subject with the name
*		void printTable() prints the values and names of the subject and object in the system
*/
class ReferenceMonitor{
	private:
	int total_number_of_objects_and_subjects = 0;
	int number_of_objects = 0;
	int number_of_subjects = 0;
	Subject *subjects[6];	//Subjects in system
	Object *objects[6];		//Objects in system
	public:
	ReferenceMonitor(){}
	~ReferenceMonitor(){
		
	}
	SecurityLevel security_level_list[12]; //List of security level for all twelve subjects and objects
	
	/*
	*	Executes write by the subject to the object if possible and return true.
	*	If not possible returns false. 
	*	@param string, subjectName the name of the subject 
	*	@param string, objectName the name of the object 
	*	@return bool
	*		true: the execution was successful
	*		false: the execution was unsuccessful
	*/	
	bool executeRead(string subjectName, string objectName){
		string action = subjectName + " reads " + objectName;

		Subject *subject = this->getSubject(subjectName);
		Object *object = this->getObject(objectName) ;
		
		if(areValid(subject, object)){

			if(canRead(subject, object)){
				subject->value = stoi(object->getData());
				printAccessResponse(subjectName, objectName, true, true);
			}else{
				printAccessResponse(subjectName, objectName, false, true);
			}

			return true;
		}
		return false;
	
	}

	/*
	*	Executes write by the subject to the object if possible and return true.
	*	If not possible returns false. 
	*	@param string, subjectName the name of the subject 
	*	@param string, objectName the name of the object 
	*	@return bool
	*		true: the execution was successful
	*		false: the execution was unsuccessful
	*/
	bool executeWrite(string subjectName, string objectName, string value){

		Subject *subject = this->getSubject(subjectName);
		Object *object = this->getObject(objectName);
		if(areValid(subject, object)){

			if(canWrite(subject, object)){
				object->value = stoi(value);
				printAccessResponse(subjectName, objectName, true, false, value);
			} else{
				printAccessResponse(subjectName, objectName, false, false, value);

			}
			return true;
		}
		
		return false;
	}
	/*
	* 	Prints the response for when a subject tries to access a method.
	*	@param string subjectName, the subject name trying to access the object
	*	@param strong objectName, the object name trying being accessed
	*	@param bool isGranted
					true: if access was granted
					false: if access was denied
		@param bool isReading
					true: if subject is reading to object
					false: if subject is writing to object
		@param string value, the value being that is attempted to be written default is "" when reading
	*/
	void printAccessResponse(string subjectName, string objectName, bool isGranted, bool isReading, string value = ""){
		string action;
		if(isGranted){
			if(isReading)
				action = subjectName + " writes value " + value + " to " + objectName;
			else
				action = subjectName + " reads " + objectName;
			
			cout << "Access Granted: " << action << endl;
			printTable();
		}else {
			if(isReading)
				action = "read " + subjectName + " " + objectName;
			else
				action = "write " + subjectName + " " + objectName + " " + value;
			cout << "Access Denied: " << action << endl;
 		}
	}
	
	/*
	*	Checks if the subject can read to the object
	*	@param Subject, subject that is trying to read
	*	@param Subject, object that is being read from
	*	
	*	@return bool if the access level of the subject is greater or the same 
    *				as the object returns true otherwise returns false
	*/
	bool canRead(Subject *subject, Object *object){
		return  getAccessLevel(subject) >= getAccessLevel(object);
	}
	
	/*
	*	Checks if the subject can write to the object
	*	@param Subject, subject that is trying to write
	*	@param Subject, object that is being written to
	*	
	*	@return bool if the access level of the subject is lower or the same 
    *				as the object returns true otherwise returns false
	*/
	bool canWrite(Subject *subject, Object *object){

		return getAccessLevel(subject) <= getAccessLevel(object);
	}

	/*
	*	Creates an subject and stores the security level in an array of this object
	* 	
	*	@param string, object_name the name to reference the object
	*	@param SecurityLevel, level the security level asscociated with the object 
	*	@param bool, isSubject boolean describing what is being created 
	*					true if a subject is being created
	*					false if an object is being created
	*
	*/
	void createSubject(string subjectName, SecurityLevel level, bool isSubject = true){
		int newIndex = this->total_number_of_objects_and_subjects++;
		this->security_level_list[newIndex] = level;
		
		if(isSubject){
			this->subjects[this->number_of_subjects++] = new Subject(subjectName, newIndex); 
		}else{
			this->objects[this->number_of_objects++] = new Object(subjectName, newIndex); 
		}

	}

	/*
	*	Creates an object
	* 	@param object_name the name to reference the object
	*	@param level the security level asscociated with the object 
	*/
	void createObject(string objectName, SecurityLevel level){
		this->createSubject(objectName, level, false);
	}

	/*
	*	Gets the Security level of Subject passed in from the security_level_list 
	*	array and returns it.
	*	@param Subject, subject for which to get access level for.
	*	@return SecurityLevel(int) the security level of the subject 
	*/
	int getAccessLevel(Subject *subject){
		return this->security_level_list[subject->getAccessLevelIndex()];		
	}

	/*
	* Tests if the subject and object both don't equal NULL	
	*	@param Subject
	*	@param Subject
	*	@return bool
	*		true: both subject and object are not null
	*		false: subject or object is null
	*/
	bool areValid(Subject *subject, Object *object){
		return (subject != NULL) and (object != NULL);
	}

	/*
	* 	Returns the a pointer to the corresponding subject 
	* 	with the name passed in from the array of subjects.
	* 	If that subject doesn't exist it returns NULL.
	* 	@param string, name of the subject
	* 	@return Subject *, pointer to the subject or NULL	
	*/
	Subject * getSubject(string name){
		for(int i = 0; i < this->number_of_subjects; i++){
			if(name == this->subjects[i]->getName())
				return this->subjects[i];
				
		}
		return NULL;

	}

	/*
	* 	Returns the a pointer to the corresponding object 
	* 	with the name passed in from the array of objects.
	* 	If that object doesn't exist it returns NULL.
	* 	@param string, name of the object
	* 	@return Object *, pointer to the object or NULL
	*/
	Object * getObject(string name){
		for(int i = 0; i < this->number_of_objects; i++){
			if(name == this->objects[i]->getName())
				return this->objects[i];
		}
		return NULL;

	}
	/*
	* prints the values and names of the subject and object in the system
	*/
	void printTable(){
		string temp = "";
		temp += "+-------------------------The current state---------------------+\n";
		bool printedNames = false;
		for (int i = 0; i <= this->number_of_subjects; i++){
				if (!printedNames and i == this->number_of_subjects)
				{
					printedNames = true;
					temp +="|\n";
					i = 0;
				}else if(i >= this->number_of_subjects){
					break;
				}
				//print Names
				if(!printedNames){
					if(i == 0)
						temp += "| subject \t";
					temp += this->subjects[i]->getName();
				}else{
					if(i == 0)
						temp += "| values \t";
					temp += this->subjects[i]->getData();
				}
				temp += "\t";
		}
		temp +=  "|\n+---------------------------------------------------------------";

		temp += "|\n";
		printedNames = false;
		for (int i = 0; i <= this->number_of_objects; i++){
				if (!printedNames and i == this->number_of_objects)
				{
					printedNames = true;
					temp +="|\n";
					i = 0;
				}else if(i >= this->number_of_objects){
					break;
				}
				//print Names
				if(!printedNames){
					if(i == 0)
						temp += "| object \t";
					temp += this->objects[i]->getName();
				}else{
					//print values
					if(i == 0)
						temp += "| values \t";
					temp += this->objects[i]->getData();
				}
				temp += "\t";
		}
		temp += "|\n+---------------------------------------------------------------+";

		cout << temp << endl;
	}
};

