#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>

const int AirValue = 900; //you need to replace this value with Value_1
const int WaterValue = 300; //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;
int soilMoistureValue = 0;

Servo monservo;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 11);

EthernetServer server(80);


void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  monservo.attach(2);  // utilise la broche 2 pour le contrôle du servomoteur
  monservo.write(0); // positionne le servomoteur à 0°
  
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.println("Setup OK");
}

void loop(){
  // listen for incoming clients
  Serial.println("wait");
  EthernetClient client = server.available();
  Serial.println("finish");
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          //client.print("analog input ");
          client.println(soilMoistureValue);
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  
  soilMoistureValue = analogRead(A2); //put Sensor insert into soil
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    monservo.write(0);
  }
  else if(soilMoistureValue >= (WaterValue + intervals) && soilMoistureValue <= (AirValue - intervals))
  {
    monservo.write(0);
  }
  else if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
  {
    monservo.write(90);
  }
}
