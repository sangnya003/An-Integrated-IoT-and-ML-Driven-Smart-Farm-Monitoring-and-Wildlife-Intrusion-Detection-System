/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to use ESP8266 Shield (with AT commands)
  to connect your project to Blynk.

  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#include <SoftwareSerial.h>
#include <DHT.h>
#include <LiquidCrystal595.h>    // include the library
LiquidCrystal595 lcd(6, 5, 4); 

static const int RXPin = 2, TXPin = 3;
SoftwareSerial Blue (RXPin, TXPin);

#define DHTPIN  14
#define DHTTYPE DHT11         // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int soil = 7;
int Pump = 8;
int Buz  = 9;

String D1; // sensor1 data
String TD2; // sensor2 data
String HD3; // sensor3 data

String readString;
String cdata;

//*************************************************************************************
void setup()
 {
  Serial.begin(9600);
  Blue.begin(9600);
  dht.begin();
  lcd.begin(16, 2);               // 16 characters, 2 rows
  lcd.setLED2Pin(HIGH);
  lcd.clear();
  lcd.home ();
  pinMode(Pump,OUTPUT);  digitalWrite(Pump,HIGH);
  pinMode(Buz,OUTPUT); digitalWrite(Buz,HIGH);
  pinMode(soil,INPUT);
  
  lcd.setCursor(0,0); lcd.print("Animal detection  ");
  lcd.setCursor(0,1); lcd.print("    using ML.   ");
  delay(3000);  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Animal:");
 }
 
//*************************************************************************************
void loop()
 {
  while(Blue.available()) 
     {
      delay(50);
      char c = Blue.read();
      readString += c;
      lcd.setCursor(0,1); lcd.print("                ");
     }
  Check_ML(); T_DATA();
  cdata = cdata + D1 + "," + TD2 + "," + HD3 + ",";  // comma will be used a delimeter
  Serial.println(cdata); 
  cdata = "";
  delay(1000);
 }
//*************************************************************************************
void Check_ML()
 {  
  if(readString.length() > 0) 
    {
     if(readString == "D1") 
       { 
        D1 = "Elephant";
        lcd.setCursor(0,1); lcd.print("Elephant");
        for(int t=20; t>0; t--) { blinkLed(50,100,Buz); }
        delay(5000);
        lcd.setCursor(0,1); lcd.print("                ");
        readString = "";
       }
//----------------------------------------------------------------
     else if(readString == "D2") 
       { 
        D1 = "Monkey";
        lcd.setCursor(0,1); lcd.print("Monkey");
        for(int t=20; t>0; t--) { blinkLed(50,100,Buz); }
        delay(5000);
        lcd.setCursor(0,1); lcd.print("                ");
        readString = "";
       } 
//----------------------------------------------------------------
     else if(readString == "D3") 
       { 
        D1 = "Boar";
        lcd.setCursor(0,1); lcd.print("Boar");
        digitalWrite(Buz,LOW); delay(5000);
        digitalWrite(Buz,HIGH);    
        readString = "";
       }
   }
//-----------------------------------------------------------------
  else 
   {
    D1 = "NA";
   }
 }

//*******************************************************************************  
void T_DATA()  
 {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  TD2 = t;
  HD3 = h;
   if(isnan(h) || isnan(t)) 
    {
     lcd.setCursor(0,1);           
     lcd.print("Failed sensor...");
     return;
    }

   else
    { 
     lcd.setCursor(0,1);
     lcd.print(String("T:") + t + "'C    ");
     lcd.setCursor(10, 1);
     lcd.print(String("H:") + h + "% ");
    }
//.........................................................
  int val = digitalRead(soil);
  if(val == 0)  {  digitalWrite(Pump,HIGH);  }
  else  {  digitalWrite(Pump,LOW);  }
 }

//*******************************************************************************
//*******************************************************************************
 void blinkLed(int onTime, int offTime, int pin) 
  {
   digitalWrite(pin, LOW);   // turn the LED on (HIGH is the voltage level)
   delay(onTime);              // wait for a second
   digitalWrite(pin, HIGH);    // turn the LED off by making the voltage LOW
   delay(offTime);              // wait for a second
 }