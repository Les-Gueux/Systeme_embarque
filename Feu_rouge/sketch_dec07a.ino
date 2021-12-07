// Programme simulant dex feux rouge de deux cotés pour les véhicules et un feu pour les piétons
// Un bouton est mis à disposition aux piétons pour diviser par deux le temps d'éclairage du feu vert des véhicules


int LedR = 8;
int LedJ = 9;
int LedV = 10;

int LedCR = 6;
int LedCJ = 5;
int LedCV = 4;

int LedPV = 7;
int LedPR = 11;
int LedCPV = 3;
int LedCPR = 12;

int Btn = 2;

volatile int cpt = 0;

bool flag = true;

void appuisPieton(){
  if(flag){
    Serial.println(cpt);
    cpt = cpt/2;
    Serial.println(cpt);
    flag = false;
  }
}


void setup() {
  // put your setup code here, to run once:

  pinMode(LedR,OUTPUT);
  pinMode(LedJ,OUTPUT);
  pinMode(LedV,OUTPUT);
  pinMode(LedCR,OUTPUT);
  pinMode(LedCJ,OUTPUT);
  pinMode(LedCV,OUTPUT);
  pinMode(LedPV,OUTPUT);
  pinMode(LedPR,OUTPUT);
  pinMode(LedCPV,OUTPUT);
  pinMode(LedCPR,OUTPUT);
  pinMode(Btn,INPUT_PULLUP);
  attachInterrupt(0, appuisPieton, RISING);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LedPV,LOW);
  digitalWrite(LedPR,HIGH);
  digitalWrite(LedCV,LOW);
  digitalWrite(LedCJ,HIGH);

  delay(1000);
  digitalWrite(LedCJ,LOW);
  digitalWrite(LedR,LOW);
  digitalWrite(LedV,HIGH);
  digitalWrite(LedCR,HIGH);
  digitalWrite(LedCPR,LOW);
  digitalWrite(LedCPV,HIGH);
  for(cpt = 9000; cpt > 0; cpt--){
    delay(1);
   //Serial.println(cpt);
  }
  digitalWrite(LedV,LOW);
  digitalWrite(LedJ,HIGH);
  delay(2000);
  digitalWrite(LedJ,LOW);
  digitalWrite(LedCPR,HIGH);
  digitalWrite(LedCR,LOW);
  digitalWrite(LedCV,HIGH);
  digitalWrite(LedCPV,LOW);
  digitalWrite(LedR,HIGH);
  delay(1000);
  digitalWrite(LedPR,LOW);
  digitalWrite(LedPV,HIGH);
  delay(9000);
  flag = true;
}
