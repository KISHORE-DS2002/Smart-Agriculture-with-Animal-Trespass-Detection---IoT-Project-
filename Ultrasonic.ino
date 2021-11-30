#define trigpin 11
#define echopin 10
#define buzzer 7
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //objsize = Serial.parseInt();
  //int size = ((2*actual) - (left+right));
  int i = 0;
  long distance, duration;
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance = (duration/2)/29.1;
  if(distance<=10)
  {
    Serial.println("Animal Tresspass!");
    Serial.print("Object approaching at a Distance of: ");
    Serial.println(distance);
      do
      {
        i++;
        tone(buzzer,450);
        delay(500);
        noTone(buzzer);
        delay(500);
      }while(i<4);
  }
  else
  {
    Serial.print("Distance Monitoring Range: ");
    Serial.println(distance);
  }
  delay(300);
}
