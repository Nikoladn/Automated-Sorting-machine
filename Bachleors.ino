#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include<Servo.h>
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 8

Servo Myservo;
int pos;
int motorinit;

/* Инициализиране на сензора със специфично време и стойности */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup()
{
  //Задаване режима на пиновете
  Myservo.attach(9);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //Проверка за сензора
  Serial.begin(9600);
  if (tcs.begin()) 
    {
    Serial.println("Found sensor");
    } else 
      {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
        delay(100);
      }
}

//Иницилизация на алгоритама
void initialization()
{
   // Задаваме посока на въртене
   digitalWrite(dirPin, HIGH);
   // Стъпковият мотор прави 3 стъпки
   for (int motorinit = 0; motorinit < 3; motorinit++) 
   {
     digitalWrite(stepPin, HIGH);
     delayMicroseconds(2000);
     digitalWrite(stepPin, LOW);
     delayMicroseconds(2000);
   }
}

void loop()
{
   //Снемаме стойностите на сензора
   uint16_t r, g, b, c, colorTemp, lux;
   tcs.getRawData(&r, &g, &b, &c);
   colorTemp = tcs.calculateColorTemperature(r, g, b);
   lux = tcs.calculateLux(r, g, b);
   //Следващата част от кода е за оператор да може да провери дали сезора работи правилно
   Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
   Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
   Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
   Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
   Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
   Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
   Serial.println(" ");
   delay(1000);

   //Мотора прави 1 стъпка
   digitalWrite(dirPin, HIGH);
   digitalWrite(stepPin, HIGH);
   delayMicroseconds(2000);
   digitalWrite(stepPin, LOW);
   delayMicroseconds(2000);
   delay(1000);

   //В зависимост от стойностите снимани от сензора завъртаме сервото
   if (r > g && r > b) 
   {
      pos=40;
      Myservo.write(pos);
   }
   else if (g > r && g > b) 
   {
      pos=80;
      Myservo.write(pos);
   }
   else if (b > g && b > r)
   {
      pos=120;
      Myservo.write(pos); 
   }
   delay(1000);
}
