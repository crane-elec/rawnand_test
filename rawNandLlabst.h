#ifndef RAWNANDLLABST_H
#define RAWNANDLLABST_H

// filename: rawnand_llabst.h
// 
// This file provides low level abstraction layer.
// If you want to use rawnand functions in your specific environement, modify these functions. 

#include "pico/stdlib.h"
#include "hardware/timer.h"

class rawNandLlabst
{
 public:
  rawNandLlabst();
  void init();
  void setWpbIo(uint8_t wpb);
  void setCsIo(uint8_t ceb0,uint8_t ceb1);
  void setCmdIo(uint8_t cle, uint8_t ale, uint8_t web);
  void setDqDir(uint8_t direction);
  void setDqIo(uint8_t dq);
  void setRebIo(uint8_t reb);
  uint8_t getRbbIo();
  uint8_t getDqIo();
  void waitUs(uint64_t us);
  void nop();
  
private:
  const static int io1_pin  = 0;
  const static int io2_pin  = 1;
  const static int io3_pin  = 2;
  const static int io4_pin  = 3;
  const static int io5_pin  = 4;
  const static int io6_pin  = 5;
  const static int io7_pin  = 6;
  const static int io8_pin  = 7;
  const static int ceb0_pin = 8;
  const static int ceb1_pin = 9;
  const static int cle_pin  = 10;
  const static int ale_pin  = 11;
  const static int wpb_pin  = 12;
  const static int web_pin  = 13;
  const static int reb_pin  = 14;
  const static int rbb_pin  = 15;
};
#endif
