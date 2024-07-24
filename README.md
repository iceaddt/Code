( Pengujian Pencacah Geiger Muller)

#include <SPI.h>
#define LOG_PERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch
unsigned long counts;     //variable for GM Tube events
float cpm;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
float usv;
float cf;


void tube_impulse(){       //subprocedure for capturing events from Geiger Kit
  counts++;
}
void setup(){             //setup subprocedure
  counts = 0;
  cpm = 0;
  cf = 151;
  usv = 0;
 // usv = cpm / cf;
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  Serial.begin(9600);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts 
  Serial.println ("counting start");
}

void loop(){                                 //main cycle
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > LOG_PERIOD){
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    usv = (cpm / cf) * 1;

    Serial.println(cpm);
    //Serial.println(multiplier);
Serial.println(usv);
    counts = 0;
  }
}
//  Program Secara Keseluruhan

(Transmitter)
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define LOG_PERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch

const uint64_t pipeOut = 0xE8E8F0F0E1LL;
RF24 radio(10, 9);
const int voltagePin = A0;   // Pin analog untuk membaca tegangan baterai
const float referenceVoltage = 5.0;  // Tegangan referensi pada mikrokontroler (biasanya 5V)
const float batteryVoltage = 3.7;    // Tegangan nominal baterai (misalnya, baterai LiPo)
unsigned long counts;     //variable for GM Tube events
float cpm;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
float usv;
float cf;
void tube_impulse(){       //subprocedure for capturing events from Geiger Kit
  counts++;
}
void setup(){             //setup subprocedure
  counts = 0;
  cpm = 0;
  cf = 151;
  usv = 0;
 // usv = cpm / cf;
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts 
  Serial.println ("counting start");
}
void loop(){                                 //main cycle
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > LOG_PERIOD){
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    usv = (cpm / cf) * 1;

      int rawValue = analogRead(voltagePin);

  // Mengonversi nilai mentah ke tegangan
  float voltage = (rawValue / 1023.0) * referenceVoltage;
  // Menghitung kapasitas baterai berdasarkan perbandingan dengan tegangan nominal
  float batteryCapacity = (voltage / batteryVoltage) * 100;
radio.write(&usv, sizeof(usv));
radio.write(&batteryCapacity, sizeof(batteryCapacity));  
Serial.println(usv);
Serial.println(batteryCapacity);
    counts = 0;
  }
}
(Receiver)
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// #include <DHT.h>
// #include <Wire.h>
// DHT dht(2, DHT11);  //Pin, Jenis DHT
// float temp;
// int tempPin = A1;
// float humid;
// int humidPin = A2;
// int counter = 0;
// const byte address[6] = "00001";
const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(10, 9);  // CE, CSN
unsigned long counts;     //variable for GM Tube events
float cpm;    
float usv;
float cf;
float batteryCapacity;
void setup() {
  Serial.begin(9600);
  // dht.begin();
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1, pipeIn);
  // radio.openReadingPipe(0, address);
  // radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
     radio.read(&usv, sizeof(usv));
     Serial.print(usv);
     Serial.println();
     //radio.read(&batteryCapacity, sizeof(batteryCapacity));
     //Serial.print(batteryCapacity);
   
  }
}
![image](https://github.com/user-attachments/assets/aa8077b7-2b58-4dbe-8416-cfa0db03d09b)
