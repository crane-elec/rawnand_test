#include "rawNandLlabst.h"


rawNandLlabst::rawNandLlabst()
{
  this->init();
}
void rawNandLlabst::init()
{
  // Enable IO (IO direction is input)
  gpio_init_mask( 1 << io1_pin  |         \
		  1 << io2_pin  |         \
		  1 << io3_pin  |         \
		  1 << io4_pin  |         \
		  1 << io5_pin  |         \
		  1 << io6_pin  |         \
		  1 << io7_pin  |         \
		  1 << io8_pin  |         \
		  1 << ceb0_pin |         \
		  1 << ceb1_pin |         \
		  1 << ale_pin  |         \
		  1 << cle_pin  |         \
		  1 << wpb_pin  |         \
		  1 << web_pin  |         \
		  1 << reb_pin  |         \
		  1 << rbb_pin  );
  // Set the default output value. IO direction is still input.
  gpio_put_masked(1 << io1_pin  |         \
		  1 << io2_pin  |         \
		  1 << io3_pin  |         \
		  1 << io4_pin  |         \
		  1 << io5_pin  |         \
		  1 << io6_pin  |         \
		  1 << io7_pin  |         \
		  1 << io8_pin  |         \
		  1 << ceb0_pin |         \
		  1 << ceb1_pin |         \
		  1 << ale_pin  |         \
		  1 << cle_pin  |         \
		  1 << wpb_pin  |         \
		  1 << web_pin  |         \
		  1 << reb_pin  |         \
		  1 << rbb_pin,           \
		  1 << io1_pin  |         \
		  1 << io2_pin  |         \
		  1 << io3_pin  |         \
		  1 << io4_pin  |         \
		  1 << io5_pin  |         \
		  1 << io6_pin  |         \
		  1 << io7_pin  |         \
		  1 << io8_pin  |         \
		  1 << ceb0_pin |         \
		  1 << ceb1_pin |         \
		  1 << ale_pin  |         \
		  1 << cle_pin  |         \
		  0 << wpb_pin  |         \
		  1 << web_pin  |         \
		  1 << reb_pin  |         \
		  0 << rbb_pin  );
  // Set IO direction. IO bus and rbb are input. the others are output.
  gpio_set_dir_masked(1 << io1_pin  |         \
		      1 << io2_pin  |         \
		      1 << io3_pin  |         \
		      1 << io4_pin  |         \
		      1 << io5_pin  |         \
		      1 << io6_pin  |         \
		      1 << io7_pin  |         \
		      1 << io8_pin  |         \
		      1 << ceb0_pin |         \
		      1 << ceb1_pin |         \
		      1 << ale_pin  |         \
		      1 << cle_pin  |         \
		      1 << wpb_pin  |         \
		      1 << web_pin  |         \
		      1 << reb_pin  |         \
		      1 << rbb_pin,           \
		      0 << io1_pin  |         \
		      0 << io2_pin  |         \
		      0 << io3_pin  |         \
		      0 << io4_pin  |         \
		      0 << io5_pin  |         \
		      0 << io6_pin  |         \
		      0 << io7_pin  |         \
		      0 << io8_pin  |         \
		      1 << ceb0_pin |         \
		      1 << ceb1_pin |         \
		      1 << ale_pin  |         \
		      1 << cle_pin  |         \
		      1 << wpb_pin  |         \
		      1 << web_pin  |         \
		      1 << reb_pin  |         \
		      0 << rbb_pin  );
  // Enable RBB pull up
  gpio_pull_up(rbb_pin);
  // We can modify drive strength 2MA, 4MA, 8MA or 12MA 
  //gpio_set_drive_strength (web_pin,GPIO_DRIVE_STRENGTH_2MA);
  //gpio_set_drive_strength (reb_pin,GPIO_DRIVE_STRENGTH_2MA);
}

// Set WPB IO
void rawNandLlabst::setWpbIo(uint8_t wpb) {
  gpio_put(wpb_pin,wpb);
}
// Set CEB0, CEB1 IO
void rawNandLlabst::setCsIo(uint8_t ceb0, uint8_t ceb1) {
  gpio_put_masked( 1 << ceb0_pin |
		   1 << ceb1_pin ,
		   ceb0 << ceb0_pin |
		   ceb1 << ceb1_pin);
}
// Set CEB0, CEB1, CLE, ALE and WEB IO
void rawNandLlabst::setCmdIo(uint8_t cle, uint8_t ale, uint8_t web) {
  gpio_put_masked( 1 << cle_pin |
		   1 << ale_pin |
		   1 << web_pin ,
		   cle  << cle_pin  |
		   ale  << ale_pin  |
		   web  << web_pin   );
}
// Set DQ IO direction
void rawNandLlabst::setDqDir(uint8_t direction) {
  if (direction) {
    gpio_set_dir_masked(1<<io1_pin|
			1<<io2_pin|
			1<<io3_pin|
			1<<io4_pin|
			1<<io5_pin|
			1<<io6_pin|
			1<<io7_pin|
			1<<io8_pin,
			1<<io1_pin|
			1<<io2_pin|
			1<<io3_pin|
			1<<io4_pin|
			1<<io5_pin|
			1<<io6_pin|
			1<<io7_pin|
			1<<io8_pin);
  } else {
    gpio_set_dir_masked(1<<io1_pin|
			1<<io2_pin|
			1<<io3_pin|
			1<<io4_pin|
			1<<io5_pin|
			1<<io6_pin|
			1<<io7_pin|
			1<<io8_pin,
			0<<io1_pin|
			0<<io2_pin|
			0<<io3_pin|
			0<<io4_pin|
			0<<io5_pin|
			0<<io6_pin|
			0<<io7_pin|
			0<<io8_pin);
  }
}
// Set DQ IO
void rawNandLlabst::setDqIo(uint8_t dq) {
  gpio_put_masked(1<<io1_pin|
		  1<<io2_pin|
		  1<<io3_pin|
		  1<<io4_pin|
		  1<<io5_pin|
		  1<<io6_pin|
		  1<<io7_pin|
		  1<<io8_pin,
		  dq);
}
// Set REB IO
void rawNandLlabst::setRebIo(uint8_t reb) {
  gpio_put_masked(1<<reb_pin,
		  reb<<reb_pin);
}
// Get RBB IO
uint8_t rawNandLlabst::getRbbIo() {
  return (uint8_t)gpio_get(rbb_pin);
}
// Get DQ IO
uint8_t rawNandLlabst::getDqIo() {
  return (uint8_t)(gpio_get_all() & 0xff);
}
// Wait timer
void rawNandLlabst::waitUs(uint64_t us) {
  sleep_us(us);
}

void rawNandLlabst::nop() {
  asm volatile("nop");
}
