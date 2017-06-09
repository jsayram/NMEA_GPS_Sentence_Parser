//******************************************************************
// Program #: 9
//
// Programmer: Jose Ramirez
//
// Due Date: NA
//
// EGRE 246, Fall 2014       Instructor: Robert Klenke
//
// Pledge: I have neither given nor received unauthorized aid on this program.
//
// Description:Main Class take in data from a GPS unit plugged into the serial port.,
//                     parses the GPS Sentences and outputs the data taken out in real time. 
//
// Input: Serial Port connection COM5 port , and GPS reciever 
//
// Output: GPS message with data parsed from each sentence 
//
//******************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 
#include <sstream>

#include <windows.h>
#include <stdlib.h>

#include "parser9.h"


using namespace std;

int main(int argc,  char *argv[])
{
	char Port_Name[] = "COM5";
	unsigned char byte;
	DWORD baudrate = 4800;
	
	HANDLE hSerial;

//Variables for main parser *********************************
	int find_count=0; // at the end of the file checks the number of messages printed out.
	int checksum=0; //integer to check the sum of the XOR operation
	int checkthis=0; //this is the one to check it and compare too , converting string to hex to decimal
	int size=0; //this will be the size of the message buffer so it can be parsed later.

	string identify; //string variable for the first part of the message. 
	string id;     //this holds the message buffer
	string idfirst;  //this is the first 5 characters of the message buffer 
	string message1; //this hold the uncut message with the * 

    parser fone;   //good to go
	string hex_num; //string variable for the last two digits after the *
	
	struct Data num1;
//*****************************************************
	if(argc == 1) {
		cerr << "[" << Port_Name << "] [" << baudrate << "]" << endl;
	}
	else if(argc == 2) {
		strcpy(Port_Name,argv[1]);
		cerr << "[" << Port_Name << "] [" << baudrate << "]" << endl;
	}
	else if(argc == 3) {
		strcpy(Port_Name,argv[1]);
		baudrate = (DWORD)atoi(argv[2]);
		cerr << "[" << Port_Name << "] [" << baudrate << "]" << endl;
	}
	else {
		cerr << "usage:" << argv[0] << " [DEVICE] [BAUDRATE]" << endl;
		return(0);
	}
	
	hSerial = CreateFile(Port_Name,
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						0);
						
	if(hSerial==INVALID_HANDLE_VALUE){
		if(GetLastError()==ERROR_FILE_NOT_FOUND){
			//serial port does not exist. Inform user.
			cerr << "ERROR " << Port_Name << " does not exist!" << endl;
			return 0;
		}
		//some other error occurred. Inform user.
		cerr << "ERROR unknown error for " << Port_Name << endl;
		return 0;
	}
	
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		//error getting state
		cerr << "ERROR getting state for " << Port_Name << endl;
		return 0;
	}
	
	dcbSerialParams.BaudRate=baudrate;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if(!SetCommState(hSerial, &dcbSerialParams)){
		//error setting serial port state
		cerr << "ERROR setting parameters for " << Port_Name << endl;
		return 0;
	}
	
	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=50;
	timeouts.ReadTotalTimeoutMultiplier=10;
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=10;
	if(!SetCommTimeouts(hSerial, &timeouts)){
		//error occureed. Inform user
		cerr << "ERROR setting timeouts for " << Port_Name << endl;
		return 0;
	}
	
	DWORD dwBytesRead = 0;
	
	while(1) {
		if(!ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL)){ //reads bytes coming in one byte at a time
			//error occurred. Report to user.
			cerr << "ERROR reading from " << Port_Name << endl;
		}
		if(dwBytesRead == 1){
			if(fone.parser_id(byte) == true)   //state machine to parse the file
			{     
				message1.clear();   //clears the middle sentence once is printed or discarded 
				hex_num.clear();   //clears the hex_number variable 
				identify.clear();      //clears the identify (message Type ) string variable before continuing 
				identify=fone.messageType(); // returns the message type based on the last state of the state machine and then continues. 
				 if  (identify==GPGGA)    //Checks if identify variable matches the message type
				{	find_count++; //gets the count for the message 
					identify=fone.messageCase(identify);
					while(byte!='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}	
					if(byte=='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile>>hex>>byte;
						hex_num+=byte;
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile>>hex>>byte;
						hex_num+=byte;
					}
					else if (byte=='$')
					{
						hex_num.clear();
						message1.clear();
						identify.clear();
					}
				}
				if  (identify==GPGSA)     //Checks if identify variable matches the message type
				{	find_count++; //gets the count for the message 
					identify=fone.messageCase(identify);
					while(byte!='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
					}
					else if (byte=='$')
					{
						hex_num.clear();
						message1.clear();
						identify.clear();
					}
				}
				if  (identify==GPRMC)  //Checks if identify variable matches the message type
				{	find_count++; //gets the count for the message 
					identify=fone.messageCase(identify);
					while(byte!='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')   //checks for star
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
					}
					else if (byte=='$')
					{
						hex_num.clear();
						message1.clear();
						identify.clear();
					}
				}
				if (identify==GPGSV) //Checks if identify variable matches the message type
				{	find_count++; //gets the count for the message 
					identify=fone.messageCase(identify); 
					while(byte!='*')  
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')
					{
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
						ReadFile(hSerial, &byte, 1, &dwBytesRead, NULL);
						//infile.get(byte);
						hex_num+=byte;
					}
					else if (byte=='$')
					{
						hex_num.clear();
						message1.clear();
						identify.clear();
					}
				}
					  
				 size= message1.size();  //grabs the size of the string message after the message Type including the * 
					 
				//ones it gets the size subtracts one to get rid of asterisks once found and returns 
				//to the message1, all after message type and before the asterisk *
				message1.resize(size-1); 
     				
				id=identify+message1;   //appends the message type and the sentence without the $ and the * sets to ID
					
				//this grabs the id type string from the message buffer
				 //its the first five characters. 
				idfirst=id; //this sets the idfirst string variable equal to the whole sentence. 
				 idfirst.resize(5); //sets first 5 characters the the idFirst , This will be the final message type variables to be printed. 
			    
				int checksum=0;   //check sum integer variable to do a checksum on the message . 
				for (int i = 0; i < id.length(); i++)
				{
					checksum ^= id[i];   //loops the llenght of the sentence and XORS each byte and sets them equal to the check sum variable. 
				} 
				stringstream ss;  //this creates a string stream variable so the string hex_sum holding the last to digits after the * can be converted 
											// to a hexadecimal number. 
				ss  << hex_num ; 
				ss >> hex >> checkthis;  //set the converted value of hex_sum string variable to checkthis so it can be compared with checksum
				
			   if(checkthis==checksum)  // checks if the checksum checks out with the sentence 
			   {   
					//clears the console 
					const char * clear="cls";
					 system(clear);
					//this is gets the values from GPGGA sentence 
					if(idfirst==A)
					{ 
						enter_point_GGA(&num1,message1);  	//this parses the GGA sentence
						printFinal(num1); //prints what outputted to console
					}
					else if(idfirst==B)  //this gets the values from the GPGSA sentence 
					{
						enter_point_GSA(&num1,message1); //this parses the GSA sentence 
						printFinal(num1); //prints what is outputted to console 
					}
					else if(idfirst==C)  //this gets the values from the GPRMC sentence 
					{
						enter_point_RMC(&num1,message1); //this parses the RMC message 
						printFinal(num1); //prints what is outputted to the console 
					}
						else if(idfirst==D)  //this gets the values from the GPGSV sentence 
					{
						enter_point_GSV(&num1,message1); //this parses the GSV sentence 
						printFinal(num1); //prints what is outputted to the console 
					}
		    	
				}
				else     //if the checksum does not check out then all of the message and variables are deleted and the program keeps going. 
				{
					idfirst.clear();  //clears the message Type
					id.clear();				//clears the sentence 
					hex_num.clear();	//clears the hex number. 
				}
			}	
			}
	}	
}		/* end main */
