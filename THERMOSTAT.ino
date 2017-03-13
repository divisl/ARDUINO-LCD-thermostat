/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 13.03.2017 19:52:36
 Project     : Room thermostat
 Libraries   : xxx
 Author      : Libor Divis
 Description : Room thermostat
******************************************************************/

#include <OneWire.h>                  // OneWire knihovna
#include <DallasTemperature.h>        // Dallas teplotni senzory knihovna
#include <LiquidCrystal_I2C.h>        // I2C LCD knihovna
#include "EEPROM.h"                   // eeprom knihovna

unsigned long perioda;
float Td;                             // rozhodovaci teplota ovladana tlacitky tlacN and tlacD potom budu cist z eeprom
float Th = 1.0;                       // hystereze asi nepouzijzu
float Tc;                             // aktualni teplota
int tlacD = 7;                        // tlacitko Dolu
int tlacN = 8;                        // tlacitko Nahoru
int RELE = 5;   
int GLED = 3;                         // ZELENA LED
int RLED = 4;                         // CERVENA LED 
float memTd;


//ctu teplotu
#define ONE_WIRE_BUS 11               // Teplotni senzor DS18B20 pripojen k pinu 11
OneWire oneWire(ONE_WIRE_BUS);        

DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x3F, 20, 4);   // nastavení adresy (0x3F) pro LCD I2C 4 radky 20 znaku na radek

void setup(void){ 

Serial.begin(9600);                   // startuji serovy port, hodi se na vypisy z programu
  
lcd.init();                           // inicializace lcd
  lcd.backlight();                    // zapinam podsviceni
  lcd.clear();                        // mazu LCD - start s cistou obrazovkou 
  lcd.home ();                        // frajerinka pri startu pisu na LCD         
  lcd.setCursor(1, 0);                // tady je prvni radek, prvni znak lcd
  lcd.print("---INICIALIZACE---");
  lcd.setCursor(5, 1);
  lcd.print("TERMOSTATU");
  lcd.setCursor(5, 2);
  lcd.print("* v. 0.9 *");
  delay(1000);
  lcd.clear();
  
  // Start up the library
  sensors.begin();
  sensors.setWaitForConversion(false);
  pinMode(RELE, OUTPUT); 
  pinMode(GLED, OUTPUT);                // ZELENA LED
  pinMode(RLED, OUTPUT);                // CERVENA LED  
  perioda = millis();
  
  
  }


void loop(void)
{
  
int tlacitko;
  if((millis()-perioda)>800){
       Tc = sensors.getTempCByIndex(0); 
       sensors.requestTemperatures();     //vrat teplotu ze senzoru kazdych 800 ms
       perioda = millis();
                            }
  
                
  //Serial.println(Tc);                   //tady si zkusebne vypisuju na ser. konzoli
  
  lcd.setCursor(0, 0);                    //tady je prvni radek, prvni znak lcd
  lcd.print("T  akt:");
  lcd.setCursor(11, 0);
  lcd.print(Tc,1);
  lcd.setCursor(17, 0);
  lcd.print((char)223);
  lcd.setCursor(18, 0);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("T nast:");
  lcd.setCursor(11, 1);
  lcd.print(Td);
  lcd.setCursor(17, 1);
  lcd.print((char)223);
  lcd.setCursor(18, 1);
  lcd.print("C");
  lcd.setCursor(0, 3);
  lcd.print("topeni:");
 
    
  int stisk = 0;
  tlacitko=digitalRead(tlacD); 
  if(tlacitko==0) {       
       Td = Td - 0.5;                       //krok dolu pocet stupnu
       stisk = 1;
                     }
  tlacitko=digitalRead(tlacN);
  if(tlacitko==0) {
       Td = Td + 0.5;                       // krok nahoru pocet stupnu
       stisk = 1;  
                     }
  if (stisk) {                             //lip se nastavuje hodnota
  delay(200);    
           }
           
 lcd.setCursor(11, 3);  
 if(Tc < Td){                             // topim
      digitalWrite(GLED, LOW);
      digitalWrite(RLED, HIGH);
      digitalWrite(RELE, LOW);
      lcd.print("TOPI  ");
             }
 
             if(Tc > Td) {                // netopim
      digitalWrite(GLED, HIGH);
      digitalWrite(RLED, LOW);
      digitalWrite(RELE, HIGH);
      lcd.print("NETOPI");
              } 
              


}


