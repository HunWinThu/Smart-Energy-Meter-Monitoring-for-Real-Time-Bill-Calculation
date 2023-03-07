#include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20,4);//3F
#include <WiFi.h>
#include "ThingSpeak.h"
#include <EEPROM.h>
float vpeak,ipeak,voltage,current,energy,bill,Bill,Ft,service,tax;

const char* ssid = "2GST4-02";   // your network SSID (name) 
const char* password = "cPeKx37f";   // your network password
int i=1;
boolean measured=0;
WiFiClient  client;

unsigned long myChannelNumber = 1960050;  
const char * myWriteAPIKey = "WW6ZNO1L7RVLX933";

//30 second timer
unsigned long lastTimeIOTs = 0;
unsigned long timerDelayIOTs = 30000;

//10 second timer
unsigned long lastTimeMeter = 0;
unsigned long timerDelayMeter = 10000;

int adc,maxadc,iadc,imaxadc;

float storeunit;

void setup() {
  EEPROM.begin(32);delay(5000);// GG
 // EEPROM.put(0,0.0f);EEPROM.commit();
  lcd.init();
  lcd.clear(); 
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("I0T Smart Energy");
  lcd.setCursor(7,1);
  lcd.print("Meter");
  lcd.setCursor(0,2);
  lcd.print("Hun Win Thu-st121773");
  lcd.setCursor(3,3);
  lcd.print("TC&IoT,SET,AIT");
  delay(5000);
  lcd.clear();  
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
      
}

void loop() {

if ((millis() - lastTimeMeter) > timerDelayMeter) {

  lcd.setCursor(12,1); lcd.print("VIEB");
  for (int i=0;i<300;i++)
  {adc=analogRead(39);
  if (adc>maxadc)maxadc=adc;
  //delay(2);
  }
  //maxadc=2371;//for real time, delete this line
  //220Vrms==>2371adc or (2371-2047=324) adc peak   
  vpeak=((maxadc-1905)*220/258.8);//GG
  voltage =   vpeak*0.7071;
  if (voltage<0) voltage=0;
  maxadc=0;
  lcd.setCursor(0,0);lcd.print("V=");lcd.print(voltage,2);lcd.print("V    ");
    for (int i=0;i<300;i++)
  {iadc= analogRead(36);
  if (iadc>imaxadc)imaxadc=iadc;  
  //delay(2);
  }
  
  //imaxadc=2071;//for real time, delete this line
   //10Arms==>2071adc or (2071-2047)=24
  ipeak=((imaxadc-1905)*1.81818/240.9);//GG
  current =   ipeak*0.7071;
  if (current<0) current=0;
  imaxadc=0;
  lcd.setCursor(0,1);lcd.print("I=");lcd.print(current,2);lcd.print("A    ");

  float power=(voltage*current);
  float kwpower=power/1000.0;
  Serial.print("kwpower Unit=");Serial.print(kwpower,4);Serial.println("kW/10s");
  float inceasing=kwpower/360.0; //GG

  storeunit=EEPROM.get(0,storeunit);
  Serial.print("Store Unit=");Serial.println(storeunit,4);
  energy=storeunit+inceasing;
  Serial.print("Increasing Unit=");Serial.println(inceasing,4);
  Serial.print("New Unit=");Serial.println(energy,4);

  EEPROM.put(0,energy);  EEPROM.commit();
  Serial.println("#####");
  lcd.setCursor(0,2);lcd.print("Unit=");lcd.print(energy,4);lcd.print("kWh  ");
  

  //bill=energy*100;
   if (energy<1) bill =0;
   else if (energy>1&&energy<=15) bill=energy*2.3488;

   else if ( energy>15&&energy<=25) bill=35.232+ (energy-15)*2.9882;

   else if (energy>25&&energy<=35) bill=35.232+29.882+(energy-25)*3.2405;

   else if (energy>35&&energy<=100) bill=35.232+29.882+ 32.405+(energy-35)*3.6237;

   else if (energy>100&&energy<=150) bill=35.232+29.882+ 32.405 +235.5405+(energy-100)*3.7171;

   else if (energy>150&&energy<=400) bill=35.232+29.882+ 32.405 +235.5405+ 185.855+(energy-150)*4.2218;
   
   else //if (energy>401) 
   bill=35.232+29.882+ 32.405 +235.5405+ 185.855+1055.45+(energy-400)*4.4217;
   
   service = bill + 8.19;
   Ft = energy*0.1243;
   tax= (service-Ft)*7/100;
   Bill = (service+tax-Ft);
   lcd.setCursor(0,3);lcd.print("Bill=");lcd.print(Bill,2);lcd.print("baht  ");

  measured=1;
  lastTimeMeter = millis();
  }






  if ((millis() - lastTimeIOTs) > timerDelayIOTs) {
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      lcd.setCursor(16,0); lcd.print("....");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
       lcd.setCursor(16,0); lcd.print("COND");
    }
    int x =0;
    if (i==1&&measured==1)     x= ThingSpeak.writeField(myChannelNumber, 1, voltage, myWriteAPIKey);
    if (i==2&&measured==1)     x= ThingSpeak.writeField(myChannelNumber, 2, current, myWriteAPIKey);
    if (i==3&&measured==1)     x= ThingSpeak.writeField(myChannelNumber, 3, energy, myWriteAPIKey);
    if (i==4&&measured==1)     x= ThingSpeak.writeField(myChannelNumber, 4, Bill, myWriteAPIKey);
    i++;
    if (i>4) i=0;
    if(x == 200){  //GG
      Serial.println("Channel update successful.");
      lcd.setCursor(16,1); lcd.print("TxOK");
    }
    else{
      Serial.println("Waiting Data");

    }
    lastTimeIOTs = millis();
  }
}