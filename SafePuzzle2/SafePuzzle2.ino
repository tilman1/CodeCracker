//Software for Safe Puzzle -- Rewrite of code from Michael Klements + extentsions
//(C) Tilman Glötzner 2023

#include <SPI.h>                          //Import libraries to control the OLED display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#ifdef ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>                        //Import library to control the servo
#endif

const unsigned long pushButtonDebounceTime = 10;
const unsigned long pushButtonCloseSafeTime = 500;
const unsigned long pushButtonGotoMenuTime = 2500;
const unsigned long messageDelay = 4000;
const byte LeverPositionLocked = 16;
const byte LeverPositionOpened = 120;

Servo lockServo;                          //Create a servo object for the lock servo

#define SCREEN_WIDTH 128                  // OLED display width, in pixels

//#define SCREEN_HEIGHT 32                // OLED display height, in pixels
#define SCREEN_HEIGHT 64                  // OLED display height, in pixels

#if SCREEN_HEIGHT  == 64
// 'Vault60x60', 60x60px
// image downloaded from Vecteezy.com (https://www.vecteezy.com/vector-art/356727-vector-vault-icon)
// and postprocessed (rescaled, converted to black&white, and converted to array)
// The image to array converter can be found here:
// https://www.mischianti.org/2021/07/14/ssd1306-oled-display-draw-images-splash-and-animations-2/
// Attribution: <a href="https://www.vecteezy.com/free-vector/safe-icon">Safe Icon Vectors by Vecteezy</a>
const unsigned char Vault60x60 [480] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x58, 0x00, 0x1f, 0xe0, 0x00, 0x20, 0x00, 0x00, 0x58, 0x00, 0xf2, 0x38, 0x00, 0x20, 0x00, 
  0x00, 0x58, 0x01, 0xc2, 0x0e, 0x00, 0x20, 0x00, 0x00, 0x58, 0x07, 0x42, 0x1b, 0x00, 0x20, 0x00, 
  0x00, 0x58, 0x04, 0x60, 0x31, 0x80, 0x20, 0x00, 0x00, 0x58, 0x08, 0x00, 0x00, 0xc0, 0x20, 0x00, 
  0x00, 0x58, 0x1c, 0x1f, 0xc1, 0xc0, 0x20, 0x00, 0x00, 0x58, 0x17, 0x3a, 0x63, 0x60, 0x20, 0x00, 
  0x00, 0x78, 0x30, 0x6a, 0xd2, 0x20, 0x20, 0x00, 0x00, 0x20, 0x20, 0xe0, 0x38, 0x20, 0x20, 0x00, 
  0x00, 0x20, 0x20, 0xa0, 0x28, 0x30, 0x20, 0x00, 0x00, 0x20, 0x20, 0x80, 0x0c, 0x30, 0x20, 0x00, 
  0x00, 0x20, 0x3c, 0xe0, 0x3d, 0xf0, 0x20, 0x00, 0x00, 0x20, 0x20, 0x80, 0x08, 0x30, 0x20, 0x00, 
  0x00, 0x20, 0x20, 0xf0, 0x38, 0x20, 0x20, 0x00, 0x00, 0x70, 0x20, 0x48, 0x18, 0x20, 0x20, 0x00, 
  0x00, 0x78, 0x33, 0x7a, 0xf2, 0x20, 0x20, 0x00, 0x00, 0x58, 0x1e, 0x3e, 0xe3, 0xe0, 0x20, 0x00, 
  0x00, 0x58, 0x18, 0x0f, 0x80, 0xc0, 0x20, 0x00, 0x00, 0x58, 0x0c, 0x20, 0x10, 0x80, 0x20, 0x00, 
  0x00, 0x58, 0x06, 0x60, 0x11, 0x00, 0x20, 0x00, 0x00, 0x58, 0x03, 0x42, 0x1e, 0x00, 0x20, 0x00, 
  0x00, 0x58, 0x01, 0xc2, 0x1c, 0x00, 0x20, 0x00, 0x00, 0x58, 0x00, 0x7f, 0xf0, 0x00, 0x20, 0x00, 
  0x00, 0x58, 0x00, 0x0f, 0x80, 0x00, 0x20, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x01, 0x03, 0x00, 0x0c, 0x08, 0x00, 0x00, 
  0x00, 0x01, 0x82, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x07, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


#define OLED_RESET -1                     // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)


void initialMessage()
{
  display.setTextSize(2);                     //Set the display text size
  display.setCursor(35,10);                   //Set the display cursor position
  display.println(F("Crack"));                //Set the display text
  display.display();                          //Output the display text
  delay(500);
  display.clearDisplay();                     //Clear the display
  display.setCursor(45,10);
  display.println(F("The"));
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(40,10);
  display.println(F("Code"));
  display.display();
  delay(500);
  display.clearDisplay();
}


void displayPressButtonMessage()
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);                                   //Set the display text size to small
   
#if SCREEN_HEIGHT  == 64 
  display.setCursor(10,0);                                  //Set the display cursor position
  display.print(F("Push To Lock Safe"));                    //Set the display text
  display.drawBitmap(34, 10, Vault60x60, 60, 60, WHITE);     //Draw vault image
#else
  display.setCursor(35,10);                                 //Set the display cursor position
  display.print(F("Push To"));                              //Set the display text
  display.setCursor(35,20);                                 //Set the display cursor position
  display.print(F("Lock Safe"));                            //Set the display text
#endif
  display.display();                                        //Output the display text
}






   
class CodeCombination {
  public: 
    CodeCombination();
    CodeCombination(byte digit0, byte digit1, byte digit2, byte digit3);
    void operator =(CodeCombination & OtherCodeCombination);
    bool operator==( CodeCombination & OtherCodeCombination);
    bool operator!=( CodeCombination & OtherCodeCombination);
    char* getValue();
  
    
    byte operator [] (size_t idx);
    void setValue(size_t idx,byte value);
    void setValue(word value);
    
   
    
