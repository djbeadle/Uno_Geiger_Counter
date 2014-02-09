// The most up to date version of my Geiger Counter software, this has support for the Geiger Counter (of course),
// data logger shield with the RTC and a LCD display.

#include <SPI.h>

#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define ECHO_TO_SERIAL 1 // echo data to serial port

RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

//Defining the two LEDs onboard the shield, I've soldered green to D7 and red to D6
#define redLEDpin 6
#define greenLEDpin 7

// the logging file
File logfile;

// Variables for calculating CPM and total CPM
int check;
int TOTAL_COUNT;
int SECOND_CLOCK;
int MINUTE_COUNTER=0;
int CPM = 0;
float CURRENT_AVE;
boolean x = true;
boolean FIRST_RUN = true;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  digitalWrite(redLEDpin, HIGH);
  while(1);
}

void setup(void) {

  // the debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);

  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running...");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    Serial.println("...RTC should now be set and running.");
  }
  else {
    Serial.println("Setting RTC....");
    RTC.adjust(DateTime(__DATE__, __TIME__));
    rtc_serial();
    Serial.println("...RTC should now be set and running.");
  }

  Serial.begin(9600);
  Serial.println();

  //This is for the LCD display
  lcd.begin(16,2);
  lcd.print("SYSTEM ONLINE");
  delay(1000);
  lcd.clear();


  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");

  // create a new file
  char filename[] = "CPM00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[3] = i/10 + '0';
    filename[4] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break; // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);
  lcd.setCursor(0,0);
  lcd.print(filename);

  // connect to RTC
  Wire.begin();
  if (!RTC.begin()) {
    logfile.println("RTC failed");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC Failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif //ECHO_TO_SERIAL
  }

  Serial.println("RTC Says:");
  rtc_serial();
  lcd.setCursor(0,1);
  rtc_LCD();

  delay(1000);

  logfile.println("millis,TIME,CPM,TOTAL_COUNT,CURRENT_AVE,FIRST_RUN");
#if ECHO_TO_SERIAL
  Serial.println("millis,TIME,CPM,TOTAL_COUNT,CURRENT_AVE,FIRST_RUN");
#endif //ECHO_TO_SERIAL

  //This sets up the display, the different counters are each updated individually from this point on.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CurrentCPM:");
  lcd.setCursor(0,1);
  lcd.print("Min:");
  lcd.setCursor(5,1);
  lcd.print("0");
}

void loop(void)
{
  DateTime now = RTC.now();
  // SENSOR READINGS GO HERE //
  if (Serial.available() > 0) {  // If information available
    check = Serial.read();       // Read serial input
    if (check > 0){
      CPM = CPM + 1;                    // If it is a hit, increment counter
      TOTAL_COUNT++;            // The total counter
      Serial.print("CPM as of now: ");
      Serial.println(CPM);
      lcd.setCursor(13,0);
      lcd.print(CPM);



    }  
  }

  SECOND_CLOCK = now.second(), DEC;

  if (SECOND_CLOCK == 0) {
    if (x == true) {
      digitalWrite(greenLEDpin, HIGH);
      once_a_minute();
      digitalWrite(greenLEDpin, LOW);
      MINUTE_COUNTER++;
      lcd.setCursor(14,0);      //This part clears the second digit of the CurrentCPM LCD counter with a space
      lcd.print(" ");
      lcd.setCursor(5,1);
      lcd.print(MINUTE_COUNTER);
      lcd.setCursor(10,1);
      lcd.print(CURRENT_AVE);
      

    }
    if (FIRST_RUN == true){
      Serial.println("FIRST RUN DOESN'T COUNT");
      CURRENT_AVE = 0.00;
      FIRST_RUN = false;
      TOTAL_COUNT = 0;
    }
  }

  if (SECOND_CLOCK == 5) {
    x = true;
  }

  logfile.flush();
}


