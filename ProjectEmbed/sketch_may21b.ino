#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
//set the address, number of column and number of rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Data wire is conntec to the Arduino digital pin A0
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
//uint16_t is unsigned 16-bit integer
uint16_t targetSuhu;
int button1, button2, button3, jam, menit;
unsigned long durasi, previousMillis;
char buffer[33];
float tempC;

void setup() {
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  pinMode(2,INPUT_PULLUP); //button 1 set,set
  pinMode(4,INPUT_PULLUP); //button 2 - , menit
  pinMode(6,INPUT_PULLUP); //button 3 + , jam
  pinMode(11, OUTPUT); //led hijau, waktu
  pinMode(10, OUTPUT); //led merah, suhu
  pinMode(13, OUTPUT); //relay
  
  sensors.begin();
  //inisiasi lcd
  lcd.begin();
  //awalnya layar hitam
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("PROJEK EMBED");
  lcd.setCursor(0,1);
  lcd.print("KELOMPOK 4");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STERILISASI ALAT");
  lcd.setCursor(0,1);
  lcd.print("DAN SOUS VIDE");
  delay(3000);
  lcd.clear();
  
  while(1){   
    lcd.setCursor(0,0);
    lcd.print("Pilih mode");
    //Store the result of a string formatted with sprintf
    //3 : minimum number of digits to be written
    //d : Signed decimal integer
    lcd.setCursor(0,1);
    lcd.print("Man - Ste - Sou");
    if(digitalRead(6)==0){
      while(digitalRead(6)==0);
        lcd.clear();
        souvide();
        break;
    }
    else if (digitalRead(4)==0){
      while(digitalRead(4)==0);
        lcd.clear();
        steril();
        break;
    }
    else if (digitalRead(2)==0){
      while(digitalRead(2)==0);
        lcd.clear();
        manual();
        break;
    }
  }
}

void souvide(){
  targetSuhu = 60;
  durasi = 7200000;
  lcd.setCursor(0,0);
  lcd.prin+ t("Mode Souvide");
  delay(3000);
  lcd.clear();
  loop();`
}

void steril(){
  targetSuhu = 70;
  durasi = 300000;
  lcd.setCursor(0,0);
  lcd.print("Mode Steril");
  delay(3000);
  lcd.clear();
  loop();
}

void manual(){
  while(1){   
    lcd.setCursor(0,0);
    lcd.print("Input Data");
    //Store the result of a string formatted with sprintf
    //3 : minimum number of digits to be written
    //d : Signed decimal integer
    sprintf(buffer, "Input suhu : %3d", targetSuhu);
    lcd.setCursor(0,1);
    lcd.print(buffer);
    if(digitalRead(6)==0){
      while(digitalRead(6)==0);
        targetSuhu += 5;
    }
    else if (digitalRead(4)==0){
      while(digitalRead(4)==0);
        targetSuhu -= 5;
    }
    else if (digitalRead(2)==0){
      while(digitalRead(2)==0);
        lcd.clear();
        break;
    }
  }

  while(1){
    lcd.setCursor(0,0);
    lcd.print("Input Waktu");
    //Store the result of a string formatted with sprintf
    //2 : minimum number of digits to be written
    //d : Signed decimal integer
    sprintf(buffer, "%2d Jam, %2d Menit", jam, menit);
    lcd.setCursor(0,1);
    lcd.print(buffer);
    if(digitalRead(6)==0){
      while(digitalRead(6)==0);
        jam += 1;
      if (jam>=6){
        jam = 0;
      }
    }
    else if (digitalRead(4)==0){
      while(digitalRead(4)==0);
        menit += 1;
      if (menit>=59){
        menit = 0;
      }
    }
    else if (digitalRead(2)==0){
      while(digitalRead(2)==0);
      lcd.clear();
      break;
    }
  }
  durasi = (jam*3600000) + (menit*60000);
}


void loop() {
  digitalWrite(10,LOW);
  digitalWrite(11,HIGH);
  // Call sensors.requestTemperatures()
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));
  tempC = sensors.getTempCByIndex(0);
  
  lcd.setCursor(0,0);
  lcd.print("Suhu = ");
  if(tempC < targetSuhu){
    digitalWrite(13,HIGH);
    digitalWrite(10,HIGH);
    delay(1000);
    digitalWrite(10,LOW);
    lcd.print(tempC);
  }
  else{
    digitalWrite(11,HIGH);
    digitalWrite(13,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PROSES BERJALAN ");  
    Serial.println(millis());
    previousMillis = millis();
    digitalWrite(11,HIGH);
    digitalWrite(13,LOW);
    lcd.clear();
    while(millis()-previousMillis <= durasi){
    lcd.setCursor(0,0);
    lcd.print("PROSES BERJALAN ");  
    sensors.requestTemperatures();  
    if(sensors.getTempCByIndex(0) <= targetSuhu){
      Serial.println(sensors.getTempCByIndex(0));
      digitalWrite(13,HIGH);
    } else if(sensors.getTempCByIndex(0) > targetSuhu){
      Serial.println(sensors.getTempCByIndex(0));
      digitalWrite(13,LOW);  
    }
    }
  lcd.clear(); 
  while(1){
    digitalWrite(10,HIGH);
    digitalWrite(11,LOW);
    digitalWrite(13,LOW);
    lcd.setCursor(0,1);
    lcd.print("SELESAI PROSES");
  }
  }
}