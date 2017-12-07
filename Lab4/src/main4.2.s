	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	leds: .byte 0

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
	.equ X, 0x2EE
	.equ Y, 0x2EE

main:
  	BL   GPIO_init
	movs	r5, #0
	LDR		r0, =leds
	STRB	r5, [r0]

Loop:
	//TODO: Write the display pattern into leds variable
	ldr		r3,[r2]
	movs	r4,#1
	lsl		r4,#13
	ands	r3,r4
	beq		do_pushed
L:	BL		DisplayLED
	ldr		r3,[r2]
	movs	r4,#1
	lsl		r4,#13
	ands	r3,r4
	beq		do_pushed
	BL   	Delay
	ldr		r3,[r2]
	movs	r4,#1
	lsl		r4,#13
	ands	r3,r4
	beq		do_pushed
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

	ldr  	r1, =GPIOC_MODER
	ldr		r0,[r1]
	ldr		r2,=#0xF3FFFFFF
	and		r0,r2
	str		r0,[r1]

	ldr		r2,=GPIOC_IDR
	ldr		r1, =GPIOB_ODR
 	BX LR

DisplayLED:
	cmp r5,#0
	beq C1
	cmp r5,#1
	beq C2
	cmp r5,#2
	beq C3
	cmp r5,#3
	beq C4
	cmp r5,#4
	beq C5
	cmp r5,#5
	beq C6
	cmp r5,#6
	beq C7
	cmp r5,#7
	beq C8
C1:	movs	r0, #0x70
	b exit
C2: movs	r0, #0x60
	b exit
C3: movs	r0, #0x48
	b exit
C4: movs	r0, #0x18
	b exit
C5: movs	r0, #0x38
	b exit
C6: movs	r0, #0x18
	b exit
C7: movs	r0, #0x48
	b exit
C8: movs	r0, #0x60
	movs	r5, -1
	b exit
exit:
	strh	r0,[r1]
	LDR		r0, =leds
	STRB	r5, [r0]
	adds r5, #1
	BX LR

Delay:
   //TODO: Write a delay 1sec function
   	ldr		r3, =X
L1: ldr		r4, =Y
L2:
	ldr		r7,[r2]
	movs	r8,#1
	lsl		r8,#13
	ands	r7,r8
	beq		do_pushed
	subs	r4, #1
	bne 	L2
	subs	r3, #1
	bne		L1
	BX LR

do_pushed:
	movs	r5, #0
	B L
