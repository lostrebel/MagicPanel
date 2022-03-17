// Magic Panel FX by IA-PARTS.com 
//
//// Release History
// v011 - Cleanup of code, additional documentation added (TheJugg1er 02-18-2020)
// v010.6 - Added Serial interface support and additional sequences (TheJugg1er 01-05-2020)
// v010.5 - Re-added Working I2C and additional display sequences  (FlthyMcNsty 05-21-2014)
// v010 - Remove I2C code and clean up
// v009 - Combine Big Happy Dude functions to v008 + allow for 3 pin binary input
// v008 - Final Code Release May 24 2013 - D Dobyns
// v007 - New Functions for Program Pin Control - D Dobyns
// v006 - Jumper Pin Program - D Dobyns
// v005 - Cleaned code - Production
// v004 - Added JEDI support for JEDI Serial Address 10 decimal 
// v003 - Decode events... next add selectable jumper support.
// v002 - Added default operation & I2C support
// v001 - Initial Demo Sketch
//

/*
 * The Full description of all command sequences are listed below.  These are called using either i2c or serial and supplying
 * the mode number as listed.
 * 
 * The Code has two modes of operating.  It is possible to set this during runtime of the panel, however the default is controlled
 * in the code below.  To set the Defaul mode, change the variable "alwaysOn" to be either true or false.  The default here is false.
 * 
 * If alwaysOn is set to False, the patterns will display for a given time, and then turn off.
 * If alwaysOn is set to True, the pattern will display until the T00 (off) command is received. (or loop until T00 is received).
 * 
 * To change the Default, Command P0 will set to "false", Command P1 will set to "true", overriding the default for this session.
 * 
 * The Magic Panel uses JawaLite commands to trigger the patterns.  Currently only A, D, P, and T commands from the JawaLite 
 * spec are supported.
 * 
 * Command A will turn the panel on
 * 
 * Command D will turn the panel off
 * 
 * Command P will set the default panel mode as described above.
 * 
 * Command T will trigger a numbered pattern.  Txx where xx is the pattern number below. 
 * 
 * When sending i2c command the Panel Address is defined below to be 20.  The command type and value are needed.  To trigger a
 * pattern, send character 'T' and the value corresponding to the pattern list below to trigger the corresponding sequence.  
 * Sequences must be terminated with a carriage return (\r)
 * 
 * On the MarcDuino i2c can be connected from either the Master or Slave
 * 
 * e.g. &20,x54,2\r    -  This will turn on the MP for ~2 seconds.
 * 
 * When sending commands via serial, no address is required as Serial is a point to point communication protocol. 
 * Sequences must be terminated with a carriage return (\r)
 * 
 * To use the MarcDuino Serial triggering, connect Pin 3 of the Magic Panel (Rx) to the "To Slave" Signal pin on the 
 * MarcDuino Slave.
 * 
 * e.g. %T2      - This will turn on the MP for ~2 seconds.
 * 
 * Mode 0 - Turn Panel off
 * Mode 1  - Turn Panel on Indefinately
 * Mode 2  - Turn Panel on for 2s
 * Mode 3  - Turn Panel on for 5s
 * Mode 4  - Turn Panel on for 10s
 * Mode 5  - Begins Toggle Sequence: Top and Bottom Half of Panel Alternate
 * Mode 6  - Begins Alert Sequence (4s): Panel Rapidly Flashes On & Off
 * Mode 7  - Begins Alert Sequence (10s): Panel Rapidly Flashes On & Off
 * Mode 8  - Begins Trace Up Sequence (Type 1): Each Row of the MP lights up from bottom to top filling entire panel
 * Mode 9  - Begins Trace Up Sequence (Type 2):  Each row of the MP lights up from bottom to top individually
 * Mode 10 - Begins Trace Down Sequence (Type 1):  Each row of the MP lights up from top to bottom filling entire panel
 * Mode 11 - Begins Trace Down Sequence (Type 2):  Each row of the MP lights up from top to bottom individually
 * Mode 12 - Begins Trace Right Sequence (Type 1):  Each column of the MP lights up from left to right filling entire panel
 * Mode 13 - Begins Trace Right Sequence (Type 2):  Each column of the MP lights up from left to right individually
 * Mode 14 - Begins Trace Left Sequence (Type 1):  Each column of the MP lights up from right to left filling entire panel
 * Mode 15 - Begins Trace Left Sequence (Type 2):  Each column of the MP lights up from right to left individually
 * Mode 16 - Begins Expand Sequence (Type 1): Panel expands from center filling entire panel
 * Mode 17 - Begins Expand Sequence (Type 2): Ring of pixels expands from center of panel
 * Mode 18 - Begins Compress Sequence (Type 1): Panel compresses from outer edge filling entire panel
 * Mode 19 - Begins Compress Sequence (Type 2): Ring of pixels compresses from outer edge of panel
 * Mode 20 - Begins Cross Sequence: Panel is lit to display an X for 3s
 * Mode 21 - Begins Cyclon Column Sequence: Each column illuminated one at a time from left to right back to left. (like the Cylons from Battlestar Galactica)
 * Mode 22 - Begins Cyclon Row Sequence: Each row illuminated one at a time from top to bottom back to top. (like the Cylons from Battlestar Galactica)
 * Mode 23 - Begins Eye Scan Sequence:  Each row is illuminated from top to bottom followed by each column left to right. (like the eye ball scanners in the Mission Impossible movies)
 * Mode 24 - Begins Fade Out/In Sequence:  MP gradually and randomly fades out and then fades back in the same manner. 
 * Mode 25 - Begins Fade Out Sequence:  MP gradually and randomly fades out (Similar to the Short Circuit Sequence on Teeces).
 * Mode 26 - Begins Flash Sequence:  MP flashes rapidly for 5 seconds (Similar to Alarm Sequence)
 * Mode 27 - Begins Flash V Sequence: Left and Right Half of Panel Alternate
 * Mode 28 - Begins Flash Q Sequence:  Alternating quadrants of MP flash rapidly
 * Mode 29 - Begins Two Loop Sequence: Dual pixels are lit opposite each other completing a loop around the 2nd ring from panel edge.
 * Mode 30 - Begins One Loop Sequence: A single pixel is lit individually completing a loop around the 2nd ring from panel edge.
 * Mode 31 - Begins Test Sequence (Type 1):  Each pixel of the MP is lit sequentially from row 0, column 7 to row 7, column 0 until panel is filled, then unlit in the same order.
 * Mode 32 - Begins Test Sequence (Type 2):  Each pixel of the MP is lit indivually from row 0, column 7 to row 7, column 0. 
 * Mode 33 - Begins AI Logo Sequence:  Displays the AI Aurebesh characters for 3s (...that we see all over our awesome packages from Rotopod and McWhlr)
 * Mode 34 - Begins 2GWD Logo Sequence: Displays the Characters 2-G-W-D sequentially every 1s (...shameless, I know.) 
 * Mode 35 - Begins Quadrant Sequence (Type 1):  Each Panel Quadrant lights up individually (TL, TR, BR, BL)
 * Mode 36 - Begins Quadrant Sequence (Type 2):  Each Panel Quadrant lights up individually (TR, TL, BL, BR) 
 * Mode 37 - Begins Quadrant Sequence (Type 3):  Each Panel Quadrant lights up individually (TR, BR, BL, TL)
 * Mode 38 - Begins Quadrant Sequence (Type 4):  Each Panel Quadrant lights up individually (TL, BL, BR, TR)
 * Mode 39 - Begins Random Pixel Sequence:  Random pixels flashe individually for 6s
 * Mode 40 - Begins countdown from 9 to 0 (takes 10 seconds)
 * Mode 41 - Begins countdown from 3 to 0 (takes 4 seconds)
 * Mode 42 - Begins Alert Sequence (4s):  Panel Randomly Flashes On & Off, mimics the original MarcDuino code
 * Mode 43 - Begins Alert Sequence (8s):  Panel Randomly Flashes On & Off, mimics the original MarcDuino code
 * Mode 44 - Begins Smiley Face
 * Mode 45 - Begins Sad Face
 * Mode 46 - Begins Heart
 * Mode 47 - Begins Flash Checkerboard sequence.  Flashes small squares in a checkerboard pattern
 * Mode 48 - Begins Compress In Type 1 - Fills the rows from TL/BR in half row increments 5 times
 * Mode 49 - Begins Compress In Type 2 - Fills the rows from TL/BR in half row increments then clears in the same order 5 times
 * Mode 50 - Begins Explode Out Type 1 - Fills the rows from Centre Out in half row increments 5 times
 * Mode 51 - Begins Explode Out Type 2 - Fills the rows from Centre Out in half row increments then clears in the same order 5 times
 * Mode 52 - Begins VU Meter - Randomly ramp up/down on the columns to simulate a VU Meter. (Type 1)
 * Mode 53 - Begins VU Meter - Randomly ramp up/down on the rows to simulate a VU Meter. (Type 2)
 * Mode 54 - Begins VU Meter - Randomly ramp up/down on the columns from the top to simulate a VU Meter. (Type 3)
 * Mode 55 - Begins VU Meter - Randomly ramp up/down on the rows from the right to simulate a VU Meter. (Type 4)
 */

 
//We always have to include the library
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
   #include "WProgram.h"
#endif
#include "LedControl.h"
#include "Wire.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Select your MagicPanel hardware here  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MP_IAPARTS_V1
//#define MP_IAPARTS_V2
//#define MP_IAPARTS_V3
#define MP_RODDBOTICS_V2

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Assign IC2 Address Below   //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   byte I2CAdress = 20;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Patterns always on define  //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   bool alwaysOn = false;  // false is default behaviour.  Pattern will turn off after running (or given time) 
                           // Set to true to have patterns remain on until Mode 0 is selected.
                           // Note this can cause some slight delays in response from the panel.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Uncomment this if you want debug messages
