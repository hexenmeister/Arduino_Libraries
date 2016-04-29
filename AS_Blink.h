/*

 Copyright (c) 2015 Alexander Schulz.  All right reserved.


 This program is free software; you can redistribute it and/or 
 modify it under the terms of the GNU Lesser General Public 
 License  as published by the Free Software Foundation; either 
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 General Public License for more details.

 You should have received a copy of the GNU Lesser General Public 
 License along with this program; 
 if not, see <http://www.gnu.org/licenses/>.
 
*/

#ifndef AS_BLINK_h
#define AS_BLINK_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define MAX_U_LONG 4294967295;

class AS_Blink {
  
 typedef unsigned long (*TimeFuncPtr)(void);
  
 public:
  
  /*
   * Constructor.
   * params: pin - LED pin
   *         fTimePtr - pointer to millis() method (optional)
   */
  AS_Blink(uint8_t pin, TimeFuncPtr fTimePtr = &millis) {
    _pin = pin;
    _fTimePtr=fTimePtr;
    _lastTimeBlink=_fTimePtr();
    pinMode(_pin, OUTPUT);
  }
  
  /* set LED state */
  void setState(bool b) {pinMode(_pin, OUTPUT);digitalWrite(_pin, b ? 1 : 0);}
  /* turn LED on */
  void setOn(void) {setState(1);}
  /* turn LED off */
  void setOff(void) {setState(0);}
  /* define blink interval */
  void setBlinkInterval(unsigned int b) {_blinkInterval=b;}
  /* define blink time (0 for infinitely) */
  void setTimeToBlink(unsigned int b) {_timeToBlink=b;_startBlinkTime = 0;}
  
  //void blink(void);
  void blink(void) {
    if (_blinkInterval == 0) return;

    unsigned long time = _fTimePtr();
    unsigned long delayTime = 0;
    
    // ggf. maxBlinkTime-Delay berechnen
    if(_timeToBlink != 0) {
      if(_startBlinkTime == 0) {_startBlinkTime = time;}
      else {
        if (time < _startBlinkTime) {
          delayTime = MAX_U_LONG - _startBlinkTime + time;
        } else {
          delayTime = time - _startBlinkTime;
        }
        if (delayTime >= _timeToBlink) {
          setBlinkInterval(0);
          setTimeToBlink(0);
          setOff();
        };
      }
    }
    
    // Zeitdifferenz zum letzten Senden
    
    // Auf Ueberlauf pruefen
    if (time < _lastTimeBlink) {
      // Ueberlauf: Delay ist Zeit zum MaxWert plus Zeit ab Null
      delayTime = MAX_U_LONG - _lastTimeBlink + time;
    } else {
      // Kein Ueberlauf: einfache Differenz
      delayTime = time - _lastTimeBlink;
    }
    if (delayTime < _blinkInterval) return;
    
    _blinkMark = !_blinkMark;
    _lastTimeBlink = time;

    setState(_blinkMark);
  }

 private:
  TimeFuncPtr _fTimePtr;
  uint8_t _pin;
  bool _blinkMark = 0;
  unsigned int _blinkInterval = 0;
  unsigned long _lastTimeBlink;
  unsigned long _timeToBlink = 0;
  unsigned long _startBlinkTime = 0;
};
#endif
