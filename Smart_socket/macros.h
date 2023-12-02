#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr += (x);\
  if (flag)


#define B_TRUE(bp,bb)    bp |= bb
#define B_FALSE(bp,bb)   bp &= ~(bb)
#define B_READ(bp,bb)    bool(bp & bb)

#define SET_PIN_MODE_OUTPUT(port, pin) DDR ## port |= (1 << pin)
#define SET_PIN_HIGH(port, pin) (PORT ## port |= (1 << pin))
#define SET_PIN_LOW(port, pin) ((PORT ## port) &= ~(1 << (pin)))

#ifndef FOREVER
#define FOREVER for(;;)
#endif

