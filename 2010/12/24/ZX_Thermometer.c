/**
* Make your own programmable digital thermometer in an hour
* http://lemire.me/blog/archives/2010/12/24/make-your-own-programmable-digital-thermometer-in-an-hour/
*
* Motivation: I make yogourt, but could not find a good
* programmable digital thermometer for this purpose. Cooking
* thermometers either are not programmable, or they are designed
* for meat and won't last when exposed to water.
*
*
* This is the code for a ZX Thermometer running on 
* an LCD shield with a piezo. You can program a target
* temperature and it will play a song when the temperature is
* reached. It will keep playing the sonds every 10 seconds
* if the high temperature is maintained.
*
* You can program the temperature you want using buttons.
* The target temperature is saved permanently on the board.
* 
* Temperatures are given both in C and F. 
*
* The great thing with the ZX Thermometer is that it goes in 
* water, so you can use this device to make yogourt or beer.
* Be warned however that high temperature (>100C) are likely to
* damage it. Don't overcook it!
* 
* Buttons "right" and "left" are not used. The select button is
* used to reset the "target reached" flag. Certainly, something
* more useful could be done with these buttons.
*
* Physical components:
* - Arduino Uno board
*   (see http://www.robotshop.com/ProductInfo.aspx?pc=RB-Ard-18)
*   Bought for $30
* - Some power source (I have used a 9V battery)
* - Wiring.
* - A resistance.
* - Generic piezo element
*   Cost: a few cents (usually sold with kits)
* - DFRobot LCD Keypad Shield for Arduino
*   (see http://www.robotshop.com/dfrobot-lcd-keypad-shield-arduino-3.html)
*   Bought for $24
* - ZX-Thermometer Temperature Sensor by ZNex
*   (see http://www.robotshop.com/inex-zx-temperature-thermistor-sensor.html)
*   Bought for $8.
*
*  Overall cost : Less than $70.
*
* Assembly:
*   (1) Put the LCD shield on the Arduino. Read the instructions to 
*       to know where the analog and digital pins are, since the
*       LCD shield is not labelled.
*   (2) Connect the thermometer on the analog port 2 (middle wire). 
*       Connect another wire to the 5V and another yet to ground.
*       You need to add a resistance between the 5V and the thermometer.
*       (Any sensible resistance will do, I think.)
*   (3) Connect the piezo on digital 12. Don't forget to ground
*       one half of it.
*   (4) Connect the arduino to your computer by USB. Upload the
*       Program.
*
*  That's it!
*   
*
* Written by Daniel Lemire (http://lemire.me) on Dec. 23, 2010.
* I have used pieces of anonymous code from robotshop.com.
* As with all Arduino software, this is licensed under GPL.
*/

#include <inttypes.h>
#include <EEPROM.h>

int thermometerPin = 2;// analog (this is arbitrary, depends on your physical design)
int piezoPin = 12;// digital (this is arbitrary, depends on your physical design)


// DFRobot LCD Keypad Shield for Arduino
class LCD4Bit_mod {
public:
  LCD4Bit_mod(int num_lines);
  void commandWrite(int value);
  void init();
  void print(int value);
  void printFloat(double number, uint8_t digits);
  void printNumber(unsigned long n, uint8_t base);
  void printIn(char value[]);
  void clear();
  //non-core---------------
  void cursorTo(int line_num, int x);
  void leftScroll(int chars, int delay_time);
  //end of non-core--------
  //4bit only, therefore ideally private but may be needed by user
  void commandWriteNibble(int nibble);

private:
  void pulseEnablePin();
  void pushNibble(int nibble);
  void pushByte(int value);
};

int length = 24;//32;
char notes[]="cdeccdecefgefggagfecgagfeccGccGc";
int tempo = 150;// 300;//300;
int beats[] = {1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1
               ,2,2,1,1,1,1,2,2,1,1,2,1,1,2};

void playTone(int tone, int duration) {
 for(long i = 0; i<duration * 1000L; i+=tone*2) {
   digitalWrite(piezoPin,HIGH);
   delayMicroseconds(tone);
   digitalWrite(piezoPin,LOW);
   delayMicroseconds(tone);
 }
}

