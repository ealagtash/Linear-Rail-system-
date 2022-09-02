
//            Ehsan Al-Agtash
//            Jully 22 2022
//            All rights reserved


// LIBRARIES
#include <LiquidCrystal.h>

//defining pins
#define dir 8
#define dir2 11

#define stepPin 9
#define stepPin2 10

#define rightlimit 32
#define homeLimit 30

#define EN1 12
#define EN2 13

#define CCW HIGH
#define CW LOW

unsigned long previousMotor1Time = millis();
unsigned long previousMotor2Time = millis();
//long Motor1Interval = 200;
long Motor2Interval = 20;

// MENU ARRAYS
String menuOption[] = {{"Go home"}, {"linear"}, {"Linear with spin"}};       // Text on the top line
String measurementType[] = {{"[In per min]"}, {"[In per min]"}, {"[In per min]"}};  // Text on the bottom line

// PINS ON THE ARDUINO.
const int leftButtonPin =45 ;       // Pin 5 for "Left" command
const int rightButtonPin = 49;      // Pin 6 for "Right" command
const int upButtonPin = 51;         // Pin 7 for "Up" command
const int downButtonPin = 53;       // Pin 8 for "Down" command
const int enterButtonPin = 47;      // Pin 9 for "Enter" command
const int clearButtonPin = 43;     // Pin 10 for "Clear" command

// NUMBER COUNT OF BUTTON PRESSES AND COUNTER UNITS.
int setting1Counter = 1;
int setting2Counter = 1;
int setting3Counter = 1;
int directionPush = 0;      // This counter changes the menu option with each "left" or "right" button push.
int upPressCount = 0;       // This counter measures the amount of times the user pushes the "up" button.
int downPressCount = 0;     // This counter measures the amount of times the user pushes the "down" button.

// BUTTON PRESS STATES FOR EACH FUNCTION, ALL SET TO "LOW"
boolean buttonStateLeft = LOW;            // Button states for the "Left" command
boolean lastButtonStateLeft = LOW;
boolean currentButtonStateLeft = LOW;
boolean buttonStateRight = LOW;           // Button states for the "Right" command
boolean lastButtonStateRight = LOW;
boolean currentButtonStateRight = LOW;
boolean buttonStateUp = LOW;              // Button states for the "Up" command
boolean lastButtonStateUp = LOW;
boolean currentButtonStateUp = LOW;
boolean buttonStateDown = LOW;            // Button states for the "Down" command
boolean lastButtonStateDown = LOW;
boolean currentButtonStateDown = LOW;
boolean buttonStateEnter = LOW;           // Button states for the "Enter" command
boolean lastButtonStateEnter = LOW;
boolean currentButtonStateEnter = LOW;
boolean buttonStateClear = LOW;           // Button states for the "Clear" command
boolean lastButtonStateClear = LOW;
boolean currentButtonStateClear = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

//adjusting the speed the motor spins at
int convert (int spin2)
{
  if (setting1Counter == 1) {
    spin2 = 410;// 1-inch per min
  }
  if (setting1Counter == 2)
  {
    spin2 = 280;//1.5 inches per min
  }
  if (setting1Counter == 3)
  {
    spin2 = 205;//2 inches per min
  }
  if (setting1Counter == 4)
  {
    spin2 = 135;//3 inches per min
  }
  if (setting1Counter == 5)
  {
    spin2 = 105;//4-inches per min
  }
  if (setting1Counter == 6)
  {
    spin2 = 80;//5-inches per min
  }
  if (setting1Counter == 7)
  {
    spin2 =  4;//go home
  }
  return spin2;
}

