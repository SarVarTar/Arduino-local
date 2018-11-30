#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd( 0x27, 16, 2);

//VARIABLEN

int MILLIS = 0;
int SECONDS = 0;
int MINUTES = 0;
int MENUE = -1;
int TIMESTAMP = 0;
int HIGHSCORE[5];
int ZWISCHENZEIT[10];
int TASTE1 = 1;
int TASTE2 = 1;
int TASTE3 = 1;

void setup()
  {    
    //MENUE AUFBAUEN
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("(1) START");
    lcd.setCursor(0,1);
    lcd.print("(2) ");
    lcd.setCursor(0,0);

    //TASTEN INITIALISIEREN
    pinMode(2, INPUT); //Starttaste
    pinMode(3, INPUT); //Stopptaste
    pinMode(4, INPUT); //Zeitnehmen (und spiechern)
    pinMode(5, INPUT); //Stopp
    
    //TIMER EINSTELLEN
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 625;
    TCCR1B |= (1<<CS12);
    TIMSK1 |= (1<<OCIE1A);
  }

//ZÄHLROUTINE
ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  if(MILLIS <= 99)
  {
    MILLIS += 1;
  }
  else
  {
    MILLIS = 0;
    SECONDS += 1;
  }
  if(SECONDS > 59)
  {
    SECONDS = 0;
    MINUTES += 1;
  }
}

//UMWANDLUNG IN MILLIS
void TOTIMESTAMP(){
      TIMESTAMP = MINUTES * 60 * 100 + SECONDS * 100 + MILLIS;
  }

//UMWANDLUNG IN MINUTES SECONDS UND MILLIS
void FROMTIMESTAMP(int ZEIT){
  MINUTES = ZEIT % 100 % 60;
  SECONDS = (ZEIT - MINUTES * 100 * 60) % 100;
  MILLIS = (ZEIT - MINUTES * 100 * 60 - SECONDS * 100);
}

//ABLEGEN DES TIMESTAMPS IM ZWISCHENZEITENSPEICHER
void ZWISCHENZEITSPEICHERN(int RUNDE, int ZEIT){
      ZWISCHENZEIT[RUNDE] = ZEIT;
  }

//HIGHSCORES AKTUALISIEREN
/*void HSSPEICHERN(){
      for(int i = 0; i <= 4; i++){
         if(HIGHSCORE[i] > TIMESTAMP){
            int HELP, HELP2;
            HELP = HIGHSCORE[i];
            HIGHSCORE[i] = TIMESTAMP;
            
            for(int k = i; k >= 1; k--){
              HELP2 = HIGHSCORE[k - 1];
              HIGHSCORE[k - 1] = HELP;
              HELP = HELP2;
              }
              
          } 
      }
  } */

//AKTUELLE ZEIT FORMATIERT AUSGEBEN
void DRUCKEZEIT(){
    //lcd.setCursor(0,0);
    if(MINUTES < 10)
    {
      lcd.print("0");
    }
    lcd.print(MINUTES);
    lcd.print(".");
    if(SECONDS < 10)
    {
      lcd.print("0");
    }
    lcd.print(SECONDS);
    lcd.print(".");
    lcd.print(MILLIS);
}

//gibt zurück wie viele Tasten nicht gedrückt sind
int TASTENSTATUS()
{
  int TASTENSTATUS;
  TASTENSTATUS = TASTE1 + TASTE2 + TASTE3;
  return TASTENSTATUS;
}

//prüft ob eine Eingabe gemacht wurde und keine Tasten mehr gedrückt sind
int TASTE()
{
  int TASTE = 0;
  //WENN TASTE INAKTIV UNABHÄNGIG VON VORHERIGEM ZUSTAND
  if(digitalRead(2)==LOW) TASTE1 = 1;
  //WENN TASTE AKTIV UND ZUVOR ALLE TASTEN LOSGELASSEN WURDEN
  if(digitalRead(2)== HIGH && TASTENSTATUS()==3){
    TASTE1 = 0;
    TASTE = 1;
  }
  if(digitalRead(3)==LOW) TASTE2 = 1;
  if(digitalRead(3)== HIGH && TASTENSTATUS()==3 ){
    TASTE2 = 0;
    TASTE = 2;
  } 
  if(digitalRead(4)==LOW) TASTE3 = 1;
  if(digitalRead(4)== HIGH && TASTENSTATUS()==3 ){
    TASTE3 = 0;
    TASTE = 3;
  }
  return TASTE;
}

void LEEREZWISCHENZEITEN(){
  for(int i = 0; i < 10; i++)
  {
    ZWISCHENZEIT[i] = 0;
  }
}