void playNote(char note, int duration) {
 char names[] = {'G','c','d','e','f','g','a','b','C'};
 int tones[] = {3500,1915,1700,1519,1432,1275,1136,1014,956};
 for(int i = 0;i<9;++i) {
   if(note==names[i]) playTone(tones[i],duration);
 }
}



void playSong() {
  for(int i = 0; i<length;++i) {
  if(notes[i]==' ') {delay(beats[i]*tempo);//beats[i]
  } else {
    playNote(notes[i],beats[i]*tempo);//
 }
 }
}


LCD4Bit_mod lcd = LCD4Bit_mod(2); 

//Key message
char msgs[5][15] = {"Right Key OK ", 
                    "Up Key OK    ", 
                    "Down Key OK  ", 
                    "Left Key OK  ", 
                    "Select Key OK" };
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;

int key=-1;
int oldkey=-1;
int oldvalue = 0;

bool targetreached = 0;
int targetF = 0.0;

int loopcounter = 0;


void setup() {
 targetreached = false;
 //Serial.begin(9600); 
 pinMode(piezoPin, OUTPUT);
 //storePersistent(112);
 targetF = loadPersistent(); 
 if(targetF == 0) {
    storePersistent(112);
    targetF = loadPersistent();
 }
 pinMode(thermometerPin,INPUT);
 lcd.init();
 lcd.clear();
 printTarget();
}

void printTarget() {
 lcd.cursorTo(2, 0);  //line=2, x=0		
 lcd.printFloat(convertFromFahrenheitToCelsius(targetF),1);
 lcd.printIn("C ");
 lcd.printNumber(targetF,10);
 lcd.printIn("F ");
 if(  targetreached ) 
   lcd.printIn("Ok! ");
 else 
   lcd.printIn("    ");
}





void loop() {
  loopcounter++;
  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);		        // convert into key press
  if (key >=0){
     if(key == 1) {// UP!
          ++targetF;
          targetreached = false;
          storePersistent(targetF);  
          printTarget();  
      } else if(key == 2) {// DOWN!
          --targetF;
          targetreached = false;
           storePersistent(targetF);
           printTarget();
      } else if(key == 4) {//SELECT
           targetreached = false;
           printTarget();
      }
  }

  if(loopcounter % 5 == 0) {
    int value = analogRead(thermometerPin);
    float celsius = convertToCelsius(value);
    float fahrenheit = convertFromCelsiusToFahrenheit(celsius);
    if(oldvalue != value) {  	
      lcd.cursorTo(1, 0);  //line=2, x=0	
      lcd.printFloat(celsius,1);
      lcd.printIn("C ");
      lcd.printFloat(fahrenheit,1);
      lcd.printIn("F     ");
      value = oldvalue;
    }
    if( (fahrenheit+0.01>=targetF) and (!targetreached) ){
            targetreached = true;
            printTarget();
            playSong();  
            loopcounter = 0;
    } else if( (targetreached) and (loopcounter % 100 == 0)) {
            // every ten seconds, check the status
            if(fahrenheit<targetF) {
              targetreached = false;
              printTarget();
            } else
              playSong();  
    }
  }
  delay(100);
}



void storePersistent(int x) {
   int firstpart = x % 256;
   EEPROM.write(0,firstpart) ;
   int secondpart = x / 256;
   EEPROM.write(1,secondpart);  
}

int loadPersistent() {
  return EEPROM.read(0) + (((unsigned int)EEPROM.read(1)) <<8);
}

float  convertFromCelsiusToFahrenheit(float celsius) {
  return 9.0/5.0 * celsius + 32.0;
}

float  convertFromFahrenheitToCelsius(float fahrenheit) {
  return (5.0/9.0)*(fahrenheit-32.0);
}

