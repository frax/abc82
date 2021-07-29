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
   Ctrl + <> = del 0x7f, ??

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

// startram.bas
const char string_0[] PROGMEM = "1000 REM ?NxGruJz{HP_La]K_HLc]Jzt?FtA@?J?{KV^JK{I^{GT{EVnIH@?@pJ|HNKAHW^Nj]";
const char string_1[] PROGMEM = "1010 REM LU\\J~HAA@@nrLCGA{^LyG@?DDK@CtAK\\GNGGNGrCL}KESIwyIwH?@xEwWK@|A{XLKq";
const char string_2[] PROGMEM = "1020 REM I{ZEwUC@OE@`F`jJjwB]@I{XEweG@{IZw@\\@I{XMw\\K@~IwxMH@K@i?rKErIGB@?p@";
const char string_3[] PROGMEM = "1030 REM E^CCEKGD{A^AGAKGH~GOIKH|Ir@GE@@I`A{^IA{A^A?AAAIHI}rJ|@CE`@KpA{VI{Z";
const char string_4[] PROGMEM = "1040 REM Iq@BG@A{TI{PIquA@oIAHMa`LWvCA_I{^NnWCJ@Nv@JJAHU{MTzItpI{TIp{MV~Ir@";
const char string_5[] PROGMEM = "1050 REM ?M@HG@@KDI{V?AAAKHA~GK^iCH|E^qKHSCLzA@MC_OJp{A^AGA{I^@?U?I{^?AAI{^";
const char string_6[] PROGMEM = "1060 REM Ir@BG@?TXA{^EhAKPGNGGNGrCD}KESA{ZIyHj?@KGrNBrNB|IG{EVN?D`NJ]F^L?D`";
const char string_7[] PROGMEM = "1070 REM LyO?H|I{V@NDK`n?HCFKL?D`@NDK`KBnHA@EKG^LyOCHyF^uKPWI{VL_H@LD?`GF^y";
const char string_8[] PROGMEM = "1080 REM BOHJ|xI{ZLZ~Mr|M@GG@{GX{IP@?E`@O`A{ZI{^IyDH``@AeA{XI{VDAbA{ZI{TEyO";
const char string_9[] PROGMEM = "1090 REM DjLIUeHOUDLjDbOCdLIUe@J@C`HiO@@O|@L@C`J?@`@HMy@xI{Z@AeI{XExAG`{IZ@";
const char string_10[] PROGMEM = "1100 REM BG@I{XEHG}@~Ir@BG@I{P?XZCVO@Oi@{hFQCKEUA{^E}O?xiNAABAOJ|{I^qA@@H@{";
const char string_11[] PROGMEM = "1110 REM GlpI|{M^~K_iKHv@IAI@AK`pcd?AH@CVGA@UE@?FAMJ?{Gl]?K]IrpE?_MEtI@{CZI";
const char string_12[] PROGMEM = "1120 REM G@yE^yFEuF@?@@LHAuIVs_A@Mt}IwyNrTIwHK]@ADA?@JEQACBOG^w@\\@Mw]I@w@MD";
const char string_13[] PROGMEM = "1130 REM GTwJ]DEwM?HhMw]IHwJHwBHOGDyLOSLL[@@O@LvE@LED@DLSD@LDw@@LOJ@@?A@NaW";
const char string_14[] PROGMEM = "1140 REM EH|DHCH@sbN@J\\wIxrFPOG_w@\\@Mw]I@wDM@G`wN]@EwM?DJMw]IDwDMDGjwN]DEwM";
const char string_15[] PROGMEM = "1150 REM ?HTMw]IHwDMHGtwN]HGOwAFDDtOBIFJi}HNYK@iGJAEqAG@BIp|IrHn??ADS~?iE{T";
const char string_16[] PROGMEM = "1160 REM ASQDJ?JW{DPHL?WJzuA{lItHIUJDC@Asu~?\\CK\\Qr@";
const char string_17[] PROGMEM = "1170 Z0%=49152%-PEEK(65056%)-SWAP%(PEEK(65057%))";
const char string_18[] PROGMEM = "1180 DIM P$=Z0% AND (Z0%>=0% OR 1023%)";
const char string_19[] PROGMEM = "1190 B%=CALL(13388%-PEEK(13161%),904%)";
const char string_20[] PROGMEM = "1200 POKE B%,1%,41%,0%,9%,235%,42%,28%,254%,14%,5%,9%,126%,35%,214%,63%,56%";
const char string_21[] PROGMEM = "1210 POKE B%+16%,247%,79%,126%,35%,203%,57%,143%,203%,57%,143%,18%,19%,126%";
const char string_22[] PROGMEM = "1220 POKE B%+29%,35%,203%,57%,143%,203%,57%,143%,18%,19%,121%,214%,1%";
const char string_23[] PROGMEM = "1230 Z0%=CALL(B%)";
const char string_24[] PROGMEM = "1240 DEF FNP2%(A%)=PEEK(A%)+SWAP%(PEEK(A%+1%))";
const char string_25[] PROGMEM = "1250 DEF FNU(A%)=A%-65536*(A%<0%)";
const char string_26[] PROGMEM = "1260 P1%=1% : REM If TKN80 detected, patch INP() in BASIC for mode switch";
const char string_27[] PROGMEM = "1270 M%=CALL(B%+59%)";
const char string_28[] PROGMEM = "1280 IF M%=1% PRINT \"No \";";
const char string_29[] PROGMEM = "1290 PRINT \"MEG80 memory card detected\"";
const char string_30[] PROGMEM = "1300 IF INP(8446%)=76% AND INP(8444%)=180% AND (INP(31%) AND 31%)=8% THEN 1310 ELSE 1340";
const char string_31[] PROGMEM = "1310 PRINT \"Started from bootloader\"";
const char string_32[] PROGMEM = "1320 Z%=CALL(B%+190%)";
const char string_33[] PROGMEM = "1330 M%=6%";
const char string_34[] PROGMEM = "1340 ON M% GOTO 1980,1350,1350,1370,1390,1370";
const char string_35[] PROGMEM = "1350 Z%=CALL(B%+168%)";
const char string_36[] PROGMEM = "1360 PRINT \"Map 0 setup complete\"";
const char string_37[] PROGMEM = "1370 OUT 31%,0%";
const char string_38[] PROGMEM = "1380 PRINT \"Memory mapping enabled\"";
const char string_39[] PROGMEM = "1390 DIM P%(2%)";
const char string_40[] PROGMEM = "1400 FOR I%=0% TO 2%";
const char string_41[] PROGMEM = "1410 P%(I%)=CALL(B%+306%,I%)";
const char string_42[] PROGMEM = "1420 PRINT \"IC\" CHR$(49%+I%)\" :\";";
const char string_43[] PROGMEM = "1430 IF P%(I%) THEN 1440 ELSE 1460";
const char string_44[] PROGMEM = "1440 PRINT P%(I%)*8% \"K SRAM\"";
const char string_45[] PROGMEM = "1450 GOTO 1670";
const char string_46[] PROGMEM = "1460 IF I%=2% THEN 1470 ELSE 1660";
const char string_47[] PROGMEM = "1470 F%=CALL(B%+402%)";
const char string_48[] PROGMEM = "1480 V%=PEEK(F%)";
const char string_49[] PROGMEM = "1490 D%=PEEK(F%+1%)";
const char string_50[] PROGMEM = "1500 V0%=PEEK(F%+2%)";
const char string_51[] PROGMEM = "1510 D0%=PEEK(F%+3%)";
const char string_52[] PROGMEM = "1520 IF (V% AND D% AND V0% AND D0%)=255% THEN 1530 ELSE 1550";
const char string_53[] PROGMEM = "1530 PRINT \" WP flash or empty\"";
const char string_54[] PROGMEM = "1540 GOTO 1650";
const char string_55[] PROGMEM = "1550 IF V%=191% AND D%>=181% AND D%<=183% THEN 1560 ELSE 1580";
const char string_56[] PROGMEM = "1560 PRINT 2%**(D%-174%)\"K flash (writable)\"";
const char string_57[] PROGMEM = "1570 GOTO 1650";
const char string_58[] PROGMEM = "1580 IF V%=191% AND D%>=213% AND D%<=215% THEN 1590 ELSE 1610";
const char string_59[] PROGMEM = "1590 PRINT 2%**(D%-206%)\"K flash (writable)\"";
const char string_60[] PROGMEM = "1600 GOTO 1650";
const char string_61[] PROGMEM = "1610 IF V%=V0% AND D%=D0% THEN 1620 ELSE 1640";
const char string_62[] PROGMEM = "1620 PRINT \" WP flash\"";
const char string_63[] PROGMEM = "1630 GOTO 1650";
const char string_64[] PROGMEM = "1640 PRINT \" unknown flash (writable)\"";
const char string_65[] PROGMEM = "1650 GOTO 1670";
const char string_66[] PROGMEM = "1660 PRINT \" empty\"";
const char string_67[] PROGMEM = "1670 OUT 508%+SWAP%(I%),P%(I%)";
const char string_68[] PROGMEM = "1680 NEXT I%";
const char string_69[] PROGMEM = "1690 IF P%(0%)<9% THEN 1700 ELSE 1720";
const char string_70[] PROGMEM = "1700 PRINT \"Error: IC1 not SRAM or too small\"";
const char string_71[] PROGMEM = "1710 GOTO 1980";
const char string_72[] PROGMEM = "1720 IF M%=5% THEN 1730 ELSE 1750";
const char string_73[] PROGMEM = "1730 PRINT \"Already running with BASIC in SRAM\"";
const char string_74[] PROGMEM = "1740 GOTO 1980";
const char string_75[] PROGMEM = "1750 Z0%=CALL(B%+260%)";
const char string_76[] PROGMEM = "1760 PRINT \"Map 1-3 setup complete\"";
const char string_77[] PROGMEM = "1770 IF M%<>6% THEN Z0%=CALL(B%+207%)";
const char string_78[] PROGMEM = "1780 W%=INP(1212%)";
const char string_79[] PROGMEM = "1790 B0%=INP(1468%)";
const char string_80[] PROGMEM = "1800 IF B0%<>124% THEN 1810 ELSE 1850";
const char string_81[] PROGMEM = "1810 PRINT \"TKN80 found at\";B0%/4%;\"K in\";W%;\"-column mode\"";
const char string_82[] PROGMEM = "1820 P0%=(B0%/32%) OR 192%";
const char string_83[] PROGMEM = "1830 OUT 30772%,P0%,31284%,P0%,63572%,P0%,64084%,P0%";
const char string_84[] PROGMEM = "1840 GOTO 1880";
const char string_85[] PROGMEM = "1850 P1%=0%";
const char string_86[] PROGMEM = "1860 W%=40%";
const char string_87[] PROGMEM = "1870 PRINT \"No TKN80 found\"";
const char string_88[] PROGMEM = "1880 Z0%=CALL(B%+490%)";
const char string_89[] PROGMEM = "1890 PRINT \"Memory contents copied to SRAM\"";
const char string_90[] PROGMEM = "1900 OUT 31%,1%";
const char string_91[] PROGMEM = "1910 IF W%=80% THEN Z0%=CALL(B%+587%,4%)";
const char string_92[] PROGMEM = "1920 PRINT \"Now running with BASIC in SRAM\"";
const char string_93[] PROGMEM = "1930 Z0%=CALL(B%+543%)";
const char string_94[] PROGMEM = "1940 IF P1% THEN 1950 ELSE 1970";
const char string_95[] PROGMEM = "1950 Z0%=CALL(B%+736%)";
const char string_96[] PROGMEM = "1960 PRINT \"TKN80 mode switch BASIC patch installed\"";
const char string_97[] PROGMEM = "1970 IF FNP2%(65052%)=49152% THEN POKE 65053%,128%";
const char string_98[] PROGMEM = "1980 B=FNU(FNP2%(65052%))";
const char string_99[] PROGMEM = "1990 PRINT \"BASIC usable RAM:\" INT((65536-B)/1024);";
const char string_100[] PROGMEM= "2000 PRINT \"K (BOFA\" B \")\"";
const char string_101[] PROGMEM= "2010 CHAIN \"\"";

