#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd( 0x27, 16, 2);

unsigned long finished, elapsed;
int Zeit;
int Trick;

//testcomment

void setup()
  { 
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 625;
    TCCR1B |= (1<<CS12);
    TIMSK1 |= (1<<OCIE1A);
    
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("(1) START/RESET");
    lcd.setCursor(0,1);
    lcd.print("(2) STOPP");
    
    Trick = 0;
    Zeit = 0;
    
    pinMode(2, INPUT); //Starttaste
    pinMode(3, INPUT); //Stopptaste
    interrupts();
  }

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  Zeit ++;
  }
  
void displayResult()
  {
    int s, ms;
    unsigned long over;
    elapsed = finished - Zeit;
    over = elapsed % 3600000;
    over = over % 60000;
    s = int(over / 1000);
    ms = over % 1000;
    lcd.print(s);
    lcd.print("s ");
    lcd.print(ms);
    lcd.print("ms");
}

void loop()
  {
    if (digitalRead(2) == HIGH)
      {
        
      lcd.setCursor(0,0);
      lcd.print("GESTARTET ...");
      lcd.setCursor(0,1);
      while(1){
        displayResult();
        delay(100);
        lcd.setCursor(0,0);
        if(digitalRead(2) == LOW)break;
      }
      }
    if (digitalRead(3) == HIGH)
  {
    Trick = 1;
    }
    
    while(Trick == 1) {
      if (digitalRead(3) == LOW){
           displayResult();
           Trick = 0;
           delay(4000);
           lcd.setCursor(0,0);
           lcd.print("(1) START/RESET");
           lcd.setCursor(0,1);
           lcd.print("(2) STOPP");
           break;
        }
    finished = millis();
    delay(200);
    displayResult();
    delay(200);
     
    lcd.clear();
    lcd.setCursor(0,0);   
  }
  
}
