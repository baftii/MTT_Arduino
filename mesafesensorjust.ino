#define distancePin A0

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    float voltaj, distance;

    voltaj = analogRead(distancePin) * (5 / 1024);
    distance = 13 * (pow(voltaj, -1));
    Serial.print("Mesafe: ");
    Serial.println(distance);
    delay(100);
}