int convert2(int spin21)
{
  if (setting2Counter == 1) {
    spin21 = 410;// 1-inch per min
  }
  if (setting2Counter == 2)
  {
    spin21 = 280;//1.5 inches per min
  }
  if (setting2Counter == 3)
  {
    spin21 = 205;//2 inches per min
  }
  if (setting2Counter == 4)
  {
    spin21 = 135;//3 inches per min
  }
  if (setting2Counter == 5)
  {
    spin21 = 105;//4-inches per min
  }
  if (setting2Counter == 6)
  {
    spin21 = 80;//5-inches per min
  }
  if (setting2Counter == 7)
  {
    spin21 =  3;//go home
  }
  return spin21;
}

int convert3(int spin3)
{
  if (setting3Counter == 1) {
    spin3 = 950;// 1-inch per min
  }
  if (setting3Counter == 2)
  {
    spin3 = 850;//1.5 inches per min
  }
  if (setting3Counter == 3)
  {
    spin3 = 450;//2 inches per min
  }
  if (setting3Counter == 4)
  {
    spin3 = 280;//3 inches per min
  }
  if (setting3Counter == 5)
  {
    spin3 = 200;//4-inches per min
  }
  if (setting3Counter == 6)
  {
    spin3 = 180;//5-inches per min
  }
  if (setting3Counter == 7)
  {
    spin3 =  3;//go home
  }
  return spin3;
}
//assigning the motor to spin clockwise
void forward()
{
  int rotation = convert2(rotation);
  digitalWrite(dir, CW);
  digitalWrite (stepPin, HIGH);
  //digitalWrite(EN1, LOW);
  delay(rotation);
  digitalWrite(stepPin, LOW);
  delay(rotation);
}


//assigning the motor to spin counter clockwise
void backward()
{
  int rotation = convert(rotation);
  digitalWrite(dir, CCW);
  digitalWrite (stepPin, HIGH);
  //digitalWrite(EN1, LOW);
  delay(rotation);
  digitalWrite(stepPin, LOW);
  delay(rotation);

}

void spin()
{
  int rotation = convert3(rotation);
  unsigned long currentMotor1Time = millis();
  unsigned long currentMotor2Time = millis();
  digitalWrite(stepPin, LOW);
  digitalWrite(stepPin2, LOW);

  if (currentMotor1Time - previousMotor1Time > rotation) {
     digitalWrite(dir, CW);
    digitalWrite(stepPin, HIGH);
    previousMotor1Time = currentMotor1Time;
  }

  if (currentMotor2Time - previousMotor2Time > Motor2Interval) {
    digitalWrite(stepPin2, HIGH);
    previousMotor2Time = currentMotor2Time;
  }
}






//telling the motor to spin till it hits the limit switch
void moveTooTheRight()
{
  while (digitalRead(rightlimit) != HIGH && digitalRead(clearButtonPin) != HIGH) {
    forward();
    //spin();
  }
}
void BackHome()
{
  while (digitalRead(homeLimit) != HIGH && digitalRead(clearButtonPin) != HIGH) {
    backward();
    //spin();
  }
}

void spinning()
{
  while (digitalRead(clearButtonPin) != HIGH && digitalRead(rightlimit) != HIGH) {
    spin();
  }
}


// OBJECT DECLARATION
LiquidCrystal lcd(2, 3, 4, 5 , 6, 7);



