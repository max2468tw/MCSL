	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	X: .word 5
	Y: .word 10
	Z: .word 0

.text
	.global main

main:
	ldr r2, =X
	ldr r3, =Y
	ldr r0, [r2]
	ldr r1, [r3]
	movs r4, #10
	muls r0, r0, r4
	adds r0, r0, r1
	subs r5, r1, r0
	str r0, [r2]
	ldr r6, =Z
	str r5, [r6]

L: B L