    void verifyCode(CodeCombination & OtherCodeCombination);
    byte getNofCorrectNumber();
    byte getNofCorrectPlaces();
    void GenerateNewRandomCode();
    void GenerateNewRandomCode(unsigned long seed);
    void DumpRandomSeed();
    void DumpCode();
     
  private:
    byte code[4];
    char _codeStr[5];
    long randNumber;
    byte _correctNum = 0;                                         //Variable for the number of correct digits in the wrong place
    byte _correctPlace = 0;   
    word powword(int x, int y);
};




CodeCombination::CodeCombination()
{
  for (int i=0 ; i<= 3 ; i++)                                 
  {
    code[i] = 0; 
  }  
}

CodeCombination::CodeCombination(byte digit0, byte digit1, byte digit2, byte digit3)
{
  code[0] = digit0;
  code[1] = digit1;
  code[2] = digit2;
  code[3] = digit3;
}

char* CodeCombination::getValue()
{

  _codeStr[4] = 0x0;
  for (int i=0 ; i<= 3 ; i++)   
  {                                
     _codeStr[i]  = (char)(code[i]+48);
     
  } 
  //Serial.println( _codeStr); 
  /*
  Serial.print("'");
  Serial.print( _codeStr);
  Serial.println("'");
  */
  return _codeStr;
}

      
bool CodeCombination::operator==( CodeCombination & OtherCodeCombination)
{
  for (int i=0 ; i<= 3 ; i++)                                 
  {  
    if (code[i] != OtherCodeCombination.code[i])
     return false;
  }
  return true;
}

bool CodeCombination::operator!=( CodeCombination & OtherCodeCombination)
{
  for (int i=0 ; i<= 3 ; i++)                                 
  {  
    if (code[i] != OtherCodeCombination.code[i])
     return true;
  }
  return false;
}

void CodeCombination::operator =(CodeCombination & OtherCodeCombination)
{
  for (int i=0 ; i<= 3 ; i++)                                   
    code[i] = OtherCodeCombination.code[i];
}



byte CodeCombination::operator [] (size_t idx)
{
  if (idx < 4)
    return code[idx];
}

void CodeCombination::setValue(size_t idx,byte value) 
{
  if (idx < 4)
   code[idx] = value;
}


word CodeCombination::powword(int x, int y)
{
  word val=x;
  for(byte z=0;z<=y;z++)
  {
    if(z==0)
      val=1;
    else
      val*=x;
  }
  return val;
}

void CodeCombination::setValue(word value)
{
  byte i;
  word divider;
  
  for (int k = 0; k < 4; k++)
  {
    divider = powword(10, 3-k);
    i=(byte)(value/divider);
    value -= i*divider;
    setValue(k,i);
  }
}
 
void CodeCombination::verifyCode(CodeCombination & OtherCodeCombination)
{

  int correctNum = 0;                                         //Variable for the number of correct digits in the wrong place
  int correctPlace = 0;                                       //Variable for the number of correct digits in the correct place
  int usedDigits[4] = {0,0,0,0};                              //Mark off matching digits 

  
  for (int i=0 ; i<= 3 ; i++)                                 
  {
    if (code[i] == OtherCodeCombination.code[i])
    {
      //Digit  matches
      correctPlace++;
      correctNum++;
      usedDigits[i] = 1;
    }
    else
    {
      // digit does not match. Now check if digits exists in the code. Matching digits are excluded
      for (int j=0 ; j<= 3 ; j++)
      {
        if (usedDigits[j] != 1)
          if (code[i] == OtherCodeCombination.code[j])
          {
            correctNum++;
            usedDigits[j] = 1;
            break;    // stop if digit is found
          }
      }
    }
  }
  _correctNum = correctNum;
  _correctPlace = correctPlace;
}

void CodeCombination::GenerateNewRandomCode()
{
  randNumber = analogRead(3);
 // Serial.print("randNumber="); Serial.println(randNumber);
  randomSeed(randNumber); 
  for (int i=0 ; i<= 3 ; i++)                                 
  {
    code[i] = random(0,9);       //Generate a random number for each digit of the code
  }
}

void CodeCombination::GenerateNewRandomCode(unsigned long seed)
{
 
  randomSeed(seed); 
  for (int i=0 ; i<= 3 ; i++)                                 
  {
    code[i] = random(0,9);       //Generate a random number for each digit of the code
  }
}

void CodeCombination::DumpRandomSeed()
{
  Serial.print("CodeCombination:Seed = ");
  Serial.println(randNumber);
}

void CodeCombination::DumpCode()
{
  for (int i=0 ; i<= 3 ; i++)   
  {
    Serial.print(code[i]); 
  }
}

byte  CodeCombination::getNofCorrectNumber()
{
  return _correctNum;
}

byte CodeCombination::getNofCorrectPlaces()
{
  return _correctPlace;
}

class LEDNotifier {
  public:
   LEDNotifier(byte leftUpCorPlaLed, byte rightUpCorPlaLed, byte leftDownCorPlaLed, byte rightDownCorPlaLed, 
               byte leftUpCorNumLed, byte rightUpCorNumLed, byte leftDownCorNumLed, byte rightDownCorNumLed);
   void updateLEDs(byte nofCorrectNum, byte nofMatchedPlaces);  
   void  turnOffLEDs(); 
   void spiralLEDs(unsigned long delayMS);
   void flashingFrames(unsigned long delayMS);
     
   private:
    byte _leftUpCorPlaLed = 0;
    byte _rightUpCorPlaLed = 0;
    byte _leftDownCorPlaLed = 0;
    byte _rightDownCorPlaLed = 0;
    byte _leftUpCorNumLed = 0;
    byte _rightUpCorNumLed = 0;
    byte _leftDownCorNumLed = 0;
    byte _rightDownCorNumLed = 0;
    byte _corPlaced[4] = {0,0,0,0};
    byte _corNum[4] = {0,0,0,0};
};