void setup() {
  lcd.clear();
  lcd.begin(16, 2);

  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  pinMode(upButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
  pinMode(enterButtonPin, INPUT);
  pinMode(clearButtonPin, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dir, OUTPUT) ;
  pinMode(stepPin2, OUTPUT);
  pinMode(dir2, OUTPUT) ;

}




// MAIN LOOP
void loop() {

  int unitSetting[] = {setting1Counter, setting2Counter, setting3Counter};    // This variable holds the individual counters in one array

  lcd.setCursor(0, 0);                          // Menu displayed on the LCD.
  lcd.print(menuOption[directionPush]);         // The menuOption that is displayed is determined by the left or right push.
  lcd.setCursor(0, 1);
  lcd.print(unitSetting[directionPush]);        // The setting counter that is displayed is determined by the left or right push.
  lcd.setCursor(4, 1);                          // This cursor setting fixes the measurementType in one place, preventing offset
  lcd.print(measurementType[directionPush]);    // by the unitSetting.

  // The program at this point is waiting for a button press.
  currentButtonStateLeft = digitalRead(leftButtonPin);
  currentButtonStateRight = digitalRead(rightButtonPin);
  currentButtonStateUp = digitalRead(upButtonPin);
  currentButtonStateDown = digitalRead(downButtonPin);
  currentButtonStateEnter = digitalRead(enterButtonPin);
  currentButtonStateClear = digitalRead(clearButtonPin);

  if (currentButtonStateLeft != lastButtonStateLeft || currentButtonStateRight != lastButtonStateRight ||
      currentButtonStateUp != lastButtonStateUp || currentButtonStateDown != lastButtonStateDown || currentButtonStateEnter != lastButtonStateEnter)
    // If there is a button push on any of the buttons, the following routine runs to check if it was a valid press:
  {
    lastDebounceTime = millis();      // lastDebounceTime is set equal to the running millis() function.
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
    // If the lastDebounceTime (aka. the "snapshot" time) minus the running millis() function is higher than the set debounce delay, the following routine
    // below runs and checks which button was pushed:
  {

    // The current state for each button is set not equal to the pressed state and when it changes, the pressed state becomes equal to the current state.

    // LEFT BUTTON PRESS
    if (currentButtonStateLeft != buttonStateLeft)       // Left button scrolls the menu options to the left.
    {
      buttonStateLeft = currentButtonStateLeft;

      if (buttonStateLeft == LOW)                        // Once the button is released, the push is registered and the code below runs.
      {
        directionPush--;                                   // Both the up and down press counts will be reset to zero when the left button is pushed.
        upPressCount = 0;
        downPressCount = 0;
      }

      if (directionPush < 0)                           // If the user tries to scroll below the first menu option,
      { // the program will loop back to the last menu option.
        directionPush = 2;//change this number for a bigger menu size.
      }
      lcd.clear();
    }

    // RIGHT BUTTON PRESS
    if (currentButtonStateRight != buttonStateRight)     // Right button scrolls the menu options to the right.
    {
      buttonStateRight = currentButtonStateRight;

      if (buttonStateRight == LOW)
      {
        directionPush++;                                   // Both the up and down press counts will be reset to zero when the right button is pushed.
        upPressCount = 0;
        downPressCount = 0;
      }

      if (directionPush > 2)                           // If the user tries to scroll above the last menu option,
      { // the program will loop back to the first menu option.
        directionPush = 0;
      }
      lcd.clear();
    }

    // UP BUTTON PRESS
    if (currentButtonStateUp != buttonStateUp)           // Up button scrolls the setting upward.
    {
      buttonStateUp = currentButtonStateUp;

      if (buttonStateUp == LOW && directionPush == 0)    // The first 5 times in which the "up" button is pushed, each push will add 1 increment to the setting.
      {
        upPressCount++;
        downPressCount = 0;                                // The downPressCount is reset to zero.
        setting1Counter++;

        if (setting1Counter > 7)                       // Sets the setting counter limit to 999. The user cannot increase the counter beyond 999.
        {
          setting1Counter = 7;
        }
      }

      if (buttonStateUp == LOW && directionPush == 1)
      {
        upPressCount++;
        downPressCount = 0;
        setting2Counter++;

        if (setting2Counter > 7)                       // Sets the setting counter limit to 999. The user cannot increase the counter beyond 999.
        {
          setting2Counter = 7;
        }
      }

      if (buttonStateUp == LOW && directionPush == 2)
      {
        upPressCount++;
        downPressCount = 0;
        setting3Counter++;


        if (setting3Counter > 7)                       // Sets the setting counter limit to 999. The user cannot increase the counter beyond 999.
        {
          setting3Counter = 7;
        }
      }
      lcd.clear();
    }


    // DOWN BUTTON PRESS
    if (currentButtonStateDown != buttonStateDown)        // Down button scrolls the setting downward.
    {
      buttonStateDown = currentButtonStateDown;

      if (buttonStateDown == LOW && directionPush == 0)   // The first 5 times in which the "down" button is pushed, each push will subtract 1 increment to the setting.
      {
        downPressCount++;
        upPressCount = 0;                                   // The upPressCount is reset to zero.
        setting1Counter--;


        if (setting1Counter < 1)                       // Sets the setting counter limit to -999. The user cannot increase the counter beyond -999.
        {
          setting1Counter = 1;
        }
      }

      if (buttonStateDown == LOW && directionPush == 1)
      {
        downPressCount++;
        upPressCount = 0;
        setting2Counter--;

        if (setting2Counter < 1)                       // Sets the setting counter limit to -999. The user cannot decrease the counter beyond -999.
        {
          setting2Counter = 1;
        }
      }

      if (buttonStateDown == LOW && directionPush == 2)
      {
        downPressCount++;
        upPressCount = 0;
        setting3Counter--;

        if (setting3Counter < 1)    // This code prevents the user from entering a number below "0".
        {
          setting3Counter = 1;        // Remove this code if you want to allow in
        }                           // negative numbers on a setting.
      }
      lcd.clear();
    }
  }

  // ENTER BUTTON PRESS
  if (currentButtonStateEnter != buttonStateEnter)
  {
    buttonStateEnter = currentButtonStateEnter;

    if (buttonStateEnter == LOW && directionPush == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Going home");
      lcd.setCursor(0, 1);
      lcd.print("ENTERED");
      BackHome();
      delay(2000);
    }

    if (buttonStateEnter == LOW && directionPush == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Linear");
      lcd.setCursor(0, 1);
      lcd.print("ENTERED");
      moveTooTheRight();
      delay(2000);
      //moveTooTheRight();
    }

    if (buttonStateEnter == LOW && directionPush == 2)
    {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Linear with spin");
      lcd.setCursor(0, 1);
      lcd.print("ENTERED");
      spinning();

      delay(2000);
    }

    lcd.clear();
  }



  // CLEAR BUTTON PRESS
  if (currentButtonStateClear != buttonStateClear)
  {
    buttonStateClear = currentButtonStateClear;

    if (buttonStateClear == LOW && directionPush == 0)    // The Clear button clears all setting data depending on what menu option you are viewing.
    { // It flahses a brief message stating that the data has been cleared.
      lcd.clear();                                          // The press counts for both the up and down variables are also reset to zero.
      lcd.setCursor(0, 0);
      lcd.setCursor(0, 1);
      lcd.print("CLEARED");
      setting1Counter = 1;
      downPressCount = 0;
      upPressCount = 0;
      //delay(2000);


    }

    if (buttonStateClear == LOW && directionPush == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.setCursor(0, 1);
      lcd.print("CLEARED");
      setting2Counter = 1;
      downPressCount = 0;
      upPressCount = 0;
      //delay(2000);
    }

    if (buttonStateClear == LOW && directionPush == 2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.setCursor(0, 1);
      lcd.print("CLEARED");
      setting3Counter = 1;
      downPressCount = 0;
      upPressCount = 0;
      //delay(2000);
    }
    lcd.clear();
  }




  // After a button is pushed and the count recorded, all the states reset back to LOW for the data to be processed correctly.
  lastButtonStateLeft = currentButtonStateLeft;       // resets the left button state to LOW
  lastButtonStateRight = currentButtonStateRight;     // resets the right button state to LOW
  lastButtonStateUp = currentButtonStateUp;           // resets the up button state to LOW
  lastButtonStateDown = currentButtonStateDown;       // resets the down button state to LOW
  lastButtonStateEnter = currentButtonStateEnter;     // resets the enter button state to LOW
  lastButtonStateClear = currentButtonStateClear;     // resets the clear button state to LOW

} 
