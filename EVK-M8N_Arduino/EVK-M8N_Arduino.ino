/* This code is for the Polaris Sr. Project. Used with Arduino Uno and STM32 Nucleo L476RG
 *  
 *  This is edited code from LessonStudio
 *  https://github.com/LessonStudio/Arduino_GPS/blob/master/Arduino_GPS.ino
 *  
 *  Last edited: 02/27/20
 */
//For GPS 
#include "TinyGPS++.h"
#include "SoftwareSerial.h"

//// For the Uno
//SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
 
// For the STM32 Nucleo-L476RG added by Daniel 
HardwareSerial Serial1(PA10, PA9); //RX=pin PA10, TX=pin PA9
//end of added code by Daniel

TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  Serial1.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started
}

void loop()
{
  while(Serial1.available())//While there are characters to come from the GPS
  {
    gps.encode(Serial1.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit and print to the Arduino Serial Monitor.
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");
  }
}
