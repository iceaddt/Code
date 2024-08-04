

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





