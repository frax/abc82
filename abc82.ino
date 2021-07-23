/*
 * https://github.com/techpaul/PS2KeyAdvanced
 * https://github.com/techpaul/PS2KeyAdvanced/blob/master/examples/SimpleTest/SimpleTest.ino
 * https://www.botnroll.com/img/cms/Arduino-Nano-pinout.jpg
 * 
 * frax was here!
 * 
 * IDE Settings...
 * Arduino NANO, ATmega 168
 * 
 * TODO
   Create writeByte function
   Map keys to ABC80 keys, 7bit + strobe
   Add shortcuts for list, run etc.
   Ctrl+Alt+Del = reset
   *Installed* TM1651 digits display


                  ----USB----
  LED + CLK D13		|         |     D12   12    D6
            3V3		|         |     D11   11    D5
            REF		|         |     D10   10    D4
            A0		|         |      D9    9    D3
            A1		|         |      D8    8    D2
            A2		|         |      D7    7    D1
            A3		|         |      D6    6    D0
            A4		|         |		   D5    5    i2c-Clock
            A5		|         |      D4    4    i2c-Data
            A6		|         |      D3    3    PS2-Data
            A7		|         |      D2    2    PS2-Clock
            5V		|         |     GND
            RST		|         |     RST
            GND		|         |      D1    1    TX
            VIN		|         |      D0    0    RX
                  -----------

Displayen...
Blå = data, lila = clk, vit NC, svart = gnd, grå = +5

            PIO      Edge
        A0  15       6  Orange
        A1  14       7  Red
        A2  13       8  Brown
        A3  12       9  Black
        A4  10       2  Blue
        A5   9       3  Green
        A6   8       4  Yellow
        A7   7       1  Purple
        +5          11  White
       GND          12  Grey



A = 0x61 = 0110 0001 
_ = 0x5f = 0101 1111
O = 0x4f = 0100 1111



*/

#include "TM1651.h"
#include <PS2KeyAdvanced.h>

// Pins used for 7-bit keyboard data
const int OUTS[] = {6,7,8,9,10,11,12,13}; // Port numbers for the 7 output bits


// PS2 and i2c pins
#define PS2CLK  2
#define PS2DATA 3
#define I2CDIO 4
#define I2CCLK 5


TM1651 Display(I2CCLK,I2CDIO);
word currentKey;
PS2KeyAdvanced Keyboard;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);

  Keyboard.begin(PS2DATA, PS2CLK);
  Keyboard.setNoBreak(1);         // No break codes for keys (when key released)
  Keyboard.setNoRepeat(1);        // Don't repeat shift ctrl etc

  Display.displaySet(1);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Display.displayDP(0);
  Display.displayInteger(997);

  Serial.println("frax was here!");
  
  // pinMode(A0, OUTPUT);
  // Serial.println(LED_BUILTIN);

  // Setup 7 bits for output + clk
  for (int i=0; i<=7;i++) {
    pinMode(OUTS[i], OUTPUT);
  }

  // Set clk low
  digitalWrite(OUTS[7], LOW);
}

// the loop function runs over and over again forever
void loop() {

  if(Keyboard.available()) {
    // read the next key
    currentKey = Keyboard.read( );
    if( currentKey > 0 ) {
      handleKeyboard(currentKey);
    }
  }
}

void dumpBin(byte b) {
for (unsigned int test = 0x80; test; test >>= 1) {
    Serial.write(b  & test ? '1' : '0');
  }
}

void handleKeyboard(word k) {
  // dumpBin(k);
  // dumpBin(k >> 8);
  
  int translated = getKeycode(k & 0xff);
  Display.displayInteger(translated);
  if (translated == 255) {
    Serial.println(k & 0xff, HEX);      
    return;
  }
  
  // Dump to serial
  dumpBin(k & 0xff);
  Serial.print(' ');
  Serial.print(k & 0xff, HEX);
  Serial.print(" -> ");
  dumpBin(translated);
  Serial.print(' ');
  Serial.print(translated & 0xff, HEX);
  Serial.println();

  // Output...
  byte value = translated;
  for (int i=0;i<=6;i++) { // Only 7-bits
    digitalWrite(OUTS[i], value & 1);
    value = value >> 1;
  }
  digitalWrite(OUTS[7], HIGH);
  delay(40);
  digitalWrite(OUTS[7], LOW);
}

byte getKeycode(byte ps2) {
  switch (ps2) {
    case 0x31:
      return 0x31;
      break;
    case 0x32:
      return 0x32;
      break;
    case 0x33:
      return 0x33;
      break;
    case 0x34:
      return 0x34;
      break;
    case 0x35:
      return 0x35;
      break;
    case 0x36:
      return 0x36;
      break;
    case 0x37:
      return 0x37;
      break;
    case 0x38:
      return 0x38;
      break;
    case 0x39:
      return 0x39;
      break;
    case 0x30:
      return 0x30;
      break;
    case 0x3c:
      return 0x2b;
      break;
    case 0x5f:
      return 0x60;
      break;
    case 0x8b:
      return 0x3c;
      break;
    case 0x51:
      return 0x71;
      break;
    case 0x57:
      return 0x77;
      break;
    case 0x45:
      return 0x65;
      break;
    case 0x52:
      return 0x72;
      break;
    case 0x54:
      return 0x74;
      break;
    case 0x59:
      return 0x79;
      break;
    case 0x55:
      return 0x75;
      break;
    case 0x49:
      return 0x69;
      break;
    case 0x4f:
      return 0x6f;
      break;
    case 0x50:
      return 0x70;
      break;
    case 0x5d:
      return 0x7d;
      break;
    case 0x5e:
      return 0x7e;
      break;
    case 0x1e:
      return 0x0d;
      break;
    case 0x41:
      return 0x61;
      break;
    case 0x53:
      return 0x73;
      break;
    case 0x44:
      return 0x64;
      break;
    case 0x46:
      return 0x66;
      break;
    case 0x47:
      return 0x67;
      break;
    case 0x48:
      return 0x68;
      break;
    case 0x4a:
      return 0x6a;
      break;
    case 0x4b:
      return 0x6b;
      break;
    case 0x4c:
      return 0x6c;
      break;
    case 0x5b:
      return 0x7c;
      break;
    case 0x3a:
      return 0x7b;
      break;
    case 0x5c:
      return 0x27;
      break;
    case 0x15:
      return 0x08;
      break;
    case 0x5a:
      return 0x7a;
      break;
    case 0x58:
      return 0x78;
      break;
    case 0x43:
      return 0x63;
      break;
    case 0x56:
      return 0x76;
      break;
    case 0x42:
      return 0x62;
      break;
    case 0x4e:
      return 0x6e;
      break;
    case 0x4d:
      return 0x6d;
      break;
    case 0x3b:
      return 0x2c;
      break;
    case 0x3d:
      return 0x2e;
      break;
    case 0x3e:
      return 0x2d;
      break;
    case 0x16:
      return 0x09;
      break;
    case 0x1f:
      return 0x20;
      break;      
    default:
      return 0xff;    
      break;
  }
}



/*




      Serial.print( "Value " );
      Serial.print( c, HEX );
      // dumpBin(c);
      Serial.print( " - Status Bits " );
      // Serial.print( c >> 8, HEX );
      dumpBin(c >> 8);
      Serial.print( "  Code " );
      Serial.println( c & 0xFF, HEX );
*/
