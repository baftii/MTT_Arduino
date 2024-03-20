#include <stdint.h>
#include <stdbool.h>

// Kod şöyle çalışıyor ihtiyacımız olan 1 tane led 1 tane buton
// Bu led ve butonun pinlerini aşağıdaki define kısmında değiştirmeniz lazım
//
// Algoritma şöyle başlangıçta kalibrasyon başlıyor. Kalibrasyon başlayınca bağladığınız led yanacak 10 saniye sürecek.
// Bu süre boyunca çizgi sensörünü siyah veya beyaz çizgi üzerinde sağ sol yapmanız lazım. Her sensörden dışına içine tekrar tekrar girsin.
// Bu süre bitince led sönecek. Led söndükten sonra eğer butona basarsanız sensör çalışmaya başlayacak. Çalışmaya başladığında bağlandığınız LED yanıp sönmeye başlayacak.
// Kalibre verilerini ve sensör verilerini seri moniterden görüntüleyebilirsiniz.

// A7 pini sensördeki 8.'ye bağlanacak A0 ise 1.'ye bağlanacak

// buradaki pinlerin ayarlanması lazım sayıların yerine

#define SensorSize 8
#define BUTTON D15
#define LED_PIN D8

int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};

int minKal[SensorSize], maxKal[SensorSize], esik[SensorSize], degerler[SensorSize];

int timefor;
bool ledCheck = true;
bool maxCheck = false;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON, INPUT);

    Serial.println("Kalibrasyon Başladi.");
    digitalWrite(LED_PIN, HIGH);
    int kalibresure = millis() + 10000;
    while (millis() < kalibresure)
    {
        kalibrasyon();
        delay(20);
    }

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

    while (digitalRead(BUTTON) == LOW)
    {
        delay(100);
    }

    digitalWrite(LED_PIN, HIGH);

    timefor = millis() + 500;
}

void loop()
{
    if (millis() > timefor)
    {
        if (ledCheck == true)
        {
            ledCheck = false;
            digitalWrite(LED_PIN, LOW);
        }

        else
        {
            ledCheck = true;
            digitalWrite(LED_PIN, HIGH);
        }

        timefor = millis() + 500;
    }

    Serial.println("");

    veriOkuTersten(degerler, SensorSize);

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

    Serial.print("|| Eşik: ");

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
    int sure;
    int sample = 64;
    int tempMax[SensorSize], tempMin[SensorSize], tempValue[SensorSize];

    for (int i = 0; i < sample; i++)
    {
        veriOkuTersten(tempValue, SensorSize);

        for (int j = 0; j < SensorSize; j++)
        {
            if (i == 0 || tempValue[j] > tempMax[j])
            {
                tempMax[j] = tempValue[j];
            }

            if (i == 0 || tempValue[j] < tempMin[j])
            {
                tempMin[j] = tempValue[j];
            }
        }
    }

    if (maxCheck == false)
    {
        for (int i = 0; i < SensorSize; i++)
        {
            maxKal[i] = tempMax[i];
            minKal[i] = tempMin[i];
        }

        maxCheck = true;
    }

    else if (maxCheck == true)
    {
        for (int i = 0; i < SensorSize; i++)
        {
            if (maxKal[i] < tempMax[i])
            {
                maxKal[i] = tempMax[i];
            }

            if (minKal[i] > tempMin[i])
            {
                minKal[i] = tempMin[i];
            }
        }
    }

    for (int i = 0; i < SensorSize; i++)
    {
        esik[i] = (maxKal[i] + minKal[i]) / 2;
    }
}

void veriOku(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = analogRead(analogPins[i]);
    }
}

void veriOkuTersten(int arr[], int size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        arr[i] = analogRead(analogPins[i]);
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