LEDNotifier::LEDNotifier(byte leftUpCorPlaLed, byte rightUpCorPlaLed, byte leftDownCorPlaLed, byte rightDownCorPlaLed, 
               byte leftUpCorNumLed, byte rightUpCorNumLed, byte leftDownCorNumLed, byte rightDownCorNumLed)
{
   _leftUpCorPlaLed = leftUpCorPlaLed;
   _rightUpCorPlaLed = rightUpCorPlaLed;
   _leftDownCorPlaLed = leftDownCorPlaLed;
   _rightDownCorPlaLed = rightDownCorPlaLed;
   _leftUpCorNumLed = leftUpCorNumLed;
   _rightUpCorNumLed = rightUpCorNumLed;
   _leftDownCorNumLed = leftDownCorNumLed;
   _rightDownCorNumLed = rightDownCorNumLed;  
   _corPlaced[0] = _leftUpCorPlaLed;
   _corPlaced[1]  = _rightUpCorPlaLed;
   _corPlaced[2]  = _leftDownCorPlaLed;
   _corPlaced[3]  =_rightDownCorPlaLed;
   _corNum[0] = _leftUpCorNumLed; 
   _corNum[1] = _rightUpCorNumLed;
   _corNum[2] = _leftDownCorNumLed;
   _corNum[3] = _rightDownCorNumLed;       

  for(int i=0 ; i<=3 ; i++)                           //Define pin modes for the LEDs
  {
    digitalWrite(_corNum[i],LOW);
    pinMode(_corNum[i], OUTPUT);
    
    digitalWrite(_corPlaced[i],LOW);
    pinMode(_corPlaced[i], OUTPUT);
  }
}

// all LEDs off
void LEDNotifier::turnOffLEDs()
{
  digitalWrite(_leftUpCorPlaLed, LOW);
  digitalWrite(_rightUpCorPlaLed, LOW);
  digitalWrite(_leftDownCorPlaLed, LOW);
  digitalWrite(_rightDownCorPlaLed, LOW);
  digitalWrite(_leftUpCorNumLed, LOW);
  digitalWrite(_rightUpCorNumLed, LOW);
  digitalWrite(_leftDownCorNumLed, LOW);
  digitalWrite(_rightDownCorNumLed, LOW);
}

void LEDNotifier::spiralLEDs(unsigned long delayMS)
{
  turnOffLEDs();
  for (byte i = 0; i <= 3; i++)
  {
     digitalWrite(_corPlaced[i], HIGH);
     delay(delayMS);
  }
  for (byte i = 0; i <= 3; i++)
  {
     digitalWrite(_corNum[i], HIGH);
     delay(delayMS);
  }
}

void LEDNotifier::flashingFrames(unsigned long delayMS)
{
    delay(delayMS);
    updateLEDs (0,4);                                         //Flashing LED sequence
    delay(delayMS);
    updateLEDs (4,0);
    delay(delayMS);
    updateLEDs (0,4);
    delay(delayMS);
    updateLEDs (4,0);
    delay(delayMS);
    updateLEDs (4,4);  
  
}

//Function to update the LEDs to reflect the guess
void LEDNotifier::updateLEDs (byte nofCorrectNum, byte nofMatchedPlaces)      
{
  turnOffLEDs();  //First turn all LEDs off

  for(int j=0 ; j< nofCorrectNum; j++)                              //Turn on the number of correct digits in wrong place LEDs
  {
    digitalWrite(_corNum[j], HIGH);
  }
  for(int k=0 ; k< nofMatchedPlaces ; k++)                              //Turn on the number of correct digits in the correct place LEDs
  {
    digitalWrite(_corPlaced[k], HIGH);
  }
}

class Encoder 
{
  public:
    Encoder();
    void PinA();
    void PinB();
    bool isEncoderChanged();
    byte getValue();
    void setValue(byte value);
    void setLimits(byte lowerLimit, byte upperLimit);
     

  private:
     volatile byte bFlag = 0;                            //Reset flags for the next turn
     volatile byte aFlag = 0;    
     volatile byte reading = 0;
     volatile byte encoderPos = 0;
     volatile byte encoderPosOld = 0;
     byte _lowerLimit = 0;
     byte _upperLimit = 9;
  
};

Encoder::Encoder()
{
  _lowerLimit = 0;
  _upperLimit = 9;
}

bool Encoder::isEncoderChanged()
{
  
  bool rc =  (encoderPosOld != encoderPos);
  encoderPosOld = encoderPos;

  return rc;
}

byte Encoder::getValue()
{
  return encoderPos;
}

void Encoder::setValue(byte value)
{
  encoderPos = value;
}

void Encoder::setLimits(byte lowerLimit, byte upperLimit)
{
   _lowerLimit = lowerLimit;
   _upperLimit = upperLimit;
}

void Encoder::PinA()                   //Rotary encoder interrupt service routine for one encoder pin
{
  volatile byte reading = 0;  
        
  reading = PIND & 0xC;                   //Read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag)       //Check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {     
    if(encoderPos>_lowerLimit)
      encoderPos --;                      //Decrement the encoder's position count
    else
      encoderPos = _upperLimit;                     //Go back to 9 after 0
    bFlag = 0;                            //Reset flags for the next turn
    aFlag = 0;                            //Reset flags for the next turn
  }
  else if (reading == B00000100)          //Signal that we're expecting pinB to signal the transition to detent from free rotation
    bFlag = 1;                                  
}

