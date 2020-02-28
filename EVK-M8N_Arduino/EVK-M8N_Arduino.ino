/* This code is for the Polaris Sr. Project. Used with Arduino Uno and STM32 Nucleo L476RG
 *  
 *  This is edited code from LessonStudio
 *  https://github.com/LessonStudio/Arduino_GPS/blob/master/Arduino_GPS.ino
 *  
 *  SD card attached to SPI bus as follows:
 ** MOSI - Uno pin 11 -> PA7
 ** MISO - Uno pin 12 -> PA6
 ** CLK - Uno pin 13 -> PA5
 ** CS - Uno pin 10 -> PB6 (STM32 Nucleo L476RG)
 *  
 *  created 01/2020
 *  by Daniel Hirst
 *  Last edited: 02/27/20
 */
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <SPI.h>
#include <SD.h>

File gps_csv; //creating an object named gps_csv in the class named file. 

//// For the Uno
//SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
 
// For the STM32 Nucleo-L476RG added by Daniel 
HardwareSerial Serial1(PA10, PA9); //RX=pin PA10, TX=pin PA9


TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  Serial1.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started

  //Begining of SD card set up and test 
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(PB6)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  //Open SD file called test.csv 
  gps_csv = SD.open("test.csv", FILE_WRITE);
  //name each of the col with units
  gps_csv.println("Satalite Count, Speed MPH, Altitude feet"); 
  gps_csv.close();
}

void loop()
{
  while(Serial1.available())//While there are characters to come from the GPS
  {
    gps.encode(Serial1.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
      //Get the latest info from the gps object which it derived from the data sent by the GPS unit and print to the Arduino Serial Monitor.
    
    Serial.print(gps.satellites.value());
    Serial.print(", ");
    Serial.print(gps.speed.mph());
    Serial.print(", ");
    Serial.println(gps.altitude.feet());

    //  For writing to SD card
    
    //Open SD file called test.csv 
    gps_csv = SD.open("test.csv", FILE_WRITE);
    
    if (gps_csv) 
    {
      gps_csv.print(gps.satellites.value());
      gps_csv.print(", ");
      gps_csv.print(gps.speed.mph());
      gps_csv.print(", ");
      gps_csv.println(gps.altitude.feet());
      //close the file to ensure it is saved to the SD card
      gps_csv.close(); 
     }
     else 
     {
      // if the file didn't open, print an error:
      Serial.println("error opening test.csv");
     }
     // re-open the file for reading:
//  gps_csv = SD.open("test.csv");
//  if (gps_csv) 
//  {
//    Serial.println("test.csv: has been successfully reopened to read the csv.");
//
//    // read from the file until there's nothing else in it:
//    while (gps_csv.available()) 
//    {
//      Serial.write(gps_csv.read());
//    }
//    // close the file:
//    gps_csv.close();
//  } 
//  else 
//  {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }
  }
}
