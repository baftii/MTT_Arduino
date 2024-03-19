#include <stdio.h>
#include <stdbool.h>

#define MOT1_EN 11 // PWM
#define MOT1_IN1 12
#define MOT1_IN2 8

#define MOT2_IN1 7
#define MOT2_IN2 4
#define MOT2_EN 3 // PWM

#define MOT1_LED 10
#define MOT2_LED 9

// Motoru test etmek için 2 tane led, motor sürücü ve 2 motor lazım
// yukarıdaki pinleri ayarladıktan sonra, motorlar sırasıyla ileri, geri, yerindesag, yerinde sol, sag,
// sol hareket yapacaktır. Hangi motorların çalıştığını da takip edebilmeniz için ledleri ekledim.

void setup()
{
    Serial.begin(9600);
    pinMode(MOT1_IN1, OUTPUT);
    pinMode(MOT1_IN2, OUTPUT);
    pinMode(MOT2_IN1, OUTPUT);
    pinMode(MOT2_IN2, OUTPUT);

    pinMode(MOT1_LED, OUTPUT);
    pinMode(MOT2_LED, OUTPUT);

    digitalWrite(MOT1_IN1, LOW);
    digitalWrite(MOT1_IN2, LOW);
    digitalWrite(MOT2_IN1, LOW);
    digitalWrite(MOT2_IN2, LOW);

    digitalWrite(MOT1_LED, LOW);
    digitalWrite(MOT2_LED, LOW);

    analogWrite(MOT1_EN, 0);
    analogWrite(MOT2_EN, 0);

    Serial.println("Motor Başlatildi.");
}

void loop()
{
ileri(255);
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

void YerindeSagHareket(int hiz)
{
    sagMotorGeri(hiz);
    solMotorIleri(hiz);
}

void YerindeSolHareket(int hiz)
{
    sagMotorIleri(hiz);
    solMotorGeri(hiz);
}

void SolHareket(int hiz)
{
    sagMotorIleri(hiz);
    solMotorGeri(0);
}

void SagHareket(int hiz)
{
    solMotorIleri(hiz);
    sagMotorGeri(0);
}

void MotorDurdur(void)
{
    sagMotorIleri(0);
    solMotorIleri(0);
}

void sagMotorIleri(int hiz)
{
    analogWrite(MOT2_EN, hiz);
    digitalWrite(MOT2_IN1, LOW);
    digitalWrite(MOT2_IN2, HIGH);
}

void sagMotorGeri(int hiz)
{
    analogWrite(MOT2_EN, hiz);
    digitalWrite(MOT2_IN1, HIGH);
    digitalWrite(MOT2_IN2, LOW);
}

void solMotorIleri(int hiz)
{
    analogWrite(MOT1_EN, hiz);
    digitalWrite(MOT1_IN1, LOW);
    digitalWrite(MOT1_IN2, HIGH);
}

void solMotorGeri(int hiz)
{
    analogWrite(MOT1_EN, hiz);
    digitalWrite(MOT1_IN1, HIGH);
    digitalWrite(MOT1_IN2, LOW);
}