void Encoder::PinB()                   //Rotary encoder interrupt service routine for the other encoder pin
{
  volatile byte reading = 0;  
        
  reading = PIND & 0xC;                   //Read all eight pin values then strip away all but pinA and pinB's values
  
  if (reading == B00001100 && bFlag)      //Check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {
    if(encoderPos<_upperLimit)
      encoderPos ++;                      //Increment the encoder's position count
    else
      encoderPos = _lowerLimit;           //Go back to 0 after 9
    bFlag = 0;                            //Reset flags for the next turn
    aFlag = 0;                            //Reset flags for the next turn
  }
  else if (reading == B00001000)          //Signal that we're expecting pinA to signal the transition to detent from free rotation
    aFlag = 1;                            
}

class Button
{
  public:
   Button();
   void risingFlank();
   void fallingFlank();
   unsigned long  getTimeButtonPressed();
   bool isPressed();
   bool isReleased();

   private:
    unsigned long _pressTime;
    unsigned long _releaseTime;
    unsigned long _deltaTime;
    bool _engaged;
    bool _released;
   
};

Button::Button()
{
  _engaged = false;
}

void Button::risingFlank()
{


  if ((millis() - _releaseTime) > pushButtonDebounceTime)                         // debounce switch.
  {
    _engaged = false;
    _released = true;
    _releaseTime = millis();
    _deltaTime = _releaseTime - _pressTime;
  }
  
}

void Button::fallingFlank()
{
  
  if (((millis() - _pressTime) > pushButtonDebounceTime) 
  && ((millis() - _releaseTime) > pushButtonDebounceTime))
  
  {
    _pressTime = millis();
    _releaseTime = 0;
    _engaged = true;
    _released = false;
    
  }
}

unsigned long Button::getTimeButtonPressed()
{
  return _deltaTime;
}

bool Button::isPressed()
{
  return _engaged;
}

bool Button::isReleased()
{
  bool rc = _released;
  _released = false;
  return rc;
}

enum ModeT
{
  none = 0,
  enterCode,
  guessCode
};

class YesNoDialog
{
  public:
   YesNoDialog(int ypos, Encoder* en, Button* bot, bool presetYes);
   bool isYes();
   void setYes();
   void setNo();
   void refresh();
   bool isFinished();

  private:

   int _ypos;
   Encoder* _encoder;
   Button* _button;
};


YesNoDialog::YesNoDialog(int ypos, Encoder* en,  Button* but, bool presetYes)
{
  _ypos = ypos;
  en->setLimits(0, 1);
  
  en->setValue(0);
  refresh();
  display.display(); 
 
  _encoder = en;  
  _button = but;
  if (presetYes) 
   _encoder->setValue(0);
  else
   _encoder->setValue(1);  
    
}

bool YesNoDialog::isYes()
{
  return  (_encoder->getValue() == 0); 
}

void YesNoDialog::setYes()
{
  _encoder->setValue(0);
  
}

void YesNoDialog::setNo()
{
  _encoder->setValue(1);

}

void YesNoDialog::refresh()
{
  int i =  _encoder->getValue();
  display.setTextSize(1);
  display.setCursor(30,_ypos);
  if (i == 0)
    display.setTextColor(BLACK, WHITE);
  else
    display.setTextColor(WHITE, BLACK);
    
  display.print(F("Yes")); 
  display.setCursor(77,_ypos);
  if (i == 1)
    display.setTextColor(BLACK, WHITE);
  else
    display.setTextColor(WHITE, BLACK);
  display.print(F("No"));
  display.setTextColor(WHITE);
  display.display(); 
}


bool YesNoDialog::isFinished()
{
  bool _rc; 
  refresh();
  if (_button->isReleased())                              // if the encoder button was pressed  
  {
      _rc = true;
  }
  else
  {
      _rc = false;
  }
  return _rc;
}

class Dial
{
  private:
  int _currentDigit = 0;
  Encoder* _encoder;
  Button*  _button;
  CodeCombination _enteredCode;
  bool    _codeEntered = false;
  uint16_t _ypos = 4;

  public:
   Dial(Encoder* en, Button* but, uint16_t ypos);
   void enterDigits();
   bool isFinished();
   void updateDisplayCode();
   char* getValue();
   CodeCombination& getCombination();
   void presetEnteredCode(CodeCombination* code);
   void reset();
};


Dial::Dial(Encoder* en, Button* but,uint16_t ypos)
{
  _encoder = en;
  _button = but;
  _codeEntered = false;
  _encoder->setLimits(0,9);
  _currentDigit = 0;
  _encoder->setValue(_enteredCode[0]);
  _ypos = ypos;
  updateDisplayCode();
}

char* Dial::getValue()
{
  return  _enteredCode.getValue();
}

CodeCombination& Dial::getCombination()
{
  return _enteredCode;
}

void Dial::enterDigits()
{
    if (_encoder->getValue() != _enteredCode[_currentDigit]) // update digits changed via encoder on display
    {
     _enteredCode.setValue(_currentDigit,_encoder->getValue());
     updateDisplayCode();
    }

    if (_button->isReleased())                              // if the encoder button was pressed goto next digit
    {
      if (_currentDigit < 3)
      {
        _codeEntered = false;
       _currentDigit++; 
      }
      else
      {  // if last digit was entered set indicator flag
        _codeEntered = true;
        _currentDigit = 0;
      }
      _encoder->setValue(_enteredCode[_currentDigit]);
      updateDisplayCode();
    }
}

void Dial::updateDisplayCode()                    //Function to update the display with the input code
{
 //Serial.print("updateDisplayCode=");Serial.print(_enteredCode.getCombination());Serial.println('.');                                     //Output to Serial monitor for debugging
 display.setTextSize(2);                                  //Set the display text size
 
 display.fillRect(40, _ypos, 50, 25, BLACK);              //Clear part of the display showing combination
 display.setCursor(40,_ypos+6);                           //Set the display cursor position
 display.setTextColor(WHITE);
 display.println(_enteredCode.getValue());                //Set the display text
 display.drawRect(40 + (_currentDigit * 12), _ypos, 10, 2, WHITE);      // Display cursor top line
 display.drawRect(40 + (_currentDigit * 12), _ypos + 23, 10, 2, WHITE); // Display cursor button line
 display.display();                                        //Update the display

}

