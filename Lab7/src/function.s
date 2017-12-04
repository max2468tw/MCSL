	.syntax	unified
	.cpu	cortex-m4
	.thumb

.text
	.global	delay_1s
	.global	GPIO_init
	.equ	RCC_AHB2ENR,	0x4002104c
	.equ	timer,		0x100000	// 2^22 / 4 = 2^20

	.equ	GPIOA_MODER,	0x48000000
	.equ	GPIOA_OTYPER,	0x48000004
	.equ	GPIOA_OSPEEDR,	0x48000008
	.equ	GPIOA_PUPDR,	0x4800000c
	.equ	GPIOA_ODR,	0x48000014

	.equ	GPIOC_MODER,	0x48000800
	.equ	GPIOC_OTYPER,	0x48000804
	.equ	GPIOC_OSPEEDR,	0x48000808
	.equ	GPIOC_PUPDR,	0x4800080c
	.equ	GPIOC_ODR,	0x48000814

delay_1s:
	/* delay for one second */
	ldr	r0, =timer	// 2
	delay_L:
	subs	r0, r0, 1	// 1
	bne	delay_L		// 3 or 1
	bx	lr		// 1

GPIO_init:
	/* enable AHB2 clock for port A, C */
	movs	r0, 0x5
	ldr	r1, =RCC_AHB2ENR
	str	r0, [r1]

	/* set PA5 as output mode */
	movs	r0, 0x400
	ldr	r1, =GPIOA_MODER
	ldr	r2, [r1]
	ands	r2, r2, 0xfffff3ff
	orrs	r2, r2, r0
	str	r2, [r1]

        /* set PC13 as input mode */
        ldr     r1, =GPIOC_MODER
        ldr     r2, [r1]
        ands    r2, r2, 0xf3ffffff
        str     r2, [r1]

	/* set PA5 as high speed mode */
	movs	r0, 0x800
	ldr	r1, =GPIOA_OSPEEDR
	str	r0, [r1]
	bx	lr
