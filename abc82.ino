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
      LED   D13		|         |     D12   12    D6
            3V3		|         |     D11   11    D5
            REF		|         |     D10   10    D4
      D7    A0		|         |      D9    9    D3
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
Blå = data, lila = clk, vit NC, svart = gnd, grå = +5

const char SBC_ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
void set_address(const unsigned long address) {
  unsigned long value = address;
  for (int i = 15; i >= 0; i--) {
    digitalWrite(SBC_ADDR[i], value & 1);
    value = value >> 1;
  }
}
*/

#include <TM1651.h>
#include <PS2KeyAdvanced.h>

const byte OUTS[] PROGMEM = {6,7,8,9,10,11,12}; // Port numbers for the 7 output bits


// PS2 and i2c pins
#define PS2CLK  2
#define PS2DATA 3
#define I2CDIO 4
#define I2CCLK 5


TM1651 Display(I2CCLK,I2CDIO);
int i = 999;
uint16_t c;
PS2KeyAdvanced keyboard;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  keyboard.begin(PS2DATA, PS2CLK);
  keyboard.setNoBreak(1);         // No break codes for keys (when key released)
  keyboard.setNoRepeat(1);        // Don't repeat shift ctrl etc

  Display.displaySet(1);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Display.displayDP(0);
  Display.displayInteger(i);

  Serial.println("frax was here!");
  dumpBin(0x01);
  dumpBin(0x80);
  dumpBin(0x55);
  dumpBin(0xaa);

  pinMode(A0, OUTPUT);
  Serial.println(14);
  Serial.println(A0);
  // Serial.println(LED_BUILTIN);

  // Setup 7 bits for output
  for (int i=0; i++<=7;) {
    pinMode(OUTS[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {

  if( keyboard.available( ) ) {
    // read the next key
    c = keyboard.read( );
    if( c > 0 ) {
      handleKeyboard(c);
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
    }
  }

  /*
  digitalWrite(A0, HIGH);
  delay(500);
  digitalWrite(A0, LOW);
  delay(500);
  */
  /*
  delay(20);
  Display.displayInteger(i--);
  if (i == 0) { i = 999; }
  */
}

void dumpBin(byte b) {
  for (int i=0; i++<=7;) {
    Serial.print (b & 0x80 ? '1' : '0');
    b = b << 1;
  }
  Serial.println();
}

void handleKeyboard(word k) {
  dumpBin(k >> 8);
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
  }
}