bool Dial::isFinished()
{
    enterDigits();
    return _codeEntered;
}

void Dial::presetEnteredCode(CodeCombination* code)
{
  _enteredCode = *code;
  _encoder->setValue(_enteredCode[0]);  
  updateDisplayCode();
}

void Dial::reset()
{
  _codeEntered = false;
  _currentDigit = 0;
  
 
}

class CombinationLock 
{
    private:
     ModeT           _mode;
     byte            _state;
     CodeCombination _code;
     CodeCombination _enteredCode;
     int             _currentDigit;
     Encoder*        _encoder;
     Button*         _button;
     LEDNotifier*    _leds;
     bool            _codeActive;
     bool    _locked;
     byte    _numGuess;
     unsigned long _timestamp;
     bool _delayEngaged = false;
     Dial* dial = NULL;
     YesNoDialog* confirmDialog = NULL;
     void displayGuessCode();
     void displayCrackedMessage(int numGuesses);
     void displayEnterNewSafeCode();
     void displaySaveRequest(char* combination);
     void displaySaving();
     void displaySaved();
     void displayDeactivateCode(char* combination);
     

    public: 
     CombinationLock(Encoder *en, Button *bot, LEDNotifier *leds);
     void run();
     void reset();
     void updateLedNotifier();
     bool isCodeCorrect();
     bool isLocked();
     void setLocked(bool lockState);
     void setRandomCode();
     void setRandomCode(unsigned long seed);
     void evalEnteredCombination();
     void runSetCode();
     void runGuessCombination();

};

CombinationLock::CombinationLock(Encoder *en, Button *but, LEDNotifier *leds)
{
    _currentDigit = 0;
    _encoder = en;
    _button = but;
    _leds = leds;
    _locked = false;
    _mode = none;
    
}

void CombinationLock::displayCrackedMessage(int numGuesses)
{
  display.clearDisplay();                                 //Clear the display
  display.setTextSize(1);                                 //Set the display text size to small
  display.setCursor(35,10);                               //Set the display cursor position
  display.print(F("In "));                                //Set the display text
  display.print(numGuesses);                              //Set the display text
  display.setCursor(35,20);                               //Set the display cursor position
  display.print(F("Attempt(s)"));                           //Set the display text
  display.display();                                      //Output the display text
  Serial.print(F("Cracked in "));
  Serial.print(numGuesses);
  Serial.println(F(" attempt(s)"));
}

void CombinationLock::displayGuessCode()
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);
  display.setCursor(10,0);
  display.print(F("Enter your Guess"));  
  display.display();   
}

void CombinationLock::displayEnterNewSafeCode()
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(F("Enter new Combination")); 
}

void CombinationLock::displayDeactivateCode(char* combination)
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(F("Deactivate Code "));                      //Set the display cursor position
  display.print(combination);
  display.print(F("?"));
  display.display();
}

void CombinationLock::displaySaveRequest(char* combination)
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);  
  display.setCursor(0,0);                                  //Set the display cursor position
  display.print(F("Save & Activate " ));                    //Set the display text
  display.print(combination);
  display.print(F("?"));
  
  display.display();                                        //Output the display text
}

void CombinationLock::displaySaving()
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);  
  display.setCursor(40,20);                                   //Set the display cursor position
  display.print(F("Saving..." ));                           //Set the display text

  display.display();                                        //Output the display text
}

void CombinationLock::displaySaved()
{
  display.clearDisplay();                                   //Clear the display
  display.setTextSize(1);  
  display.setCursor(45,20);                                  //Set the display cursor position
  display.print(F("Saved" ));                               //Set the display text

  display.display();                                        //Output the display text
}


void CombinationLock::runSetCode()
{
  switch (_state) 
  {
    case 0:
      if (readCodeFromEEprom(&_code) == false)
      {
        // no code saved or not active
        Serial.println(F("No combination preset."));
        _enteredCode = _code;
        _state = 1;
      }
      else
      {
        // active code combination  => deactivate
        Serial.println(F("Stored combination available."));
        _state = 10;
      }
    break;
    case 1:
      displayEnterNewSafeCode();
      if (dial == NULL)                            // instantiate a dial
      { 
       dial = new Dial(_encoder, _button,15);
       dial->presetEnteredCode(&_code);
      }
      else
      { // instance of dial object is available
        if (dial->isFinished() == true)            // once all 4 digits of the code are entered ...
        {
         _enteredCode = dial->getCombination();
         _codeActive = true;  
         delete dial;
         dial = NULL;
         _state = 2;
        }
      }
      
    break; 
    case 2:
      if (confirmDialog == NULL)
      { 
       displaySaveRequest(_enteredCode.getValue());
       // display: (yes/no)
       confirmDialog = new YesNoDialog(45, _encoder,_button,  true);     
      }
      else
      {
       if  ( confirmDialog->isFinished() == true) 
       {
         if (confirmDialog->isYes() == true)
         {
          _state = 3;
         }
         else
         {
          _state = 5;
         }
         delete confirmDialog;
         confirmDialog = NULL; 
        }
      }
     break;
    case 3:
         // save code in Memory
         Serial.print(F("Saving Code '")); Serial.print(_enteredCode.getValue());Serial.println(F("'"));
         writeCodeToEEprom(_enteredCode,_codeActive);
         displaySaving();
         dumpEEPROM();
         _leds->spiralLEDs(150);
         _state = 4; 
     break;
    case 4:
         displaySaved();
         _timestamp = millis(); 
        _delayEngaged = true;
        _state = 5;
     break;
    case 5:
       //  finish
        _mode = none;
        //_timestamp = millis(); 
        _delayEngaged = false;
        _state = 0;
        displayPressButtonMessage();
     break;
     case 10:
        if (confirmDialog == NULL)
        { 
          // display: deactivate code (yes/no)
          confirmDialog = new YesNoDialog(30, _encoder,_button,  true);  
          displayDeactivateCode(_code.getValue());
          
        }
        else
        {
         if  ( confirmDialog->isFinished() == true) 
         {
           if (confirmDialog->isYes() == true)
           {
             _state = 3;
             _codeActive = false;
           }
           else
           { 
             _state = 1;
           }
           delete confirmDialog;
           confirmDialog = NULL; 
          }
        }
     break;
  }
}

