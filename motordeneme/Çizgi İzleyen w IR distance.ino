#include <stdint.h>
#include <stdbool.h>

#define SagMotorHiz 1
#define SolMotorHiz 2
#define SagMotor1 3
#define SolMotor1 4
#define SagMotor2 5
#define SolMotor2 6
#define TrigPin 7
#define EchoPin 9

#define distancePin 12

#define Button 11

#define SensorSize 8

#define baslangicHiz 150

uint8_t minKal[SensorSize], maxKal[SensorSize], esik[SensorSize];

int32_t P, I, D, oncekiHata, PID_Degeri;

uint16_t solHiz, sagHiz;

uint8_t beyazcizgicount = 0;
int32_t beyazcizgitime = 0;

bool motorstopcount = false;
uint16_t mesafe20count = 0;

bool trafikCheck = false;
bool firstTurnCheck = false;
bool hareketsizCheck = false;

bool parklockcheck = false;

bool asamaikigiris = false;
bool asamaikicikis = false;

bool asamaucgiris = false;
bool asamauccikis = false;

bool elipsGiris = false;
bool elipsCikis = false;
bool elipsOrtaCheck = false;
int32_t elipsOrtatime = 0;

bool asansorCheck = false;
bool asansorSonrasiSapma = false;

float Kp = 0;
float Kd = 0;
float Ki = 0;

void setup()
{
    while (!digitalRead(Button))
    {
    }
    kalibrasyon();
    delay(5000);
}

void loop()
{
    if (parked == 1)
    {
        while (1)
        {
            MotorDurdur();
        }
    }

    if ((beyazUstunde(1) && beyazUstunde(2) && beyazUstunde(3) && beyazUstunde(4) && beyazUstunde(5) && beyazUstunde(6) && beyazUstunde(7) && beyazUstunde(8)) && (millis() > beyazcizgitime + 750))
    {
        if (firstTurnCheck == 0 || (asamaikicikis == 1 && asamauccikis == 0) || (elipsCikis == 1 && yayaCheck == 0))
        {
            beyazcizgicount++;
            beyazcizgitime = millis();
        }
    }

    if (mesafeOlc() < 20)
    {
        if (mesafe20count > 10)
        {
            MotorDurdur();
        }
        mesafe20count++;
        if (motorstopcount == 0 && mesafe20count > 100)
        {
            trafikLambasiGecis();
        }

        else if (motorstopcount == 1 && mesafe20count > 30)
        {
            hareketsizEngelGecis();
        }

        else if (motorstopcount == 2 && mesafe20count > 100)
        {
            asansorGecis();
        }
    }

    else
    {
        mesafe20count = 0;
        genelCizgiTakip();
    }
}

void genelCizgiTakip(void)
{
    // İlk sağ dönüş
    if (firstTurnCheck == false && beyazcizgicount == 1 && (beyazUstunde(1) && (beyazUstunde(4) || beyazUstunde(3)) && siyahUstunde(8)))
    {
        YerindeSagHareket(255);
        delay(100);
        firstTurnCheck = 1;
    }

    // 2. Aşama sağ sapma giriş
    else if ((beyazUstunde(6) && beyazUstunde(3)) && beyazcizgicount == 1 && (siyahUstunde(5) || siyahUstunde(4)) && asamaikigiris == false && trafikCheck == true)
    {
        YerindeSagHareket(150);
        delay(200);
        asamaikigiris = 0;
    }

    // 2. Aşama Sol sapma çıkış
    else if (((beyazUstunde(6) || beyazUstunde(5)) && beyazUstunde(4) && beyazUstunde(3)) && asamaikicikis == false && hareketsizCheck == true)
    {
        YerindeSolHareket(255);
        delay(200);
        asamaikicikis = 1;
    }

    // 3. Aşama sol sapma giriş
    else if ((beyazUstunde(6) && beyazUstunde(5) && beyazUstunde(4)) && beyazcizgicount == 2 && (siyahUstunde(2) && siyahUstunde(1)) && asamaucgiris == false && asamaikicikis == true)
    {
        YerindeSolHareket(150);
        delay(200);
        asamaucgiris = 1;
    }

    // 3. Aşama park durumu
    else if (beyazcizgicount == 4 && parklockcheck == false && asamaucgiris == true && asamauccikis == false)
    {
        MotorDurdur();
        geri(100);
        delay(150);
        while (!digitalRead(Button))
        {
        }
        delay(500);
        parklockcheck = 1;
    }

    // 3. Aşama sol sapma çıkış
    else if (((beyazUstunde(6) || beyazUstunde(5)) && beyazUstunde(2) && beyazUstunde(3)) && asamauccikis == false && asamaucgiris == true)
    {
        YerindeSolHareket(150);
        delay(200);
        asamauccikis = 1;
    }

    // Çember giriş
    else if ((beyazUstunde(2) && beyazUstunde(4) && beyazUstunde(5) && beyazUstunde(7)) && asamauccikis == true && elipsGiris == false)
    {
        YerindeSagHareket(150);
        delay(200);
        elipsGiris = 1;
    }

    // Çember Orta sapma engelleme
    else if ((beyazUstunde(1) && beyazUstunde(2) && beyazUstunde(3)) && elipsGiris == true && elipsOrtaCheck == false)
    {
        elipsOrtatime = millis();
        elipsOrtaCheck = 1;
    }

    // Çember çıkış
    else if ((beyazUstunde(4) || beyazUstunde(5) && (beyazUstunde(6) && beyazUstunde(7))) && elipsGiris == true && elipsCikis == false && elipsOrtaCheck == true && millis() > elipsOrtatime + 750)
    {
        YerindeSagHareket(255);
        delay(200);
        elipsCikis = 1;
    }

    // Asansör sonrası sapma
    else if (((beyazUstunde(4) || beyazUstunde(5)) && beyazUstunde(2)) && asansorCheck == true && asansorSonrasiSapma == false)
    {
        asansorSonrasiSapma = 1;
        ileri(255);
        delay(200);
    }

    // Normal çizgi takip
    else if (beyazUstunde(5) || beyazUstunde(4))
    {
        Kp = 0.0006 * (1000 - (analogRead(5) + analogRead(4)) / 2);
        Kd = 10 * Kp;
        Ki = 0.00001;
        cizgitakip();
    }

    // Kaybolma
    else if (siyahUstunde(1) && siyahUstunde(2) && siyahUstunde(3) && siyahUstunde(4) && siyahUstunde(5) && siyahUstunde(6) && siyahUstunde(7) && siyahUstunde(8))
    {
        YerindeSagHareket(255);
    }
}

