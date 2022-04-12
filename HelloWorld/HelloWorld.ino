/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

//Leds
const int LED_red = 8;
const int LED_green = 9;
// PIR Sensor constants
const int PIR = 22;
const int Lamb = 23;
// Flame Sensor
const int Buzzer = 31;
const int Flame_sensor = 30;
int flame_detected;
// Temperature Sensor
const int Temp_sensor = A7;


// used for password
int i = 0;

char initial_password[] = "1234";

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {14, 15, 16, 17}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {18, 19, 20}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {

  //  PIR setup pins
  pinMode(PIR, INPUT);
  pinMode(Lamb, OUTPUT);
  //  Flame Sensor setup pins
  pinMode(Buzzer, OUTPUT);
  pinMode(Flame_sensor, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.println("Temperature is: ");
//  lcd.setCursor(0, 1);
}
int Password_flag = 0;
int lcd_cursor = 0;
int password_mode = 0;
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  checkPassword();
//  PIR code ON motion turn on lamb
  PIRSensor();

//  Flame sensor code
  FlameSensor();

//  Temperature Sensor
  if(password_mode == 0)
  {
    TemperatureSensor();
  }
}


int compareFourDigit(char *a, char *b)
{
  for(int k = 0 ; k < 4 ; k++)
  {
    if(a[k] != b[k])
    {
      return 1;
    }
  }
  return 0;
}

void TemperatureSensor()
{
   int Temperature = analogRead(Temp_sensor);
   Temperature = (Temperature*0.48828125);
//   lcd.println("Temprature is: ");
   lcd.setCursor(0, 1);
   lcd.print(Temperature);
//   delay(200);
//   lcd.clear();
}

void FlameSensor()
{
  flame_detected = digitalRead(Flame_sensor);
  if (flame_detected == 1)
  {
    tone(Buzzer,65,500);
  }
}
void PIRSensor()
{
   int pir_input = digitalRead(PIR);
  if(pir_input ==  HIGH)
  {
    digitalWrite (Lamb, HIGH);
    delay(200);
    digitalWrite (Lamb, LOW);
  }
}

void checkPassword()
{
  char password[4];
  char key = keypad.getKey();
  
  if (key){
    password_mode = 1;
    if(Password_flag ==0)
    {
//      lcd.println("Password");
      Password_flag++;
    }
//    lcd.setCursor(lcd_cursor, 1);
    password[i]=key;
    i++;
    lcd_cursor++;
//    lcd.print(key);
  }
  
  if(i == 4)
  {
    if(compareFourDigit(password, initial_password)==0)
    {
      digitalWrite (LED_green, HIGH);
      digitalWrite (LED_red, LOW);
//      delay(200);
//      lcd.clear();
      password_mode = 0;
    }
    else
    {
      digitalWrite (LED_red, HIGH);
      digitalWrite (LED_green, LOW);
//      delay(200);
//      lcd.clear();
      password_mode = 0;
    }  
  }
}
