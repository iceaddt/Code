
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
    // Serial.println(counter);
    int sensorValue = analogRead(A0);
    float temp = analogRead(A1);
    temp = temp * 0.48828125;
    float humid = analogRead(A2);

    radio.read(&batteryCapacity, sizeof(batteryCapacity));
    radio.read(&usv, sizeof(usv));
    
    Serial.println(usv);
    //  delay(1000);
    // Serial.print("kelembaban: ");
      Serial.println(batteryCapacity);


    delay(1000);
  }
}