//#define DEBUG

unsigned long time       	= 0;
unsigned long last_time  	= 0;
byte Speed 			= 1;

// State Variables

int AlertTime       = 0;
int TraceUpTime     = 0;
int TraceDownTime   = 0;
int TraceRightTime  = 0;
int TraceLeftTime   = 0;
int QuadrantTime    = 0;
int CompressTime    = 0;
int ExpandTime      = 0;
int ToggleTime      = 0;
int RandomPixelTime = 0;

byte TopRow = 0;
byte BotRow = 0;

byte RandomState = 0;
byte RandomMode;     // selected mode

unsigned long RandomTime = 0;
             
//unsigned long RandomInterval = 0;     // time between function calls
unsigned long RandomOnTime = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// To load a sketch onto Magic Panel as Arduino Duemilanove w/ ATmega328
//
// Now we need a LedControl to work with.
// ***** These pin numbers will probably not work with your hardware *****
//
// MAX7221
// Pin 1 - Data IN
// Pin 12 - Load
// Pin 13 - CLK
// Pin 24 - Data Out
//
// Top 7221    = 0
// Bottom 7221 = 1
//
// Assign the pins from the ATMega328p to LedControl
// 
// pin D8 is connected to the DataIn 
// pin D7 is connected to the CLK 
// pin D6 is connected to LOAD 
// We have two MAX7221 on the Magic Panel, so we use 2 for device count.
//
// --------------------------------------------------------------
// RoddBotics Magic Panel v2.1 using 1 Max7221 and Arduino Nano
// --------------------------------------------------------------
// pin D12 is connected to the DataIn 
// pin D11 is connected to the CLK 
// pin D10 is connected to LOAD
// 
//

#if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
# define TWO_MAX_CHIPS
# define HAVE_JUMPERS
# define HAVE_DIGITAL_CMD_INPUT

LedControl lc = LedControl(8,7,6,2);

#elif defined(MP_RODDBOTICS_V2)

LedControl lc = LedControl(12,11,10,1);

#else

hardware not defined

#endif

#if defined(HAVE_DIGITAL_CMD_INPUT) || defined(HAVE_JUMPERS)
bool first_time       = true;	// used for 4-bit (0 2 3 5) input reading - reset all variables
  			        // when 4-bit address value changes (except the first time on power up)
byte DigInState       = 0;
byte lastDigInState;
#endif

////////////////////////////////////////////////////////////////////////////////
// Board Layout and MAX chip mapping
//
// The physical and logical layout of the pixels need not match. This sketch
// uses a virtual pixel array (VMagicPanel) for the code to draw in and then maps
// that to byte array bitmap (MagicPanel) used when calling the LedControl
// routines which handle the MAX chip interface.
//
// IA-Parts
// --------
// The IA-Parts boards have two MAX chips, each driving 4 rows
// Each half row of pixels is driven by one row in the MAX chip
//
// So the physical layout is
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R0  R0  R0  R0  R0  R0  R0  R0
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R1  R1  R1  R1  R1  R1  R1  R1
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R2  R2  R2  R2  R2  R2  R2  R2
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R3  R3  R3  R3  R3  R3  R3  R3
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R4  R4  R4  R4  R4  R4  R4  R4
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R5  R5  R5  R5  R5  R5  R5  R5
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R6  R6  R6  R6  R6  R6  R6  R6
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R7  R7  R7  R7  R7  R7  R7  R7
//
// The chip grid layout is:
//
// Chip      Chip 0 Column               Chip      Chip 1 Column
// Row  7   6   5   4   3   2   1   0    Row  7   6   5   4   3   2   1   0
//  0  C7  C6  C5  C4                     0  C7  C6  C5  C4
//     R0  R0  R0  R0                        R4  R4  R4  R4
//  1                  C3  C2  C1  C0     1                  C3  C2  C1  C0
//                     R0  R0  R0  R0                        R4  R4  R4  R4
//  2  C7  C6  C5  C4                     2  C7  C6  C5  C4
//     R1  R1  R1  R1                        R5  R5  R5  R5
//  3                  C3  C2  C1  C0     3                  C3  C2  C1  C0
//                     R1  R1  R1  R1                        R5  R5  R5  R5
//  4  C7  C6  C5  C4                     4  C7  C6  C5  C4
//     R2  R2  R2  R2                        R6  R6  R6  R6
//  5                  C3  C2  C1  C0     5                  C3  C2  C1  C0
//                     R2  R2  R2  R2                        R6  R6  R6  R6
//  6  C7  C6  C5  C4                     6  C7  C6  C5  C4
//     R3  R3  R3  R3                        R7  R7  R7  R7
//  7                  C3  C2  C1  C0     7                  C3  C2  C1  C0
//                     R3  R3  R3  R3                        R7  R7  R7  R7
//
// Note: there is no harm in trying to address chip pixels that have no
//       corresponding physical led. So writing B11111111 to Chip 0, Row 0
//       has the same effect as writing B11110000, turning on Row 0 pixels
//       C4, C5, C6, and C7.
//
// RoddBotics
// ----------
//
// The Roddbotics boards have one MAX chip so conceptually easier to deal with except the
// the row/column layout is rotated. MapBoolGrid takes care of rotating the matrix just before
// output so you don't need to care. 
//
// The chip grid layout is:
//
// Chip      Chip 0 Column               
// Row  7   6   5   4   3   2   1   0    
//  0  C0  C0  C0  C0  C0  C0  C0  C0                        
//     R0  R1  R2  R3  R4  R5  R6  R7                        
//  1  C1  C1  C1  C1  C1  C1  C1  C1                        
//     R0  R1  R2  R3  R4  R5  R1  R7 
//  2  C2  C2  C2  C2  C2  C2  C2  C2
//     R0  R1  R2  R3  R4  R5  R6  R7
//  3  C3  C3  C3  C3  C3  C3  C3  C3
//     R0  R1  R2  R3  R4  R5  R6  R7
//  4  C4  C4  C4  C4  C4  C4  C4  C4
//     R0  R1  R2  R3  R4  R5  R6  R7
//  5  C5  C5  C5  C5  C5  C5  C5  C5
//     R0  R1  R2  R3  R4  R5  R6  R7
//  6  C6  C6  C6  C6  C6  C6  C6  C6
//     R0  R1  R2  R3  R4  R5  R6  R7
//  7  C7  C7  C7  C7  C7  C7  C7  C7
//     R0  R1  R2  R3  R4  R5  R6  R7
//
// 

#ifdef TWO_MAX_CHIPS
byte MagicPanel[16]; // The 1D array representing the state of each half row
#else
byte MagicPanel[8];  // The 1D array representing the state of each whole row
#endif 
boolean VMagicPanel[8][8];    // The 2D array of logical pixels


unsigned long delaytime=30;
unsigned long scrolldelaytime=50;

int NumLoops=2;

//Serial Stuff
int MPeventCode         = 0;
int lastMPeventCode     = 0;
bool firstTimeCode      = true;
// String processing from Serial
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean stringEvaluated = false; // whether the serial "packet" has been processed

//Timing stuff
unsigned long patternEndTime=0;
boolean patternActive = false;

void setup() 
{ 
  Wire.begin(I2CAdress);                   // Start I2C Bus as Master I2C Address
  Wire.onReceive(receiveEvent);            // register event so when we receive something we jump to receiveEvent();

  // Setup the Serial Interface
  Serial.begin(9600);
  inputString.reserve(200);

  /*
   * The MAX72XX is in power-saving mode on startup,
   * We have to:
   *  1. do a wakeup call,
   *  2. set the LED brightness
   *  3. and clear the display 
   */
  lc.shutdown(0, false);
#ifdef TWO_MAX_CHIPS  
  lc.shutdown(1, false);
#endif  

  lc.setIntensity(0, 15);
#ifdef TWO_MAX_CHIPS  
  lc.setIntensity(1, 15);
#endif

  lc.clearDisplay(0);
#ifdef TWO_MAX_CHIPS  
  lc.clearDisplay(1);
#endif

  randomSeed(analogRead(A3));           // Randomizer

  // Test Pixels
  TheTest(30);
  
  // SETUP 6 DIGITAL PINS FOR MANUAL CONTROL
#ifdef HAVE_JUMPERS  
# if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
  // Jumper Pins used by MP_IAPARTS_*
  pinMode(11, INPUT);             // set pin 11 to input - input 3  
  pinMode(12, OUTPUT);            // set pin PB4 to output - pin 4 - used to allow a jumper from pin 4 to adjacent pin to pull down the adjacent pin
  pinMode(13, INPUT);             // set pin 13 to input - input 5

  digitalWrite(11, HIGH);         // turn on pullup resistors
  digitalWrite(13, HIGH);         // turn on pullup resistors
  digitalWrite(12, LOW);          // set pin PC1 to output - pin 1
# endif /* MP_IAPARTS_* */
#endif /* HAVE_JUMPERS */
  
#ifdef HAVE_DIGITAL_CMD_INPUT
# if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
  digitalWrite(A0, HIGH);         // turn on pullup resistors
  digitalWrite(A1, HIGH);         // turn on pullup resistors
  digitalWrite(A2, HIGH);         // turn on pullup resistors
# endif /* MP_IAPARTS_* */
#endif /* HAVE_DIGITAL_CMD_INPUT */
}

