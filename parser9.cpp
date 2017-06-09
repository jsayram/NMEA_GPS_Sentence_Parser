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
// Description: Parser Class, finds 4 types message types from a .log file,
//								 State machine, and point extraction from each NMEA sentence found
//								and function definition and implementations 
//
// Input: none
//
// Output: none
//
//******************************************************************

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip> 

#include "parser9.h"


using namespace std;

parser::parser()									// constructor
{     
	 // sets up strings to find in incoming characters for the message Type
	 $GPGGA="$GPGGA";		
	 $GPGSA="$GPGSA";
	 $GPRMC="$GPRMC";
	 $GPGSV="$GPGSV";
	id = None;	   	  // set initial id to "None"
	this->name=X; //set initial state of name to "X" 
}
parser::~parser()			// destructor
{
}

//state machine for determining if the message type has been received. 
bool parser::parser_id(char c)
{
	if(id == None && c == $GPGGA[0]) {					// found 1st character which is $
		id = $;									// state of the id is $
	}
	else if(id == $)   //if the state is $ continue 
	{
		if(c == $GPGGA[1])	 //if found the second character ,G1 
			id = G1;					 // state of the machine is G1, 
		else if(c == $GPGGA[0]) {		//else if found the $ again 
			id = $;								// go back to state of $
		}
		else					// didn't find the 1st or 2nd character
			id = None;		// go back to the NONE id
			
	}
	else if(id == G1)  // If the state is at G1 , First G
	{
		if(c == $GPGGA[2])   	// found the P character 
		{		
			id = P;						//Set the state to P 
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;									// go back to state of $
		}
		else						// didn't find 3rd or 1st character
			id = None;			// go back to the NONE id
	}
//the above state machine is for the first 3 characters in all the Message Types $GP 
//if found all 3 in that sequence we can continue to the rest
	//finding the fourth character 
	else if(id == P)				//if the state is at P 
	{
		if(c == $GPGGA[3]) {		 //The next character is a G 
			id = G2;  //#1 set the id to found the G2 in GPGGA 
		}
		else if(c== $GPRMC[3]){  //but if next next character is an R 
			id=R;  // #2 set the id to found the R In GPRMC
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else												// didn't find 4th or 1st character
			id = None;									// go back to the NONE id
	}

//finding the fourth character for GPGGA, GPGSA, GPGSV
	else if(id == G2)  //#1 if the id is at G2 state
	{
		if(c == $GPGGA[4]) {		//if the next character is a G , found the third G GPGGA
			id = G3;   //#1a sets the id state to G3
		}
		else if(c== $GPGSA[4]){  //if the next character is a S , found S in GPGSA
			id=S1;   // #2a sets the id state to S1
		}
		else if(c== $GPGSV[4]){ // if the next character is a S , found S in GPGSV
			id=S2;   //#3a sets the id state to S2
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else						// didn't find 4th or 1st character
			id = None;			// go back to the NONE id
	}
	
	//This is just for the GPRMC message 
	else if(id == R)  //#2 if the previous state is R 
	{
		if(c == $GPRMC[4]) { //and the next character is M 
			id = M;  //#m set the state to M , for GPRMC
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else						// didn't find 5th or 1st character
			id = None;			// go back to the NONE id
	}
	//for the GPRMC 
	else if(id == M)  //#m if the id state is M 
	{
		if(c == $GPRMC[5]) {  //if the next character found is C 
			name=RMC;           //FOUND GPRMC sets the name to the enum type RMC 
			id=None;					//sets the id back to none to reset state machine 
			return true;			//return true 
		}
		else if(c == $GPGGA[0]) {		//else if found the $ again 
			id = $;								// go back to state of $
		}
		else						// didn't find 6th or 1st character
			id = None;			// go back to the NONE id
		
	}
//this is for the GPGGA scenario 
	else if(id == G3)  //#1a if previous state is third G, G3 in GPGGA 
	{ 
		if(c == $GPGGA[5]) {		//and the next chaacter is equal to A, 
			name = GGA;	//FOUND GPGGA sets the name equal to enumerated type GGA
			id=None;			//resets state machine back to NONE 
			return true;	//return true 
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else									// didn't find 6th or 1st character
			id = None;						// go back to the NONE id
			
	}	
//this is for the GPGSA  and GPGSV scenario 
	else if(id == S1)  //#2a //if the previous state is S1 for the GPGSA 
	{ 
		if(c == $GPGSA[5]) {	 //found A , for A2 state in GPGSA 
			name= GSA;		//FOUND GPGSA sets the name to the enumerated type GSA 
			id=None;				//resets state machine back to NONE 
			return true;		//return true 
		}
		else if(c== $GPGSV[5])	//if the next character is a V for GPGSV 
		{
			name= GSV;		//FOUND GPGSV , sets name to enumerated type GSV 
			id=None;				//resets state machine back to NONE 
			return true;		// returns true 
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else							// didn't find 6th or 1st character
			id = None;				// go back to the NONE id
		
	}	
//this is safe for GPGSV state 
	else if(id == S2)  //#2a //if is S2 state 
	{ 
		if(c == $GPGSV[5]) {	 //if the next character is V 
			name=GSV;		//FOUND GPGSV, sets the name to the enumerated type GSV
			id=None;				//resets state machine id to NONE
			return true;		//returns true . 
		}
		else if(c == $GPGGA[0]) {	//else if found the $ again 
			id = $;								// go back to state of $
		}
		else						// didn't find 6th or 1st character
			id = None;			// go back to the NONE id
	}	

return false;       // WHILE not found the next in sequence return false until one of the above conditions are true. 
}

//returns the message type
string parser::messageType()
{  
	if(name==GGA)
	{
		return GPGGA;
	}
	else if (name==GSA)
	{
		return GPGSA;
	}
	else if (name==RMC)
	{
		return GPRMC;
	}
	else if(name==GSV)
	{
		return GPGSV;
	}
}

//returns the message case for the identifier
string parser::messageCase(string identify)
{  
	if(identify==GPGGA)
	{
		return "GPGGA";
	}
	else if (identify==GPGSA)
	{
		return "GPGSA";
	}
	else if (identify==GPRMC)
	{
		return "GPRMC";
	}
	else if(identify==GPGSV)
	{
		return "GPGSV";
	}
}

void enter_point_GGA(struct Data *point,string message1){

	double UTC_time; //UTC TIME
	int lat_first; //Latitude days part
	double lat_last; //second part after the day in latitude
	 int lon_first;  //Longitude  days part
	double lon_last;//second part of after the day longitude
	//char ns_GA; //North South Fix Indicator (N,S) //named ns //already took care of negative 
	//char ew_GA;//East or West Idicator //named ew , aready took care of negative 
	int position_fix; //se the postion fix 
									/*Value Description
									0 =Fix not available or invalid
									1 =GPS SPS Mode, fix valid
									2 =Differential GPS, SPS Mode , fix valid
									3 =Not supported
									6 =Dead Reckoning Mode, fix valid*/
	
	int satelites_A;  //number of satelites in the fix 
	double HDOP; //horizontal Dilution of precision 
	double MSL_Altitude_A; //altitude in meters 
	//char units; //the unit for the altitude in meters //named unit1 and units two 
	double geoidSeperation; //goid-to-ellipsoid seperation Ellipsoid altitude=MLS altitude +Geoid Seperation 
	int diff_ref_station; // usually 0000
    
//variable to parse message 	
string toParse;
int size;
toParse=message1;

int count=0;
size=toParse.length();
//string variables of all elements in the sentence 
string UTC,lat,ns,lon,ew,posFix,sat_used,hdop,msl,unit1,geoid,unit2,age,stat_id;

/*this parses the sentence takes all elements after the 
comas and puts them in to their corresponding variables at this point
they are all strings but soon to be converted into their respected parts*/
for(int i=0; i<size;i++)
{ 
	if(toParse[i]==',')
	{	
		count++;
	}
			switch(count)
			{
				case 1 :  UTC+=toParse[i]; 
				break;
				case 2:  lat+=toParse[i]; 
				break;
				case 3: ns+=toParse[i]; 
				break;
				case 4: lon+=toParse[i]; 
				break;
				case 5: ew+=toParse[i]; 
				break;
				case 6: posFix+=toParse[i]; 
				break;
				case 7: sat_used+=toParse[i]; 
				break;
				case 8: hdop+=toParse[i]; 
				break;
				case 9: msl+=toParse[i]; 
				break;
				case 10: unit1+=toParse[i]; 
				break;
				case 11: geoid+=toParse[i]; 
				break;
				case 12: unit2+=toParse[i]; 
				break;
				case 13: age+=toParse[i]; 
				break;
				case 14: stat_id+=toParse[i]; 
				break;
			}	
}
//UCT time conversion from string to double 
    UTC.erase(UTC.begin());
	const char* a=UTC.c_str();
	UTC_time=atof(a);

//latitude split into its parts and converted integer and double 
	lat.erase(lat.begin());
	string latfirst;
	latfirst+=lat[0];
	latfirst+=lat[1];
	int size2=0;
	string latlast;
	size2= lat.size();
	for(int i=2;i<size2;i++)
	{
		latlast+=lat[i];
	}
	const char *b =latfirst.c_str();
	lat_first=atoi(b);
	const char *c=latlast.c_str();
	lat_last=atof(c);
	
	
//north or south variable for N is + , S is -
	ns.erase(ns.begin());  
	if(ns=="S") //changes the lat_first if is south to negative
	{
		lat_first*=-1;
	}

//longitude split into its parts and converted 
	lon.erase(lon.begin());
	string lonfirst;
	lonfirst+=lon[0];
	lonfirst+=lon[1];
	lonfirst+=lon[2];
	int size3=0;
	string lonlast;
	size3= lon.size();
	
	for(int i=3;i<size3;i++)
	{
		lonlast+=lon[i];
	}
	const char * d =lonfirst.c_str();
	lon_first=atoi(d);
	const char * e=lonlast.c_str();
	lon_last=atof(e);
	
//this is the string fro east or west
	ew.erase(ew.begin());
	if(ew=="W")
	{
		lon_first*=-1;
	}

//the postion fix indicator 
	posFix.erase(posFix.begin());
	const char *f=posFix.c_str();
	position_fix=atoi(f);
	
//sat_being used range between 0-12 is an integer 
	sat_used.erase(sat_used.begin());
	const char *g=sat_used.c_str();
	satelites_A=atoi(g);
	
//this is the horizontal position is a double 	
	hdop.erase(hdop.begin());
	const char *h=hdop.c_str();
	HDOP=atof(h);
	
//This is for the MSL Altitude	
	msl.erase(msl.begin());
	const char *i=msl.c_str();
	MSL_Altitude_A=atof(i);

//this unit is for the MSL_Altitoude goes right after
	unit1.erase(unit1.begin()); //good as it is
	
//geoid to ellipsoid seperation 
//ellipsoid altitude=MSLALtitude+Goid Separation
	geoid.erase(geoid.begin());
	const char *j=geoid.c_str();
	 geoidSeperation=atof(j);

//this unit is for the geoid seperation	 
unit2.erase(unit2.begin()); //good as is 

//ik what this , is blank but its suppose to be seconds something 
	age.erase(age.begin());
	
//station id ill keep it as a string for now is good 
stat_id.erase(stat_id.begin());
	
	point->UTC=UTC; //check if is empty string 
	point->UTC_time=UTC_time; //pointer to UTC Time 
	point->latitude=lat; //this is just for checking if there is a latidute if not it will be zero 
	point->lat_first_A=lat_first; // pointer to the first part of latitude winch is the day 
	point->lat_last_A=lat_last; //pointer to the second part of latitude 
	
	point->longitude=lon; //checking if there is a longitude if not then it will be zero
	point->lon_first_A=lon_first; //pointer to first part of longitude day 
    point->lon_last_A=lon_last; //pointer to second part of longitude 
	point->position_fix=position_fix;
	point->satelites_A=satelites_A;
	point->HDOP=HDOP;
	
	point->MSL=msl; // checkinf if there is an msl , it will be zero otherwise. 
    point->MSL_Altitude_A=MSL_Altitude_A;
	point->unit1=unit1;
    point->unit2=unit2; 
	point->geoidSeperation=geoidSeperation;
	point->age=age;
    point->diff_ref_station=stat_id;
}

//Parses GPGSA Sentence 
void enter_point_GSA(struct Data *point, string message1)
{   
	int mode1_SA; /* Value Description
							M Manual-forced to operate in 2D or 3D mode
							A 2Dautomatic-allowed to automatically switch 2D/3D*/
							
	int mode2_SA;    /*Value Description
							1 Fix Not Available
							2 2D (<4 SVs used)
							3 3D (>3 SVs used)*/
	//the number of satellites used can be a Max of 12
	//string sat1_s,sat2_s,sat3_s,sat4_s,sat5_s,sat6_s,sat7_s,sat8_s,sat9_s,sat10_s,sat11_s,sat12_s;  

	double PDOP; //Position dillution of Precision 
	double HDOP; // Horizontal Dilution of Precision
	double VDOP;	//Vertical Dilution of Precison 
	
    
//variable to parse message 	
string toParse;
int size;
toParse=message1;

int count=0;
size=toParse.length();

//string variables of all elements in the sentence 
string mode1,mode2,sat1,sat2,sat3,sat4,sat5,sat6,sat7,sat8,sat9,sat10,sat11,sat12,pdop,hdop,vdop;

/*this parses the sentence takes all elements after the 
comas and puts them in to their corresponding variables at this point
they are all strings but soon to be converted into their respected parts*/
for(int i=0; i<size;i++)
{ 
	if(toParse[i]==',')
	{	
		count++;
	}
			switch(count)
			{
				case 1 :  mode1+=toParse[i]; 
				break;
				case 2:  mode2+=toParse[i]; 
				break;
				case 3: sat1+=toParse[i]; 
				break;
				case 4: sat2+=toParse[i]; 
				break;
				case 5: sat3+=toParse[i]; 
				break;
				case 6: sat4+=toParse[i]; 
				break;
				case 7: sat5+=toParse[i]; 
				break;
				case 8: sat6+=toParse[i]; 
				break;
				case 9: sat7+=toParse[i]; 
				break;
				case 10: sat8+=toParse[i]; 
				break;
				case 11: sat9+=toParse[i]; 
				break;
				case 12: sat10+=toParse[i]; 
				break;
				case 13: sat11+=toParse[i]; 
				break;
				case 14: sat12+=toParse[i]; 
				break;
				case 15:pdop+=toParse[i]; 
				break;
				case 16:hdop+=toParse[i];
				break;
				case 17:vdop+=toParse[i];
				break;
			}	
}
//Mode1 extracted 
    mode1.erase(mode1.begin());

//Mode two extracted 
	mode2.erase(mode2.begin());
	const char *z=mode2.c_str();
	mode2_SA=atoi(z);
	

//sat1 extracted 	
sat1.erase(sat1.begin());
sat2.erase(sat2.begin());
sat3.erase(sat3.begin());
sat4.erase(sat4.begin());
sat5.erase(sat5.begin());
sat6.erase(sat6.begin());
sat7.erase(sat7.begin());
sat8.erase(sat8.begin());
sat9.erase(sat9.begin());
sat10.erase(sat10.begin());
sat11.erase(sat11.begin());
sat12.erase(sat12.begin());

//postion dilution of precision
	pdop.erase(pdop.begin());
	const char *a=pdop.c_str();
	PDOP=atof(a);
//this is the horizontal position is a double 	
	hdop.erase(hdop.begin());
	const char *b=hdop.c_str();
	HDOP=atof(b);
//this is the vertical dilution of precison is a double
	vdop.erase(vdop.begin());	
	const char *c=vdop.c_str();
	VDOP=atof(c);


	
	//pointers to the struct variables holding the data 
	point->mode1_SA=mode1;
	point->mode2_SA=mode2_SA;
	point->satelites1_SA=sat1;
	point->satelites2_SA=sat2;
	point->satelites3_SA=sat3;
	point->satelites4_SA=sat4;
	point->satelites5_SA=sat5;
	point->satelites6_SA=sat6;
	point->satelites7_SA=sat7;
	point->satelites9_SA=sat8;
	point->satelites10_SA=sat9;
	point->satelites11_SA=sat10;
	point->satelites12_SA=sat11;
	point->PDOP=PDOP;
	point->HDOP=HDOP;
	point->VDOP=VDOP;
}

//Parses GPRMC Sentence 
void enter_point_RMC(struct Data *point, string message1)
{	double UTC_time=0; //UTC TIME
	string status; // A=Data valid and V=Data not valid
	int lat_first=0; //Latitude days part
	double lat_last=0; //second part after the day in latitude
	//need NS
	
	 int lon_first=0;  //Longitude  days part
	double lon_last=0;//second part of after the day longitude
	//NEED EW
	
	
	double speed_R=0; //this is in knots
	double course_R=0; //degrees This is true 
	
	string date_day; //ddmmyy
	string date_month;
	string date_year;
	int year=0; //actual day in 2014 et.. added 2000
	
	//string eastWest; //east west indicator 
	string  magnetidVariation; 
	char mode_R; /*A=Autonomous, D=DGPS,
										E=DR
										N=Output Data Not Valid
										R= Coarse Position3
										S=Simulator */	
    
//variable to parse message 	
string toParse;
int size;
toParse=message1;

int count=0;
size=toParse.length();
//string variables of all elements in the sentence 
string UTC,lat,ns,lon,ew,speed,course,date,eastWest,magnet,mode;

/*this parses the sentence takes all elements after the 
comas and puts them in to their corresponding variables at this point
they are all strings but soon to be converted into their respected parts*/
for(int i=0; i<size;i++)
{ 
	if(toParse[i]==',')
	{	
		count++;
	}
			switch(count)
			{
				case 1 :  UTC+=toParse[i]; 
				break;
				case 2: status+=toParse[i];
				break;
				case 3:  lat+=toParse[i]; 
				break;
				case 4: ns+=toParse[i]; 
				break;
				case 5: lon+=toParse[i]; 
				break;
				case 6: ew+=toParse[i]; 
				break;
				case 7: speed+=toParse[i]; 
				break;
				case 8: course+=toParse[i]; 
				break;
				case 9: date+=toParse[i]; 
				break;
				case 10: eastWest+=toParse[i]; 
				break;
				case 11: magnet+=toParse[i]; 
				break;
				case 12: mode+=toParse[i]; 
				break;
			}	
}
//UCT time conversion from string to double 
    UTC.erase(UTC.begin());
	const char* a=UTC.c_str();
	UTC_time=atof(a);

//latitude split into its parts and converted integer and double 
	lat.erase(lat.begin());
	string latfirst;
	latfirst+=lat[0];
	latfirst+=lat[1];
	int size2=0;
	string latlast;
	size2= lat.size();
	for(int i=2;i<size2;i++)
	{
		latlast+=lat[i];
	}
	const char *b =latfirst.c_str();
	lat_first=atoi(b);
	const char *c=latlast.c_str();
	lat_last=atof(c);
	
	
//north or south variable for N is + , S is -
	ns.erase(ns.begin());  
	if(ns=="S") //changes the lat_first if is south to negative
	{
		lat_first*=-1;
	}

//longitude split into its parts and converted 
	lon.erase(lon.begin());
	string lonfirst;
	lonfirst+=lon[0];
	lonfirst+=lon[1];
	lonfirst+=lon[2];
	int size3=0;
	string lonlast;
	size3= lon.size();
	
	for(int i=3;i<size3;i++)
	{
		lonlast+=lon[i];
	}
	const char * d =lonfirst.c_str();
	lon_first=atoi(d);
	const char * e=lonlast.c_str();
	lon_last=atof(e);
	
//this is the string fro east or west
	ew.erase(ew.begin());
	if(ew=="W")
	{
		lon_first*=-1;
	}

//the speed in knots double 
	speed.erase(speed.begin());
	const char * f=speed.c_str();
	speed_R=atof(f);
	 
//the course in degrees double 
	course.erase(course.begin());
	const char * g=course.c_str();
	course_R=atof(g);

//extract the date : dd mm yy 
	date.erase(date.begin());
	date_day+=date[0];
	date_day+=date[1];
	date_month+=date[2];
	date_month+=date[3];
	date_year+=date[4];
	date_year+=date[5];
	const char *h=date_year.c_str();
	year=atoi(h);
	year+=2000;
	
//east west indicator 
	eastWest.erase(eastWest.begin());
	
//magnetic variation 
	magnet.erase(magnet.begin());

//mode 
    mode.erase(mode.begin());
	
	//pointers to thes struct variables holding the data 
	point->UTC_time=UTC_time; //pointer to UTC Time 
	point->lat_first_A=lat_first; // pointer to the first part of latitude winch is the day 
	point->lat_last_A=lat_last; //pointer to the second part of latitude 
	point->lon_first_A=lon_first; //pointer to first part of longitude day 
    point->lon_last_A=lon_last; //pointer to second part of longitude 
	point->speed_R=speed_R;
	point->speed_C=speed;  //for check if is there or not 
	point->course_C=course; //for check if is there or not
	point->course_R=course_R;
	
	point->DATE=date;
	point->date_day=date_day;
	point->date_month=date_month;
	point->year=year;
	
								
								
}

//Parses GPGSV Sentence 
void enter_point_GSV(struct Data *point, string message1)
{
    
	int num_messages_V; //range of 1-3
	int message_number_V; //range for 1-3
	int satelite_in_view_V; 
	int satelite_id_V; //channel 1 (range 1-32)
	int elevation_V; //IN DEGREES maximum 90 degrees
	int azimuth_1_V; //DEGREES (true , range of 0-359)
	int SNR_cNo_V; //Range 0 -99, null when not tracking (dBHz)
	int satelite2_id_4_V; //this is the satellite id is range 1-32, channel 4
	int elevation_4_V; //channel 4, MAX 90 Degrees 
	int azimuth_4_V; //DEGREES 0-359
	int SNR4_V;
	
	
	
//variable to parse message 	
string toParse;
int size;
toParse=message1;

int count=0;
size=toParse.length();

//string variables for all the elements of the sentence 
string num_mess,mess_num,sat_in,sat_id,elevation,azimuth,SNR,sat2,elevation4,azimuth4,SNR4;

/*this parses the sentence takes all elements after the 
comas and puts them in to their corresponding variables at this point
they are all strings but soon to be converted into their respected parts*/
for(int i=0; i<size;i++)
{ 
	if(toParse[i]==',')
	{	
		count++;
	}
			switch(count)
			{
				case 1 :  num_mess+=toParse[i]; 
				break;
				case 2:  mess_num+=toParse[i]; 
				break;
				case 3: sat_in+=toParse[i]; 
				break;
				case 4: sat_id+=toParse[i]; 
				break;
				case 5: elevation+=toParse[i]; 
				break;
				case 6: azimuth+=toParse[i]; 
				break;
				case 7: SNR+=toParse[i]; 
				break;
				case 8: sat2+=toParse[i]; 
				break;
				case 9: elevation4+=toParse[i]; 
				break;
				case 10: azimuth4+=toParse[i]; 
				break;
				case 11: SNR4+=toParse[i]; 
				break;
			}	
}

//number of messages extracted 
    num_mess.erase(num_mess.begin());
	const char *a=num_mess.c_str();
	num_messages_V=atoi(a);

//Message Number extracted 
	mess_num.erase(mess_num.begin());
	const char *b=mess_num.c_str();
	message_number_V=atoi(b);
		
//satelites in extracted 
	sat_in.erase(sat_in.begin());
	const char *c=sat_in.c_str();
	satelite_in_view_V=atoi(c);

//satelites id extracted 
	sat_id.erase(sat_id.begin());
	const char *d=sat_id.c_str();
	satelite_id_V=atoi(d);

//elevation extracted 
	elevation.erase(elevation.begin());
	const char *e=elevation.c_str();
	elevation_V=atoi(e);

//azimuth extracted 
	azimuth.erase(azimuth.begin());
	const char *f=azimuth.c_str();
	azimuth=atoi(f);

//SNR Extracted 
	SNR.erase(SNR.begin());
	const char *g=SNR.c_str();
	SNR_cNo_V=atoi(g);

//satelite ID exgracted 
	sat2.erase(sat2.begin());
	const char *h=sat2.c_str();
	satelite2_id_4_V=atoi(h);

//elevation channel 4 extracted 
	elevation4.erase(elevation4.begin());
	const char *i=elevation4.c_str();
	elevation_4_V=atoi(i);

//azimuth channel 4 
	azimuth4.erase(azimuth4.begin());
	const char *j=azimuth4.c_str();
	azimuth_4_V=atoi(j);

//SNR channel 4
	SNR4.erase(SNR4.begin());
	const char *k=SNR4.c_str();
	SNR4_V=atoi(k);
	
	point->num_messages_V=num_messages_V;
	point->message_number_V=message_number_V;
	
	point->satelite_in_view_V=satelite_in_view_V;
	point->satelite_id_V=satelite_id_V;
	point->elevation_V=elevation_V;
	point->azimuth_4_V=azimuth_4_V;
	point->SNR_cNo_V=SNR_cNo_V;
	point->satelite2_id_4_V=satelite2_id_4_V;
	point->elevation_4_V=elevation_4_V;
	point->azimuth_4_V=azimuth_4_V;
	point->SNR4_V=SNR4_V;

}
//prints the final message to the screen what is displayed after each message is parsed
void printFinal(struct Data point)
{
     bool present=false;
     if ( point.course_C.empty()==true)
	 {
		present=false; 
	 }
	 else
	 {
	 present=true;
	 }
	 
	 string fix;
	 switch (point.mode2_SA){
	 case (1): fix="Fix Not Available";
	 break;
	 case(2):fix="2D Fix";
	 break;
	 case(3):fix="3D Fix";
	 break;
	 default: fix="Fix Not Available";
	 break;
	 }

if(point.UTC.empty()==true)
{
	point.UTC_time=0;
}	 
if(point.DATE.empty()==true)
{
	point.date_day="00";
	point.date_month="00";
	point.year=0;
}
if(point.latitude.empty() ==true )
{
	point.lat_first_A=0;
	point.lat_last_A=0;
}
if(point.longitude.empty() ==true )
{
	point.lon_first_A=0;
	point.lon_last_A=0;
}
if(point.MSL.empty() ==true)
{
	point.MSL_Altitude_A=0;
}
if(point.speed_C.empty() ==true)
{
	point.speed_R=0.0;
}
if(point.course_C.empty() ==true)
{
	point.course_R=0.0;
}

cout<<"\nUTC time: "<<fixed<<setprecision(3)<<point.UTC_time<<endl;
cout<<"Date: "<<point.date_day<<"/"<<point.date_month<<"/"<<point.year<<endl;

cout<<"Latitude: "<<point.lat_first_A<<"d "<< point.lat_last_A<<"'"<<endl;
cout<<"Longitude: "<<point.lon_first_A<<"d "<<point.lon_last_A<<"'"<<endl;
cout<<"Altitude: "<<setprecision(2)<<point.MSL_Altitude_A<<point.unit1<<endl;
cout<<"Speed: "<<setprecision(2)<<point.speed_R<<" knots,"<<" Course: "<<point.course_R<<" degrees,"<<boolalpha<< present<<"\n"<<endl;

cout<<fix<<endl;
cout<<"Satellite count: "<<point.satelites_A<<endl;
cout<<setprecision(1);
cout<<"PDOP: "<<point.PDOP<<endl;
cout<<"HDOP: "<<point.HDOP<<endl;
cout<<"VDOP: "<<point.VDOP<<endl;

}