void CombinationLock::runGuessCombination()
{
  if (dial == NULL)               // instantiate a dial
  {
    displayGuessCode();
    dial = new Dial(_encoder, _button,15);
    _numGuess = 1;
    
    
  }
  else
  { // instance of dial object is available
    if (dial->isFinished() == true)            // once all 4 digits of the code are entered ...
    {
      _enteredCode = dial->getCombination();
      evalEnteredCombination();     // ... check if it matches the combination
      updateLedNotifier();          // set leds to indicate correct number and numbers on correct places within the code

      if (isCodeCorrect())         // If the code was correctly guessed, display sucess message
      {
        lockServo.write(LeverPositionOpened);      // open lock
        displayCrackedMessage(_numGuess); // display message                   
        _timestamp = millis();    // record timestamp to be used for delay
        _delayEngaged = true;
        _locked = false;
        // reset everything
        
        _mode = none; 
        delete dial;
        dial = NULL;
       } 
       else
       {
         _numGuess++;
         dial->reset();
       }
      }
  }
}

void CombinationLock::run()
{ 
  if (_delayEngaged == true)             // delay once the combination was cracked
  {
   if ((millis() - _timestamp) > messageDelay)
   {
     _delayEngaged = false;
     displayPressButtonMessage();
     _leds->turnOffLEDs();
     
   }
   else
     return;
  }
  
  if (_locked == true)                 // if the safe is locked, the combination can be entered
  {
    if (_mode == guessCode)
    {
      runGuessCombination();
    }
  }
  else
  {   
    switch (_mode)
    {
      case none:  
       // if the safe is not locked and ideling, check for the encoder button being pressed
       if (_button->isReleased())                              // if the encoder button was pressed  
       {
        if (_button->getTimeButtonPressed() < pushButtonCloseSafeTime)   //short press of button
        {
         // close safe
         Serial.println("Close Safe");
         lockServo.write(LeverPositionLocked); 
         _locked = true;
         if (readCodeFromEEprom(&_code) == false) // if no valid combination is saved ...
         {
           Serial.println(F("No Combination set."));
           setRandomCode(_button->getTimeButtonPressed());           //  ... set random code is to be guessed
           Serial.print(F("Random code: ")); // debug: dump code on serial port
           _code.DumpCode();
           Serial.println();   
         }
         else
         {
          Serial.print(F("Combination '")); _code.DumpCode(); Serial.println(F("' set."));
         }
         _mode = guessCode;                       // user can now try to guess the code
        }
        if ((_button->getTimeButtonPressed() >= pushButtonCloseSafeTime) &&  // medium press of button
         (_button->getTimeButtonPressed() < pushButtonGotoMenuTime))
        {
         // user wishes to set a code combination for the safe
         Serial.println(F("Set Code requested"));
         _mode =  enterCode;  // user can now enter code combination
         _state = 0;          // reset state machine controlling the process of inputing the combination
        }
        if (_button->getTimeButtonPressed() >= pushButtonGotoMenuTime)  // long press of button
        { 
          // Other menu
          Serial.println(F("Undefined Service requested"));
        }
       }
      break;
      case enterCode:
         runSetCode();
      break;
     
   }
  }
}

// reset lock to allow user to enter his next guess of the combination



bool CombinationLock::isCodeCorrect()
{
  return _code == _enteredCode;
}

bool CombinationLock::isLocked()
{
  return _locked;
}

void CombinationLock::setLocked(bool lockState)
{
  _locked = lockState;  
  
   if (readCodeFromEEprom(&_code) == true)
   {
      _mode = guessCode;  
   }
}

void CombinationLock::evalEnteredCombination()
{
  _code.verifyCode(_enteredCode);  // check entered code against code combination
  Serial.print(F("Entered Code: "));  // debug: dump entered code onto serial port
  _enteredCode.DumpCode();
   Serial.println();
}

void CombinationLock::setRandomCode()
{
  _code.GenerateNewRandomCode();   // Generate random code that needs to be guessed             
}

void CombinationLock::setRandomCode(unsigned long seed)
{
  _code.GenerateNewRandomCode(seed);
}

void CombinationLock::updateLedNotifier()
{
  _leds->updateLEDs(_code.getNofCorrectNumber(), _code.getNofCorrectPlaces());
}


#ifdef __AVR_ATmega328P__
#define IRAM_ATTR
static int ledpin = LED_BUILTIN;
static int pinA = 2;                      //Hardware interrupt digital  D2/GPIO5
static int pinB = 3;                      //Hardware interrupt digital  D3/GPIO6
static int buttonPin = A0;                 //A0/GPIO19 number for encoder push button
static int servoPin  = 11;                //D11/GPIO14 number for servo to lock door

//const unsigned long pushButtonDebounceTime = 10;
LEDNotifier Leds(4,6,10,8,5,7,12,9); // GPIO number for correct place LEDs (Indicate a correct digit in the correct place)
                                     // and correct number LEDs  (Indicate a correct digit)

#endif


