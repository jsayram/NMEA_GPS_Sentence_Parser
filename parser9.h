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
// Description: parser class - finds a 6-character sequence in a "stream" 
//              			of characters, holds function prototypes used in main
//
// Input: none
//
// Output: none
//
//******************************************************************

using namespace std;

enum nameType{X,GGA,GSA,RMC,GSV};      //enumerated type for the message type return 
enum find_id{None,$,G1,P,G2,R,M,G3,S1,S2,V,C,A1,A2};  //enumerated type for state machine for each MessageType character. 


#define GPGGA	"$GPGGA" //None,G1,P,G2,G3,A1
#define GPGSA	"$GPGSA" //None,G1,P,G2,S1,A2
#define GPRMC	"$GPRMC" //None,G1,P,R,M,C
#define GPGSV	"$GPGSV" //None,G1,P,G2,S2,V

#define A	"GPGGA" 
#define B	"GPGSA" 
#define C	"GPRMC" 
#define D	"GPGSV" 

struct Data {
//VARIABLES OF GPGGA
	double UTC_time; //UTC TIME
	string UTC; //to check if is empty or not 
	string latitude;
	int lat_first_A; //Latitude first part 
	double lat_last_A;//Latitude Second Part
	
	string longitude;
	int lon_first_A;  //Longitude first part
	double lon_last_A; //longitude second part 
	
	int position_fix; //se the postion fix 
									/*Value Description
									0 =Fix not available or invalid
									1 =GPS SPS Mode, fix valid
									2 =Differential GPS, SPS Mode , fix valid
									3 =Not supported
									6 =Dead Reckoning Mode, fix valid*/
	
	int satelites_A;  //number of satelites in the fix 
	double HDOP; //horizontal Dilution of precision 
	string MSL; //to check if is empty or not. 
	double MSL_Altitude_A; //altitude in meters 
	string unit1; //the unit for the altitude in meters
	string unit2;

	double geoidSeperation; //goid-to-ellipsoid separation Ellipsoid altitude=MLS altitude +Geoid Seperation 
	string age;
	string diff_ref_station; // usually 0000

//VARIBLES FOR GPGSA 
	string mode1_SA; /* Value Description
							M Manual-forced to operate in 2D or 3D mode
							A 2Dautomatic-allowed to automatically switch 2D/3D*/
							
	int mode2_SA;    /*Value Description
							1 Fix Not Available
							2 2D (<4 SVs used)
							3 3D (>3 SVs used)*/
	string satelites1_SA;  
	string satelites2_SA;  
	string satelites3_SA;  
	string satelites4_SA;  
	string satelites5_SA;  
	string satelites6_SA;  
	string satelites7_SA;  
	string satelites8_SA;  
	string satelites9_SA;  
	string satelites10_SA;  
	string satelites11_SA;  
	string satelites12_SA;  
	
	
	double PDOP; //Position dillution of Precision 
	//double HDOP_SA; // Horizontal Dilution of Precision   //aready defined in GGA
	double VDOP;	//Vertical Dilution of Precison 

//VARIABLES FOR GPGSV 
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
	int SNR4_V; //Range 0 -99, null when not tracking (dBHz)
	
//Variables For GPRMC
	//double UTC_R  hhmmss.sss
	char status; // A=Data valid and V=Data not valid
//	double Latitude_R  ddmm.mmm
//double longitude_R ddmm.mmm
//	NS //taken care off 
//	EW // taken care off 
	double speed_R; //this is in knots
	string speed_C; //for check. 
	double course_R; //degrees This is true 
	string course_C; //for check 
	
	string DATE;
	string date_day; //ddmmyy
	string date_month;
	string date_year;
	int year; //added 2000 to the year. 
	
	string eastWest;
	string magneticVariation; 
	string mode_R; /*A=Autonomous, D=DGPS,
								E=DR
								N=Output Data Not Valid
								R= Coarse Position3
								S=Simulator */	
};

class parser
{
	private:
		enum find_id id;		// Holds current state of the string id 
		enum nameType name; //enumerated type name for the state machine once found message Type. 
		
		string $GPGGA;      // string to GPGGA
		string $GPGSA;		// string to GPGSA
		string $GPRMC;		// string to GPRMC
		string $GPGSV;		// string to GPGSV
		
	public:	
		parser();                      // Constructor
		~parser();					// Destructor
		bool parser_id(char c);			// Main implementation for the state machine for id
		string messageType();   //identifies what message type it is
		string messageCase(string); //finds the message case and returns it clean with no $ sign. 
};

//parses GPGGA Sentence 
void enter_point_GGA(struct Data *point, string id);

//Parses GPGSA Sentence 
void enter_point_GSA(struct Data *point, string id);

//Parses GPRMC Sentence 
void enter_point_RMC(struct Data *point, string id);

//Parses GPGSV Sentence 
void enter_point_GSV(struct Data *point, string id);

//what will be shown to the console
void printFinal(struct Data point);







