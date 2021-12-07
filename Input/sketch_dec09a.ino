// Allume une led en fonction du caractère entré 

int LedR = 8;
int LedG = 9;
int LedB = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(LedR,OUTPUT);
  pinMode(LedG,OUTPUT);
  pinMode(LedB,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    // read the incoming byte:
    char incomingByte = Serial.read();
    switch (incomingByte) {
    case 'R':
      digitalWrite(LedR,HIGH);
      digitalWrite(LedG,LOW);
      digitalWrite(LedB,LOW);
      Serial.write(incomingByte);
      break;
    case 'G':
      digitalWrite(LedR,LOW);
      digitalWrite(LedG,HIGH);
      digitalWrite(LedB,LOW);
      Serial.write(incomingByte);
      break;
    case 'B':
      digitalWrite(LedR,LOW);
      digitalWrite(LedG,LOW);
      digitalWrite(LedB,HIGH); 
      Serial.write(incomingByte);   
      break;
    }
    delay(1000);
  }
}