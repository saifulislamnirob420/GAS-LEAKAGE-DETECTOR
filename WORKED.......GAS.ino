#define BLYNK_TEMPLATE_ID "TMPL6LJiZJDcK"
#define BLYNK_TEMPLATE_NAME "LPG DETECTOR"
#define BLYNK_AUTH_TOKEN "NX7HaBB9gRniGEGMr8FLvOk-ttu8NAri"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SAIFUL";  // type your wifi name
char pass[] = "nirob777xz";  // type your wifi password

BlynkTimer timer;

int GasSensor = A0;

#define GreenLED D4
#define RedLED D5
#define Buzzer D6

int sensorThreshold = 100;

void sendSensor()
{
   int gasValue = analogRead(GasSensor);
  Blynk.virtualWrite(V0, gasValue);
  Serial.print("Gas Value: ");
    Serial.println(gasValue);

    if(gasValue < 250) {
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      digitalWrite(Buzzer, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Gas Value: ");
      lcd.print(gasValue);
      lcd.setCursor(0, 1);
      lcd.print("   Normal");
      Serial.println("Gas Normal");
      delay(4000);
      lcd.clear();
    }
    else if(gasValue >= 250 && gasValue <= 500) {
      digitalWrite(GreenLED, LOW);
      digitalWrite(RedLED, LOW);
      digitalWrite(Buzzer, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Gas Value: ");
      lcd.print(gasValue);
      lcd.setCursor(0, 1);
      lcd.print("   Moderate");
      Serial.println("Gas Moderate");
      delay(4000);
      lcd.clear();
    }
    else if(gasValue > 500) {
      digitalWrite(GreenLED, LOW);
      digitalWrite(RedLED, HIGH);
      digitalWrite(Buzzer, HIGH);
      Blynk.logEvent("gas_leakage_alert", "EMERGENCY: Gas leak detected in your home gas line! Evacuate immediately. Do NOT turn on or off any electrical switches, use gas stoves, or ignite any flames. Turn off all gas valves, open windows to ventilate, and call emergency services or a gas professional for immediate assistance.");
      lcd.setCursor(0, 0);
      lcd.print("Gas Value: ");
      lcd.print(gasValue);
      lcd.setCursor(0, 1);
      lcd.print("HAZARDOUS !!!");
      Serial.println("Gas is High");
      delay(4000);
      lcd.clear();
    }
}

void setup()
{   
  Serial.begin(115200);
  pinMode(GasSensor, INPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Blynk.begin(auth, ssid, pass);
   
  timer.setInterval(1000L, sendSensor);

  lcd.begin();

  lcd.setCursor(0, 0);
  lcd.print("HOME GAS LEVEL");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  Blynk.run();
  timer.run();
}