float convertToCelsius(int valueread) {
    if(valueread<340) {
    return -5.0 + (valueread-120.0)/(340-120) * ( 15.0 + 5.0 ) + 3.5;
  } else if(valueread<500) {
    return 15.0 + (valueread-340.0)/(500-340) * (30 - 15.0) + 3.5;
  } else if(valueread<660) {
    return 30.0 + (valueread -500.0) /(660-500) * (50 - 30) + 3.5;
  } else {
    return 50.0 + (valueread-660.0) /(790 - 660) * (75 - 50) + 3.5;
  }
}



// Convert ADC value to key number
int get_key(unsigned int input) {
	int k;
	for (k = 0; k < NUM_KEYS; k++) {
		if (input < adc_key_val[k]) {
                  return k;
                }
	}
        if (k >= NUM_KEYS)
            k = -1;     // No valid key pressed
        return k;
}




extern "C" {
  #include <stdio.h>  //not needed yet
  #include <string.h> //needed for strlen()
  #include <inttypes.h>

//#include "WConstants.h"  //all things wiring / arduino
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
}



//command bytes for LCD

#define CMD_CLR 0x01
#define CMD_RIGHT 0x1C
#define CMD_LEFT 0x18
#define CMD_HOME 0x02



// --------- PINS -------------------------------------
//is the RW pin of the LCD under our control?  If we're only ever going to write to the LCD, we can use one less microcontroller pin, and just tie the LCD pin to the necessary signal, high or low.
//this stops us sending signals to the RW pin if it isn't being used.
int USING_RW = false;



//RS, RW and Enable can be set to whatever you like
int RS = 8;
int RW = 11;
int Enable = 9;
//DB should be an unseparated group of pins  - because of lazy coding in pushNibble()
int DB[] = {4, 5, 6, 7};  //wire these to DB4~7 on LCD.

//--------------------------------------------------------

//how many lines has the LCD? (don't change here - specify on calling constructor)
int g_num_lines = 2;



//pulse the Enable pin high (for a microsecond).
//This clocks whatever command or data is in DB4~7 into the LCD controller.
void LCD4Bit_mod::pulseEnablePin(){
  digitalWrite(Enable,LOW);
  delayMicroseconds(1);
  // send a pulse to enable
  digitalWrite(Enable,HIGH);
  delayMicroseconds(1);
  digitalWrite(Enable,LOW);
  delay(1);  // pause 1 ms.  TODO: what delay, if any, is necessary here?

}



//push a nibble of data through the the LCD's DB4~7 pins, clocking with the Enable pin.
//We don't care what RS and RW are, here.
void LCD4Bit_mod::pushNibble(int value){
  int val_nibble= value & 0x0F;  //clean the value.  (unnecessary)
  for (int i=DB[0]; i <= DB[3]; i++) {
    digitalWrite(i,val_nibble & 01);
    val_nibble >>= 1;
  }
  pulseEnablePin();
}



//push a byte of data through the LCD's DB4~7 pins, in two steps, clocking each with the enable pin.
void LCD4Bit_mod::pushByte(int value){
  int val_lower = value & 0x0F;
  int val_upper = value >> 4;
  pushNibble(val_upper);
  pushNibble(val_lower);
}





//stuff the library user might call---------------------------------
//constructor.  num_lines must be 1 or 2, currently.
LCD4Bit_mod::LCD4Bit_mod (int num_lines) {
  g_num_lines = num_lines;
  if (g_num_lines < 1 || g_num_lines > 2)
  {
    g_num_lines = 1;
  }
}



void LCD4Bit_mod::commandWriteNibble(int nibble) {
  digitalWrite(RS, LOW);
  if (USING_RW) { digitalWrite(RW, LOW); }
  pushNibble(nibble);
}





void LCD4Bit_mod::commandWrite(int value) {
  digitalWrite(RS, LOW);
  if (USING_RW) { digitalWrite(RW, LOW); }
  pushByte(value);
  //TODO: perhaps better to add a delay after EVERY command, here.  many need a delay, apparently.
}









