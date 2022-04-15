// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

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

// KeyPad
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {14, 15, 16, 17}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {18, 19, 20}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

// LCD 
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
  lcd.println("Temperature is: ");
}

void loop() {
  
  checkPassword();
  
  PIRSensor();

  FlameSensor();

  TemperatureSensor();
}

void checkPassword()
{
  char password[4];
  char key = keypad.getKey();
  if (key)
  {
    if(i == 0)
    {
      digitalWrite (LED_green, LOW);
      digitalWrite (LED_red, LOW);
    }
    if(key == 35) //#
    {
      i = 0;
      digitalWrite (LED_green, LOW);
      digitalWrite (LED_red, LOW);
    }
    else
    {
      password[i]=key;
      i++;
    }
  }
  
  if(i == 4)
  {
    if(compareFourDigit(password, initial_password)==0)
    {
      digitalWrite (LED_green, HIGH);
      digitalWrite (LED_red, LOW);
    }
    else
    {
      digitalWrite (LED_red, HIGH);
      digitalWrite (LED_green, LOW);
    }
    i = 0;  
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
   Temperature = round(Temperature*0.48828125);
   lcd.setCursor(0, 1);
   if(Temperature > 30)
   {
     lcd.print("High");
     lcd.setCursor(0,1);
   }
   else if(Temperature < 20)
   {
      lcd.print("Low");
      lcd.setCursor(0,1);
   }
   else
   {
     lcd.print(Temperature);
     lcd.print("     ");
     lcd.setCursor(0,1);
   } 
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
