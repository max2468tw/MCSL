	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	answer: .byte 0 //TODO: put your student id here

.text
	.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIOA_BASE, 0x48000000
	.equ GPIO_BSRR_OFFSET, 0x18
	.equ GPIO_BRR_OFFSET, 0x28
	.equ GPIOA_MODER, 0x48000000
	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_IDR, 0x48000810
	.equ GPIOA_OTYPER, 0x48000004
	.equ GPIOA_OSPEEDR, 0x48000008
	.equ GPIOA_PUPDR, 0x4800000C
	.equ GPIOA_ODR, 0x48000014
	.equ DECODE_MODE,	0x09
	.equ SHUTDOWN,	0x0C
	.equ INTENSITY,	0x0A
	.equ SCAN_LIMIT, 0x0B
	.equ DISPLAY_TEST, 0x0F
	.equ DATA, 0x20 //PA5
	.equ LOAD, 0x40 //PA6
	.equ CLOCK, 0x80 //PA7

main:
    BL   GPIO_init
    BL   max7219_init
    //TODO: display your student id on 7-Seg LED
    movs r11, #0
K:
	movs r0, r11
    bl fib
    movs r5,#0
    cmp r4, -1
    beq minus1
    ldr r9, = answer
    ldr r10, =#10000000
    udiv r1, r4, r10
    cmp r5, #0
    bne C1
    cmp r1, #0
    beq C
    bne C1
C:  movs r1, #0xf
	strb r1, [r9,0]
	movs r1, #0
	b CC
C1: movs r5, #1
	strb r1, [r9,0]
CC: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#1000000
    udiv r1, r4, r10
    cmp r5, #0
    bne D1
    cmp r1, #0
    beq D
    bne D1
D:  movs r1, #0xf
	strb r1, [r9,1]
	movs r1, #0
	b DD
D1: movs r5, #1
	strb r1, [r9,1]
DD: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#100000
    udiv r1, r4, r10
    cmp r5, #0
    bne E1
    cmp r1, #0
    beq E
    bne E1
E:  movs r1, #0xf
	strb r1, [r9,2]
	movs r1, #0
	b EE
E1: movs r5, #1
	strb r1, [r9,2]
EE: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#10000
    udiv r1, r4, r10
    cmp r5, #0
    bne F1
    cmp r1, #0
    beq F
    bne F1
F:  movs r1, #0xf
	strb r1, [r9,3]
	movs r1, #0
	b FF
F1: movs r5, #1
	strb r1, [r9,3]
FF: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#1000
    udiv r1, r4, r10
    cmp r5, #0
    bne G1
    cmp r1, #0
    beq G
    bne G1
G:  movs r1, #0xf
	strb r1, [r9,4]
	movs r1, #0
	b GG
G1: movs r5, #1
	strb r1, [r9,4]
GG: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#100
    udiv r1, r4, r10
    cmp r5, #0
    bne H1
    cmp r1, #0
    beq H
    bne H1
H:  movs r1, #0xf
	strb r1, [r9,5]
	movs r1, #0
	b HH
H1: movs r5, #1
	strb r1, [r9,5]
HH: mul r1, r1, r10
    subs r4, r4, r1
    ldr r10, =#10
    udiv r1, r4, r10
    cmp r5, #0
    bne I1
    cmp r1, #0
    beq I
    bne I1
I:  movs r1, #0xf
	strb r1, [r9,6]
	movs r1, #0
	b II
I1: movs r5, #1
	strb r1, [r9,6]
II: mul r1, r1, r10
    subs r4, r4, r1
    strb r4, [r9,7]
back:
	ldr r2, =#0
    ldr r4, =#9
.for_loop:
	ldrb r1, [r9,r2]
	add r0, r2, #1
	sub r0, r4, r0
	BL MAX7219Send
	add r2, r2, #1
	cmp r2, r4
	bne .for_loop
	movs r4, #0
	b Debounce
Program_end:
	B Program_end
