#include <machine.h>
#include "init_rx220.h"
#include "general_io.h"
#include "sci.h"
#include "analog_read.h"
#include "pwm_drive.h"
#include "control_motor.h"
#include "sound_buzzer.h"
#include "sonar.h"
#include "photo_reflector.h"
#include "encoder.h"
#include "cmtimer.h"
#include "iodefine.h"


static void init_clk(void) {
	SYSTEM.PRCR.WORD 		= 0xA50B;			//レジスタ書き込み許可
	SYSTEM.SCKCR.BIT.ICK	= 0x0;				//ICLK 1分周
	SYSTEM.SCKCR.BIT.PCKB	= 0x0;				//PCLKB 1分周
	SYSTEM.SCKCR.BIT.PCKD	= 0x0;				//PCLKD 1分周
	SYSTEM.SCKCR3.BIT.CKSEL = 0x1;				//HOCO選択
	SYSTEM.HOCOCR.BIT.HCSTP = 0x0;				//HOCO動作
	SYSTEM.HOCOCR2.BIT.HCFRQ = 0x0;				//HOCO 32MHz
	SYSTEM.HOCOPCR.BIT.HOCOPCNT = 0x0;			//HOCO起動
}

void init_rx220(void) {
    clrpsw_i();
	init_clk();
	init_adc();
	init_sci();
	init_general_io();
	init_sonar();
	init_photo_reflector();
	init_pwm_drive();
	init_motor();
	init_encoder();
	init_cmtimer();
	init_buzzer();
}