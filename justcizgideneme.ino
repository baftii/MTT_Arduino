void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int a0 = analogRead(A0);
    int a1 = analogRead(A1);
    int a2 = analogRead(A2);
    int a3 = analogRead(A3);
    int a4 = analogRead(A4);
    int a5 = analogRead(A5);
    int a6 = analogRead(A6);
    int a7 = analogRead(A7);

    Serial.print(a0);
    Serial.print(" ");
    Serial.print(a1);
    Serial.print(" ");
    Serial.print(a2);
    Serial.print(" ");
    Serial.print(a3);
    Serial.print(" ");
    Serial.print(a4);
    Serial.print(" ");
    Serial.print(a5);
    Serial.print(" ");
    Serial.print(a6);
    Serial.print(" ");
    Serial.print(a7);
    Serial.println("");
}