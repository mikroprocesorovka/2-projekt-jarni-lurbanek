#include "stm8s.h"
#include "milis.h"

/*#include "delay.h"*/
#include <stdio.h>
//#include "uart1.h"
#include "swi2c.h"
#include "stm8_hd44780.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);


    init_milis();
    //init_uart1();
    swi2c_init();
    lcd_init();
}


int main(void)
{
    uint32_t time = 0;
    uint8_t precteno[10] = {0,0,0,0,0,0,0,0,0,0};
    uint8_t zapsano[10] = {0,0,0,0,0,0,0,0,0,0};
    uint8_t err;
    //uint16_t *pteplota;
    //uint8_t teplota;
    //uint8_t desetina;
    char text[32];
    char ddmmmrr[32];



    setup();

    /*error=swi2c_read_buf(0b11010000,0x00,precteno,4) */
    zapsano[0] = 0x30;  // sekundy
    zapsano[1] = 0x04;  // minuty
    zapsano[2] = 0x23;  // hodiny
    zapsano[3] = 0x06;  // den v týdnu
    zapsano[4] = 0x23;  // den
    zapsano[5] = 0x04;  // měsíc
    zapsano[6] = 0x22;  // rok

    // když tento řádek odkomentuješ, tak se čas zapíše do obvodu
    //printf("ErrorCode: %X\n",  swi2c_write_buf(0x68, 0x00, zapsano, 7));

   

    while (1) {
        if (milis() - time > 1111 /*&& BTN_PUSH*/) {

            

            // čtu RTC
            err = swi2c_read_buf(0x68, 0x00, precteno, 7);
            
            lcd_gotoxy(0,0);
            
            lcd_puts("cas:");

            lcd_gotoxy(8,0);
            sprintf(text,"%d%d:%d%d:%d%d ",       // v RTC obvodu je čas uložen v BCD ************************************************************
                    precteno[2] >> 4, precteno[2] & 0x0F,
                    precteno[1] >> 4, precteno[1] & 0x0F,
                    precteno[0] >> 4, precteno[0] & 0x0F);
            lcd_puts(text);

            lcd_gotoxy(0,1);
            
            lcd_puts("datum:");
            
            lcd_gotoxy(6,1);
            sprintf(ddmmmrr,"%d%d.%d%d.20%d%d",       // v RTC obvodu je čas uložen v BCD ************************************************************
                    precteno[4] >> 4, precteno[4] & 0x0F,
                    precteno[5] >> 4, precteno[5] & 0x0F,
                    precteno[6] >> 4, precteno[6] & 0x0F,
                    precteno[2] >> 4, precteno[2] & 0x0F);
            lcd_puts(ddmmmrr);

            time = milis();
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
