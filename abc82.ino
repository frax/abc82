/*
 * https://github.com/techpaul/PS2KeyAdvanced
 * https://github.com/techpaul/PS2KeyAdvanced/blob/master/examples/SimpleTest/SimpleTest.ino
 * https://www.botnroll.com/img/cms/Arduino-Nano-pinout.jpg
 *
 * PS2, ABC, Kommentar
 * \t(..)\t(..)\t(.*)
 * case 0x$1:\t\t//$3\n\treturn 0x$2;\n\tbreak;
 * 
 * #define PS2_BREAK   0x8000
 * #define PS2_SHIFT   0x4000
 * #define PS2_CTRL    0x2000
 * #define PS2_CAPS    0x1000
 * #define PS2_ALT      0x800
 * #define PS2_ALT_GR   0x400
 * #define PS2_GUI      0x200
 * #define PS2_FUNCTION 0x100
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
    currentKey = Keyboard.read();
    if(currentKey > 0) {
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
  Serial.print("Keycode: ");
Serial.print(k&0xff, HEX);  
  dumpBin(k & 0xff);
  Serial.println();

  Serial.print(" Status: ");
  dumpBin(k >> 8);
  Serial.println();

/*  
  if (k & PS2_BREAK) {
    Serial.println("Break");
  }
  if (k & PS2_SHIFT) {
    Serial.println("Shift");    
  }
  if (k & PS2_CTRL) {
    Serial.println("Ctrl");    
  }
  if (k & PS2_CAPS) {
    Serial.println("Caps lock");    
  }
  if (k & PS2_ALT) {
    Serial.println("Alt");    
  }
  if (k & PS2_ALT_GR) {
    Serial.println("Alt gr");    
  }
  if (k & PS2_GUI) {
    Serial.println("GUI");    
  }
  if (k & PS2_FUNCTION) {
    Serial.println("Function");    
  }
*/

  int translated = 255;  // Default to fail

  if (k & PS2_SHIFT) { // Get shifted codes...
    translated = getShiftedKeycode(k & 0xff);
  }  

  if (translated == 255) {
    translated = getKeycode(k & 0xff);
  }

  // Check special cases
  if (translated == 255) {
    translated = getSpecial(k & 0xff);    
  }

  if (translated == 255) {
    if (k & PS2_FUNCTION) {
      doMacro(k & 0xff);
    }
  }

  if (translated == 255) return;
  Display.displayInteger(translated);

  // Output...
  byte value = translated;
  for (int i=0;i<=6;i++) { // Only 7-bits
    digitalWrite(OUTS[i], value & 1);
    value = value >> 1;
  }
  digitalWrite(OUTS[7], HIGH);
  delay(5);
  digitalWrite(OUTS[7], LOW);
}

byte getKeycode(byte ps2) {
  switch (ps2) {
    case 0x31:		// 1
      return 0x31;
      break;
    case 0x32:		// 2
      return 0x32;
      break;
    case 0x33:		// 3
      return 0x33;
      break;
    case 0x34:		// 4
      return 0x34;
      break;
    case 0x35:		// 5
      return 0x35;
      break;
    case 0x36:		// 6
      return 0x36;
      break;
    case 0x37:		// 7
      return 0x37;
      break;
    case 0x38:		// 8
      return 0x38;
      break;
    case 0x39:		// 9
      return 0x39;
      break;
    case 0x30:		// 0
      return 0x30;
      break;
    case 0x3c:		// +/?
      return 0x2b;
      break;
    case 0x5f:		// É
      return 0x60;
      break;
    case 0x8b:		// </>
      return 0x3c;
      break;
    case 0x51:		// q
      return 0x71;
      break;
    case 0x57:		// w
      return 0x77;
      break;
    case 0x45:		// e
      return 0x65;
      break;
    case 0x52:		// r
      return 0x72;
      break;
    case 0x54:		// t
      return 0x74;
      break;
    case 0x59:		// y
      return 0x79;
      break;
    case 0x55:		// u
      return 0x75;
      break;
    case 0x49:		// i
      return 0x69;
      break;
    case 0x4f:		// o
      return 0x6f;
      break;
    case 0x50:		// p
      return 0x70;
      break;
    case 0x5d:		// å
      return 0x7d;
      break;
    case 0x5e:		// ü
      return 0x7e;
      break;
    case 0x1e:		// enter
      return 0x0d;
      break;
    case 0x41:		// a
      return 0x61;
      break;
    case 0x53:		// s
      return 0x73;
      break;
    case 0x44:		// d
      return 0x64;
      break;
    case 0x46:		// f
      return 0x66;
      break;
    case 0x47:		// g
      return 0x67;
      break;
    case 0x48:		// h
      return 0x68;
      break;
    case 0x4a:		// j
      return 0x6a;
      break;
    case 0x4b:		// k
      return 0x6b;
      break;
    case 0x4c:		// l
      return 0x6c;
      break;
    case 0x5b:		// ö
      return 0x7c;
      break;
    case 0x3a:		// ä
      return 0x7b;
      break;
    case 0x5c:		// /*
      return 0x27;
      break;
    case 0x15:		// pil, vänster
      return 0x08;
      break;
    case 0x5a:		// z
      return 0x7a;
      break;
    case 0x58:		// x
      return 0x78;
      break;
    case 0x43:		// c
      return 0x63;
      break;
    case 0x56:		// v
      return 0x76;
      break;
    case 0x42:		// b
      return 0x62;
      break;
    case 0x4e:		// n
      return 0x6e;
      break;
    case 0x4d:		// m
      return 0x6d;
      break;
    case 0x3b:		// ,;
      return 0x2c;
      break;
    case 0x3d:		// .:
      return 0x2e;
      break;
    case 0x3e:		// -_
      return 0x2d;
      break;
    case 0x16:		// pil, höger
      return 0x09;
      break;
    case 0x1f:		// space
      return 0x20;
      break;    
    default:
      return 0xff;    
      break;
  }
}

