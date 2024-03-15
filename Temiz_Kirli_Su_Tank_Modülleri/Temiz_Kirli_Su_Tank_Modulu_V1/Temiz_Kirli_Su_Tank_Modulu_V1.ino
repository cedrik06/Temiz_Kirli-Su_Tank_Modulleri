#include <LiquidCrystal_I2C.h>
#include<Wire.h>


// mesafe sensörü analog deger 260 ölçünce full dolu  %100
// kirli tank 725-260 arası okusun onun dışında okuma yapmasın

//Temiz su tank 180 de %0 320 de %100 olması gerekiyor

//175 ile 310 analog değerleri arasında motor duracak. Bu  degerler ilk start için geçerli
//Motor çalışma sonrası 1 bar ile 2 bar arasında yani 240 ile 310 arasında çalış dur şeklinde devam edicek.

//KİRLİ SU TANKI %100 Ü GÖRÜNCE DE MOTOR STOP OLACAK.


#define motor 6


LiquidCrystal_I2C lcd( 0x27, 16 , 2);


float analog_pressure_kirli;
float analog_pressure_temiz;
float analog_pressure_3;


int min_temiz_tank = 180;
int max_kirli_tank = 260;

int role_1 =2;
int role_2 =3;

float yuzde_deger_kirli;
float yuzde_deger_temiz;
float yuzde_deger_3;

void lcd_ekran()
{

          lcd.setCursor(0,0);
          lcd.print("TemizTank=%");
          lcd.setCursor(12,0);
          lcd.print(yuzde_deger_temiz);
          lcd.setCursor(0,1);
          lcd.print("KirliTank=%");
          lcd.setCursor(12,1);
          lcd.print(yuzde_deger_kirli);

}
void motor_on()
{
    analogWrite(motor,255);
    Serial.println("Motor Aktif");
}

void motor_off()
{
    analogWrite(motor,0);
    Serial.println("Motor Kapalı");
}


void setup() {
    Serial.begin(9600);

    pinMode(motor, OUTPUT);
    pinMode(role_1,OUTPUT);
    pinMode(role_2,OUTPUT);
    
    lcd.begin();

    digitalWrite(role_1,HIGH);
    digitalWrite(role_2,LOW); 

    analog_pressure_3 = analogRead(A2);
    if(175< analog_pressure_3<310)
          motor_on();
          delay(3000);
          motor_off();

}

void loop() {
 
analog_pressure_kirli = analogRead(A0);
analog_pressure_temiz = analogRead(A1);
if(analog_pressure_kirli> max_kirli_tank && analog_pressure_temiz > min_temiz_tank)
{

    Serial.println("Temiz Su Tankı Dolu, Kirli Su Tank Boş");
    yuzde_deger_kirli = map(analog_pressure_kirli,725,260,0,100);
    yuzde_deger_temiz = map(analog_pressure_temiz,180,320,0,100);
    lcd_ekran();

    Serial.print("Kirli Tank Analog  : ");
    Serial.println(analog_pressure_kirli);
    Serial.print("Kirli Tank Yüzde  : ");
    Serial.println(yuzde_deger_kirli);

    Serial.print("Temiz Tank Analog  : ");
    Serial.println(analog_pressure_temiz);
    Serial.print("Temiz Tank Yüzde  : ");
    Serial.println(yuzde_deger_temiz);
    Serial.println("        ");


    Serial.println("Motor Çıkış basıncı kontrol ediliyor");

    analog_pressure_3 = analogRead(A2);
    if(240<analog_pressure_3<310)
    {     
          Serial.print("Motor Çıkış Basıncı: ");
          Serial.println(analog_pressure_3);
          Serial.println("Motor Su basıncı Yetersiz");
          motor_on();
          delay(1000);
    }
    else if(analog_pressure_3>310)
    {
        Serial.print("Motor Çıkış Basıncı: ");
        Serial.println(analog_pressure_3);
        Serial.println("Motor Çıkışı basınç Yeterli, ");
        motor_off();
        delay(1000);
    }
}

else{
      Serial.println("Motor Kapatılıyor");
      motor_off();
      Serial.println("Hangi Tank Arızalı Kontrol Ediliyor");

      analog_pressure_temiz = analogRead(A1);
      if(analog_pressure_temiz<180 || analog_pressure_temiz >320)
      {     
            Serial.println("Temiz Su Tank Arızalı");
            lcd.setCursor(0,0);
            lcd.print("TemizTankBos");
            delay(1000);
            lcd.print("             ");


      }
      else
      {
          Serial.println("Temiz Su Tankı Sağlam");
  
          yuzde_deger_temiz = map(analog_pressure_temiz,180,320,0,100);
          lcd.setCursor(0,0);
          lcd.print("TemizTank=%");
          lcd.setCursor(12,0);
          lcd.print(yuzde_deger_temiz);
      }
      analog_pressure_kirli = analogRead(A0);  
      if(analog_pressure_kirli <260 ||analog_pressure_kirli >720)
        {
            Serial.println("Kirli Su Tankı Arızalı");
            lcd.setCursor(0,1);
            lcd.print("KirliTankDolu");
            delay(1000);
            lcd.print("              ");
        }
      else
        {
          
          Serial.println("Kirli Su Tankı Sağlam");
          yuzde_deger_kirli = map(analog_pressure_kirli,725,260,0,100);
          lcd.setCursor(0,1);
          lcd.print("KirliTank=%");
          lcd.setCursor(12,1);
          lcd.print(yuzde_deger_kirli);
          }

        }
      
}

