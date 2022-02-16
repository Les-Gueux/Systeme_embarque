#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

const int AirValue = 550; //you need to replace this value with Value_1
const int WaterValue = 260; //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;
int soilMoistureValue = 0;

float roomTemperature = 0;
float roomMoisture = 0;

DHT dht(3, DHT11);


void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  
  pinMode(4,OUTPUT);
  
  dht.begin();
  Serial.println("Setup OK");
}

void loop(){
  
  soilMoistureValue = analogRead(A2); //put Sensor insert into soil

  if(soilMoistureValue > 500){
    digitalWrite(4,HIGH);
    delay(10000);
  }else{
    digitalWrite(4,LOW);
  }

  delay(50);
  roomMoisture = dht.readTemperature();
  roomTemperature = dht.readHumidity();
  delay(50);

  Serial.println("Humidite du sol = " + String(soilMoistureValue));
  Serial.println("Temperature = " + String(dht.readTemperature())+ " °C");
  Serial.println("Humidite de l'aire = " + String(dht.readHumidity())+ " %");

  delay(1000);

}