void loop() { 

  time = millis();		        // get an updated time stamp
  if (time - last_time > Speed)  	// check if time has passed to change states - used to slow down the main loop
  {
    last_time = time;			// reset the timer

#if defined(HAVE_DIGITAL_CMD_INPUT) || defined(HAVE_JUMPERS)
    DigInState = 0;
# ifdef HAVE_DIGITAL_CMD_INPUT
    //***********************************************
    // These are the Binary Coded Input Pins
    //   
    DigInState = 0;			// read in the 4-bit address line
    if (digitalRead(A0) == LOW)        	// 0
    { DigInState = DigInState + 4; }
    if (digitalRead(A1) == LOW)        	// 1
    { DigInState = DigInState + 2; }
    if (digitalRead(A2) == LOW)       	// 2
    { DigInState = DigInState + 1; }
# endif /* HAVE_DIGITAL_CMD_INPUT */

# ifdef HAVE_JUMPERS
   //***********************************************
   // These are the Jumper Pins
   //
    if (digitalRead(11) == LOW)        	// 3
    { DigInState = 8;    }              // if Jumper-1 is Placed - Don't look at Rotary Sw Values
    if (digitalRead(13) == LOW)        	// 5
    { DigInState = 9;    }              // if Jumper-2 is Placed - Don't look at any other Input Values
# endif /* HAVE_JUMPERS */
      
    if(first_time)
    {
      lastDigInState = DigInState;	// dont allow a change in DigInState values if this is the first time through
      first_time = 0;
    }

    if (DigInState != lastDigInState)  	//if DigInState has changed...
    {
       blankPANEL();                    // Clear LED's
    }

    lastDigInState = DigInState;	// Store the current Input Status  
      
    switch (DigInState)			// Call the appropriate code routine - based on the input address values of the control lines A B C
    {
       case 0: 			        // DO NOTHING - ALLOW I2C FUNCTIONS TO OVER-RIDE 6-PIN INPUT
       {
        
        ///////////////////////////////////
        //  Toggle(100);
        //  Alert(100);
        //  TraceUp(1);
        //  TraceDown(1);
        //  Compress(5, 100);
        //  Expand (10, 50);
        //  Random();       
        
         //////////////////////////////////
         
          break;
       }
       case 1: 			        // FADE IN AND OUT: 
       {  allOFF();
          FadeOutIn(1);
          allOFF();
          break;
       }
       case 2: 			        // RANDOM FAST: 
       {  
          allOFF();
          FlashAll(8, 200);
          allOFF();
          break;
       }
       case 3: 			        // 2 LOOP: 
       {  
          allOFF();
          TwoLoop(2);
          allOFF();
          break;
       }
       case 4: 			        // TRACE DOWN: 
       {  
          allOFF();
          TraceDown(5,1);
          allOFF();
          break;
       }
       case 5: 			        // ONE TEST: 
       {  
          allOFF();
          OneTest(30);
          allOFF();
          break;
       }   
       case 6: 			        // Random Fast: 
       {  
          Random(random(8000,14000));
          break;
       }
       case 7: 			        // RANDOM SLOW: 
       {
          Random(random(40000,60000));    // 5 to 8 minute interval between light patterns
          break;
       }
        case 8: 			// Jumper 1  : 
       {  
          allONTimed(0);   
          break;
       }
       case 9: 			        // Jumper 2  : 
       {  
          Random(random(8000,14000));  // 1 to 2 min interval between light patterns
          break;
       }

       default:
       {                                // OFF: 
          allOFF();                     // Clear LED's
          break;
       }
    }//end switch
#endif /* HAVE_DIGITAL_CMD_INPUT || HAVE_JUMPERS */

    // End the current Pattern if there is one being "played"
    // This replaces the use of delay() which is a blocking call.
    if (patternActive)
    {
      if (time >= patternEndTime)
      {
        allOFF();
        patternActive=false;
      }
    }

    // If we want the pattern to always be on, then re-call the process function
    // with the same code as the last time.
    // This only applies to patterns that are not "all on" patterns, to avoid a 
    // LED blink due to re-calling the code.
    if (alwaysOn && 
       (lastMPeventCode > 4)){
      runPattern(lastMPeventCode);
    }


  }// end of if(time)
}//end of loop

void setPatternEndTime(unsigned long patternTime)
{
  time = millis();
  patternEndTime = time + patternTime;
  patternActive = true;
}


