#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd( 0x27, 16, 2);
//VARIABLEN
int MILLIS;
int SECONDS;
int MINUTES;
int MENUE;
int ZAEHLEN; 

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
    
    pinMode(2, INPUT); //Starttaste
    pinMode(3, INPUT); //Stopptaste
    pinMode(4, INPUT); //Zeitnehmen (und spiechern)
    pinMode(5, INPUT); //Stopp

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 625;
    TCCR1B |= (1<<CS12);
    TIMSK1 |= (1<<OCIE1A);
  }

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

void ZEITZEIGEN(){
    lcd.setCursor(0,0);
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
    lcd.print(".");
}

void Timer(){
      MENUE = 1;
      ZAEHLEN = 1;
      MILLIS = SECONDS = MINUTES = 0;
      TCNT1 = 0;
      lcd.setCursor(0,1);
      lcd.print("(2) STOPP       ");
      lcd.setCursor(0,0);
      lcd.print("                ");
      
      while(ZAEHLEN){
        ZEITZEIGEN();
        lcd.setCursor(0,0);
        if(digitalRead(3)== HIGH){
          ZAEHLEN = 0;
          lcd.setCursor(0,1);
          lcd.print("(3) ZURUCK");
        }
      }
      while(digitalRead(4) != HIGH){
        
      }
}

void loop()
  {
    if(MENUE != 0){
    lcd.setCursor(0,0);
    lcd.print("(1) START       ");
    lcd.setCursor(0,1);
    lcd.print("(2)             ");
    }
    if (digitalRead(2) == HIGH)
    {
      Timer();
    }
}