Debounce:
	//buttonState r0
	//lastButtonState r1
	ldr r2, =GPIOC_IDR
	ldr r3, [r2]
	lsr r3, #13
	ldr r6 ,=#0x10000
	cmp r4, r6
	bgt reset
	cmp r3, #0
	beq add
	bne noadd
add:
	adds r4,#1
noadd:
	cmp r3, r1
	bne wait
	beq Debounce
wait: Bl Delay
	  ldr r2, =GPIOC_IDR
	  ldr r3, [r2]
	  lsr r3, #13
	  cmp r3,r0
	  bne change
	  beq Debounce
change:movs r0, r3
		movs r1, r3
		cmp r0, #0
		beq do_pushed
 		bne Debounce
do_pushed:
	adds r11,#1
	B K
reset:
	movs r11,#0
	B K
minus1:
	ldr r0, =#0x0f0f0f0f
	str r0, [r9]
	ldr r0, =#0x010a0f0f
	str r0, [r9,4]
	b back

GPIO_init:
	//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
	movs 	r0, #0x5
	ldr		r1, =RCC_AHB2ENR
	str		r0,[r1]

	movs	r0, #0x5400
	ldr  	r1, =GPIOA_MODER
	ldr		r2, [r1]
	and		r2, #0xFFFF03FF
	orrs	r2,r2,r0
	str		r2,[r1]

	movs	r0, #0xA800
	ldr		r1, =GPIOA_OSPEEDR
	strh	r0,[r1]

	ldr 	r1, =GPIOC_MODER
	ldr		r0, [r1]
	ldr		r2, =#0xF3FFFFFF
	and		r0, r2
	str		r0, [r1]

	BX LR

MAX7219Send:
    //input parameter: r0 is ADDRESS , r1 is DATA
	//TODO: Use this function to send a message to max7219
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, lr}
	lsl r0, r0, #8
	add r0, r0, r1
	ldr r1, =#GPIOA_BASE
	ldr r2, =#LOAD
	ldr r3, =#DATA
	ldr r4, =#CLOCK
	ldr r5, =#GPIO_BSRR_OFFSET
	ldr r6, =#GPIO_BRR_OFFSET
	mov r7, #16//r7 = i
.max7219send_loop:
	mov r8, #1
	sub r9, r7, #1
	lsl r8, r8, r9 // r8 = mask
	str r4, [r1,r6] //HAL_GPIO_WritePin(GPIOA, CLOCK, 0);
	tst r0, r8
	beq .bit_not_set //bit not set
	str r3, [r1,r5]
	b .if_done
.bit_not_set:
	str r3, [r1,r6]
.if_done:
	str r4, [r1,r5]
	subs r7, r7, #1
	bgt .max7219send_loop
	str r2, [r1,r6]
	str r2, [r1,r5]
	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, pc}
	BX LR

max7219_init:
	//TODO: Initial max7219 registers.
	push {r0, r1, r2, lr}
	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	BL MAX7219Send
	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	BL MAX7219Send
	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x7
	BL MAX7219Send
	ldr r0, =#INTENSITY
	ldr r1, =0xA
	BL MAX7219Send
	ldr r0, =#SHUTDOWN
	ldr r1, =#0x1
	BL MAX7219Send
	pop {r0, r1, r2, pc}
	BX LR

fib:
    //TODO
    movs r2, #0
    movs r3, #1
    cmp r0, #0
    beq L1
    cmp r0, #39
    bgt L2
    cmp r0, #1
	beq L3
	b L4
L1: movs r4, #0
	bx lr
L2: movs r4, #0
	subs r4, #1
    bx lr
L3: movs r4, #1			//return 1
	bx lr				//return to caller
L4:	movs r5, #1
Loop:
	cmp r5, r0
	bge exit
	adds r4, r2, r3
	movs r2,r3
	movs r3,r4
	adds r5, #1
	b Loop
exit:
	bx lr			//return to the caller

Delay:
	/* delay for one second */
	push	{r0, lr}
	ldr	r0, =#0x1000	// 2

delay_L:
	subs	r0, r0, 1	// 1
	bne	delay_L		// 3 or 1
	pop	{r0, pc}
