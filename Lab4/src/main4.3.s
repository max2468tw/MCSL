	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	passwords: .byte 0xA

.text
	//.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIOB_MODER, 0x48000400
	.equ GPIOB_OTYPER, 0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 0x4800040C
	.equ GPIOB_ODR, 0x48000414
	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_IDR, 0x48000810
	.equ X, 0x3E8
	.equ Y, 0x3E8
	.equ password,0xC

main:
  	BL   GPIO_init

Loop:
	//TODO: Write the display pattern into leds variable
	movs	r0, #0xFF
	strh	r0,[r1]
	ldr		r3,[r2]
	movs	r4,#1
	lsl		r4,#13
	ands	r3,r4
	bne		do_pushed
	ldr		r3,[r2]
	ldr 	r7, =password
	and  	r3, #0xf
	eor		r3,#0xf
	cmp		r3, r7
	beq		L
	bne		L3
L:	BL		DisplayLED
	BL   	Delay
	movs	r0, #0xFF
	strh	r0,[r1]
	BL   	Delay
	BL		DisplayLED
	BL   	Delay
	movs	r0, #0xFF
	strh	r0,[r1]
	BL   	Delay
	BL		DisplayLED
	BL   	Delay
	movs	r0, #0xFF
	strh	r0,[r1]
	BL   	Delay
	B		Loop
L3: BL		DisplayLED
	BL   	Delay
	movs	r0, #0xFF
	strh	r0,[r1]
	BL   	Delay
	B		Loop
GPIO_init:
  //TODO: Initial LED GPIO pins as output
	movs 	r0, #0x6
	ldr		r1, =RCC_AHB2ENR
	str		r0,[r1]

	movs	r0, #0x1540
	ldr  	r1, =GPIOB_MODER
	ldr		r2, [r1]
	and		r2, #0xFFFFC03F
	orrs	r2,r2,r0
	str		r2,[r1]

	movs	r0, #0x2A80
	ldr		r1, =GPIOB_OSPEEDR
	strh	r0,[r1]

	ldr		r1, =GPIOB_ODR
	movs	r0, #0xFF
	strh	r0,[r1]

	ldr  	r1, =GPIOC_MODER
	ldr		r0,[r1]
	ldr		r2,=#0xF3FFFF00
	and		r0,r2
	str		r0,[r1]

	ldr		r2,=GPIOC_IDR
	ldr		r1, =GPIOB_ODR
 	BX LR

DisplayLED:
	movs	r0, #0x0
	strh	r0,[r1]
	BX LR

Delay:
   //TODO: Write a delay 1sec function
   	ldr		r3, =X
L1: ldr		r4, =Y
L2:	subs	r4, #1
	bne 	L2
	subs	r3, #1
	bne		L1
	BX LR

do_pushed:
	B Loop
