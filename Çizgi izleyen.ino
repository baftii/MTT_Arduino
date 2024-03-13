#define SagMotorHiz 1
#define SolMotorHiz 2
#define SagMotor1 3
#define SolMotor1 4
#define SagMotor2 5
#define SolMotor2 6
#define TrigPin 7
#define EchoPin 9

#define Button 11

#define SensorSize 8

int minKal[SensorSize], maxKal[SensorSize], esik[SensorSize];

int P, D, I, previousError, PIDvalue;

int solHiz, sagHiz;

int baslangicHiz = 150;

float Kp = 0;
float Kd = 0;
float Ki = 0;

void setup()
{
    // put your setup code here, to run once:
}

void loop()
{
    while (!digitalRead(Button))
    {
    }
    kalibrasyon();
    delay(5000);

    while (1)
    {
        if (mesafeOlc() < 30)
        {
            MotorDurdur();
            delay(500);
        }

        else
        {
            genelCizgiTakip();
        }
    }
}

void genelCizgiTakip(void)
{
    if (analogRead(1) < esik[1] && analogRead(8) > esik[8])
    {
        YerindeSolHareket(255);
    }

    else if (analogRead(8) < esik[8] && analogRead(1) > esik[1])
    {
        YerindeSagHareket(255);
    }

    else if (analogRead(5) < esik[5] || analogRead(4) < esik[4])
    {
        Kp = 0.0006 * (1000 - (analogRead(5) + analogRead(4)) / 2);
        Kd = 10 * Kp;
        Ki = 0.00001;
        cizgitakip();
    }
}

void cizgitakip(void)
{
    int error = (analogRead(6) - analogRead(3));

    P = error;
    I = I + error;
    D = error - previousError;

    PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = error;

    solHiz = baslangicHiz - PIDvalue;
    sagHiz = baslangicHiz + PIDvalue;

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
            maxminKalbComp(&maxKal[0], &minKal[0], 8);
            if (millis() > (time + 500))
            {
                firsttime = 0;
                time = millis();
            }
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
    maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
    if (millis() > (sure + 1000))
    {
        *SolCheck = 0;
        *SagCheck = 1;
        *sure = millis();
    }
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
    maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
    if (millis() > (sure + 1000))
    {
        *SolCheck = 1;
        *SagCheck = 0;
        *sure = millis();
    }
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

static void maxminKalbComp(int *max, int *min, int size)
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

double mesafeOlc(void)
{
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(3);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    double sure = pulseIn(EchoPin, HIGH);

    double mesafe = (sure / 2) * 0.0343;
    return mesafe;
}
