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
          viewHtml(soilMoistureValue)
          // output the value of each analog input pin
          //client.print("analog input ");
          //client.println("</html>");
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
void viewHtml(int soilMoistureValue) {
  const char header[] = "<head><meta charset='UTF-8'><link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css'><script src='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js'></script><title>Arrosage automatique Arduino</title></head>";
  const char body []= "<body id='body' class='overview'><section class='main'><a class='titre' position='center'>Arrosage automatique</a></section><section class='section-doc'><div class='documentation'><a class='title-doc'>Données</a> <a class="text-data"> Taux d'humidité : </a><br><a class="text-data"> Température : </a><div class='button-doc'><a class='waves-effect waves-light btn-large black' id='dogy' onclick='bonToutou()'>Clique</a></div></div></section></body>";
  const char style[]= "<style>.main {margin-top: 10%;text-align: center;font-family: 'Comic Sans MS', 'Comic Sans', cursive;font-style: normal;font-weight: normal;font-size: 30px;line-height: 37px;letter-spacing: -0.005em;}.section-doc, .section-true-doc {margin-top: 10%;}.titre {color: #FFFFFF !important;}.documentation {width: 50%;background: rgba(255, 255, 255, 0.18);border-radius: 4px;margin-left: auto;margin-right: auto;height: 35%;border: 10px solid;}.overview{background: rgba(0, 0, 0, 0.91);}.button-doc {margin-top: 10%;font-family: Damion;}.title-doc{font-family: Fredoka One;font-style: normal;font-weight: normal;font-size: 35px;letter-spacing: -0.005em;color: #000000;}.text-data{font-family: Fredoka One;font-style: normal;font-weight: normal;font-size: 15px;letter-spacing: -0.005em;color: #FFFFFF;}.title-doc, .button-doc {text-align: center;display: block;}</style>";

  char view[];
  strcpy(view,header);
  strcat(view,body);
  strcat(view,"</html>");
  strcat(view,style);

  client.println(view);
}
