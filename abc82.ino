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
  Map keys to ABC80 keys, 7bit + strobe
  Add shortcuts for list, run, run lib etc.
  Ctrl+Alt+Del = reset
  Ctrl + <> = del 0x7f, ??
  Smartaid mapping, cursor
  Style fix, variables, functions etc.  

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

*/

// If you have an old Gotek led display, define this and
// attach it to pins 4 (data) and 5 (clk) and abc82
// will output each pressed key to the display.
// Great for debugging, quit useless othewise :)
#define I2CDISPLAY
#ifdef I2CDISPLAY
  #include "TM1651.h"
#endif

// If defined, F12 will enter hpa's startram.bas used to boot up the MEG80
// startram.bas is quite big, including it will probablt not fit in a
// ATmega168 but if your running on a 328 you probably are fine.
// Booting your MEG80 from flash or don't have one? Then you don't need this.
#define STARTRAM
#ifdef STARTRAM
  #include "startram.h"
#endif

#include <PS2KeyAdvanced.h>


// Pins used for 7-bit keyboard data + bit 8 for clk
// LSB first
const int OUTS[] = {6,7,8,9,10,11,12,13};

// This array is for mapping ASCII chars to keycodes so we can
// send "list\n" to the ABC80. I realized to late that is a
// 100% 1-1 mapping and no translation was necessary, except
// for \n or \r or something.
// ÅÄÖ, sol, Ü etc is mapped using ISO646-SE2
// https://sv.wikipedia.org/wiki/ISO/IEC_646
const byte ASCII[128] = {
  0,0,0,0,0,0,0,0x07,0,0,0x0d,0,0,0x0d,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  // SP,!,",#,$,%,&,',(,),*,+,,,-,.,/
  0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
  // 0,1,2,3,4,5,6,7,8,9,:,;,<,=,>,?
  0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
  // @,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O
  // @ = É = 12 = 0x40
  0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
  // P,Q,R,S,T,U,V,W,X,Y,Z,[,\,],^,_    [ = Ä, \ = Ö, ] = Å, ^ = Ü
  0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
  // `,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o    ` = é
  0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
  // p,q,r,s,t,u,v,w,x,y,z,{,|,},~,DEL?
  0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f
};

// ps2 really need these pins coz they use interrupts
// See https://github.com/techpaul/PS2KeyAdvanced for more info
// but feel free to move the i2c pin to where ever.
#define PS2CLK  2
#define PS2DATA 3
#define I2CDIO 4
#define I2CCLK 5

// Globals
PS2KeyAdvanced Keyboard;
word CurrentKey;
#ifdef I2CDISPLAY
TM1651 Display(I2CCLK,I2CDIO);
#endif

// Initalize stuff
void setup() {
  Serial.begin(115200);
  Serial.println("frax was here!");

  Keyboard.begin(PS2DATA, PS2CLK);
  Keyboard.setNoBreak(1);         // No break codes for keys (when key released)
  Keyboard.setNoRepeat(1);        // Don't repeat shift ctrl etc

#ifdef I2CDISPLAY
  Display.displaySet(1);          //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Display.displayDP(0);
  Display.displayNum(0,10);       // A
  Display.displayNum(1,8);        // 8
  Display.displayNum(2,2);        // 2
#endif

  
  // Setup 7 bits for output + clk and set clk low
  for (int i=0; i<=7;i++) {
    pinMode(OUTS[i], OUTPUT);
  }
  digitalWrite(OUTS[7], LOW);
}

// the loop function runs over and over again forever
void loop() {
  if(Keyboard.available()) {
    CurrentKey = Keyboard.read();
    if(CurrentKey > 0) {
      handleKeyboard(CurrentKey);
    }
  }
}

void handleKeyboard(word k) {
/*
  Serial.print("Keycode: ");
  Serial.print(k&0xff, HEX);
  dumpBin(k & 0xff);
  Serial.println();

  Serial.print(" Status: ");
  dumpBin(k >> 8);
  Serial.println();
*/

  int translated = 255;  // Default to fail

  // Need to add ctrl and ctrl + shift

  if (k & PS2_SHIFT) { // Get shifted codes...
    translated = getShiftedKeycode(k & 0xff);
  }  

  if (translated == 255) {
    translated = getKeycode(k & 0xff);
  }

  // Do cool stuff like esc = ctrl+x, handle backspace
  if (translated == 255) {
    translated = getSpecial(k & 0xff);    
  }

  // No hits, ok let's do function keys macros
  if (translated == 255) {
    if (k & PS2_FUNCTION) {
      doMacro(k & 0xff);
    }
  }

  if (translated == 255) return;
  #ifdef I2CDISPLAY
  Display.displayInteger(translated);
  #endif

  // Output...
  outputKeycode(translated);
}

void outputKeycode(byte c) {
  byte value = c;
  for (int i=0;i<=6;i++) { // Only 7-bits
    digitalWrite(OUTS[i], value & 1);
    value = value >> 1;
  }
  digitalWrite(OUTS[7], HIGH);
  delay(c == 0x0d ? 100 : 4);  // Enter needs longer delay so basic has enuff time to parse
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

// Sends a string to the ABC80
// Used by macros below
void sendString(String s) {
  for (int i=0;i<=s.length();i++) {
    outputKeycode(ASCII[s[i]]);
  }
}

void doMacro(byte ps2) {
  switch (ps2) {
    case 0x61:          // f1
      sendString("list\n");
      break;
    case 0x62:
      sendString("run\n");
      break;
    case 0x64:
      sendString("z9%=call(630)\n");
      break;
    case 0x6a:          // f10
      sendString("run lib\n");
      break;
  }

  // startram.bas?
  #ifdef STARTRAM
  if (ps2==0x6c) {  // F12
    for (int i=0; i<=STARTRAMLINES;i++) {
      dumpProgmemString(string_table[i]);
    }
  }
  #endif
}

#ifdef STARTRAM
void dumpProgmemString(char *line) {
  int counter = 0;
  char c = 255;
  while(c != 0) {
      c = pgm_read_byte_near(line + counter); 
      outputKeycode(ASCII[c]);
      counter++;
  }
  outputKeycode(ASCII['\n']);
  Serial.println();
}
#endif

void dumpBin(byte b) {
for (unsigned int test = 0x80; test; test >>= 1) {
    Serial.write(b  & test ? '1' : '0');
  }
}
