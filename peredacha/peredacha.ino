#include <SPI.h>
#include "Wire.h"
#include <RF24.h>
#include <nRF24L01.h>
#include <Adafruit_Sensor.h>
 #include "I2Cdev.h"                  
#include "MPU6050.h"             
#include <Adafruit_BMP280.h>
#include <SD.h>
MPU6050 CY531; 


const uint64_t pipe = 0xF0F0F0F0F0LL; // индитификатор передачи, "труба"
 const int PIN_CHIP_SELECT = 4;
RF24 radio(8, 9); // CE, CSN
 
 Adafruit_BMP280 bmp;
 int strt_alt;
 int maxalt;
struct values{
  int Altitude;
  float Voltage;
  int16_t Axel;
  boolean Start_point;
   boolean Apogee_point ;
    boolean Activate_point;
     boolean Landing_point;
     long Tlme;
     
     
  
  };
 // const byte address[5] = "00001";
struct values control;

 
void setup(){
  Serial.begin(9600);
Wire.begin();
  radio.begin();
  delay(2);
  radio.setChannel(77); // канал (0-127)
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_MAX);   
  radio.openWritingPipe(pipe);
  radio.stopListening();// открываем трубу на передачу.
delay(100);
Serial.println("Initializing I2C devices..."); 
  CY531.initialize();  
                  
  delay(100);
  CY531.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
pinMode(2, OUTPUT);
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");

    return;
  }
   Serial.println("card initialized.");
    Serial.println(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not bmp"));}

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     
                  Adafruit_BMP280::SAMPLING_X2,   
                  Adafruit_BMP280::SAMPLING_X16,   
                  Adafruit_BMP280::FILTER_X16,      
                  Adafruit_BMP280::STANDBY_MS_63);

  delay(2000);
  strt_alt =bmp.readAltitude(1040.08);
  maxalt=bmp.readAltitude(1040.08)-strt_alt;
  delay(2000);
  } 


void loop()   
{String zap;
  control.Tlme=millis()/100;
  
  control.Altitude=bmp.readAltitude(1040.08)-strt_alt;
  control.Axel=CY531.getAccelerationX();
  control.Voltage = analogRead(A2)* (5.0/1023.0) / 0.18;
  if(maxalt<control.Altitude){
    maxalt=control.Altitude;
  }
  
 Serial.println(maxalt);
 if (control.Altitude>3){
  control.Start_point=1;
 }
 if (maxalt-3>control.Altitude){
  control.Apogee_point=1;
  control.Activate_point=1;
 }
 if (control.Apogee_point==1 and control.Altitude<80){
  control.Landing_point=1;
 }
  
 zap=("TeamId"+String(control.Tlme)+";"+String(bmp.readAltitude(1040.08)-strt_alt)+";"+String(control.Voltage)+";"+String(CY531.getAccelerationX())+";"+String(control.Start_point)+";"+String(control.Apogee_point)+";"+String(control.Activate_point)+";"+String(control.Landing_point)+";");
  Serial.println(zap);

   
   File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(zap);
    dataFile.close();
    Serial.println("записал ");
  }
  else {
    Serial.println("error opening datalog.txt");
  }
  int data=control.Tlme;
if(radio.write(&control, sizeof(control))){  //send control values
    Serial.println("Data sent successfully"); }
  else{
    Serial.println("Data sending failed");}
  Serial.println(zap);
 //digitalWrite(2, HIGH);
  delay(100);
 //digitalWrite(2, LOW);
} 
