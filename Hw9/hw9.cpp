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
// Description: state machine 
//
// Input: NONE
//
// Output: GPs log files
//
//******************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>


#include "parser9.h"

using namespace std;

int main(int argc,  char *argv[])
{
	ifstream infile;
    char byte;  //the byte variable for each byte received

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
	
	if(argc > 2 || argc <= 1 ||  argv[1] == "/?" )
	{
		cout << "NMEA GPS Sentence Parser" << endl << "Usage: prog1 [input filename]" << endl;
		return(0);
	}
	infile.open(argv[1]);
	if(!infile.is_open()){		
		cout << "ERROR opening input log file " << argv[1] << endl;
		return(0);
	}
/* read the input file, byte by byte and print each line */
	while(!infile.eof()) 
	{ 
			infile.get(byte);
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
						infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}	
					if(byte=='*')
					{
						infile>>hex>>byte;
						hex_num+=byte;
						infile>>hex>>byte;
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
						infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')
					{
						infile.get(byte);
						hex_num+=byte;
						infile.get(byte);
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
						infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')   //checks for star
					{
						infile.get(byte);
						hex_num+=byte;
						infile.get(byte);
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
						infile.get(byte);
						message1+=byte;
						if(byte=='$')
						{
							message1.clear();
							identify.clear();
						}
					}
					if(byte=='*')
					{
						infile.get(byte);
						hex_num+=byte;
						infile.get(byte);
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
			   {    const char * clear="cls";
					system(clear);
					// print message Type, Data, and The check Sum hexadecimal number. 
					cout<<"Message type: "<<idfirst<<endl;
					cout<<"Message data: "<<id<<endl;
					cout<<"Message checksum: "<<hex_num<<"\n"<<endl;	
					
					//this is gets the values from GPGGA sentence 
					if(idfirst==A)
					{ 
						enter_point_GGA(&num1,message1); 
						//print_point_GGA(num1); 	
						printFinal(num1);
					}
					else if(idfirst==B)  //this gets the values from the GPGSA sentence 
					{
						enter_point_GSA(&num1,message1);
						//print_point_GSA(num1);
						printFinal(num1);
					}
					else if(idfirst==C)  //this gets the values from the GPRMC sentence 
					{
						enter_point_RMC(&num1,message1);
						//print_point_RMC(num1);
						printFinal(num1);
					}
						else if(idfirst==D)  //this gets the values from the GPGSV sentence 
					{
						enter_point_GSV(&num1,message1);
						//print_point_GSV(num1);
						printFinal(num1);
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
	infile.close();		/* close file */
}		/* end main */