void cizgitakip(void)
{
    int hata = (analogRead(6) - analogRead(3));

    P = hata;
    I = I + hata;
    D = hata - oncekiHata;

    PID_Degeri = (Kp * P) + (Ki * I) + (Kd * D);
    oncekiHata = hata;

    solHiz = baslangicHiz - PID_Degeri;
    sagHiz = baslangicHiz + PID_Degeri;

    if (solHiz > 255)
    {
        solHiz = 255;
    }
    else if (solHiz < 0)
    {
        solHiz = 0;
    }

    if (sagHiz > 255)
    {
        sagHiz = 255;
    }
    else if (sagHiz < 0)
    {
        sagHiz = 0;
    }

    sagMotorIleri(sagHiz);
    solMotorIleri(solHiz);
}

// Main Kalibrasyon Fonksiyonu
// ***************************
// Parameter
// None
// ***************************
// Returns
// Maksimum alınan değerler, Minimum alınan değerler, eşik değerleri
// ***************************
// Amaç
// İlk başta yarım açı sol, sonrasında sağ ve sol şeklinde gidip gelecek şekilde hareket yaptırmak.
// Bu hareket sürecinde sensörlerden veriler alacak. Alınan maksimum ve minimum değerleri tespit edecektir.
// Kalibrasyon bittiğinde maksimum ve minimum değerlerin ortalamasını esik değere kaydetecektir.

void kalibrasyon(void)
{
    int checkSol = 0, checkSag = 1, firsttime = 1, time, maintime, firstloopcheck = 1;
    for (int i = 0; i < SensorSize; i++)
    {
        minKal[i] = analogRead(i);
        maxKal[i] = analogRead(i);
    }

    maintime = millis();
    while (millis() > (maintime + 5000))
    {
        if (firstloopcheck == 1)
        {
            firstloopcheck = 0;
            time = millis();
        }

        if (firsttime == 1)
        {
            YerindeSolHareket(255);
            while (millis() > (time + 500))
            {
                maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
            }
            firsttime = 0;
            time = millis();
        }

        else
        {
            if (checkSol == 1)
            {
                SolKalb(255, &checkSol, &checkSag, &time);
            }

            if (checkSag == 1)
            {
                SagKalb(255, &checkSol, &checkSag, &time);
            }
        }
    }

    MotorDurdur();

    for (int i = 0; i < 8; i++)
    {
        esik[i] = (maxKal[i] + minKal[i]) / 2;
    }
}

// Sol Hareket Kalibrasyon Fonksiyonu
// ***************************
// Parameter
// hiz, Sol dönüş flag adresi, Sağ dönüş flag adresi, millis timer adresi
// ***************************
// Returns
// Sol dönüş flag, Sağ dönüş flag, millis timer
// ***************************
// Amaç
// Sola dönüş hareketi yapmak.

