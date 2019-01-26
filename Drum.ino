#include <AttinyBassDrum.h>

Drum drum;

void just_drum() {

int i;
  for (i=1000; i<5000; i+=10) {
  drum.updatePot1(i);
  drum.updatePot2(1000);
  drum.hitDrum();
  delay(10000);
  }

}
//--------------------------------------------------------------------------------
// General use utility timer
//--------------------------------------------------------------------------------
uint16_t g_timer = 0;

//--------------------------------------------------------------------------------
// Fast PWM interrupt
//--------------------------------------------------------------------------------
ISR(TIM0_COMPA_vect)
{
  // Update sample at 31.25kHz
  OCR0A = drum.getSample();

  // Update timer if it's running.
  if (g_timer)
  {
    g_timer++;
  }

  // Process next sample.
  drum.update();
}

//--------------------------------------------------------------------------------
// Analogue port read
//--------------------------------------------------------------------------------
void myanalogRead()
{
  ADCSRA |= _BV(ADEN);            // Analog-Digital enable bit
  ADCSRA |= _BV(ADSC);            // Discarte first conversion
  while (ADCSRA & _BV(ADSC)) {};  // Wait until conversion is done
  ADCSRA |= _BV(ADSC);            // Start single conversion
  while (ADCSRA & _BV(ADSC)) {};  // Wait until conversion is done
  ADCSRA &= ~_BV(ADEN);           // Shut down ADC
}

//--------------------------------------------------------------------------------
// Main loop
//--------------------------------------------------------------------------------
void interactive_drum(void)
{
  //------------------------------------------------
  // Pot 1 determines type of drum and param 1.
  // ADC2 (PINB 4, chip pin 3).
  ADMUX = _BV(ADLAR) | _BV(MUX1);
  myanalogRead();
  Serial.print(ADCH); Serial.print("\t");
  drum.updatePot1(ADCH);

  //------------------------------------------------
  // Pot 2 determines trigger mode and param 2.
  // ADC3 (PINB 3, chip pin 2).
  ADMUX = _BV(ADLAR) | _BV(MUX0) | _BV(MUX1);
  myanalogRead();
  Serial.println(ADCH); 
  drum.updatePot2(ADCH);

  //------------------------------------------------
  // Read PINB 2 (chip pin 7) to trigger drum
  if (PINB & _BV(2))
  {
    if (!g_timer)
    {
      drum.hitDrum();
      g_timer++;
    }
    else if (g_timer > DEBOUNCE_TICKS)
    {
      g_timer = 0;
    }
  }
}

void setup( void ) {
  drum.setup();

  // Analogue init.
  ADCSRA |=
    _BV(ADEN) |               //  ADC Enable
    _BV(ADPS1) | _BV(ADPS0);  // Div 8 prescaler

  // Enable interrupts.
  sei();

  Serial.begin(9600);
}

void loop( void ) {
//for (;;) interactive_drum();
for (;;) just_drum();
}


