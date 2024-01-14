#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr += (x);\
  if (flag)


#define B_TRUE(bp,bb)    bp |= bb
#define B_FALSE(bp,bb)   bp &= ~(bb)
#define B_READ(bp,bb)    bool(bp & bb)

#ifndef FOREVER
#define FOREVER for(;;)
#endif

#define RELAY_ON digitalWrite(RELAY_PIN, FALSE)
#define RELAY_OFF digitalWrite(RELAY_PIN, TRUE)
#define RELAY_STATE(x) digitalWrite(RELAY_PIN, x)

