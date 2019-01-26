#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

// Workaround for PROGMEM issues under g++: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734 
#ifdef PROGMEM 
#undef PROGMEM 
#define PROGMEM __attribute__((section(".progmem.data"))) 
#endif

#include "Tables.h"

//--------------------------------------------------------------------------------
#define PLAYBACK_RATE  31250L
#define DEBOUNCE_MS    200L
#define DEBOUNCE_TICKS (DEBOUNCE_MS * PLAYBACK_RATE / 1000L)
//--------------------------------------------------------------------------------
// 8 bit drum interface
//--------------------------------------------------------------------------------
class Drum
{
public:
  Drum();
  void setup();
  void updatePot1(uint8_t pot1);
  void updatePot2(uint8_t pot2);
  void hitDrum();
  void update();
  uint8_t getSample();
private:
  bool      m_playing;
  uint8_t   m_startPos;  // 
  uint8_t   m_speed;     // 
  uint16_t  m_impactPos; // Impact sample offset
  uint16_t  m_sinePos;   // Sine sample offset
  uint8_t   m_sample;    // Next sample to play.
};

