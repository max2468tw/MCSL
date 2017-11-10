	.syntax unified
	.cpu cortex-m4
	.thumb

.data
    result: .byte 0

.text
    .global main
	.equ X, 0x55AA
	.equ Y, 0xAA55

hamm:
    //TODO
	eors r3, r0, r1
	cmp r3, #0
	bne L1
	beq L2
L1:	adds r5, #1
	subs r4, r3, #1
	ands r3, r4
	cmp r3, #0
	bne L1
L2:	strb r5, [r2]
	bx lr

main:
    ldr r0, =X //This code will cause assemble error. Why? And how to fix.
    ldr r1, =Y
    ldr r2, =result
    bl hamm

L: b L
