int buzzer = 9;
int buton = 10;
int ledler[] ={2,3,4,5,6,7};
int echo_pin = 12;
int trig_pin = 11;
long mesafe;
int alarm = 0;

void setup() {
Serial.begin(9600);
Serial.println("Sistem Aktiflestirildi...");
Serial.println(alarm);
pinMode(buzzer,OUTPUT);
pinMode(trig_pin,OUTPUT);
pinMode(echo_pin,INPUT);
for (int i = 0; i<6;i++)
{
  pinMode(ledler[i],OUTPUT);
}

}

void loop() {
if (alarm == 0 && digitalRead(buton))
{
  alarm_aktif();
  Serial.println(alarm);
  delay(1000);
  while (!digitalRead(buton))
  {
    mesafe = mesafeolcme();
    Serial.print("Mesafe: ");
    Serial.print(mesafe);
    Serial.println(" Cm");
    if (mesafe<20)
    {
      isikyak();
      digitalWrite(buzzer,HIGH);
      delay(100);
      digitalWrite(buzzer,LOW);
      delay(100);
    }
  }
  
}
else if(alarm == 1 && digitalRead(buton))
{
  delay(1000);
  alarm_pasif();
}
}

int mesafeolcme()
{
  long mesafe, sure;
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  sure = pulseIn(echo_pin, HIGH);
  mesafe = (sure/2)/29.154;
  return mesafe;
}

void alarm_aktif()
{
  alarm = 1; 
  Serial.println("Alarm Aktiflestiriliyor...");
  for (int i = 0; i<10;i++)
  {
    digitalWrite(ledler[0],HIGH);
    delay(100);
    digitalWrite(ledler[0],LOW);
    delay(100);
  }
  digitalWrite(ledler[0],HIGH);
  digitalWrite(ledler[5],LOW);
  Serial.println("Alarm Aktiflestirildi...");
}

void alarm_pasif()
{
  alarm = 0; 
  Serial.println("Alarm Pasiflestiriliyor...");
  for (int i = 0; i<10;i++)
  {
    digitalWrite(ledler[5],HIGH);
    delay(100);
    digitalWrite(ledler[5],LOW);
    delay(100);
  }
  digitalWrite(ledler[5],HIGH);
  digitalWrite(ledler[0],LOW);
  Serial.println("Alarm Pasiflestirildi...");
}

void isikyak()
{
  for (int i = 1; i<5; i++)
  {
    digitalWrite(ledler[i], HIGH);
  }
  delay(100);
  for (int i = 1; i<5; i++)
  {
    digitalWrite(ledler[i], LOW);
  }
  delay(100);
}