static void SolKalb(int hiz, int *SolCheck, int *SagCheck, int *sure)
{
    YerindeSolHareket(hiz);
    while (millis() > *sure + 1000)
    {
        maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
    }

    *SolCheck = 0;
    *SagCheck = 1;
    *sure = millis();
}

// Sağ Hareket Kalibrasyon Fonksiyonu
// ***************************
// Parameter
// hiz, Sol dönüş flag adresi, Sağ dönüş flag adresi, millis timer adresi
// ***************************
// Returns
// Sol dönüş flag, Sağ dönüş flag, millis timer
// ***************************
// Amaç
// Sağa dönüş hareketi yapmak.

static void SagKalb(int hiz, int *SolCheck, int *SagCheck, int *sure)
{
    YerindeSagHareket(hiz);
    while (millis() > *sure + 1000)
    {
        maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
    }

    *SolCheck = 1;
    *SagCheck = 0;
    *sure = millis();
}

// Maksimum Minimum değer bulma Fonksiyonu
// ***************************
// Parameter
// maksimum array adresi, minimum array adresi, sensor sayısı
// ***************************
// Returns
// minimum array değeri, maksimum array değeri
// ***************************
// Amaç
// Her sensörden veri almak. Bu verileri maksimum ve minimum değer arrayindeki değerler ile
// karşılaştırıp minimum ve maksimum değerleri düzenlemek

static void maxminKalbComp(uint8_t *max, uint8_t *min, int size)
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

// Sol hareket Fonksiyonu
// ***************************
// Parameter
// hiz değeri
// ***************************
// Returns
// none
// ***************************
// Amaç
// Parametre olarak alınan hızda yerinde sola dönme yapar

void YerindeSolHareket(int hiz)
{
    sagMotorIleri(hiz);
    solMotorGeri(hiz);
}

// Sağ hareket Fonksiyonu
// ***************************
// Parameter
// hiz değeri
// ***************************
// Returns
// none
// ***************************
// Amaç
// Parametre olarak alınan hızda yerinde sağa dönme yapar

void YerindeSagHareket(int hiz)
{
    sagMotorGeri(hiz);
    solMotorIleri(hiz);
}

void hareketsizEngelGecis(void)
{
    YerindeSolHareket(255);
    if (mesafeOlc() > 20)
    {
        delay(100);
    }

    while (siyahUstunde(1) && siyahUstunde(2) && siyahUstunde(3) && siyahUstunde(4) && siyahUstunde(5) && siyahUstunde(6) && siyahUstunde(7) && siyahUstunde(8))
    {
        ileri(255);
    }

    motorstopcount = 2;
    hareketsizCheck = 1;
}

void asansorGecis(void)
{
    while (mesafeOlc() < 20)
    {
        delay(2000);
    }
    motorstopcount = 3;
    asansorCheck = 1;
}

void trafikLambasiGecis(void)
{
    while (mesafeOlc() < 20)
    {
        delay(2000);
    }
    motorstopcount = 1;
    trafikCheck = 1;
}

void ileri(int hiz)
{
    sagMotorIleri(hiz);
    solMotorIleri(hiz);
}

void geri(int hiz)
{
    sagMotorGeri(hiz);
    solMotorGeri(hiz);
}

void MotorDurdur(void)
{
    sagMotorIleri(0);
    solMotorIleri(0);
}

void sagMotorIleri(int hiz)
{
    analogWrite(SagMotorHiz, hiz);
    digitalWrite(SagMotor1, LOW);
    digitalWrite(SagMotor2, HIGH);
}

void sagMotorGeri(int hiz)
{
    analogWrite(SagMotorHiz, hiz);
    digitalWrite(SagMotor1, HIGH);
    digitalWrite(SagMotor2, LOW);
}

void solMotorIleri(int hiz)
{
    analogWrite(SolMotorHiz, hiz);
    digitalWrite(SolMotor1, LOW);
    digitalWrite(SolMotor2, HIGH);
}

void solMotorGeri(int hiz)
{
    analogWrite(SolMotorHiz, hiz);
    digitalWrite(SolMotor1, HIGH);
    digitalWrite(SolMotor2, LOW);
}

float mesafeOlc()
{
    float voltaj, distance;

    voltaj = analogRead(distancePin) * (5 / 1024);
    distance = 13 * (pow(voltaj, -1));
    delay(17);

    return distance;
}

bool beyazUstunde(int x)
{
    if (analogRead(x) < esik[x])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool siyahUstunde(int x)
{
    if (analogRead(x) > esik[x])
    {
        return true;
    }
    else
    {
        return false;
    }
}
