#include <stdint.h>
#include <stdbool.h>

#define SensorSize 8

uint8_t minKal[SensorSize], maxKal[SensorSize], esik[SensorSize], degerler[SensorSize];

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Kalibrasyon Başladi.");
    digitalWrite(LED_PIN, HIGH);
    kalibrasyon();

    for (int i = 0; i < SensorSize; i++)
    {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" Max: ");
        Serial.print(maxKal[i]);
        Serial.print(" Min: ");
        Serial.print(minKal[i]);
        Serial.print(" Esik Deger: ");
        Serial.println(esik[i]);
    }

    Serial.println("Kalibrasyon Bitti.");
    digitalWrite(LED_PIN, LOW);

    while (!digitalRead(BUTTON))
    {
    }
}

void loop()
{
    Serial.println("");

    degerler[0] = analogRead();
    degerler[1] = analogRead();
    degerler[2] = analogRead();
    degerler[3] = analogRead();
    degerler[4] = analogRead();
    degerler[5] = analogRead();
    degerler[6] = analogRead();
    degerler[7] = analogRead();

    for (int i = SensorSize - 1; 0 <= i; i--)
    {
        if (beyazUstunde(i))
        {
            Serial.print("B ");
        }

        else
        {
            Serial.print("S ");
        }
    }

    Serial.print("|| ");

    Serial.print("Okumalar: ");
    for (int i = SensorSize - 1; 0 <= i; i--)
    {
        Serial.print(degerler[i]);
        Serial.print(" ");
    }

    Serial.print("|| ");

    for (int i = SensorSize - 1; 0 <= i; i--)
    {
        Serial.print(esik[i]);
        Serial.print(" ");
    }

    Serial.println(" ");

    delay(50);
}

void kalibrasyon(void)
{
    int time;
    for (int i = 0; i < SensorSize; i++)
    {
        minKal[i] = analogRead(i);
        maxKal[i] = analogRead(i);
    }

    time = millis();

    while (millis() < time + 5000)
    {
        maxminKalbComp(maxKal[0], minKal[0], SensorSize);
    }

    for (int i = 0; i < 8; i++)
    {
        esik[i] = (maxKal[i] + minKal[i]) / 2;
    }
}

void maxminKalbComp(uint8_t *max, uint8_t *min, int size)
{
    for (int j = 0; j < size; j++)
    {
        int temp = analogRead(j);

        if (*(min + j) > temp)
        {
            *(min + j) = temp;
        }

        if (*(max + j) < temp)
        {
            *(max + j) = temp;
        }
    }
}

bool beyazUstunde(int x)
{
    if (degerler[x] < esik[x])
    {
        return true;
    }
    else
    {
        return false;
    }
}
