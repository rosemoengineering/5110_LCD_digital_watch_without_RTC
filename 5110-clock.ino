#include <LCD5110_Basic.h>

#include <TimeLib.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message  
/*
 RST --- > 11
 CE  --- > 12
 DC  --- > 10
 DIN --- > 9
 CLK --- > 8
 */

LCD5110 myGLCD(8,9,10,11,12);
 
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern unsigned char BigNumbers[];
void setup()
{

  myGLCD.InitLCD();
  Serial.begin(9600);
  myGLCD.setContrast(70);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}
 
void loop()
{
     if (Serial.available()) {
    processSyncMessage();
  }
  
myGLCD.clrScr();
myGLCD.setFont(SmallFont);
myGLCD.print("ROSEMO", CENTER, 0);
myGLCD.print("ENGINEERING", CENTER, 8);

String hourStr;
hourStr = String(hour()+3);
int hours =hour();
String minStr;
minStr = String(minute());
int minutes =minute();
String secStr;
secStr = String(second());
int seconds =second();

if(seconds < 10){
myGLCD.print( "0", 53, 24);
myGLCD.print(secStr, 60, 24);
}
else
{
myGLCD.print(secStr, 53, 24);
}
myGLCD.print(":", 48, 24);
if(minutes < 10)
{
myGLCD.print( "0", 35, 24);
myGLCD.print( minStr, 42, 24);
}
else
{
 myGLCD.print( minStr, 35, 24); 
}

myGLCD.print(":", 30, 24);
if(hours+3 < 10)
{
myGLCD.print( "0", 18, 24);
myGLCD.print( hourStr, 25, 24);
}
else
{
  myGLCD.print( hourStr, 18, 24);
}

String yearStr;
yearStr = String(year());
String dayStr;
dayStr = String(day());
int d =day();
String monthStr;
monthStr = String(month());
int m =month();
myGLCD.print(yearStr, 51, 40);
myGLCD.print("/", 43, 40);
if(m < 10)
{
myGLCD.print( "0", 30, 40);
myGLCD.print( monthStr, 37, 40);
}
else
{
 myGLCD.print( monthStr, 30, 40); 
}
myGLCD.print("/", 23, 40);
if(d < 10)
{
myGLCD.print( "0", 10, 40);
myGLCD.print( dayStr, 16, 40);
}
else
{
myGLCD.print( dayStr, 10, 40);
}


}



void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { 
       setTime(pctime); 
     }
  }
}



time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