void Random(int RandomInterval)
{
 switch (RandomState) 
  {
    case 0:
    {
      RandomMode = random(0,35); // randomly select DisplayMode 0 to 25
      //RandomMode = 25;
        //  0 = Toggle(100);
        //  1 = Alert(100);
        //  2 = TraceUp(1);
        //  3 = TraceDown(1);
        //  4 = Compress(5, 100);
        //  5 = Expand (10, 50);
        //  6 = AllON();
        //  7 = AllOFF();
        //...
        
        
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
 //     RandomInterval = 1500;// 2500;//random(50000, 100000);  // randomly select the interval between function calls - Panel is OFF
//      RandomInterval = random(500, 1000);  // randomly select the interval between function calls - Panel is OFF
      ///
      ///  5000 = apprx 40 sec
      ///  50000 = 400 secs or 6.5 mins
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      RandomOnTime   = random(1000, 1500);// 8 to 12 seconds // ON Time - THIS TIME WILL CHANGE BASED ON THE SEQUENCE CALLED (For State Machine Based Functions)
      RandomTime = 0;     
      RandomState++;         // proceed to next state
      break;
    }
    case 1:
    {
      switch(RandomMode)
      {
        case 0:              //  0 = Turns Panel Off
        {    
          allOFF();
          break;
        }
        case 2:              //  2 = Turns Panel on for 2s
        {
          allOFF();
          allONTimed(2000);
          RandomTime = RandomOnTime + 1;
          allOFF();
          break;  
        }
        case 3:              //  3 = Begins Toggle Sequence: Top and Bottom Half of Panel Alternate
        {
          allOFF();
          Toggle(10);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 4:              // 4 = Begins Alert Sequence (4s):  Panel Rapidly Flashes On & Off
        {
          allOFF();
          Alert(8);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }       
        case 5:              //  5 = Begins Trace Up Sequence (Type 1):  Each row of the MP lights up from bottom to top filling entire panel
        {
          allOFF();
          TraceUp(5, 1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
        case 6:              //  6 = Begins Trace Up Sequence (Type 2):  Each row of the MP lights up from bottom to top individually
        {
          allOFF();
          TraceUp(5, 2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }        
        case 7:             //  7 = Begins Trace Down Sequence (Type 1):  Each row of the MP lights up from top to bottom filling entire panel
        {
          allOFF();
          TraceDown(5, 1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 8:              //  8 = Begins Trace Down Sequence (Type 2):  Each row of the MP lights up from top to bottom individually
        {
          allOFF();
          TraceDown(5, 2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 9:              //  9 = Begins Expand Sequence (Type 1): Panel expands from center filling entire panel 
        {
          allOFF();
          Expand(5,1);
          RandomTime = RandomOnTime + 1;  
          allOFF();  
          break;
        }
        case 10:               //  10 = Begins Expand Sequence (Type 2): Ring of pixels expands from center of panel
        {
          allOFF();
          Expand(5,2); 
          RandomTime = RandomOnTime + 1; 
          allOFF();  
          break;
        }
        case 11:              //  11 = Begins Compress Sequence (Type 1): Panel compresses from outer edge filling entire panel 
        {
          allOFF();
          Compress(5,1);
          RandomTime = RandomOnTime + 1;  
          allOFF();  
          break;
        }
        case 12:               //  12 = Begins Compress Sequence (Type 2): Ring of pixels compresses from outer edge of panel
        {
          allOFF();
          Compress(5,2);
          RandomTime = RandomOnTime + 1;  
          allOFF();  
          break;
        }
        case 13:              //  13 = Begins Cross Sequence: Panel is lit to display an X for 3s
        {
          allOFF();
          Cross();
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 14:              //  14 = Begins Cyclon Column Sequence:
        {
          allOFF();
          CylonCol(2, 140);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 15:              //  15 = Begins Cyclon Row Sequence:
        {
          allOFF();
          CylonRow(2, 140);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 16:              // 16 = Begins Eye Scan Sequence:
        {
          EyeScan(2, 100);
          RandomTime = RandomOnTime + 1;     
          allOFF(); 
          break;
        }        
        case 17:             //  17 = Begins Fade Out/In Sequence:
        {
          FadeOutIn(1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
        case 18:             //  18 = Begins Fade Out Sequence:
        {
          FadeOutIn(2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }          
        case 19:              //  19 = Begins Flash Sequence:
        {
          FlashAll(8, 200);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 20:              //  20 = Begins Flash V Sequence:
        {
          FlashV(8, 200);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 21:              //  21 = Begins Flash Q Sequence:  Alternating quadrants of MP flash rapidly
        {
          FlashQ(8, 200);
          RandomTime = RandomOnTime + 1;  
          allOFF();  
          break;
        }
        case 22:              //  22 = Begins Two Loop Sequence:
        {
          allOFF();
          TwoLoop(2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 23:              //  23 = Begins One Loop Sequence:
        {
          allOFF();
          OneLoop(2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 24:              //  24 = Begins Test Sequence (Type 1):
        {
          allOFF();
          TheTest(30);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 25:              //  25 = Begins Test Sequence (Type 2):
        {
          allOFF();
          OneTest(30);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
        case 26:              //  26 = Begins AI Logo Sequence:  Displays the AI Aurebesh characters for 3s that we see all over our awesome packages from Rotopod and McWhlr 
        {
          allOFF();
          Symbol();
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 27:              //  27 = Begins Quadrant Sequence (Type 1):  Each Panel Quadrant lights up individually (TL, TR, BR, BL) 
        {
          allOFF();
          Quadrant(5, 1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 28:              //  28 = Begins Quadrant Sequence (Type 2):  Each Panel Quadrant lights up individually (TR, TL, BL, BR) 
        {
          allOFF();
          Quadrant(5, 2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
        case 29:              //  29 = Begins Quadrant Sequence (Type 3):  Each Panel Quadrant lights up individually (TR, BR, BL, TL) 
        {
          allOFF();
          Quadrant(5, 3);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
        case 30:              //  30 = Begins Quadrant Sequence (Type 4):  Each Panel Quadrant Rights up individually (TL, BL, BR, TR) 
        {
          allOFF();
          Quadrant(5, 4);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }  
        case 31:              //  31 = Begins Random Pixel Sequence:  Random pixels flashe individually for 6s  
        {
          allOFF();
          RandomPixel(40);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 32:             //  32 = Begins Trace Right Sequence (Type 1):  Each column of the MP lights up from left to right filling entire panel
        {
          allOFF();
          TraceRight(5, 1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 33:              //  33 = Begins Trace Right Sequence (Type 2):  Each column of the MP lights up from left to right individually
        {
          allOFF();
          TraceRight(5, 2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 34:             //  34 = Begins Trace Left Sequence (Type 1):  Each column of the MP lights up from right to left filling entire panel
        {
          allOFF();
          TraceLeft(5, 1);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        }
        case 35:              //  35 = Begins Trace Left Sequence (Type 2):  Each column of the MP lights up from right to left individually
        {
          allOFF();
          TraceLeft(5, 2);
          RandomTime = RandomOnTime + 1;  
          allOFF();
          break;
        } 
               
      }// end of nested switch(RandomMode)  
       if(RandomTime++ > RandomOnTime)
      {
        RandomTime = 0;    // reset timer
        //allOFF();         // turn off All Leds     
        RandomState++;     // proceed to next state
      }        
      break;
    }
    
    //
    case 2:
    {
       allOFF();         // turn off All Leds    
       
       if(RandomTime++ > RandomInterval)        // OFF until timer expires
       {
        RandomTime  = 0;    // reset timer
        RandomState = 0;     // proceed to next state
       }
       break;     
    }     
  }// end switch
}//end Random



////////////////////////////////////////////////////////////////////////////////////////
// BHD Functions
////////////////////////////////////////////////////////////////////////////////////////

void EyeScan(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++)
  {
    for(int j=0; j<8; j++){
      SetRow(j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetRow(j, B00000000);
    }
    allOFF();
    delay(FlashDelay);
    for(int j=0; j<8; j++){
      SetCol(7-j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetCol(7-j, B00000000);
    }
    allOFF();
    delay(FlashDelay);
  }
}


void CylonCol(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    for(int j=0; j<8; j++){
      SetCol(j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetCol(j, B00000000);
    }
    for(int j=0; j<6; j++){
      SetCol(6-j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetCol(6-j, B00000000);
    }
  }
}

void CylonRow(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    for(int j=0; j<8; j++){
      SetRow(j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetRow(j, B00000000);
    }
    for(int j=0; j<6; j++){
      SetRow(6-j, B11111111);
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      SetRow(6-j, B00000000);
    }
  }
}

void FlashH(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    for(int j=0; j<4; j++){
      SetRow(j, B11111111);
      SetRow(j+4, B00000000);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
    for(int j=0; j<4; j++){
      SetRow(j, B00000000);
      SetRow(j+4, B11111111);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
  }
}

void FlashV(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    for(int j=0; j<4; j++){
      SetCol(j, B11111111);
      SetCol(j+4, B00000000);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
    for(int j=0; j<4; j++){
      SetCol(j, B00000000);
      SetCol(j+4, B11111111);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
  }
}

void FlashQ(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    SetRow(0, B00001111);
    SetRow(1, B00001111);
    SetRow(2, B00001111);
    SetRow(3, B00001111);
    SetRow(4, B11110000);
    SetRow(5, B11110000);
    SetRow(6, B11110000);
    SetRow(7, B11110000);
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
    SetRow(0, B11110000);
    SetRow(1, B11110000);
    SetRow(2, B11110000);
    SetRow(3, B11110000);
    SetRow(4, B00001111);
    SetRow(5, B00001111);
    SetRow(6, B00001111);
    SetRow(7, B00001111);
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
  }
}

void FlashAll(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    allON();
    delay(FlashDelay);
    allOFF();
    delay(FlashDelay);
  }
}

void OneLoop(int Repeats){
  for(int j=0; j<Repeats; j++){
    for(int i=0; i<6; i++){
      VMagicPanel[1][7-(1+i)]=true;
      MapBoolGrid();
      PrintGrid();
      delay(100);
      VMagicPanel[1][7-(1+i)]=false;
    }
    for(int i=0; i<4; i++){
      VMagicPanel[2+i][1]=true;
      MapBoolGrid();
      PrintGrid();
      delay(150);
      VMagicPanel[2+i][1]=false;
    }
    for(int i=0; i<6; i++){
      VMagicPanel[6][(1+i)]=true;
      MapBoolGrid();
      PrintGrid();
      delay(100);
      VMagicPanel[6][(1+i)]=false;
    }
    for(int i=0; i<4; i++){
      VMagicPanel[7-(2+i)][6]=true;
      MapBoolGrid();
      PrintGrid();
      delay(150);
      VMagicPanel[7-(2+i)][6]=false;
    }
  }
}

void TwoLoop(int Repeats){
  for(int j=0; j<Repeats; j++){
    for(int i=0; i<6; i++){
      VMagicPanel[1][7-(1+i)]=true;
      VMagicPanel[6][(1+i)]=true;
      MapBoolGrid();
      PrintGrid();
      delay(100);
      VMagicPanel[1][7-(1+i)]=false;
      VMagicPanel[6][(1+i)]=false;
    }
    for(int i=0; i<4; i++){
      VMagicPanel[2+i][1]=true;
      VMagicPanel[7-(2+i)][6]=true;
      MapBoolGrid();
      PrintGrid();
      delay(150);
      VMagicPanel[2+i][1]=false;
      VMagicPanel[7-(2+i)][6]=false;
    }
    for(int i=0; i<6; i++){
      VMagicPanel[6][(1+i)]=true;
      VMagicPanel[1][7-(1+i)]=true;
      MapBoolGrid();
      PrintGrid();
      delay(100);
      VMagicPanel[6][(1+i)]=false;
      VMagicPanel[1][7-(1+i)]=false;
    }
    for(int i=0; i<4; i++){
      VMagicPanel[7-(2+i)][6]=true;
      VMagicPanel[2+i][1]=true;
      MapBoolGrid();
      PrintGrid();
      delay(150);
      VMagicPanel[7-(2+i)][6]=false;
      VMagicPanel[2+i][1]=false;
    }
  }
}

void FadeOutIn(byte type) {      // FlthyMcNsty added a variable to this function pass a type value to allow for just a fade out sequence as well
  for(int i=0; i<2*NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)|random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, random(256));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
 ///// allOFF();
 ///// delay(1000);
 if (type == 1) {
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)&random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, random(256));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
  for(int i=0; i<2*NumLoops; i++){
    for(int i=0; i<8; i++){
      SetRow(i, (random(256)|random(256)));
    }
    MapBoolGrid();
    PrintGrid();
    delay(150);
  }
 }
 ///// allON();
/////  delay(2000);
}

void TheTest(int FlashDelay){
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      VMagicPanel[row][col]=true;
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
    }
  }
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      VMagicPanel[row][col]=false;
      MapBoolGrid();
      PrintGrid();
      delay(delaytime);
    }
  }
}

void OneTest(int FlashDelay){
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      VMagicPanel[row][col]=true;
      MapBoolGrid();
      PrintGrid();
      delay(FlashDelay);
      VMagicPanel[row][col]=false;
    }
  }
}

void Symbol(){
  SetRow(0, B00000100);
  SetRow(1, B00000010);
  SetRow(2, B11111111);
  SetRow(3, B00000000);
  SetRow(4, B11100111);
  SetRow(5, B00100100);
  SetRow(6, B00100100);
  SetRow(7, B01000010);
  MapBoolGrid();
  PrintGrid();
  delay(3000);
}

void Cross(){
  SetRow(0, B00000000);
  SetRow(1, B01000010);
  SetRow(2, B00100100);
  SetRow(3, B00011000);
  SetRow(4, B00011000);
  SetRow(5, B00100100);
  SetRow(6, B01000010);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(3000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// MapBoolGrid
//
// Map the Virtual 2D pixel grid to the 1D byte array that is used to write to the MAX chip(s).
// This routine does any transformations needed like mapping to multiple MAX chips (IA-Parts hardware)
// or screen rotation (RoddBotics hardware)
//

void MapBoolGrid()
{
  for(int Row = 0; Row < 8; Row++)
  {
#ifdef TWO_MAX_CHIPS
    MagicPanel[2*Row]=128*VMagicPanel[Row][7]+64*VMagicPanel[Row][6]+32*VMagicPanel[Row][5]+16*VMagicPanel[Row][4];       // 0, 2, 4, 6, 8, 10, 12, 14
    MagicPanel[2*Row+1]=8*VMagicPanel[Row][3]+4*VMagicPanel[Row][2]+2*VMagicPanel[Row][1]+VMagicPanel[Row][0];            // 1, 3, 5, 7, 9, 11, 13, 15
#else
# ifdef MP_RODDBOTICS_V2
    /*
     * V2 PCB has X/Y rotated 90 clockwise so transpose the matrix 90 anti-clockwise
     */
    MagicPanel[Row]=VMagicPanel[7][Row]+2*VMagicPanel[6][Row]+4*VMagicPanel[5][Row]+8*VMagicPanel[4][Row]+
      16*VMagicPanel[3][Row]+32*VMagicPanel[2][Row]+64*VMagicPanel[1][Row]+128*VMagicPanel[0][Row];
# endif
#endif
  }
}

    
////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintGrid
//
// PrintGrid invokes the LedControl routines to push the state to the MAX chips.

void PrintGrid()
{
#ifdef TWO_MAX_CHIPS  
  for(int i = 0; i < 8 * 2; i++)
  {
    if(i < 8)
      lc.setRow(0, i, MagicPanel[i]);
    else
      lc.setRow(1, i-8, MagicPanel[i]);
  }
#else
  for(int i = 0; i < 8; i++)
    lc.setRow(0, i, MagicPanel[i]);
#endif
}


void SetRow(int LEDRow, unsigned char RowState){
  for(int Col=0; Col<8; Col++){
    VMagicPanel[LEDRow][Col]=((RowState >> Col) & 1);
  }
}

void SetCol(int LEDCol, unsigned char ColState){
  for(int Row=0; Row<8; Row++){
    VMagicPanel[Row][LEDCol]=((ColState >> Row) & 1);
  }
}

void allON() {  //all LEDs ON simple style - Huh how does this work?
  for(int ic=0;ic<1;ic++) {
    for(int row=0;row<8;row++) {
      for(int col=0;col<8;col++) {
        VMagicPanel[row][col]=true;
      }
    }
  }
  MapBoolGrid();
  PrintGrid();
}

void allOFF() {
  for(int ic=0;ic<1;ic++) {
    for(int row=0;row<8;row++) {
      for(int col=0;col<8;col++) {
        VMagicPanel[row][col]=false;
      }
    }
  }
  MapBoolGrid();
  PrintGrid();
}


////////////////////////////////////// end BHD //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// FlthyMcNsty Functions
////////////////////////////////////////////////////////////////////////////////////////
void allONTimed(int timer) 
{ 
    allOFF();
    for(int row=0;row<8;row++)      
    {
      SetRow(row, B11111111);
    }
    MapBoolGrid();
    PrintGrid();
    if (timer < 1)            // Passing a value of 0 or below turns panel on indefinately (well for 1000s anyway)
    {
      setPatternEndTime(1000000);  
    }
    else {       
      setPatternEndTime(timer);  // Otherwise it stays on for the number of ms passed.
    }     
}

void TraceDown(int timer, byte type)
{
  while (TraceDownTime < timer) {
        for(int row=0;row<8;row++) 
        {
          SetRow(row, B11111111);
          MapBoolGrid();
          PrintGrid();
          delay(200);
          if (type == 2) {
            SetRow(row, B00000000);
            MapBoolGrid();
            PrintGrid();
          }
        } 
        allOFF();
        TraceDownTime++;
  }  
  TraceDownTime=0;  
}

void TraceUp(int timer, byte type)
{
  while (TraceUpTime < timer) {
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111111);
          MapBoolGrid();
          PrintGrid();
          delay(200);
          if (type == 2) {
            SetRow(row, B00000000);
            MapBoolGrid();
            PrintGrid();
          }
        }
        allOFF(); 
        TraceUpTime++;
  }  
  TraceUpTime=0;  
}

void TraceLeft(int timer, byte type)
{
  while (TraceLeftTime < timer) {
       if( type== 1 ) {
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000001);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000011);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00011111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00111111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B01111111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
       }  
       else if (type == 2) {
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000001);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000010);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000100);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00001000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00010000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00100000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B01000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B10000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
       }
       allOFF(); 
       TraceLeftTime++;
  }  
  TraceLeftTime=0;  
}

void TraceRight(int timer, byte type)
{
  while (TraceRightTime < timer) {
       if( type== 1 ) {
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B10000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11100000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111100);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111110);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B11111111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
       }  
       else if (type == 2) {
                for(int row=7;row>=0;--row) 
        {
          SetRow(row, B10000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B01000000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00100000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00010000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00001000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000100);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000010);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        for(int row=7;row>=0;--row) 
        {
          SetRow(row, B00000001);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
       }
       allOFF(); 
       TraceRightTime++;
  }  
  TraceRightTime=0;  
}

void RandomPixel(int timer) {
  int randRow;
  int randCol;
  while (RandomPixelTime < timer) {
    randRow = random(0,7);
    randCol = random(0,7);
    switch(randCol)
      {
        case 0:              
        {    
          SetRow(randRow, B10000000);
          break;
        }
        case 1:              
        {    
          SetRow(randRow, B01000000);
          break;
        }
        case 2:              
        {    
          SetRow(randRow, B00100000);
          break;
        }
        case 3:              
        {    
          SetRow(randRow, B00010000);
          break;
        }
        case 4:              
        {    
          SetRow(randRow, B00001000);
          break;
        }
        case 5:              
        {    
          SetRow(randRow, B00000100);
          break;
        }
        case 6:              
        {    
          SetRow(randRow, B00000010);
          break;
        }
        case 7:              
        {    
          SetRow(randRow, B00000001);
          break;
        }
      }
    MapBoolGrid();
    PrintGrid();
    delay(150); 
    allOFF();
    RandomPixelTime++;
  }  
  RandomPixelTime=0;  
}
void Quadrant(int timer, byte type)
{
  while (QuadrantTime < timer) {
    if ( type == 1 ) {
        for(int row=0;row<4;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
        for(int row=0;row<4;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
        for(int row=4;row<8;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);    
    }
    else if ( type == 2 ) {
        for(int row=0;row<4;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
        for(int row=0;row<4;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
        for(int row=4;row<8;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
    }
    else if ( type == 3 ) {
        for(int row=0;row<4;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();  
        for(int row=0;row<4;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
    }
    else if ( type == 4 ) {
        for(int row=0;row<4;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B11110000);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200); 
        allOFF();
        for(int row=4;row<8;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allOFF(); 
        for(int row=0;row<4;row++) {
          SetRow(row, B00001111);
        }
        MapBoolGrid();
        PrintGrid();
        delay(200);
    }   
    allOFF();
    QuadrantTime++;
  }  
  QuadrantTime=0;  
}

void Toggle(int timer)
{
  while (ToggleTime < timer) {
        for(int row=0;row<4;row++) 
        {
          SetRow(row, B11111111);
        }
        for(int row=4;row<8;row++) 
        {
          SetRow(row, B00000000);
        } 
        MapBoolGrid();
        PrintGrid();
        delay(500);
        for(int row=0;row<4;row++) 
        {
          SetRow(row, B00000000);
        }
        for(int row=4;row<8;row++) 
        {
          SetRow(row, B11111111);
        } 
        MapBoolGrid();
        PrintGrid();
        delay(500); 
        ToggleTime++;
  }  
  ToggleTime=0;  
}

void Alert(int timer)
{
    while (AlertTime < timer) {
        for(int row=0;row<8;row++) 
        allON();
        delay(250);
        allOFF();
        delay(250); 
        AlertTime++;
  }  
  AlertTime=0;  
}

void Expand(int timer, byte type)
{
    while (ExpandTime < timer) { 
      if(type == 1) {   
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00000000);
        SetRow(3, B00011000);
        SetRow(4, B00011000);
        SetRow(5, B00000000);
        SetRow(6, B00000000);
        SetRow(7, B00000000);  
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00111100);
        SetRow(3, B00111100);
        SetRow(4, B00111100);
        SetRow(5, B00111100);
        SetRow(6, B00000000);
        SetRow(7, B00000000);   
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B01111110);
        SetRow(2, B01111110);
        SetRow(3, B01111110);
        SetRow(4, B01111110);
        SetRow(5, B01111110);
        SetRow(6, B01111110);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        allON();
        delay(200);
      }
      else if (type == 2) {
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00000000);
        SetRow(3, B00011000);
        SetRow(4, B00011000);
        SetRow(5, B00000000);
        SetRow(6, B00000000);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00111100);
        SetRow(3, B00100100);
        SetRow(4, B00100100);
        SetRow(5, B00111100);
        SetRow(6, B00000000);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B01111110);
        SetRow(2, B01000010);
        SetRow(3, B01000010);
        SetRow(4, B01000010);
        SetRow(5, B01000010);
        SetRow(6, B01111110);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B11111111);
        SetRow(1, B10000001);
        SetRow(2, B10000001);
        SetRow(3, B10000001);
        SetRow(4, B10000001);
        SetRow(5, B10000001);
        SetRow(6, B10000001);
        SetRow(7, B11111111);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
      }
        allOFF();
        delay(200); 
        ExpandTime++;
  }  
  ExpandTime=0;  
}

void Compress(int timer, byte type)
{
    while (CompressTime < timer) {
      if(type == 1) { 
        allON();
        delay(200);  
        SetRow(0, B00000000);
        SetRow(1, B01111110);
        SetRow(2, B01111110);
        SetRow(3, B01111110);
        SetRow(4, B01111110);
        SetRow(5, B01111110);
        SetRow(6, B01111110);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00111100);
        SetRow(3, B00111100);
        SetRow(4, B00111100);
        SetRow(5, B00111100);
        SetRow(6, B00000000);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00000000);
        SetRow(3, B00011000);
        SetRow(4, B00011000);
        SetRow(5, B00000000);
        SetRow(6, B00000000);
        SetRow(7, B00000000);       
        MapBoolGrid();
        PrintGrid();
        delay(200);
      }
      else if (type == 2) {
        SetRow(0, B11111111);
        SetRow(1, B10000001);
        SetRow(2, B10000001);
        SetRow(3, B10000001);
        SetRow(4, B10000001);
        SetRow(5, B10000001);
        SetRow(6, B10000001);
        SetRow(7, B11111111);         
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B01111110);
        SetRow(2, B01000010);
        SetRow(3, B01000010);
        SetRow(4, B01000010);
        SetRow(5, B01000010);
        SetRow(6, B01111110);
        SetRow(7, B00000000);     
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00111100);
        SetRow(3, B00100100);
        SetRow(4, B00100100);
        SetRow(5, B00111100);
        SetRow(6, B00000000);
        SetRow(7, B00000000);     
        MapBoolGrid();
        PrintGrid();
        delay(200);
        SetRow(0, B00000000);
        SetRow(1, B00000000);
        SetRow(2, B00000000);
        SetRow(3, B00011000);
        SetRow(4, B00011000);
        SetRow(5, B00000000);
        SetRow(6, B00000000);
        SetRow(7, B00000000);      
        MapBoolGrid();
        PrintGrid();
        delay(200);        
      }
        allOFF();
        delay(200); 
        CompressTime++;
  }  
  CompressTime=0;  
}


void MySymbol(){
  SetRow(0, B00000000);
  SetRow(1, B01111110);
  SetRow(2, B00000010);
  SetRow(3, B01111110);
  SetRow(4, B01000000);
  SetRow(5, B01000000);
  SetRow(6, B01111110);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(1000);
  SetRow(0, B00000000);
  SetRow(1, B01111110);
  SetRow(2, B01000010);
  SetRow(3, B01000000);
  SetRow(4, B01001110);
  SetRow(5, B01000010);
  SetRow(6, B01111110);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(1000);
  SetRow(0, B00000000);
  SetRow(1, B01000010);
  SetRow(2, B01000010);
  SetRow(3, B01000010);
  SetRow(4, B01011010);
  SetRow(5, B01100110);
  SetRow(6, B01000010);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(1000);
  SetRow(0, B00000000);
  SetRow(1, B01111100);
  SetRow(2, B01000010);
  SetRow(3, B01000010);
  SetRow(4, B01000010);
  SetRow(5, B01000010);
  SetRow(6, B01111100);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(1000);
}

//////
//////////////////////////////// end FlthyMcNsty //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
//// Start The Jugg1er sequences //////
///////////////////////////////////////

void Nine(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B01000100);
  SetRow(3, B01000100);
  SetRow(4, B00111100);
  SetRow(5, B00000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Eight(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B01000100);
  SetRow(3, B00111000);
  SetRow(4, B01000100);
  SetRow(5, B01000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Seven(){
  SetRow(0, B01111100);
  SetRow(1, B00000100);
  SetRow(2, B00000100);
  SetRow(3, B00001000);
  SetRow(4, B00010000);
  SetRow(5, B00100000);
  SetRow(6, B00100000);
  SetRow(7, B00100000);
  MapBoolGrid();
  PrintGrid();
}

void Six(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B01000000);
  SetRow(3, B01111000);
  SetRow(4, B01000100);
  SetRow(5, B01000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Five(){
  SetRow(0, B01111100);
  SetRow(1, B01000000);
  SetRow(2, B01000000);
  SetRow(3, B01111000);
  SetRow(4, B00000100);
  SetRow(5, B00000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Four(){
  SetRow(0, B00000100);
  SetRow(1, B00001100);
  SetRow(2, B00010100);
  SetRow(3, B00100100);
  SetRow(4, B01000100);
  SetRow(5, B01111100);
  SetRow(6, B00000100);
  SetRow(7, B00000100);
  MapBoolGrid();
  PrintGrid();
}

void Three(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B00000100);
  SetRow(3, B00011000);
  SetRow(4, B00000100);
  SetRow(5, B00000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Two(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B00000100);
  SetRow(3, B00000100);
  SetRow(4, B00001000);
  SetRow(5, B00010000);
  SetRow(6, B00100000);
  SetRow(7, B01111100);
  MapBoolGrid();
  PrintGrid();
}

void One(){
  SetRow(0, B00010000);
  SetRow(1, B00110000);
  SetRow(2, B00010000);
  SetRow(3, B00010000);
  SetRow(4, B00010000);
  SetRow(5, B00010000);
  SetRow(6, B00010000);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Zero(){
  SetRow(0, B00111000);
  SetRow(1, B01000100);
  SetRow(2, B01000100);
  SetRow(3, B01000100);
  SetRow(4, B01000100);
  SetRow(5, B01000100);
  SetRow(6, B01000100);
  SetRow(7, B00111000);
  MapBoolGrid();
  PrintGrid();
}

void Countdown(){
  Nine();
  delay(1000);
  Eight();
  delay(1000);
  Seven();
  delay(1000);
  Six();
  delay(1000);
  Five();
  delay(1000);
  Four();
  delay(1000);
  Three();
  delay(1000);
  Two();
  delay(1000);
  One();
  delay(1000);
  Zero();
  delay(1000);
}

void ShortCountdown(){
  Three();
  delay(1000);
  Two();
  delay(1000);
  One();
  delay(1000);
  Zero();
  delay(1000);
}

// Attempt to remove delay on the pattern functions
// Currently failed so commented out.
/*
void waitTime(unsigned long waitTime){
  unsigned long startTime = millis();            // get an updated time stamp
  do {
    // just wait around in a non-blocking fashion
  } while (millis() <= startTime + waitTime);
}
*/

void Smile(){
  SetRow(0, B00111100);
  SetRow(1, B01000010);
  SetRow(2, B10100101);
  SetRow(3, B10000001);
  SetRow(4, B10100101);
  SetRow(5, B10011001);
  SetRow(6, B01000010);
  SetRow(7, B00111100);
  MapBoolGrid();
  PrintGrid();
  delay(1000);  
}

void Sad(){
  SetRow(0, B00111100);
  SetRow(1, B01000010);
  SetRow(2, B10100101);
  SetRow(3, B10000001);
  SetRow(4, B10011001);
  SetRow(5, B10100101);
  SetRow(6, B01000010);
  SetRow(7, B00111100);
  MapBoolGrid();
  PrintGrid();
  delay(1000);  
}

void Heart(){
  SetRow(0, B00000000);
  SetRow(1, B01100110);
  SetRow(2, B11111111);
  SetRow(3, B11111111);
  SetRow(4, B01111110);
  SetRow(5, B00111100);
  SetRow(6, B00011000);
  SetRow(7, B00000000);
  MapBoolGrid();
  PrintGrid();
  delay(1000);  
}

void CheckerBoard(int Repeats, int FlashDelay){
  for(int i=0; i<Repeats; i++){
    for(int j=0; j<2; j++){
      SetRow(j, B11001100);
      SetRow(j+2, B00110011);
      SetRow(j+4, B11001100);
      SetRow(j+6, B00110011);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
    for(int j=0; j<2; j++){
      SetRow(j, B00110011);
      SetRow(j+2, B11001100);
      SetRow(j+4, B00110011);
      SetRow(j+6, B11001100);
    }
    MapBoolGrid();
    PrintGrid();
    delay(FlashDelay);
  }
}


// This more closely matches the flicker from the MarDuino
void RandomAlert(int timer)
{
    while (AlertTime < timer) {
        for(int row=0;row<8;row++) 
        allON();
        int randomDelay = random(5, 40);
        delay(randomDelay);
        allOFF();
        randomDelay = random(3, 25);
        delay(randomDelay); 
        AlertTime++;
  }  
  AlertTime=0;  
}

void compressIN(int loops, int type) {
  int brow=8;
  for (int count = 0; count < loops; count++)
  {
    for(int trow=0;trow<8;trow++) {
      brow=brow-1;
        lc.setRow(0,trow,B11111111);
        lc.setRow(1,brow,B11111111);
        delay(100);
    } 
    if (type == 2)
    {
      brow=8;
      for(int trow=0;trow<8;trow++) {
        brow=brow-1;
          lc.setRow(0,trow,B00000000);
          lc.setRow(1,brow,B00000000);
          delay(100);
      }
    }
    allOFF();
    brow=8;
  }
}

void explodeOUT(int loops, int type) {
  int brow=0;
  for (int count = 0; count < loops; count++)
  {
    for(int trow=8;trow>=0;trow--) {
        lc.setRow(1,brow,B11111111);
        lc.setRow(0,trow,B11111111);
      brow=brow+1;
      delay(100);
    } 
    if (type == 2)
    {
      brow=0;
      for(int trow=8;trow>=0;trow--) {
        lc.setRow(1,brow,B00000000);
        lc.setRow(0,trow,B00000000);
      brow=brow+1;
      delay(100);
    } 
    }
    allOFF();
    brow=0;
  }
}

// Used in the VU Meter to display "levels"
byte bargraph[] = {
      B00000000,
      B10000000,
      B11000000,
      B11100000,
      B11110000,
      B11111000,
      B11111100,
      B11111110,
      B11111111,
      B01111111,
      B00111111,
      B00011111,
      B00001111,
      B00000111,
      B00000011,
      B00000001,
      B00000000
};

// Simulated VU meter
// Randomly selects an up or down shift in the bars of either 1 or 2 pixels
// on each update of the display.  4 Modes supported based on where the bars emanate from.
void VUMeter(int loops, int type) {
  int level[8] = {0,0,0,0,0,0,0,0};  

  // Set a default start level for each column.
  for (int i=0; i< 8; i++)
  {
    level[i] = random(0,9);
  }

  for (int count = 0; count < loops; count++)
  {
     for(int j=0; j<8; j++){
      if (type == 1)
      {
       SetCol(7-j, bargraph[level[j]]);
      }
      else if (type == 2)
      {
        SetRow(7-j, bargraph[level[j]]);
      }
      else if (type == 3)
      {
       SetCol(7-j, bargraph[level[j]+8]);
      }
      else if (type == 4)
      {
        SetRow(7-j, bargraph[level[j]+8]);
      }
       MapBoolGrid();
       PrintGrid();
     }
  
     delay(250);

     // calc the next position of the bars
     for (int y=0; y < 8; y++)
     {
       byte upDown = random(0,2);
       byte changeSize = random(1,3);
       // go up
       if (upDown == 1)
       {
         ((level[y] + changeSize) <= 8) ? level[y] += changeSize : level[y] = 8;
       }
       // go down
       else 
       {
         ((level[y] - changeSize) >= 0) ? level[y] -= changeSize : level[y] = 0;
       }
     }
  }
}


/////////////////////////////////////
//// End The Jugg1er sequences //////
/////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// blankPANEL
//
// blank the display

void blankPANEL()
{
  lc.clearDisplay(0);
#ifdef TWO_MAX_CHIPS  
  lc.clearDisplay(1);
#endif  
}

// The following takes the Pattern code, and executes the relevant function
// This allows i2c and serial inputs to use the same function to start patterns
// so we avoid the need to duplicate this code.
void runPattern(int pattern) {

  if (lastMPeventCode != pattern)
  {
    lastMPeventCode = pattern;
    firstTimeCode = true;
  }
  else
  {
    firstTimeCode = false;
  }
  
  switch (pattern) {
        case 0:              //  0 = Turns Panel Off
        {    
          allOFF();
          break;
        }
        case 1:              //  1 = Turns Panel On Indefinately
        {
          allONTimed(0);
          RandomTime = RandomOnTime + 1;  // force time to expire.
          break;
        }
        case 2:              //  2 = Turns Panel on for 2s
        {
          allONTimed(2000);
          RandomTime = RandomOnTime + 1;  
        }
        case 3:              //  3 = Turns Panel on for 5s
        {
          allONTimed(5000);
          RandomTime = RandomOnTime + 1; 
          break;
        }
        case 4:              //  4 = Turns Panel on for 10s
        {
          allONTimed(10000);
          RandomTime = RandomOnTime + 1; 
          break;
        }
        case 5:              //  5 = Begins Toggle Sequence: Top and Bottom Half of Panel Alternate
        {
          allOFF();
          alwaysOn ? Toggle(1) : Toggle(10);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 6:              // 6 = Begins Alert Sequence (4s):  Panel Rapidly Flashes On & Off
        {
          allOFF();
          alwaysOn ? Alert(1): Alert(8);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 7:              // 7 = Begins Alert Sequence (10s):  Panel Rapidly Flashes On & Off
        {
          allOFF();
          alwaysOn ? Alert(1): Alert(20);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }         
        case 8:              //  8 = Begins Trace Up Sequence (Type 1):  Each row of the MP lights up from bottom to top filling entire panel
        {
          allOFF();
          alwaysOn ? TraceUp(1, 1): TraceUp(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 9:              //  9 = Begins Trace Up Sequence (Type 2):  Each row of the MP lights up from bottom to top individually
        {
          allOFF();
          alwaysOn ? TraceUp(1, 2): TraceUp(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }        
        case 10:             //  10 = Begins Trace Down Sequence (Type 1):  Each row of the MP lights up from top to bottom filling entire panel
        {
          allOFF();
          alwaysOn ? TraceDown(1, 1): TraceDown(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 11:              //  11 = Begins Trace Down Sequence (Type 2):  Each row of the MP lights up from top to bottom individually
        {
          allOFF();
          alwaysOn ? TraceDown(1, 2): TraceDown(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 12:             //  12 = Begins Trace Right Sequence (Type 1):  Each column of the MP lights up from left to right filling entire panel
        {
          allOFF();
          alwaysOn ? TraceRight(1, 1): TraceRight(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 13:              //  13 = Begins Trace Right Sequence (Type 2):  Each column of the MP lights up from left to right individually
        {
          allOFF();
          alwaysOn ? TraceRight(1, 2): TraceRight(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 14:             //  14 = Begins Trace Left Sequence (Type 1):  Each column of the MP lights up from right to left filling entire panel
        {
          allOFF();
          alwaysOn ? TraceLeft(1, 1): TraceLeft(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 15:              //  15 = Begins Trace Left Sequence (Type 2):  Each column of the MP lights up from right to left individually
        {
          allOFF();
          alwaysOn ? TraceLeft(1, 2): TraceLeft(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 16:              //  16 = Begins Expand Sequence (Type 1): Panel expands from center filling entire panel 
        {
          allOFF();
          alwaysOn ? Expand(1, 1): Expand(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();  
          break;
        }
        case 17:               //  17 = Begins Expand Sequence (Type 2): Ring of pixels expands from center of panel
        {
          allOFF();
          alwaysOn ? Expand(1, 2): Expand(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();  
          break;
        }
        case 18:              //  18 = Begins Compress Sequence (Type 1): Panel compresses from outer edge filling entire panel 
        {
          allOFF();
          alwaysOn ? Compress(1, 1): Compress(5, 1);
          RandomTime = RandomOnTime + 1; 
          turnOff();  
          break;
        }
        case 19:               //  19 = Begins Compress Sequence (Type 2): Ring of pixels compresses from outer edge of panel
        {
          allOFF();
          alwaysOn ? Compress(1, 2): Compress(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();  
          break;
        }
        case 20:              //  20 = Begins Cross Sequence: Panel is lit to display an X for 3s
        {
          if (firstTimeCode) allOFF();
          Cross();
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 21:              //  21 = Begins Cyclon Column Sequence: Each column illuminated one at a time from left to right back to left. (like the Cylons from Battlestar Galactica)
        {
          allOFF();
          alwaysOn ? CylonCol(1, 140): CylonCol(2, 140);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 22:              //  22 = Begins Cyclon Row Sequence: Each row illuminated one at a time from top to bottom back to top. (like the Cylons from Battlestar Galactica)
        {
          allOFF();
          alwaysOn ? CylonRow(1, 140): CylonRow(2, 140);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 23:              // 23 = Begins Eye Scan Sequence:  Each row is illuminated from top to bottom followed by each column left to right. (like the eye ball scanners in the Mission Impossible movies)
        {
          alwaysOn ? EyeScan(1, 100): EyeScan(2, 100);
          RandomTime = RandomOnTime + 1;     
          turnOff(); 
          break;
        }    
        case 24:             //  24 = Begins Fade Out/In Sequence:  MP gradually and randomly fades out and then fades back in the same manner. 
        {
          FadeOutIn(1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 25:             //  25 = Begins Fade Out Sequence:  MP gradually and randomly fades out (Similar to the Short Circuit Sequence on Teeces). 
        {
          FadeOutIn(2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }           
        case 26:              //  26 = Begins Flash Sequence:  MP flashes rapidly for 5 seconds (Similar to Alarm Sequence)
        {
          alwaysOn ? FlashAll(1, 200): FlashAll(8, 200);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 27:              //  27 = Begins Flash V Sequence: Left and Right Half of Panel Alternate
        {
          alwaysOn ? FlashV(1, 200): FlashV(8, 200);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 28:              //  28 = Begins Flash Q Sequence:  Alternating quadrants of MP flash rapidly
        {
          alwaysOn ? FlashQ(1, 200): FlashQ(8, 200);
          RandomTime = RandomOnTime + 1;  
          turnOff();  
          break;
        }
        case 29:              //  29 = Begins Two Loop Sequence: Dual pixels are lit opposite each other completing a loop around the 2nd ring from panel edge.
        {
          if (firstTimeCode) allOFF();
          alwaysOn ? TwoLoop(1): TwoLoop(2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 30:              //  30 = Begins One Loop Sequence: A single pixel is lit individually completing a loop around the 2nd ring from panel edge.
        {
          if (firstTimeCode) allOFF();
          alwaysOn ? OneLoop(1): OneLoop(2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 31:              //  31 = Begins Test Sequence (Type 1):  Each pixel of the MP is lit sequentially from row 0, column 7 to row 7, column 0 until panel is filled, then unlit in the same order.
        {
          allOFF();
          TheTest(30);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 32:              //  32 = Begins Test Sequence (Type 2):  Each pixel of the MP is lit indivually from row 0, column 7 to row 7, column 0. 
        {
          allOFF();
          OneTest(30);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 33:              //  33 = Begins AI Logo Sequence:  Displays the AI Aurebesh characters for 3s (...that we see all over our awesome packages from Rotopod and McWhlr) 
        {
          if (firstTimeCode) allOFF();
          Symbol();
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 34:              //  34 = Begins 2GWD Logo Sequence: Displays the Characters 2-G-W-D sequentially every 1s (...shameless, I know.) 
        {
          if (firstTimeCode) allOFF();
          MySymbol();
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 35:              //  35 = Begins Quadrant Sequence (Type 1):  Each Panel Quadrant lights up individually (TL, TR, BR, BL) 
        {
          allOFF();
          alwaysOn ? Quadrant(1, 1): Quadrant(5, 1);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 36:              //  36 = Begins Quadrant Sequence (Type 2):  Each Panel Quadrant lights up individually (TR, TL, BL, BR) 
        {
          allOFF();
          alwaysOn ? Quadrant(1, 2): Quadrant(5, 2);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 37:              //  37 = Begins Quadrant Sequence (Type 3):  Each Panel Quadrant lights up individually (TR, BR, BL, TL) 
        {
          allOFF();
          alwaysOn ? Quadrant(1, 3): Quadrant(5, 3);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        } 
        case 38:              //  38 = Begins Quadrant Sequence (Type 4):  Each Panel Quadrant lights up individually (TL, BL, BR, TR) 
        {
          allOFF();
          alwaysOn ? Quadrant(1, 4): Quadrant(5, 4);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }  
        case 39:              //  39 = Begins Random Pixel Sequence:  Random pixels flash individually for 6s  
        {
          allOFF();
          RandomPixel(40);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 40:             //  40 = Begins countdown from 9 to 0 (takes 10 seconds)
        {
          Countdown();
          RandomTime = RandomOnTime + 1;
          turnOff();
          break; 
        }
        case 41:             //  40 = Begins countdown from 3 to 0 (takes 4 seconds)
        {
          ShortCountdown();
          RandomTime = RandomOnTime + 1;
          turnOff();
          break;
        }
        case 42:              // 42 = Begins Alert Sequence (4s):  Panel Randomly Flashes On & Off, mimics MarcDuino Code
        {
          allOFF();
          RandomAlert(20);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }
        case 43:              // 43 = Begins Alert Sequence (8s):  Panel Randomly Flashes On & Off, mimics MarcDuino Code
        {
          allOFF();
          RandomAlert(40);
          RandomTime = RandomOnTime + 1;  
          turnOff();
          break;
        }  
        case 44:              // 44 = Begins Smiley Face
        {
          if (firstTimeCode) allOFF();
          Smile();
          RandomTime = RandomOnTime + 1;
          turnOff();
          break;
        }  
        case 45:              // 45 = Begins Sad Face
        {
          if (firstTimeCode) allOFF();
          Sad();
          RandomTime = RandomOnTime + 1;
          turnOff();
          break;
        } 
        case 46:              // 46 = Begins Heart
        {
          if (firstTimeCode) allOFF();
          Heart();
          RandomTime = RandomOnTime + 1;
          turnOff();
          break;
        }
        case 47:              // 47 = Begins Checkerboard flash
        {
          if (firstTimeCode) allOFF();
          alwaysOn ? CheckerBoard(1, 200): CheckerBoard(8, 200);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }    
        case 48:              // 48 = Begins Compress In Type 1 - Fills the rows from TL/BR in half row increments 5 times
        {
          allOFF();
          alwaysOn ? compressIN(1, 1): compressIN(5, 1);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }   
        case 49:              // 49 = Begins Compress In Type 2 - Fills the rows from TL/BR in half row increments then clears in the same order 5 times
        {
          allOFF();
          alwaysOn ? compressIN(1, 2): compressIN(5, 2);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }  
        case 50:              // 50 = Begins Explode Out Type 1 - Fills the rows from Centre Out in half row increments 5 times
        {
          allOFF();
          alwaysOn ? explodeOUT(1, 1): explodeOUT(5, 1);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }   
        case 51:              // 51 = Begins Explode Out Type 2 - Fills the rows from Centre Out in half row increments then clears in the same order 5 times
        {
          allOFF();
          alwaysOn ? explodeOUT(1, 2): explodeOUT(5, 2);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }
        case 52:              // 52 = Begins VU Meter - Randomly ramp up/down on the columns to simulate a VU Meter. (Type 1)
        {
          allOFF();
          alwaysOn ? VUMeter(1, 1): VUMeter(15, 1);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }
        case 53:              // 53 = Begins VU Meter - Randomly ramp up/down on the rows to simulate a VU Meter. (Type 2)
        {
          allOFF();
          alwaysOn ? VUMeter(1, 2): VUMeter(15, 2);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }  
        case 54:              // 54 = Begins VU Meter - Randomly ramp up/down on the columns from the top to simulate a VU Meter. (Type 3)
        {
          allOFF();
          alwaysOn ? VUMeter(1, 3): VUMeter(15, 3);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }
        case 55:              // 55 = Begins VU Meter - Randomly ramp up/down on the rows from the right to simulate a VU Meter. (Type 4)
        {
          allOFF();
          alwaysOn ? VUMeter(1, 4): VUMeter(15, 4);
          RandomTime = RandomOnTime + 1;
          turnOff();  
          break;
        }          
        default:
          // Do nothing
          break;    
     }   
  
}

/////////////////////////////////////////////////////////////////////////////////////////
// Function to centralise decoding of Jawa Commands for both i2c and serial interfaces //
// The currently supported JawaLite commands are:                                      //
// T - Set the mode (triggers a pattern)                                               //
/////////////////////////////////////////////////////////////////////////////////////////
int decodeJawaCommand(char jawaCommand, int commandValue[])
{
  // In i2c we have already stripped the address from the start of the JawaLiteCommand
  // ie 20T0 is now just T0, so we can ignore the address parsing.  On Serial, we have no
  // need to send an address, so again let's just ignore it.

  // Check that the first Character we have received is a valid JawaLite command ie:
  // A,B,C,D,I,L,M,N,O,P,S,R,T,V,W,Z
  // And limit that to only the commands supported in the MP currently:
  // A,D,T

  String value_string = "";

  //char jawaCommand = commandReceived[0];
  //commandReceived.remove(0, 1);
  

  if (jawaCommand == 'A' ||
      jawaCommand == 'D' ||
      jawaCommand == 'T' ||
      jawaCommand == 'P')
      {
        #ifdef DEBUG
          Serial.print("Jawa Command Processing ="); Serial.println(jawaCommand); // debug
        #endif
        if (jawaCommand == 'T')
        {
          // This is kinda clunky and I should probably rewrite it, but for now it works
          // Since we support only up to 99 commands (as opposed to 999 in JawaLite)
          // We can just take the first byte after the command and convert it/
          //TODO:  Add some error checking in here!
          //String value = commandReceived.substring(0, 1);
          //commandReceived.copy(value, 0, 1);
          //return value.toInt();
          return commandValue[0];
        }
        else if (jawaCommand == 'A')
        {
          // Main mode of operation is to turn the panel on?
          // I have no idea, but it seems sensible enough!
          // Lets turn the panel on.
          allON();
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
        else if (jawaCommand == 'D')
        {
          // Go to Default mode.  So turn the panel off?
          // Seems sensible enough as a default mode!
          // Lets turn the panel off.
          allOFF();
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
        else if (jawaCommand == 'P')
        {
          // Set the Panel Mode.  
          // Note this isn't quite right, as there is only one parameter, so I ignore the parameter number.
          // Setting P0 will set the panel so that Patterns will turn off after the given time
          // Setting P1 will leave the patterns on until the T00 (off) command is received.
          if (commandValue[0] == 0)
          {
            alwaysOn = false;
            confirm(0);
          }
          if (commandValue[0] == 1)
          {
            alwaysOn = true;
            confirm(1);
          }
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
      }
      else
      {
        #ifdef DEBUG
          Serial.print("Unsupported Command ="); Serial.println(jawaCommand); // debug
        #endif
        return 0;
      }
  
}

void confirm(int param)
{
  allOFF();
  allON();
  delay(200);
  allOFF();
  delay(100);
  allON();
  delay(200);
  allOFF();
  if (param == 0)
  {
    Zero();
  }
  else if (param == 1)
  {
    One();
  }
  delay(2000);
  allOFF();
  
  
}

void turnOff(){
  if (!alwaysOn)
  {
    allOFF();
  }
}


// function that executes whenever data is received from an I2C master
// this function is registered as an event, see setup()
void receiveEvent(int eventCode) {
  RandomOnTime   = random(1000, 1500);// 8 to 12 seconds // ON Time - THIS TIME WILL CHANGE BASED ON THE SEQUENCE CALLED (For State Machine Based Functions)
  RandomTime = 0;
  
  char i2cCommand;
  int i2cValue[3] = {0,0,0};
  int i2cCommandLen;

  // Check to see how much data is on the bus
  i2cCommandLen = Wire.available();

  // If the command is too big, return effectively ignoring it
  // 4 characters is the current max we support
  if (i2cCommandLen > 4)
  {
    #ifdef DEBUG
      Serial.print("Command too large. "); Serial.print(i2cCommandLen); Serial.println(" bytes.  Ignoring.");
    #endif
    for (int i=0; i<i2cCommandLen; i++)
    {
      Wire.read();
    }
    return;
  }

  #ifdef DEBUG
    Serial.print("Available len? ="); Serial.println(i2cCommandLen);
  #endif

  for (int i=0; i<i2cCommandLen; i++)
  {

    // First data should be the command type, it's a hex value / character
    if (i==0)
    {
      i2cCommand = Wire.read();
    }
    // Everything else should be int if it's a good JawaLite Command
    else
    {
      i2cValue[i-1] = Wire.read();
      #ifdef DEBUG
        Serial.println(i2cValue[i-1]);
      #endif
    }

    // Don't seem to get the end character from MarcDuino, but for now leave this check as it is valid.
    if (i2cValue[i-1] == '\r')
    {
      break;
    }
    #ifdef DEBUG
      Serial.print("i2c loop ="); Serial.print(i2cCommand); Serial.print(i2cValue[0]); Serial.print(i2cValue[1]); Serial.println(i2cValue[2]); // debug
    #endif
  }

  #ifdef DEBUG
    Serial.print("Command ="); Serial.println(i2cCommand);
    Serial.print("Value ="); Serial.print(i2cValue[0]); Serial.print(i2cValue[1]); Serial.println(i2cValue[2]);
  #endif

  int event = decodeJawaCommand(i2cCommand, i2cValue);

  runPattern(event);
}

/*
   SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
  */
void serialEvent() {
   while (Serial.available()) {

     char inChar = (char)Serial.read();                     // get the new byte

     if (inChar == '\r') {
       stringComplete = true;
     } else {
       inputString += inChar;                                 // add it to the inputString
     }
     if (stringComplete == true){
        #ifdef DEBUG
          Serial.print("In the eval... "); Serial.print(inputString);Serial.println('\r'); // debug
        #endif

        // Get the length of the input string
        int len = inputString.length();

        char jawaCommand = inputString[0];
        inputString.remove(0,1);

        // Since everything on serial comes in one block, assume everything is a single uint_8.
        // This might be a bad assumption.
        int jawaValue[3] = {0,0,0};
        jawaValue[0] = inputString.toInt();

        int code = decodeJawaCommand(jawaCommand, jawaValue);

        
        stringEvaluated = true;
        MPeventCode = code;

        #ifdef DEBUG
         Serial.print("EventReceived ="); Serial.println(inputString); // debug
        #endif
         if (stringEvaluated == true){
             inputString ="";
             stringComplete = false;
             stringEvaluated = false;
             #ifdef DEBUG
               Serial.println("inputString and String Complete reset");  //debug
               Serial.println(MPeventCode);  //debug
             #endif
             runPattern(MPeventCode);
          }
       }
    }
    sei();
}
