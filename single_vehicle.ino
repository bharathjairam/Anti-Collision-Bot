#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>



#include <Wire.h> 
#include <SPI.h>

#define gpsrx 6
#define gpstx 5

#define motor1p A0
#define motor1n A1

#define motor2p A3
#define motor2n A2

#define bluerx 2
#define bluetx 3


#define ultratrig 9
#define ultraecho 10





SoftwareSerial bluetoothserial(bluerx,bluetx);

SoftwareSerial gpsSerial(gpsrx, gpstx);

LiquidCrystal_I2C lcd(0x27,16,2); 

TinyGPSPlus gps;

void setup(){
  
  lcd.init();
 
  pinMode(motor1p,OUTPUT);
  pinMode(motor1n,OUTPUT);
  pinMode(motor2p,OUTPUT);
  pinMode(motor2n,OUTPUT);
  //upinMode(buzzer,OUTPUT);
  pinMode(ultratrig,OUTPUT);
  pinMode(ultraecho,INPUT);

  Serial.begin(9600);

  bluetoothserial.begin(9600);
   
  lcd.backlight();  
  lcd.setCursor(0,0);
  lcd.print(" PROJECT DONE BY ");
  lcd.setCursor(0,1);
  lcd.print("Sai and Team");
  delay(2000);
  lcd.clear();
  
  //gpsSerial.begin(9600);
   
  }

bool stop_moving_forward = false;

unsigned long previousmillis = 0;
void loop(){
  int sonardistance = get_sonar_distance();
  //Serial.println("The ultrasonic distance : "+String(sonardistance));
  bluetoothserial.listen();
  if (bluetoothserial.available()>0){
    char bluetoothcommand = bluetoothserial.read();
    Serial.println(bluetoothcommand);
    
    switch (bluetoothcommand){
      case 'F':{
        if(sonardistance>18){
          navigate_forward();
          stop_moving_forward = true;
        }
        break;
      }
      case 'B':{
        navigate_backward();
        break;
      }
      case 'L':{
        navigate_left();
        break;
      }
      case 'R':{
        navigate_right();
        break;
      }
      case 'S':{
        stop_navigation();
        break;
      }
    }
  }
  sonardistance = get_sonar_distance();
  if((sonardistance<18)&&(stop_moving_forward)){
    stop_navigation();
    stop_moving_forward = false;
  }
    delay(100);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LOC : "+String(get_location()));
    lcd.setCursor(0,1);
    lcd.print("DIST : "+String(sonardistance)+" CM");
  
}


String previouslat = "12.97";
String previouslng = "77.59";

String get_location(){
  gpsSerial.listen();
  while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){{
      if (gps.location.isValid()){
          previouslat=String(gps.location.lat(),5);
          previouslng=String(gps.location.lng(),5);
        }
    }
    }
  }
  return previouslat+","+previouslng;
}


int get_sonar_distance(){
  digitalWrite(ultratrig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(ultratrig, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(ultratrig, LOW);
  
  long duration = pulseIn(ultraecho, HIGH);
  
  int distance= duration*0.034/2;

  return distance;
  
}

void navigate_forward(){
    digitalWrite(motor1p, LOW);
    digitalWrite(motor1n, HIGH);
    digitalWrite(motor2p, LOW);
    digitalWrite(motor2n, HIGH);
}


void navigate_backward(){
  digitalWrite(motor1p, HIGH);
  digitalWrite(motor1n, LOW);
  digitalWrite(motor2p, HIGH);
  digitalWrite(motor2n, LOW);
}

void navigate_left(){
  digitalWrite(motor1p, LOW);
  digitalWrite(motor1n, HIGH);
  digitalWrite(motor2p, HIGH);
  digitalWrite(motor2n, LOW);
}


void navigate_right(){
  digitalWrite(motor1p, HIGH);
  digitalWrite(motor1n, LOW);
  digitalWrite(motor2p, LOW);
  digitalWrite(motor2n, HIGH);
}

void stop_navigation(){
  digitalWrite(motor1p, LOW);
  digitalWrite(motor1n, LOW);
  digitalWrite(motor2p, LOW);
  digitalWrite(motor2n, LOW);
}