Encoder Encoder;
Button Button;
CombinationLock Lock(&Encoder,&Button, &Leds);

void PinAISR() 
{
  cli();
  Encoder.PinA();
  sei();             //Restart interrupts
}

void PinBISR() 
{
  cli();
  Encoder.PinB();
  sei();            //Restart interrupts
}

void ButtonRisingISR()
{
  Button.risingFlank();
}


void ButtonFallingISR()
{
  Button.fallingFlank();
}

void deleteEEprom()
{
  for ( unsigned short k = 0; k < 1024; k ++)
  { 
     EEPROM.update( k, 0xff );
  }
}

bool findFirstFreeEEpromEntry(unsigned short* address )
{
   word eeAddress = 0;
   word value;
   bool found = false;
   
   for ( eeAddress = 0; eeAddress < 1024; eeAddress +=2)
   { 
     EEPROM.get( eeAddress, value );
     if (value == 0xffff)
     {
       found = true;
       break;
     }
   }

   *address = eeAddress;
   return found;
}

void writeCodeToEEprom(class CodeCombination& code, bool active)
{
  word address;
  word value = atoi(code.getValue());
  word oldValue;
  if (active == false) // if combination is to be deactivated, set deactivation flag
     value |= 1<<14;
  bool found = findFirstFreeEEpromEntry(&address);

  if (found == true)
  {
     EEPROM.get(address - 2,oldValue );
  }
  else
  {
    // EEPROM completely used => oldValue in 2 highest address, i.e. address as returned  
    // from findFirstFreeEEpromEntry
     EEPROM.get(address,oldValue);
  }
  
  if (oldValue != value)
  { 
    // new value needs to be written
    if (found == false)
    {
      // EEPROM needs to be cleared before writing new value
      deleteEEprom();
      address = 0;
    }
    //Serial.print(F("writeCodeToEEprom: address=")); Serial.print(address);Serial.print(F(";val="));Serial.println(value);
    EEPROM.put( address, value );
  }
}

bool dumpEEPROM()
{
  Serial.println(F("EEPROM Dump"));
  Serial.println(F("==========="));
  for ( unsigned short k = 0; k < 1024; k ++)
  { 
     byte value = EEPROM.read (k);
     if (value < 16)
       Serial.print('0');
     Serial.print( value,HEX);Serial.print(' ');
     if ((k+1)%20 == 0)
      Serial.println();
  }
  Serial.println();
}

bool readCodeFromEEprom(class CodeCombination* code)
{
  word address;
  word value;
  bool found = findFirstFreeEEpromEntry(&address);

  if (found == true)
  {
    if (address == 0)
    {
      // no entries => set code to 0
      code->setValue(0);  
      return false; 
    }
  }
  
  address -=2; 
  EEPROM.get(address,value);
  //Serial.print("read: ");Serial.println(value);
 
  code->setValue(value& 0x3FFF);       // mask out bit 14 and 15

  if (value & 0x4000)                  // bit 14 marks the entry as inactive
    return false;
  //Serial.print(F("Read @")); Serial.print(address);Serial.print(F("="));Serial.println(*value);
  return true;
}

void setup() {
  // put your setup code here, to run once:

  Leds.turnOffLEDs();
  Serial.begin(115200);                                 //Starts the Serial monitor for debugging
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))      //Connect to the OLED display
  {
    Serial.println("SSD1306 allocation failed");   //If connection fails
    for(;;);                                          //Don't proceed, loop forever
  }
  display.clearDisplay();                             //Clear display
  display.setTextColor(SSD1306_WHITE); 
  Serial.println("SSD1306 succeeded");

  // Setup interrupt for Pin A0 
  PCICR |= B00000110;     //Bit1,2 = 1 -> "PCIE1","PCIE2"  enabeled (PCINT8 to PCINT14 and PCINT16 to PCINT23)
  PCMSK1 |= B00000001;    //Bit0 = 1 -> "PCINT16" enabeled -> A0 will trigger interrupt
  PCMSK2 |= B00001100;    //Bit2,3= 1 ->   PCINT18,  PCINT19 enabeled -> D2,D3 will trigger interrupt
  // setup encoder pins
  pinMode(pinA, INPUT_PULLUP);                        //Set pinA as an input, pulled HIGH to the logic voltage
  pinMode(pinB, INPUT_PULLUP);                        //Set pinB as an input, pulled HIGH to the logic voltage
  pinMode(buttonPin, INPUT_PULLUP);                   //Set the encoder button as an input, pulled HIGH to the logic voltage
  
  // if a combintation is set, assume the safe to be closed
  CodeCombination code;
  if (readCodeFromEEprom(&code) == false)
  {
    lockServo.write(LeverPositionOpened);
    Lock.setLocked(false);
  }
  else
  {
    lockServo.write(LeverPositionLocked);
    Lock.setLocked(true);
  }
  lockServo.attach(servoPin);
#ifdef __AVR_ATmega328P__
  Serial.println(F("Interrupts for Encoder installed"));
  Serial.print(F("Pin A =  ")); Serial.println(pinA);
  Serial.print(F("Pin B =  ")); Serial.println(pinB);
  Serial.print(F("Button =  ")); Serial.println(buttonPin);
  Serial.print(F("Servo Pin =  ")); Serial.println(servoPin);
#endif
 
 
  //Leds.spiralLEDs(300);
  initialMessage();
  Leds.turnOffLEDs();
  if (Lock.isLocked() == false)
    displayPressButtonMessage();
  Serial.print(F("isLock = "));Serial.println(Lock.isLocked());
  
  
}

 bool A0State = HIGH;
 bool A0StateOld= HIGH;
 bool D2State = HIGH;
 bool D2StateOld = HIGH;
 bool D3State = HIGH;
 bool D3StateOld = HIGH;



void loop() {
  Lock.run();
}


