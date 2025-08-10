#ifndef QTR_lib
#define QTR_lib

#include "Arduino.h"

class QTR{
  public:
    QTR(int pin);
    void set_pin(int pin);
    int read();
  private:
    int _pin;
    unsigned long sV;
    unsigned long time;
};

#endif
