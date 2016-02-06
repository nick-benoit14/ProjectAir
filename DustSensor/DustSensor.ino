/*
Grove - Dust Sensor Demo v1.0
 Interface to Shinyei Model PPD42NS Particle Sensor
 Program by Christopher Nafis 
 Written April 2012
 
 http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html
 http://www.sca-shinyei.com/pdf/PPD42NS.pdf
 
 JST Pin 1 (Green Wire)  //Arduino GND
 JST Pin 3 (Yellow wire) //Arduino 5VDC
 JST Pin 4 (Black wire) //Arduino Digital Pin 8
 */

#include <SPI.h>
#include <SD.h>
#include <Time.h>



const int sensorPin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; //air quality sensor sample time
unsigned long lowpulseoccupancy = 0; //store LPO for spensor
float ratio = 0;
float concentration = 0;

File dataFile; //SD card logger object
const int chipSelect = 10;
unsigned int counter = 0;

void setup() {
  
  Serial.begin(9600);

  /*       Shinyei Sensor         */
  pinMode(sensorPin,INPUT);
  starttime = millis();//get the current time;
  

  /*  Initialize SD Card   */
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
   dataFile = SD.open("datalog.txt", FILE_WRITE); 
   if (dataFile) {
    // Print csv headers
    dataFile.println("Concentration (pcs/0.01cf),Time (Unix Timestamp) ,Location (Lat / Long)");
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
 
void loop() {
  
  duration = pulseIn(sensorPin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
 
  if ((millis()-starttime) >= sampletime_ms)//if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
     
    lowpulseoccupancy = 0; //Reset low time
    starttime = millis(); //Restart timer
    

//  /*              Log to SD Card                     */
//    // if the file is available, write to it:

    String dataString = ""; // Hold data info    
    dataString += String(concentration);
    dataString += ", ";
    dataString += String(now());
    dataString += ", ";
    dataString += String(counter++);
    

    
    dataFile = SD.open("datalog.txt", FILE_WRITE); 
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}
