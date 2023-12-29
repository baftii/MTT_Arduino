#define buzzer 9
int ledler[] = {2,3,4,5,6,7};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i<6;i++)
  {
    pinMode(ledler[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  Serial.println("Seri iletisim basladi.");
  while (!Serial);


}

void loop() {
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'a')
    {
      digitalWrite(buzzer, HIGH);
      delay(20);
      digitalWrite(buzzer, LOW);
      Serial.println("Ses Üretildi");
    }
    
    else if (ch == 'k')
    {
      digitalWrite(ledler[0], HIGH);
      digitalWrite(ledler[3], HIGH);
      Serial.println("Kırmızı Ledler yakıldı.");
    }

    else if (ch == 'm')
    {
      digitalWrite(ledler[2], HIGH);
      digitalWrite(ledler[5], HIGH);
      Serial.println("Mavi Ledler yakıldı.");
    }

    else if (ch == 's')
    {
      digitalWrite(ledler[1], HIGH);
      digitalWrite(ledler[4], HIGH);
      Serial.println("Sarı Ledler yakıldı.");
    }

    else if (ch == 'o')
    {
      for (int i = 0; i<6;i++)
      {
        digitalWrite(ledler[i], LOW);
      }
    }
  }
}