//print the given character at the current cursor position. overwrites, doesn't insert.
void LCD4Bit_mod::print(int value) {
  //set the RS and RW pins to show we're writing data
  digitalWrite(RS, HIGH);
  if (USING_RW) { digitalWrite(RW, LOW); }
  //let pushByte worry about the intricacies of Enable, nibble order.
  pushByte(value);
}





//print the given string to the LCD at the current cursor position.  overwrites, doesn't insert.
//While I don't understand why this was named printIn (PRINT IN?) in the original LiquidCrystal library, I've preserved it here to maintain the interchangeability of the two libraries.
void LCD4Bit_mod::printIn(char msg[]) {
  uint8_t i;  //fancy int.  avoids compiler warning when comparing i with strlen()'s uint8_t
  for (i=0;i < strlen(msg);i++){
    print(msg[i]);
  }
}

void LCD4Bit_mod::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}


void LCD4Bit_mod::printFloat(double number, uint8_t digits) 
{ 
  // Handle negative numbers
  if (number < 0.0)
  {
     print('-');
     delay(1);
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  printNumber(int_part,10);
  

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    printIn("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    printNumber(toPrint,10);;
    remainder -= toPrint; 
  } 
}




//send the clear screen command to the LCD

void LCD4Bit_mod::clear(){
  commandWrite(CMD_CLR);
  delay(1);
}





// initiatize lcd after a short pause
//while there are hard-coded details here of lines, cursor and blink settings, you can override these original settings after calling .init()
void LCD4Bit_mod::init () {
  pinMode(Enable,OUTPUT);
  pinMode(RS,OUTPUT);
  if (USING_RW) { pinMode(RW,OUTPUT); }
  pinMode(DB[0],OUTPUT);
  pinMode(DB[1],OUTPUT);
  pinMode(DB[2],OUTPUT);
  pinMode(DB[3],OUTPUT);
  delay(50);
  //The first 4 nibbles and timings are not in my DEM16217 SYH datasheet, but apparently are HD44780 standard...
  commandWriteNibble(0x03);
  delay(5);
  commandWriteNibble(0x03);
  delayMicroseconds(100);
  commandWriteNibble(0x03);
  delay(5);
  // needed by the LCDs controller
  //this being 2 sets up 4-bit mode.
  commandWriteNibble(0x02);
  commandWriteNibble(0x02);
  //todo: make configurable by the user of this library.
  //NFXX where
  //N = num lines (0=1 line or 1=2 lines).
  //F= format (number of dots (0=5x7 or 1=5x10)).
  //X=don't care
  int num_lines_ptn = g_num_lines - 1 << 3;
  int dot_format_ptn = 0x00;      //5x7 dots.  0x04 is 5x10
  commandWriteNibble(num_lines_ptn | dot_format_ptn);
  delayMicroseconds(60);
  //The rest of the init is not specific to 4-bit mode.
  //NOTE: we're writing full bytes now, not nibbles.
  // display control:
  // turn display on, cursor off, no blinking
  commandWrite(0x0C);
  delayMicroseconds(60);
  //clear display
  commandWrite(0x01);
  delay(3);
  // entry mode set: 06
  // increment automatically, display shift, entire shift off
  commandWrite(0x06);
  delay(1);//TODO: remove unnecessary delays
}





//non-core stuff --------------------------------------
//move the cursor to the given absolute position.  line numbers start at 1.
//if this is not a 2-line LCD4Bit_mod instance, will always position on first line.
void LCD4Bit_mod::cursorTo(int line_num, int x){
  //first, put cursor home
  commandWrite(CMD_HOME);
  //if we are on a 1-line display, set line_num to 1st line, regardless of given
  if (g_num_lines==1){
    line_num = 1;
  }

  //offset 40 chars in if second line requested
  if (line_num == 2){
    x += 40;
  }

  //advance the cursor to the right according to position. (second line starts at position 40).
  for (int i=0; i<x; i++) {
    commandWrite(0x14);
  }

}



//scroll whole display to left
void LCD4Bit_mod::leftScroll(int num_chars, int delay_time){
  for (int i=0; i<num_chars; i++) {
    commandWrite(CMD_LEFT);
    delay(delay_time);
  }
}
