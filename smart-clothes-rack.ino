#include <Stepper.h>

const int stepsPerRevolution = 2048; 
Stepper myStepper(stepsPerRevolution, PA2, PA4, PA3, PA5); 
const int ldrPin = PA0;      
const int rainPin = PA1;      
const int rainThreshold = 3200; 
int rackState = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  myStepper.setSpeed(10); 
  Serial.println("ระบบราวตากผ้าอัตโนมัติพร้อมทำงาน...");
}

void loop() {
  int ldrValue = digitalRead(ldrPin); 
  int rainValue = analogRead(rainPin); 
  Serial.print("LDR (0=สว่าง, 1=มืด): ");
  Serial.print(ldrValue);
  Serial.print("\t Rain (มาก=แห้ง, น้อย=เปียก): ");
  Serial.println(rainValue);

  bool isBright = (ldrValue == LOW);        
  bool isDark = (ldrValue == HIGH);          
  bool isDry = (rainValue > rainThreshold);  
  bool isWet = (rainValue <= rainThreshold); 

  if (isBright && isDry) {
    if (rackState == 0) { 
      Serial.println("อากาศดี: กำลังกางราวตากผ้า...");
      myStepper.step(stepsPerRevolution * 2);  
      rackState = 1; 
    }
  } 
  
  else if (isDark || isWet) {
    if (rackState == 1) { 
      Serial.println("ฝนตกหรือมืดแล้ว: กำลังหุบราวตากผ้า...");
      myStepper.step(-stepsPerRevolution * 2); 
      rackState = 0; 
    }
  }
  delay(1000); 
}