const char *const string_table[] = {
  string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9,
  string_10,string_11,string_12,string_13,string_14,string_15,string_16,string_17,string_18,string_19,

  string_20,string_21,string_22,string_23,string_24,string_25,string_26,string_27,string_28,string_29,
  string_30,string_31,string_32,string_33,string_34,string_35,string_36,string_37,string_38,string_39,

  string_40,string_41,string_42,string_43,  string_44,string_45,string_46,string_47,string_48,string_49,
  string_50,string_51,string_52,string_53,string_54,string_55,string_56,string_57,string_58,string_59,

  string_60,string_61,string_62,string_63,string_64,string_65,string_66,string_67,string_68,string_69,
  string_70,string_71,string_72,string_73,string_74,string_75,string_76,string_77,string_78,string_79,

  string_80,string_81,string_82,string_83,string_84,string_85,string_86,string_87,string_88,string_89,
  string_90,string_91,string_92,string_93,string_94,string_95,string_96,string_97,string_98,string_99,
  string_100,string_101
};

// Pins used for 7-bit keyboard data + bit 8 for clk
const int OUTS[] = {6,7,8,9,10,11,12,13}; // Port numbers for the 7 output bits
const byte ASCII[256] = {
  0,0,0,0,0,0,0,0,0,0,0x0d,0,0,0x0d,0,0,
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
  Display.displayNum(0,10);
  Display.displayNum(1,8);
  Display.displayNum(2,2);

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
/*
  Serial.print("Keycode: ");
  Serial.print(k&0xff, HEX);
  dumpBin(k & 0xff);
  Serial.println();

  Serial.print(" Status: ");
  dumpBin(k >> 8);
  Serial.println();
*/

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
  outputKeycode(translated);
}

void outputKeycode(byte c) {
  // Serial.println(c, HEX);
  byte value = c;
  for (int i=0;i<=6;i++) { // Only 7-bits
    digitalWrite(OUTS[i], value & 1);
    value = value >> 1;
  }
  digitalWrite(OUTS[7], HIGH);
  delay(c == 0x0d ? 100 : 4);  // Enter needs longer delay
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

void sendString(String s) {
  for (int i=0;i<=s.length();i++) {
    outputKeycode(ASCII[s[i]]);
  }
}

void doMacro(byte ps2) {
  switch (ps2) {
    case 0x61:
      sendString("list\n");
      break;
    case 0x62:
      sendString("run\n");
      break;
    case 0x63:
      sendString("\n");
      break;
    case 0x64:
      sendString("z9%=call(630)\n");
      break;
  }



  Serial.println(ps2);

  if (ps2==0x6c) {  // F12
    for (int i=0; i<=101;i++) {
      xxx(string_table[i]);
    }
  }  
}

void xxx(char *line) {
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



