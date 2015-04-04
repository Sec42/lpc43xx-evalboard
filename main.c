#define CMSIS_BITPOSITIONS
#include "LPC43xx.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg_default.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_utils.h"

#define LED1_BIT			11 //LEDUSB
#define LED1_PORT			1
#define LED2_BIT			12 //LEDUSB
#define LED2_PORT			1

#define CD_BIT			10 // CD for display
#define CD_PORT			1

void delay(void)
{
    int i;
    for (i = 0; i < 2000000; i++)   /* Wait a bit. */
        __asm__("nop");
}

int main(void)
{

    CGU_Init();

    scu_pinmux(0x2 ,10 , MD_PUP, FUNC0); 	// P2.11 : GPIO1_10: Xplorer BOARD (LED D2)
    GPIO_SetDir(CD_PORT,(1<<CD_BIT), 1);
    GPIO_ClearValue(CD_PORT,(1<<CD_BIT));

	// SPI setup: Plan is to use SPI1
	// We want to set CPOL=1 und CPHA=1 (Clock starts high, write
    // on falling edge)
/*
typedef struct {                            
  __IO uint32_t  CR;         // SPI Control Register.
  __I  uint32_t  SR;         // SPI Status Register.
  __IO uint32_t  DR;         // SPI Data Register.
  __IO uint32_t  CCR;        // SPI Clock Counter Register.
  __IO uint32_t  TCR;        // SPI Test Control register.
  __IO uint32_t  TSR;        // SPI Test Status register.
  __I  uint32_t  RESERVED0;
  __IO uint32_t  INT;        // SPI Interrupt Flag.
} LPC_SPI_Type;
*/


	/*
	   1. Set the SPI Clock Counter Register to the desired clock rate.
	   2. Set the SPI Control Register to the desired settings for master mode.
	*/

	// CCR needs to be even and >=8
LPC_SPI->CCR= 16; // 72MHz (default) / 8  is smaller than 33MHz (display max)
LPC_SPI->CR= SPI_CR_CPHA_Msk | SPI_CR_CPOL_Msk | SPI_CR_MSTR_Msk;

	/*
	   1. Optionally, verify the SPI setup before starting the transfer.
	   2. Write the data to transmitted to the SPI Data Register. This write
		  starts the SPI data transfer.
	   3. Wait for the SPIF bit in the SPI Status Register to be set to 1. The
          SPIF bit will be set after the last cycle of the SPI data transfer.
	   4. Read the SPI Status Register.
	   5. Read the received data from the SPI Data Register (optional).
	   6. Go to step 2 if more data is to be transmitted.  Note: A read or
          write of the SPI Data Register is required in order to clear the
		  SPIF status bit. Therefore, if the optional read of the SPI Data
		  Register does not take place, a write to this register is
		  required in order to clear the SPIF status bit.
	 */

delay();
//LPC_SPI->DR=0b10100101; 
LPC_SPI->DR=0x40;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xA1;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xC0;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xA4;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xA6+1;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0x2F;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0x81;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0x10;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xFA;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0x90;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
LPC_SPI->DR=0xAF;
while((LPC_SPI->SR & SPI_SR_SPIF_Msk) ==0)
        __asm__("nop");
	
    scu_pinmux(0x2 ,11 , MD_PUP, FUNC0); 	// P2.11 : GPIO1_11: Xplorer BOARD (LED D2)
    GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);
    GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
    scu_pinmux(0x2 ,12 , MD_PUP, FUNC0); 	// P2.12 : GPIO1_12: Xplorer BOARD (LED D3)
    GPIO_SetDir(LED2_PORT,(1<<LED2_BIT), 1);
    GPIO_ClearValue(LED2_PORT,(1<<LED2_BIT));

    while (1)
    {
//        timer_delay_ms(50);
        delay();
        GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
//        timer_delay_ms(50);
        delay();
        GPIO_ClearValue(LED2_PORT,(1<<LED2_BIT));
        GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));
//        timer_delay_ms(50);
        delay();
        GPIO_SetValue(LED2_PORT,(1<<LED2_BIT));
    }

}