#ifdef __AVR_ATmega328P__
ISR (PCINT1_vect) 
{
    // For PCINT of pins A0 to A5, Reset
    A0State = digitalRead(buttonPin);
   if (A0State == HIGH && A0StateOld == LOW)
   {
    // Rising flank
    ButtonRisingISR(); 
   
   }
   if (A0State == LOW && A0StateOld == HIGH)
   {
    // Falling flank
    ButtonFallingISR();
   }
   A0StateOld = A0State;
} 

ISR (PCINT2_vect) 
{
   // For PCINT of pins  D0 to D7 
   D2State = digitalRead(pinA);
   if (D2State == HIGH && D2StateOld == LOW)
   {
    // Rising flank
    PinAISR();
   }
   if (D2State == LOW && D2StateOld == HIGH)
   {
    // Falling flank
   }
   
   D2StateOld = D2State;
  
   D3State = digitalRead(pinB);
   if (D3State == HIGH && D3StateOld == LOW)
   {
    // Rising flank
    PinBISR();
   }
   if (D3State == LOW && D3StateOld == HIGH)
   {
    // Falling flank
   }
   D3StateOld = D3State;
   
}
#endif
/*
bool unittestCodeCombination()
{
  bool rc = true;
  unsigned int testNr = 0;
  CodeCombination code0(0,0,0,0);
  CodeCombination code1(1,2,3,4);
  CodeCombination code2(2,2,3,4);
  CodeCombination code3(1,2,3,4);
  CodeCombination code4;
  
  CodeCombination code5(0,0,0,0);
  CodeCombination code6(9,0,0,0);
  CodeCombination code7(9,8,0,0);
  CodeCombination code8(9,8,9,0);
  CodeCombination code9(9,8,9,9);
 
  code4.GenerateNewRandomCode();
  
  Serial.print("Code 1:" );code1.DumpCode();
  Serial.print("Code 2:" );code2.DumpCode();
  Serial.print("Code 3:" );code3.DumpCode();
  
  code4.DumpRandomSeed();
  Serial.print("Code 4:" );code4.DumpCode();

 
  if (code1 == code2)
  {
     testNr = 1;
     rc = false;
  }

  if (code1 != code3)
  {
    testNr += 2;
    rc = false;
  }

  code1.verifyCode(code2);
  //res->DumpResult();
 code1.verifyCode(code3);
  //res->DumpResult();

  
  if (code2 == code4)
  {
     testNr += 4;
     rc = false;
  }
  
  code2.verifyCode(code4);
   
  code4 = code2;
  //Serial.print("Code 2:" );code2.DumpCode();
  //Serial.print("Code 4:" );code4.DumpCode();

  if (code2 != code4)
  {
     testNr += 8;
     rc = false;
  }
   
 code2.verifyCode(code4);

  ComparisonResult* cr;
  cr = code5.verifyCode(code5);
  cr->DumpResult();
  if (cr->getNofCorrectNumbers() != 4 && cr->getNofMatchingPlaces() != 4)
  {
     Serial.println("Error: verify(code5,code5)");
  }
    
  cr = code5.verifyCode(code6);
  cr->DumpResult();
  if (cr->getNofCorrectNumbers() != 3 && cr->getNofMatchingPlaces() != 3)
  {
     Serial.println("Error: verify(code5,code6)");
  }
  
  cr = code5.verifyCode(code7);
  cr->DumpResult();
  if (cr->getNofCorrectNumbers() != 2 && cr->getNofMatchingPlaces() != 2)
  {
     Serial.println("Error: verify(code5,code7)");
  }
  
  cr = code5.verifyCode(code8);
  cr->DumpResult();
  if (cr->getNofCorrectNumbers() != 1 && cr->getNofMatchingPlaces() != 1)
  {
     Serial.println("Error: verify(code5,code8)");
  }
  
  cr = code5.verifyCode(code9);
  cr->DumpResult();
  if (cr->getNofCorrectNumbers() != 0 && cr->getNofMatchingPlaces() != 0)
  {
     Serial.println("Error: verify(code5,code9)");
  }
  
}
*/

/*
void unittestLedNofifier()
{
  long int delayMS = 1000;
  Leds.spiralLEDs(700);
  Leds.turnOffLEDs();
  Leds.updateLEDs(0,1);
  delay(delayMS);
  Leds.updateLEDs(1,0);
  delay(delayMS);
  Leds.updateLEDs(3,3);
  delay(delayMS);
  Leds.updateLEDs(4,3);
  delay(delayMS);
  Leds.updateLEDs(4,4);
  delay(delayMS);

    
}

void unittest_YesNo() 
{
  YesNoDialog* Dialog = new YesNoDialog(20, &Encoder,&Button,  true);
  
  while ( Dialog->isFinished() == false);
  if (Dialog->isYes() == true)
    Leds.spiralLEDs(300);
  delete Dialog;
  

 }

void unittest_Dial()
{
  Dial* Dial1 = new Dial(&Encoder, &Button);
  
  while (Dial1->isFinished() == false);
  char* digits  = Dial1->getValue();
  Serial.print(F("Dial1: "));Serial.println(digits);
  CodeCombination combi = Dial1->getCombination();
  Serial.print(F("Dial1.combi: "));Serial.println(combi.getValue());
  
  delete Dial1;
 }

 void unittest_Mem()
 {
  unsigned short adr;
  findFirstFreeEEpromEntry(&adr);
  unsigned short value;
  CodeCombination code;
  bool rc = readCodeFromEEprom(&code);
  Serial.print(F("readCodeFromEEprom:"));Serial.println(code.getValue());
  writeCodeToEEprom(CodeCombination::CodeCombination(1,0,2,3).getValue());
  rc = readCodeFromEEprom(&code);
  Serial.print(F("readCodeFromEEprom:"));Serial.println(code.getValue());
  dumpEEPROM();
 }
*/
