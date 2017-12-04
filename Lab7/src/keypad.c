#include "stm32l476xx.h"
#include "keypad.h"

/**********************************************************************
 * connected to		pin name	MCU pin		mode
 * ===============================================================
 *
 * keypad col 4		A3		PB0		output
 * keypad col 3		A2		PA4		output
 * keypad col 2		A1		PA1		output
 * keypad col 1		A0		PA0		output
 *
 * keypad row 4		D2		PA10		input
 * keypad row 3		D4		PB5		input
 * keypad row 2		D7		PA8		input
 * keypad row 1		D8		PA9		input
 *
 **********************************************************************/

void keypad_init (void)
{
	/* enable AHB2 clock for port A, B */
	RCC->AHB2ENR |= 0x3;

	/* set PA8, 9, 10 as input mode and PA0, 1, 4 as output mode */
	GPIOA->MODER &= ~ (GPIO_MODER_MODE0_Msk |
			GPIO_MODER_MODE1_Msk |
			GPIO_MODER_MODE4_Msk |
			GPIO_MODER_MODE8_Msk |
			GPIO_MODER_MODE9_Msk |
			GPIO_MODER_MODE10_Msk);
	GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE4_0;
	/* set PB5 as input mode and PB0 as output mode */
	GPIOB->MODER &= ~ (GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE5_Msk);
	GPIOB->MODER |= GPIO_MODER_MODE0_0;

	/* set PA8, PA9, PA10, PB5 as pull-down inputs */
	GPIOA->PUPDR &= ~ (GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk | GPIO_PUPDR_PUPD10_Msk);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD8_1 | GPIO_PUPDR_PUPD9_1 | GPIO_PUPDR_PUPD10_1;
	GPIOB->PUPDR &= ~ GPIO_PUPDR_PUPD5_Msk;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD5_1;
	/* set PA0, PA1, PA4, PB0 as pull-up outputs */
	GPIOA->PUPDR &= ~ (GPIO_PUPDR_PUPD0_Msk | GPIO_PUPDR_PUPD1_Msk | GPIO_PUPDR_PUPD4_Msk);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD4_0;
	GPIOB->PUPDR &= ~ GPIO_PUPDR_PUPD0_Msk;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD0_0;

	/* set PA0, 1, 4, 8, 9, 10 as medium speed mode */
	GPIOA->OSPEEDR &= ~ (GPIO_OSPEEDR_OSPEED0_Msk |
			GPIO_OSPEEDR_OSPEED1_Msk |
			GPIO_OSPEEDR_OSPEED4_Msk |
			GPIO_OSPEEDR_OSPEED8_Msk |
			GPIO_OSPEEDR_OSPEED9_Msk |
			GPIO_OSPEEDR_OSPEED10_Msk);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0 |
			GPIO_OSPEEDR_OSPEED1_0 |
			GPIO_OSPEEDR_OSPEED4_0 |
			GPIO_OSPEEDR_OSPEED8_0 |
			GPIO_OSPEEDR_OSPEED9_0 |
			GPIO_OSPEEDR_OSPEED10_0;
	/* set PB0, 5 as medium speed mode */
	GPIOB->OSPEEDR &= ~ (GPIO_OSPEEDR_OSPEED0_Msk | GPIO_OSPEEDR_OSPEED5_Msk);
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED5_0;

	/* set outputs as high */
	allhigh ();
}

int row4 (void)
{
	return !!(GPIOA->IDR & GPIO_IDR_ID10);
}

int row3 (void)
{
	return !!(GPIOB->IDR & GPIO_IDR_ID5);
}

int row2 (void)
{
	return !!(GPIOA->IDR & GPIO_IDR_ID8);
}

int row1 (void)
{
	return !!(GPIOA->IDR & GPIO_IDR_ID9);
}

void col4 (int val)	/* PB0 */
{
	if (val) {
		GPIOB->OTYPER &= ~(GPIO_OTYPER_OT0);	/* push-pull */
		GPIOB->BSRR = GPIO_BSRR_BS0;
	} else {
		GPIOB->OTYPER |= GPIO_OTYPER_OT0;	/* open-drain */
		GPIOB->BRR = GPIO_BRR_BR0;
	}
}

void col3 (int val)	/* PA4 */
{
	if (val) {
		GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);	/* push-pull */
		GPIOA->BSRR = GPIO_BSRR_BS4;
	} else {
		GPIOA->OTYPER |= GPIO_OTYPER_OT4;	/* open-drain */
		GPIOA->BRR = GPIO_BRR_BR4;
	}
}

void col2 (int val)	/* PA1 */
{
	if (val) {
		GPIOA->OTYPER &= ~(GPIO_OTYPER_OT1);	/* push-pull */
		GPIOA->BSRR = GPIO_BSRR_BS1;
	} else {
		GPIOA->OTYPER |= GPIO_OTYPER_OT1;	/* open-drain */
		GPIOA->BRR = GPIO_BRR_BR1;
	}
}

void col1 (int val)	/* PA0 */
{
	if (val) {
		GPIOA->OTYPER &= ~(GPIO_OTYPER_OT0);	/* push-pull */
		GPIOA->BSRR = GPIO_BSRR_BS0;
	} else {
		GPIOA->OTYPER |= GPIO_OTYPER_OT0;	/* open-drain */
		GPIOA->BRR = GPIO_BRR_BR0;
	}
}

void allhigh (void)
{
	col4 (1);
	col3 (1);
	col2 (1);
	col1 (1);
}

int keypad_scan (void)
{
	int	bits = 0;

	/* set the col 4 */
	col4 (1);
	col3 (0);
	col2 (0);
	col1 (0);
	/* read through row 4 to row 1 */
	if (row4 ())
		bits |= 0x1 << 13;
	if (row3 ())
		bits |= 0x1 << 12;
	if (row2 ())
		bits |= 0x1 << 11;
	if (row1 ())
		bits |= 0x1 << 10;

	/* set the col 3 */
	col4 (0);
	col3 (1);
	col2 (0);
	col1 (0);
	/* read through row 4 to row 1 */
	if (row4 ())
		bits |= 0x1 << 14;
	if (row3 ())
		bits |= 0x1 << 9;
	if (row2 ())
		bits |= 0x1 << 6;
	if (row1 ())
		bits |= 0x1 << 3;

	/* set the col 2 */
	col4 (0);
	col3 (0);
	col2 (1);
	col1 (0);
	/* read through row 4 to row 1 */
	if (row4 ())
		bits |= 0x1 << 0;
	if (row3 ())
		bits |= 0x1 << 8;
	if (row2 ())
		bits |= 0x1 << 5;
	if (row1 ())
		bits |= 0x1 << 2;

	/* set the col 1 */
	col4 (0);
	col3 (0);
	col2 (0);
	col1 (1);
	/* read through row 4 to row 1 */
	if (row4 ())
		bits |= 0x1 << 15;
	if (row3 ())
		bits |= 0x1 << 7;
	if (row2 ())
		bits |= 0x1 << 4;
	if (row1 ())
		bits |= 0x1 << 1;

	return bits;
}
