#define MOT1_IN1 1
#define MOT1_IN2 2
#define MOT1_EN 3 // PWM
#define MOT2_IN1 4
#define MOT2_IN2 5
#define MOT2_EN 6 // PWM

#define TrigPin 7
#define EchoPin 8

// Motor sürekli ileriye çalışacak. Eğer 20 santimetreye düşerse uzaklık motor duracak.
// 20 cm olan mesafeyi alttaki uzakliksinir degiskeninden değiştirebilirsiniz.

int uzakliksinir = 20;
int mesafe20counter = 0;

void setup()
{
    Serial.begin(9600);

    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);

    pinMode(MOT1_IN1, OUTPUT);
    pinMode(MOT1_IN2, OUTPUT);
    pinMode(MOT2_IN1, OUTPUT);
    pinMode(MOT2_IN2, OUTPUT);

    digitalWrite(MOT1_IN1, LOW);
    digitalWrite(MOT1_IN2, LOW);
    digitalWrite(MOT2_IN1, LOW);
    digitalWrite(MOT2_IN2, LOW);

    analogWrite(MOT1_EN, 0);
    analogWrite(MOT2_EN, 0);

    Serial.println("Motor Başlatildi.");
}

void loop()
{
    double mesafe = mesafeOlc();
    if (mesafe > uzakliksinir)
    {
        Serial.print("Mesafe: ");
        Serial.println(mesafe);
        mesafe20counter = 0;
        ileri(255);
    }

    else if(mesafe < uzakliksinir)
    {
        Serial.print("Mesafe: ");
        Serial.println(mesafe);
        mesafe20counter++;
        if (mesafe20counter > 5)
        {
            MotorDurdur();
            Serial.println("");
            Serial.println("Motor Durduruldu");
            Serial.println("");
        }
    }

    delay(20);
}

void ileri(int hiz)
{
    sagMotorIleri(hiz);
    solMotorIleri(hiz);
}

void sagMotorIleri(int hiz)
{
    analogWrite(MOT2_EN, hiz);
    digitalWrite(MOT2_IN1, LOW);
    digitalWrite(MOT2_IN2, HIGH);
}

void solMotorIleri(int hiz)
{
    analogWrite(MOT1_EN, hiz);
    digitalWrite(MOT1_IN1, LOW);
    digitalWrite(MOT1_IN2, HIGH);
}

void MotorDurdur(void)
{
    sagMotorIleri(0);
    solMotorIleri(0);
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
