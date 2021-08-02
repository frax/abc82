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

#define STARTRAMLINES 101