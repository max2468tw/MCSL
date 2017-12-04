	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	result: .word  0
	max_size:  .word  0

.text
	//.global main
	m: .word  0x5E
	n: .word  0x60

main:
	LDR	r0, =m
	LDR r0,[r0]
	LDR r1, =n
	LDR r1,[r1]
	push {r0}
	push {r1}
	movs r7, #0
	bl GCD
	ldr r2, =result
	str r5, [r2]
	ldr r2, =max_size
	str r7,[r2]
end:
	B end

GCD:
    //TODO: Implement your GCD function
    // r0 m r1 n r5 return
	cmp r0, #0
	beq L1;
	cmp r1, #0
	beq L2;
	ands r2,r0,1
	ands r3,r1,1
	orrs r4,r2,r3
	cmp r4, #0
	beq L3;
	cmp r2,#0
	beq L4
	cmp r3,#0
	beq L5
	b L6
L1:	movs r5, r1
	bx lr
L2:	movs r5, r0
	bx lr
L3: push {r0}
	push {r1}
	push {lr}
	adds r7, #3
	lsr r0,1
	lsr r1,1
	bl GCD
	movs r6, #2
	muls r5, r5, r6
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
L4:	push {r0}
	push {r1}
	push {lr}
	adds r7, #3
	lsr r0,1
	bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
L5: push {r0}
	push {r1}
	push {lr}
	adds r7, #3
	lsr r1,1
	bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
L6: push {r0}
	push {r1}
	push {lr}
	adds r7, #3
	cmp r1,r0
	bge LG
	subs r0,r0,r1
	b L
LG: subs r6,r1,r0
	movs r1,r0
	movs r0,r6
L:  bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	BX LR
