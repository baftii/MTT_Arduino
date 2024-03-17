#include <stdint.h>
#include <stdbool.h>

// Kod şöyle çalışıyor ihtiyacımız olan 1 tane led 1 tane buton
// Bu led ve butonun pinlerini aşağıdaki define kısmında değiştirmeniz lazım
// ledon_pin çizgi sensöründe ledon pinine bağlı pin olacak
//
// Algoritma şöyle başlangıçta kalibrasyon başlıyor. Kalibrasyon başlayınca bağladığınız led yanacak 10 saniye sürecek.
// Bu süre boyunca çizgi sensörünü siyah veya beyaz çizgi üzerinde sağ sol yapmanız lazım. Her sensörden dışına içine tekrar tekrar girsin.
// Bu süre bitince led sönecek. Led söndükten sonra eğer butona basarsanız sensör çalışmaya başlayacak. Çalışmaya başladığında bağlandığınız LED yanıp sönmeye başlayacak.
// Kalibre verilerini ve sensör verilerini seri moniterden görüntüleyebilirsiniz.

// buradaki pinlerin ayarlanması lazım sayıların yerine
#define SensorSize 8
#define BUTTON 9
#define LED_PIN 10
#define LEDON_PIN 11

uint8_t minKal[SensorSize], maxKal[SensorSize], esik[SensorSize], degerler[SensorSize];

int time;
bool ledCheck = true;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LEDON_PIN, OUTPUT);
    pinMode(BUTTON, INPUT);

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

    digitalWrite(LED_PIN, HIGH);

    time = millis() + 500;
}

void loop()
{
    if (millis() > time)
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
        time = millis() + 500;
    }

    Serial.println("");

    // buradaki analogreadlerin içine pinler girilmesi lazım
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
    digitalWrite(LEDON_PIN, HIGH);
    for (int i = 0; i < SensorSize; i++)
    {
        minKal[i] = analogRead(i);
        maxKal[i] = analogRead(i);
    }

    time = millis();

    while (millis() < time + 10000)
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
