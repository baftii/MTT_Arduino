int trig_pin = 11;
int echo_pin = 12;
long sure, mesafe;
int ledler[] = {2,3,4,5,6,7};
int gecikme = 10;            // Delay

void setup() {
pinMode(trig_pin, OUTPUT);
pinMode(echo_pin, INPUT);
Serial.begin(9600);
for (int i;i<6;i++)
{
  pinMode(ledler[i],OUTPUT);      // Identifying LED ports
}
}

void loop() {
  mesafe = mesafe_algilama();
  if (mesafe<10)
  {
    sondur();
    for (int i=0; i<6;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<20)
  {
    sondur();
    for (int i=0; i<5;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<40)
  {
    sondur();
    for (int i=0; i<4;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<60)
  {
    sondur();
    for (int i=0; i<3;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<100)
  {
    sondur();
    for (int i=0; i<2;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<150)
  {
    sondur();
    for (int i=0; i<1;i++)
    {
      digitalWrite(ledler[i],HIGH);
    }
    delay(gecikme);
  }
  else if (mesafe<250)
  {
    sondur();
    Serial.println("Algilanmadi");
    delay(gecikme);
  }
}

int mesafe_algilama()          // Calculates how far away the object
{
digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin,LOW);
  sure = pulseIn(echo_pin, HIGH);    // Calculates time interval between the sounds out from the trigger and in to the echo
  mesafe = (sure/2)/29.154;          // To find the how far away, first we are getting half our time interval because in this interval the sound goes two times in same road.
  Serial.print("Mesafe: ");
  Serial.print(mesafe);
  Serial.println(" CM");
  delay(200);
  return mesafe;
}

void sondur()
{
  for (int i = 0; i<6; i++)
    {
      digitalWrite(ledler[i],LOW);
    }
}
