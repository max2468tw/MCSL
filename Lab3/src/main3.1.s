	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	user_stack:	.zero 128
	expr_result:	.word   0

.text
	.global main
	postfix_expr:  .asciz "-100 10 20 + - 10 +"
	.align 4

main:
	LDR	R0, =postfix_expr
	movs r1, r0
	ldr sp, =user_stack
	adds sp, sp, 128
L5:	ldrb r2, [r1]
	ldrb r6,[r1,1]
	cmp r2, #48
	bge L4
	cmp r6, #48
	bge L4
L3: cmp r2, #43
	beq L6
	bne L7
L6: pop {r7}
	pop {r8}
	adds r8,r8,r7
	push {r8}
	adds r1, 1
	ldrb r2, [r1]
	cmp r2, #0
	beq program_end
	adds r1, 1
	b L5
L7:	pop {r7}
	pop {r8}
	subs r8,r8,r7
	push {r8}
	adds r1, 1
	ldrb r2, [r1]
	cmp r2, #0
	beq program_end
	adds r1, 1
	b L5
L4: bl atoi
	push {r3}
	b L5
//TODO: Setup stack pointer to end of user_stack and calculate the expression using PUSH, POP operators, and store the result into expr_result

program_end:
	pop {r0}
	LDR	R1, =expr_result
	str r0,[r1]
	B	end
end:
 	b end

atoi://TODO: implement a ¡§convert string to integer¡¨ function
     //r3:num r4:minus r1:p r2:*p r5 10
    movs r3, #0
    movs r4, #1
while:
    ldrb r2, [r1]
	cmp r2, #32
	beq exit
	movs r5, #10
	muls r3, r3, r5
	cmp r2, #45
	beq L1
	bne L2
L1:
	movs r4, -1
	b L
L2:
	adds r3, r3, r2
	subs r3,r3, #48
L:	adds r1,r1, #1
	b while
exit:
	muls r3, r3, r4
	adds r1, #1
    BX LR
