#include "general_io.h"
#include "iodefine.h"

#define DBG_LED0_PORT	PORT5.PODR.BIT.B4	//P54
#define DBG_LED1_PORT	PORT5.PODR.BIT.B5	//P55
#define DBG_LED2_PORT	PORTH.PODR.BIT.B0	//PH0

#define M_LED0_PORT		PORT1.PODR.BIT.B5	//P15
#define M_LED1_PORT		PORT1.PODR.BIT.B6	//P16

#define M_R_IN1_PORT	PORTB.PODR.BIT.B5	//PB5
#define M_L_IN1_PORT	PORTB.PODR.BIT.B6	//PB6
#define M_R_IN2_PORT	PORTB.PODR.BIT.B7	//PB7
#define M_L_IN2_PORT    PORTC.PODR.BIT.B2	//PC2
#define M_STBY_PORT     PORTC.PODR.BIT.B3	//PC3

void init_general_io(void){
    //出力設定
    PORT5.PDR.BIT.B4 = 0x1;	    //P54
    PORT5.PDR.BIT.B5 = 0x1;     //P55
    PORTH.PDR.BIT.B0 = 0x1;     //PH0
    PORT1.PDR.BIT.B5 = 0x1;	    //P15
    PORT1.PDR.BIT.B6 = 0x1;	    //P16    
    PORTB.PDR.BIT.B5 = 0x1;	    //PB5
    PORTB.PDR.BIT.B6 = 0x1;	    //PB6
    PORTB.PDR.BIT.B7 = 0x1;	    //PB7
    PORTC.PDR.BIT.B2 = 0x1;	    //PC2
    PORTC.PDR.BIT.B3 = 0x1;	    //PC3

    //汎用IO設定
    PORT5.PMR.BIT.B4 = 0x0;	    //P54
    PORT5.PMR.BIT.B5 = 0x0;     //P55
    PORTH.PMR.BIT.B0 = 0x0;     //PH0
    PORT1.PMR.BIT.B5 = 0x0;	    //P15
    PORT1.PMR.BIT.B6 = 0x0;	    //P16 
    PORTB.PMR.BIT.B5 = 0x0;	    //PB5
    PORTB.PMR.BIT.B6 = 0x0;	    //PB6
    PORTB.PMR.BIT.B7 = 0x0;	    //PB7
    PORTC.PMR.BIT.B2 = 0x0;	    //PC2
    PORTC.PMR.BIT.B3 = 0x0;	    //PC3
}

void digital_write(port_t port, outlevel_t level) {
    switch (port) {
        case DBG_LED0:
            DBG_LED0_PORT = (char)level;
            break;
        case DBG_LED1:
            DBG_LED1_PORT = (char)level;
            break;
        case DBG_LED2:
            DBG_LED2_PORT = (char)level;
            break;
        case M_LED0:
            M_LED0_PORT = (char)level;
            break;
        case M_LED1:
            M_LED1_PORT = (char)level;
            break;
        case M_R_IN1:
            M_R_IN1_PORT = (char)level;
            break;
        case M_L_IN1:
            M_L_IN1_PORT = (char)level;
            break;
        case M_R_IN2:
            M_R_IN2_PORT = (char)level;
            break;
        case M_L_IN2:
            M_L_IN2_PORT = (char)level;
            break;
        case M_STBY:
            M_STBY_PORT = (char)level;
            break;
        default:
            break;
    }
}