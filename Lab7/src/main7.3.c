#include "stm32l476xx.h"
#include "keypad.h"
#include "7-seg.h"

int intlen (int n);
void timer_init (TIM_TypeDef *timer);
void timer_start (TIM_TypeDef *timer);
void timer_stop (TIM_TypeDef *timer);
void C4 (TIM_TypeDef *timer);
void D4 (TIM_TypeDef *timer);
void E4 (TIM_TypeDef *timer);
void F4 (TIM_TypeDef *timer);
void G4 (TIM_TypeDef *timer);
void A4 (TIM_TypeDef *timer);
void B4 (TIM_TypeDef *timer);
void C5 (TIM_TypeDef *timer);

void timer_init (TIM_TypeDef *timer)
{
	// Sound freq = 4 MHz / (pres + 1) / 100
	// pres = 4 MHz / Sound freq / 100 - 1
	timer->PSC = (uint32_t) 152;
	timer->ARR = (uint32_t) 99;

	/* CH1 */
	timer->CCR1 = 50;
	timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC1E;	/* CH1 */
}

void timer_start (TIM_TypeDef *timer)
{
	timer->CR1 |= TIM_CR1_CEN;
}

void timer_stop (TIM_TypeDef *timer)
{
	timer->CR1 &= ~TIM_CR1_CEN;
}

int main (void)
{
	int		t = 0, key = 0, duty = 50;
	TIM_TypeDef	*timer = TIM3;

	max7219_init ();
	display (duty, intlen (duty));
	keypad_init ();

	/* GPIO: set PB4 as alternate function */
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER |= GPIO_MODER_MODE4_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_1;	/* PB4: AF2 (TIM3_CH1) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	timer_init (timer);

	while (1) {
		if (!row4 () && !row3 () && !row2 () && !row1 ()) {
			t = 0;
			key = 0;
			timer_stop (timer);
		} else {
			if (key == 0)
				key = keypad_scan ();
			if (t == 700) {
				if (key & 0x1 << 1) {
					C4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 2) {
					D4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 3) {
					E4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 4) {
					F4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 5) {
					G4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 6) {
					A4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 7) {
					B4 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 8) {
					C5 (timer);
					timer_start (timer);
				} else if (key & 0x1 << 14) {
					duty += 500;
					if (duty > 100000)
						duty = 90;
					display (duty, intlen (duty));
					timer->CCR1 = duty;
				} else if (key & 0x1 << 15) {
					duty -= 5;
					if (duty < 10)
						duty = 10;
					display (duty, intlen (duty));
					timer->CCR1 = duty;
				}
			}
			allhigh ();
			++t;
		}
	}

	return 0;
}

void C4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 152;	// 4 MHz / 261.6 Hz / 100 - 1 = 151.90 = 152;
}

void D4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 135;	// 4 MHz / 293.7 Hz / 100 - 1 = 135.19 = 135
}

void E4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 120;	// 4 MHz / 329.6 Hz / 100 - 1 = 120.36 = 120
}

void F4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 114;	// 4 MHz / 349.2 Hz / 100 - 1 = 113.55 = 114
}

void G4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 101;	// 4 MHz / 392.0 Hz / 100 - 1 = 101.04 = 101
}

void A4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 90;	// 4 MHz / 440.0 Hz / 100 - 1 = 89.91 = 90
}

void B4 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 80;	// 4 MHz / 493.9 Hz / 100 - 1 = 79.99 = 80
}

void C5 (TIM_TypeDef *timer)
{
	timer->PSC = (uint32_t) 75;	// 4 MHz / 523.3 Hz / 100 - 1 = 75.44 = 75
}

int intlen (int n)
{
	int len = 1;
	while (n > 9) {
		n /= 10;
		++len;
	}
	return len;
}
