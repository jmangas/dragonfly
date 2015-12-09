#include <serLCD.h>
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
int led = 11;
int potPin = 21;
int lcdPin = 7;
int motor1pwmpin = 10;
int motor1pin1 = 0;
int motor1pin2 = 1;

boolean estadoMotor1 = false;
boolean sentidoMotor1 = false;

serLCD lcd(lcdPin);
int cont  = 0;
boolean estadoLed = true;

const int medidas = 10;
int arrayMedidas[medidas];
int indiceMedidas = 0;
int total = 0;
int average = 0;

void setup() {                
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  pinMode(motor1pwmpin, OUTPUT); 
  pinMode(motor1pin1, OUTPUT); 
  pinMode(motor1pin2, OUTPUT); 

  digitalWrite(motor1pwmpin, LOW); 
  digitalWrite(motor1pin1, LOW); 
  digitalWrite(motor1pin2, LOW); 
  
  delay(100);
  lcd.setBrightness(0);
  lcd.clear();   
  lcd.print("Hello, World!"); 
  Serial.println("Tensy: HI!");
  delay(400);
}

// the loop routine runs over and over again forever:
void loop() {
  int val = analogRead(potPin);
  
  total = total - arrayMedidas[indiceMedidas];
  // read from the sensor:
  arrayMedidas[indiceMedidas] = val;
  // add the reading to the total:
  total = total + arrayMedidas[indiceMedidas];
  // advance to the next position in the array:
  indiceMedidas = indiceMedidas + 1;

  // if we're at the end of the array...
  if (indiceMedidas >= medidas) {
    // ...wrap around to the beginning:
    indiceMedidas = 0;
  }

  // calculate the average:
  average = total / medidas;
  
  
  int barras = map(average, 0, 1015, 0, 16);
  int motor1pwm = map(average, 0, 1015, 0, 255);
  
  Serial.print("value ");
  Serial.print( val );  
  Serial.print("   average ");
  Serial.print(average);
  Serial.print("  ");
  Serial.println(motor1pwm);
    
  String string =  String("Loop: " + cont);
  cont++;
  char incomingByte;
  if (Serial.available()) {
      incomingByte = Serial.read();  // will not be -1
      //lcd.clearLine(1);
      lcd.selectLine(1); 
      lcd.print(incomingByte);
      if( incomingByte == 'l'  ){
        digitalWrite(led, estadoLed); 
        estadoLed = !estadoLed;      
      }
      if( incomingByte == 'e'  ){
        estadoMotor1 = !estadoMotor1;
      }
      
      if( incomingByte == 'a'  ){
        digitalWrite(motor1pin1, sentidoMotor1); 
        sentidoMotor1 = !sentidoMotor1;
        digitalWrite(motor1pin2, sentidoMotor1);                
      }
     
    }

  if(estadoMotor1) {
    analogWrite(motor1pwmpin, motor1pwm);
    digitalWrite(led, HIGH);
  } else {
    analogWrite(motor1pwmpin, LOW);
    digitalWrite(led, LOW);
  }
  
  //lcd.clearLine(2);
  lcd.selectLine(2); 
 
  
  for(int i = 0; i <= 15; i++){
    if( i < barras) lcd.print("#");
    else lcd.print(" ");
  }
  
  
  
}