byte getShiftedKeycode(byte ps2) {
  switch (ps2) {
    case 0x31:		//1
			return 0x21;
			break;
		case 0x32:		//2
			return 0x22;
			break;
		case 0x33:		//3
			return 0x23;
			break;
		case 0x34:		//4
			return 0x24;
			break;
		case 0x35:		//5
			return 0x25;
			break;
		case 0x36:		//6
			return 0x26;
			break;
		case 0x37:		//7
			return 0x2f;
			break;
		case 0x38:		//8
			return 0x28;
			break;
		case 0x39:		//9
			return 0x29;
			break;
		case 0x30:		//0
			return 0x3d;
			break;
		case 0x3c:		//+/?
			return 0x3f;
			break;
		case 0x5f:		//É
			return 0x40;
			break;
		case 0x8b:		//</>
			return 0x3e;
			break;
		case 0x51:		//q
			return 0x51;
			break;
		case 0x57:		//w
			return 0x57;
			break;
		case 0x45:		//e
			return 0x45;
			break;
		case 0x52:		//r
			return 0x52;
			break;
		case 0x54:		//t
			return 0x54;
			break;
		case 0x59:		//y
			return 0x59;
			break;
		case 0x55:		//u
			return 0x55;
			break;
		case 0x49:		//i
			return 0x49;
			break;
		case 0x4f:		//o
			return 0x4f;
			break;
		case 0x50:		//p
			return 0x50;
			break;
		case 0x5d:		//å
			return 0x4d;
			break;
		case 0x5e:		//ü
			return 0x4e;
			break;
		case 0x1e:		//enter
			return 0x0d;
			break;
		case 0x41:		//a
			return 0x31;
			break;
		case 0x53:		//s
			return 0x53;
			break;
		case 0x44:		//d
			return 0x44;
			break;
		case 0x46:		//f
			return 0x46;
			break;
		case 0x47:		//g
			return 0x47;
			break;
		case 0x48:		//h
			return 0x48;
			break;
		case 0x4a:		//j
			return 0x4a;
			break;
		case 0x4b:		//k
			return 0x4b;
			break;
		case 0x4c:		//l
			return 0x4c;
			break;
		case 0x5b:		//ö
			return 0x5c;
			break;
		case 0x3a:		//ä
			return 0x5b;
			break;
		case 0x5c:		///*
			return 0x2a;
			break;
		case 0x15:		//pil, vänster
			return 0x08;
			break;
		case 0x5a:		//z
			return 0x5a;
			break;
		case 0x58:		//x
			return 0x58;
			break;
		case 0x43:		//c
			return 0x43;
			break;
		case 0x56:		//v
			return 0x56;
			break;
		case 0x42:		//b
			return 0x42;
			break;
		case 0x4e:		//n
			return 0x4e;
			break;
		case 0x4d:		//m
			return 0x4d;
			break;
		case 0x3b:		//,;
			return 0x3b;
			break;
		case 0x3d:		//.:
			return 0x3a;
			break;
		case 0x3e:		//-_
			return 0x5f;
			break;
		case 0x16:		//pil, höger
			return 0x09;
			break;
		case 0x1f:		//space
			return 0x20;
			break;
    default:
      return 0xff;    
      break;
  }
}

byte getSpecial(byte ps2) {
  switch (ps2) {
    case 0x1c:    // Backspace = pil vänster
      return 0x08;
    case 0x1b:    // ESC = CTRL + c
      return 0x03;
    default:
      return 0xff;
  }
}

void doMacro(byte ps2) {
  String macro = "";
  switch (ps2) {
    case 0x61:
      macro = "LIST\n";
      break;
  }

  if (macro != "") {
    Serial.println("Doing macro:");
    Serial.println(macro);
  }

}