void STOPPUHR()
{
  MENUE = 1;
  int RUNDE = 0;
  int ZEIT = 0;
  MILLIS = SECONDS = MINUTES = 0;
  TCNT1 = 0;
  lcd.setCursor(0,1);
  lcd.print("(1)ZZ (2)S (3)<-");
  lcd.setCursor(0,0);
  lcd.print("                ");
  LEEREZWISCHENZEITEN();
  while(1){
    lcd.setCursor(0,0);
    DRUCKEZEIT();
    switch(TASTE()){
      case 1:   TOTIMESTAMP();
                ZEIT = TIMESTAMP - ZEIT;
                ZWISCHENZEITSPEICHERN(RUNDE, ZEIT); 
                RUNDE++;
                delay(1000);
                break;
      case 2:   TOTIMESTAMP();
                ZEIT = TIMESTAMP - ZEIT;
                ZWISCHENZEITSPEICHERN(RUNDE, ZEIT); 
                RUNDE = 10;
                TIMESTAMP = 0;
                delay(1000);
                break;  
      case 3:   RUNDE = 10;
                TIMESTAMP = 0;  
                delay(1000);
      default:  break;
      }
    if(RUNDE == 10) break;
  }
}

void ZWISCHENZEITENMENU(){
      int RUNDE = 0;
      MENUE = 3;
      lcd.setCursor(0,0);
      lcd.print(RUNDE+1);
      lcd.print(":");
      FROMTIMESTAMP(ZWISCHENZEIT[RUNDE]);
      DRUCKEZEIT();
      lcd.setCursor(0,1);
      lcd.print("(1)v (2)   (3)<-");
      while(RUNDE != 10)
      {
        switch(TASTE()){
          case 1:   if(RUNDE < 9)RUNDE++;
                    if(RUNDE == 9)
                    {
                      lcd.setCursor(0,1);
                      lcd.print("(1)  (2)^  (3)<-");
                    }
                    else // RUNDE zwischen 1 und 8
                    {
                      lcd.setCursor(0,1);
                      lcd.print("(1)v (2)^  (3)<-");
                    }
                    lcd.setCursor(0,0);
                    lcd.print(RUNDE+1);
                    lcd.print(":");
                    FROMTIMESTAMP(ZWISCHENZEIT[RUNDE]);
                    DRUCKEZEIT(); 
                    break;          
          case 2:   if(RUNDE > 0) RUNDE--;
                    if(RUNDE == 0)
                    {
                      lcd.setCursor(0,1);
                      lcd.print("(1)v (2)   (3)<-");
                    }
                    else // RUNDE zwischen 1 und 8
                    {
                      lcd.setCursor(0,1);
                      lcd.print("(1)v (2)^  (3)<-");
                    }
                    lcd.setCursor(0,0);
                    lcd.print(RUNDE+1);
                    lcd.print(":");
                    FROMTIMESTAMP(ZWISCHENZEIT[RUNDE]);
                    DRUCKEZEIT(); 
                    break; 
          case 3:   RUNDE = 10;
                    break;
          default:  break;
      }
    }
}

void GESAMTZEIT(){
  int ZEIT;
  for(int i = 0; i < 10; i++){
     ZEIT += ZWISCHENZEIT[i];
  }
  FROMTIMESTAMP(ZEIT);
  DRUCKEZEIT();
  lcd.setCursor(0,1);
  lcd.print("(1)  (2)   (3)<-");
  while(1){
    if(TASTE()==3)break;
    delay(1000);
  }
  MENUE = 4;
}

void SPEICHERMENU(){
  int EBENE = 0;
  while(EBENE != -2){
  EBENE = 0;
  if(MENUE != 2)
  {
  MENUE = 2;
  lcd.setCursor(0,1);
  lcd.print("(1)ZZ(2)GZ (3)<-");
  lcd.setCursor(0,0);
  lcd.print("                ");
  }
    while(EBENE == 0){
      switch(TASTE()){
        case 1: ZWISCHENZEITENMENU();
                EBENE = -1;
                break;
        case 2: GESAMTZEIT();
                EBENE = -1;
                break;
        case 3: EBENE = -2;
                break;
        default: break;
      }
    }
  } 
}

//prüft ob das Menu neu angezeigt werden muss und wartet auf Nutzereingabe
void loop(){
  if(MENUE != 0)
  {
    MENUE = 0;
    lcd.setCursor(0,0);
    lcd.print("(1) STOPPUHR    ");
    lcd.setCursor(0,1);
    lcd.print("(2) ZEITEN      ");
  }
  switch(TASTE()){
    case 1: STOPPUHR();
    case 2: SPEICHERMENU();
            break;
    case 3: break;
    default: break;
  }
}
