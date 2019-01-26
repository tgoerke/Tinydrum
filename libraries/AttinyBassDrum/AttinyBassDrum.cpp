#include "AttinyBassDrum.h"

//--------------------------------------------------------------------------------
// 8 bit drum implementation
//--------------------------------------------------------------------------------

// constructor
Drum::Drum() {
  m_playing = false;
  m_startPos = 0;
  m_speed = 0;
  m_impactPos = 0;
  m_sinePos = 0;
  m_sample = 0;
}

// public functions
void Drum::setup() {

  // PWM output on PORTB0 = pin 5.
  DDRB = _BV(0);

  // PWM init, 8Mhz / 256 gives 31.25kHz
  TCCR0A =
    _BV(COM0A1) |             // Clear OC0A/OC0B on Compare Match.
    _BV(WGM00) |_BV(WGM01);   // Fast PWM, top 0xff, update OCR at bottom.
  TCCR0B = _BV(CS00);         // No prescaling, full 8MHz operation.
  TIMSK = _BV(OCIE0A);       // Timer/Counter0 Output Compare Match A Interrupt Enable
}

void Drum::updatePot1(uint8_t pot1)
{
  m_startPos = pot1;
}
void Drum::updatePot2(uint8_t pot2)
{
  m_speed = pot2;
}

void Drum::hitDrum()
{
  m_playing = false;
  m_impactPos = m_startPos << 8;
  m_sinePos = 0;
  m_playing = true;
}

void Drum::update()
{
  if (m_playing)
  {
    // Increment the impact sample pos by the speed and use it's value
    // to update the sine pos.
    m_impactPos += m_speed;
    m_sinePos += pgm_read_byte(&impact_tbl[(m_impactPos >> 8) & 0xff]);
    m_sample = pgm_read_byte(&sine_tbl[(m_sinePos >> 4) & 0xff]);

    // Stop the drum at the end of the sample.
    if (m_impactPos > 0xff00)
    {
      m_playing = false;
    }
  }
  else if (m_sample)
  {
    // Fade out sample if nothing playing
    m_sample--;
  }
}

uint8_t Drum::getSample()
{
  return m_sample;
}


