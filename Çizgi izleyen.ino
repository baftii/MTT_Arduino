#define SagMotorHiz 1
#define SolMotorHiz 2
#define SagMotor1 3
#define SolMotor1 4
#define SagMotor2 5
#define SolMotor2 6

#define SensorSize 8

int minKal[SensorSize], maxKal[SensorSize], esik[SensorSize];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

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

void kalibrasyon(void){
  int checkSol = 0, checkSag = 1, firsttime = 1, time;
  for(int i = 0; i < 8; i++)
  {
    minKal[i] = analogRead(i);
    maxKal[i] = analogRead(i);
  }

  for(int i = 0; i < 3000; i++){
    if(i == 1){
      time = millis();
    }

    if(firsttime == 1){
      SolHareket(255);
      maxminKalbComp(&maxKal[0], &minKal[0], 8);
      if(millis() > (time + 500)){
        firsttime = 0;
        time = millis();
      }
    }

    else{
      if(checkSol == 1){
        SolKalb(255, &checkSol, &checkSag, &time);
      }

      if(checkSag == 1){
        SagKalb(255, &checkSol, &checkSag, &time);
      }
    }
  }

  for(int i = 0; i < 8; i++){
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

static void SolKalb(int hiz, int *SolCheck, int *SagCheck, int *sure){
  SolHareket(hiz);
  maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
  if(millis() > (sure + 1000)){
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

static void SagKalb(int hiz, int *SolCheck, int *SagCheck, int *sure){
  SagHareket(hiz);
  maxminKalbComp(&maxKal[0], &minKal[0], SensorSize);
  if(millis() > (sure + 1000)){
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

static void maxminKalbComp(int *max, int *min, int size){
  for(int j = 0; j < size; j++){
    int temp = analogRead(j);

    if(*(min + j) > temp){
      *(min + j) = temp;
    }

    if(*(max + j) < temp){
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

void SolHareket(int hiz){
  analogWrite(SagMotorHiz, hiz);
  digitalWrite(SagMotor1, HIGH);
  digitalWrite(SagMotor2, LOW);
  analogWrite(SolMotorHiz, hiz);
  digitalWrite(SolMotor1, LOW);
  digitalWrite(SolMotor2, HIGH);
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

void SagHareket(int hiz){
  analogWrite(SagMotorHiz, hiz);
  digitalWrite(SagMotor1, LOW);
  digitalWrite(SagMotor2, HIGH);
  analogWrite(SolMotorHiz, hiz);
  digitalWrite(SolMotor1, HIGH);
  digitalWrite(SolMotor2, LOW);
}
