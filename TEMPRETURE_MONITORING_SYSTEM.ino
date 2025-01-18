#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int tempPin = A0;

float temperatureC = 0.0;
float voltage = 0.0;
int sensorValue = 0;


LiquidCrystal_I2C lcd(0x27, 16, 2);

const float upperThreshold = 30.0;
const float lowerThreshold = 10.0;

bool temperatureWarning = false;

void setup() {
  
  Serial.begin(9600);

  
  lcd.begin(20,4);
  lcd.backlight(); 

  displayCustomMessage("Temp Monitoring\nSystem Initializing");
  delay(2000); 
  lcd.clear();

  
}

void loop() {

  sensorValue = analogRead(tempPin);
  voltage = sensorValue * (5.0 / 1023.0);
  temperatureC = voltage * 100.0; 
 
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" - Voltage: ");
  Serial.print(voltage);
  Serial.print("V - Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" C");

  
  checkTemperatureAlert(temperatureC);

  updateLCD("Temp: " + String(temperatureC) + " C", 0);

  if (temperatureWarning) {
    updateLCD("ALERT! Temp Issue", 1);
  } else {
    updateLCD("System Normal", 1);
  }

  delay(1000);
}


void checkTemperatureAlert(float currentTemp) {
  
  if (currentTemp > upperThreshold) {
    temperatureWarning = true;
    Serial.println("Warning: Temperature is too high!");
  }
  
  else if (currentTemp < lowerThreshold) {
    temperatureWarning = true;
    Serial.println("Warning: Temperature is too low!");
  }
  
  else {
    temperatureWarning = false;
  }
}


void initializeSensor() {
  Serial.println("Initializing Temperature Sensor...");
  updateLCD("Sensor Initializing", 1);
  delay(1000); 
  Serial.println("Sensor Initialized.");
  lcd.clear();
}


void updateLCD(String message, int line) {
  
  lcd.setCursor(0, line);
  lcd.print("                "); 
  lcd.setCursor(0, line);
  lcd.print(message); 
}

void displayCustomMessage(String message) {
  lcd.clear();
  int line = 0;
  int pos = 0;
  while (pos < message.length()) {
    int nextPos = message.indexOf('\n', pos);
    if (nextPos == -1) nextPos = message.length();
    lcd.setCursor(0, line);
    lcd.print(message.substring(pos, nextPos));
    pos = nextPos + 1;
    line++;
  }
  delay(2000); 
}