// First, include the mandatory headers.
#include <Wire.h>                          
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h> 
#include <Servo.h>
#include <dht.h>

// Declare the objects.
dht DHT;
Servo servo;
hd44780_I2Cexp lcd; 


// Constants.
const int LCD_COLS = 16; // 16 columns.
const int LCD_ROWS = 2; // 2 rows.
int wlvlcount = 0;  // A counter for water level sensor
int wlvlpin = A0; // Setting water level sensor to pin 8
const int buzzer = 9; //Setting buzzer to pin 9

// Setting up servo, Buzzer and LCD.
void setup()
{ 
  // Initilize the servo to 0 degrees.
  servo.attach(10);//PIN 10
  servo.write(0);
  
  //  Initilize the buzzer.
  pinMode(buzzer, OUTPUT);
  
  // The following code was inspired by the example of I2Cexp module.
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
    status = -status; // convert negative status value to positive number
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }
}


// The loop which loops every 1.5 seconds.
void loop() {
  // Check the reading of TEMP&HUM sensor from PIN 8.
  int chk = DHT.read11(8);
  // Use the first row of LCD to print the data.
  lcd.setCursor(0,0); 
  lcd.print("T: ");
  lcd.print(DHT.temperature,0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(8,0);
  lcd.print("H: ");
  lcd.print(DHT.humidity   ,0);
  lcd.print("%");
  
  // Use the second row of LCD to print the water level.
  lcd.setCursor(0, 1); 
  lcd.print("WtrLvL: ");
  wlvlcount = analogRead(wlvlpin); //Read data from A0 and store the data into counter.


  // If conditions to determine the actions needed according to the water level.
  if (wlvlcount<=100){lcd.print("Empty  ");noTone(buzzer); servo.write(0);} // For empty: Do nothing.
  else if (wlvlcount>100 && wlvlcount<=625){ lcd.print("Low   ");noTone(buzzer); servo.write(0);}  // For Low: Do nothing.
  else if (wlvlcount>625 && wlvlcount<=660){ lcd.print("Medium   ");;tone(buzzer, 500,3000);  servo.write(45);} // For medium: Buzz a alarm and half close the tap.
  else if (wlvlcount>660){ lcd.print("High  ");tone(buzzer, 1000,5000); servo.write(90);} // For high: Buzz a alarm and close the tap fully.
  
  // Test code for determining the limits.
//  lcd.setCursor(8, 1); 
//  lcd.print(wlvlcount);

  // Loop every 1.5 seconds due to the temp/hum sensor.
  delay(1500); 
  }
