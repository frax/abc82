# abc82, PS/2 for the ABC80
Från början ett ersättningskretskort till det numeriska tangentbordet ABCxx. Men jag ledsnade på att alltid behöva fixa tangentbordet så fort jag tog fram en ABC80 så jag byggde ut kortet med en Arduino Nano som läser PS/2 och konverterar detta till koder för ABC80.

## Saker som fungerar
 - Alla tangentbordskoder på plats inkl. shift, ctrl och shift+ctrl
 - CRTL+C ligger på Escape
 - Backspace = pil vänster
 - F12 skjuter in HPAs startram.bas för dig med MEG80
 - F1 = list, F2 = run, F4 = z9%=call(630)

## Todo
 - Fixa nya kretskort
 - Reducera minneshanteringen, påbörjat
 - Ctrl+alt+del = reset
 - Pil upp/ner för Smartaid
 - Numeriska tangentdelen på PS/2-tangenbordet måste mappas