int ledler[] = {2,3,4,5,6,7};
int gecikme = 50;
int buton = 10;
int sayac = 0;
void setup()
{
  Serial.begin(9600);
  for(int i=0;i<6;i++){
  	pinMode(ledler[i], OUTPUT);
  }
  pinMode(buton, INPUT);
}

void loop()
{
Serial.print("Buton: ");
Serial.println(buton);
Serial.print("Sayac: ");
Serial.println(sayac);
button();
switch(sayac)
{
  case 0:
      seriyakma();
      if (digitalRead(buton)==HIGH)
      {
    	  break;
      }
      break;

  case 1:
      tumyakma();
  	  if (digitalRead(buton)==HIGH)
      {
    	  break;
      }
      break;

  case 2:
      tekyakma();
  	  if (digitalRead(buton)==HIGH)
      {
    	  break;
      }
      break;

  case 3:
      ciftyakma();
  	  if (digitalRead(buton)==HIGH)
      {
    	  break;
      }
      break;
}
}

void seriyakma()
{
  Serial.println("Seri 1");
  for(int i=0;i<6;i++){
    Serial.print(ledler[i]);
    if (i == 0){
      if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Kirmizi");
    }
	else if (i == 1){
    if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Mavi");
    }
	else if (i == 2){
    if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Sari");
    }
	else if (i == 3){
    if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Yesil");
    }
	else if (i == 4){
    if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Beyaz");
    }
	else if (i == 5){
    if (digitalRead(buton)==HIGH)
      {
    	  return loop();
      }
   		Serial.println(": Kirmizi");
    }
    digitalWrite(ledler[i],HIGH);
    delay(gecikme);
    digitalWrite(ledler[i],LOW);
  }
  Serial.println("Seri 2");
  for(int i=4;i >= 1;i--){
  	Serial.print(ledler[i]);
    if (i == 4){
   		Serial.println(": Beyaz");
    }
	else if (i == 3){
   		Serial.println(": Yesil");
    }
	else if (i == 2){
   		Serial.println(": Sari");
    }
	else if (i == 1){
   		Serial.println(": Mavi");
    }
    digitalWrite(ledler[i],HIGH);
    delay(gecikme);
    digitalWrite(ledler[i],LOW);
  }
}

void tumyakma()
{
  digitalWrite(ledler[0],LOW);
  for (int i = 0; i<6;i++)
  {
    digitalWrite(ledler[i],HIGH);
  }

}

void tekyakma()
{
  digitalWrite(ledler[0],LOW);
	for (int i = 1; i<6;i=i+2)
    {
      digitalWrite(ledler[i],HIGH);
    }

}

void ciftyakma()
{
  digitalWrite(ledler[0],LOW);
	for (int i = 0; i<6;i=i+2)
    {
      digitalWrite(ledler[i],HIGH);
    }

}

void sondurme()
{
  delay(1000);
	for (int i = 0; i<6;i++)
    {
    	digitalWrite(ledler[i],LOW); 
    }
  
}

void button()
{
if (digitalRead(buton) == HIGH)
{
	sayac++;
  	sondurme();
  	if (sayac == 4)
    {
    	sayac = 0;
    }
}
}
