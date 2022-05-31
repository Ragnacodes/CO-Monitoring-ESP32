#define ONBOARD_LED  2
#define LED          15


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");

  pinMode(ONBOARD_LED,OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  digitalWrite(ONBOARD_LED,HIGH);
  digitalWrite(LED,LOW);
  delay(1000);
  digitalWrite(ONBOARD_LED,LOW);
  digitalWrite(LED,HIGH);
}
