#include "stm32l476xx.h"
#include "7-seg.h"

/**********************************************************************
 * connected to		pin name	MCU pin		mode
 * ===============================================================
 *
 * 7-seg DIN		D10		PB6		output
 * 7-seg CS		D11		PA7		output
 * 7-seg CLK		D12		PA6		output
 *
 **********************************************************************/

void max7219_send (unsigned char addr, unsigned char reg)
{
	int	i, data = (addr << 8) + reg;
	int	din = 1 << 6;
	int	load = 1 << 7;
	int	clk = 1 << 6;

	for (i = 15; i >= 0; --i) {
		GPIOA->BRR |= clk;	/* reset the CLK */
		if (data & (1 << i))
			GPIOB->BSRR |= din;
		else
			GPIOB->BRR |= din;
		GPIOA->BSRR |= clk;	/* set the CLK */
	}
	GPIOA->BRR |= load;
	GPIOA->BSRR |= load;
}

void max7219_init (void)
{
	/* enable AHB2 clock for port A and port B */
	RCC->AHB2ENR |= 0x3;
	/* set PA6, PA7, PB6 as output mode */
	GPIOA->MODER &= ~ (GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE7_Msk);
	GPIOA->MODER |= GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
	GPIOB->MODER &= ~ GPIO_MODER_MODE6_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE6_0;
	/* set PA6, PA7, PB6 as high speed mode */
	GPIOA->OSPEEDR &= ~ (GPIO_OSPEEDR_OSPEED6_Msk | GPIO_OSPEEDR_OSPEED7_Msk);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED7_1;
	GPIOB->OSPEEDR &= ~ GPIO_OSPEEDR_OSPEED6_Msk;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;

	/* decode mode: code B decode mode for all 8 digits */
	max7219_send (0x9, 0xff);
	/* intensity: 15/32 duty cycle*/
	max7219_send (0xa, 0x7);
	/* scan limit: display all 8 digits*/
	max7219_send (0xb, 0x7);
	/* shutdown: normal operation */
	max7219_send (0xc, 0x1);
	/* display test: normal operation */
	max7219_send (0xf, 0);
}

int power (int base, int exp)
{
	int	val = 1;
	while (exp-- > 0)
		val *= base;
	return val;
}

void display (int data, int len)
{
	int	i;
	if (len == 0) {
		for (i = 1; i <= 8; ++i)
			max7219_send (i, 15);
	} else if (len > 0) {
		for (i = 8; i > len; --i)
			max7219_send (i, 15);
		if (data < 0) {
			max7219_send (len, 10);	/* display the '-' sign */
			data *= -1;
			--len;
		}
		while (len > 0) {
			max7219_send (len, data / power (10, len - 1));
			data %= power (10, len - 1);
			--len;
		}
	}
}

