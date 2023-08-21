#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24

const uint64_t pipe = 0xF0F0F0F0F0LL; // индитификатор передачи, "труба"
//const byte address[5] = "00001";
RF24 radio(8, 9); // CE, CSN

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
  //long data;
struct values control;
void setup(){
  Serial.begin(9600);
  
  radio.begin();  
  delay(2);
  radio.setChannel(77); // канал (0-127)
  
      // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
      // RF24_250KBPS на nRF24L01 (без +) неработает.
      // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_250KBPS);     
  
      // мощьность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_MAX);   
    //radio.setAutoAck(0);
  radio.openReadingPipe(1,pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу
}
 
//  radio.stopListening(); // останавливает приём (нужно перед началом передачи) 


void loop()   
{  
  //float data[4]; 
  //int data;
  if (radio.available()){ // проверяем не пришло ли чего в буфер.
    radio.read(&control, sizeof(control)); // читаем данные, указываем сколько байт читать
    
    Serial.println("data: ");
   Serial.print("teamid;");
   Serial.print(control.Tlme);
   Serial.print(";");
   Serial.print(control.Altitude);
   Serial.print(";");
    Serial.print(control.Voltage);
   Serial.print(";");
   Serial.print(control.Axel);
   Serial.print(";");
   Serial.print(control.Start_point);
   Serial.print(";");
   Serial.print(control.Apogee_point);
   Serial.print(";");
   Serial.print(control.Activate_point);
   Serial.print(";");
   Serial.println(control.Landing_point);
   
   
    delay(5);
  } 
} 
