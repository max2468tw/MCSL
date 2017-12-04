	.syntax unified
	.cpu cortex-m4
	.thumb

.text
	.global main
	.equ RCC_BASE,0x40021000
	.equ RCC_CR,0x0
	.equ RCC_CFGR,0x08
	.equ RCC_PLLCFGR,0x0c
	.equ RCC_CCIPR,0x88
	.equ RCC_AHB2ENR,0x4C
	.equ RNG_CLK_EN,18

	// Register address for RNG (Random Number Generator)
	.equ RNG_BASE,0x50060800 //RNG BASE Address
	.equ RNG_CR_OFFSET,0x00 //RNG Control Register
	.equ RNGEN,2 // RNG_CR bit 2

	.equ RNG_SR_OFFSET,0x04 //RNG Status Register
	.equ DRDY,0 // RNG_SR bit 0
	.equ RNG_DR_OFFSET,0x08 //RNG Data Register (Generated random number!)
	//Data Settings for 3.4.4
	.equ SAMPLE,1000000

set_flag:
	ldr r2,[r0,r1]
	orr r2,r2,r3
	str r2,[r0,r1]
	bx lr

enable_fpu:
  	//Your code start from here
  	// CPACR is located at address 0xE000ED88
	LDR.W R0, =0xE000ED88 // Read CPACR
	LDR R1, [R0] // Set bits 20-23 to enable CP10 and CP11 coprocessors
	ORR R1, R1, #(0xF << 20) // Write back the modified value to the CPACR
	STR R1, [R0] // wait for store to complete
	DSB // reset pipeline now the FPU is enabled
	ISB
	bx lr

enable_rng:
	//Your code start from here
	//Set the RNGEN bit to 1
	LDR.W R0, =RNG_BASE // Read CPACR
	LDR R1, [R0] // Set bits 20-23 to enable CP10 and CP11 coprocessors
	ORR R1, R1, #(0x1 << RNGEN) // Write back the modified value to the CPACR
	STR R1, [R0] // wait for store to complete
	DSB // reset pipeline now the FPU is enabled
	ISB
	bx lr

get_rand:
	//Your code start from here
	//read RNG_SR
	//check DRDY bit, wait until to 1
	//read RNG_DR for random number and store into a register for later usage
	bx lr

main:
	//RCC Settings
	ldr r0,=RCC_BASE
	ldr r1,=RCC_CR
	ldr r3,=#(1<<8) //HSION
	bl set_flag
	ldr r1,=RCC_CFGR
	ldr r3,=#(3<<24) //HSI16 selected
	bl set_flag
	ldr r1,=RCC_PLLCFGR
	ldr r3,=#(1<<24|1<<20|1<<16|10<<8|2<<0)
	bl set_flag
	ldr r1,=RCC_CCIPR
	ldr r3,=#(2<<26)
	bl set_flag
	ldr r1,=RCC_AHB2ENR
	ldr r3,=#(1<<RNG_CLK_EN)
	bl set_flag
	ldr r1,=RCC_CR
	ldr r3,=#(1<<24) //PLLON
	bl set_flag

chk_PLLON:
	ldr r2,[r0,r1]
	ands r2,r2,#(1<<25)
	beq chk_PLLON

	//Your code start from here
	//Enable FPU,RNG
	bl enable_fpu
	bl enable_rng
	ldr r4, =SAMPLE
	//Generate 2 random U32 number x,y
	LDR.W R0, =0x50060808 // Read CPACR
L3:	subs r4,r4,#1
	cmp r4,#0
	blt L4
	LDR R1, [R0] // Set bits 20-23 to enable CP10 and CP11 coprocessors
	VMOV s1,r1 // copy z from r0 to s2,
	VCVT.F32.U32 s1,s1// convert z from U32 to float representation F32 in s2
	LDR.W R0, =0x50060808 // Read CPACR
	LDR R2, [R0] // Set bits 20-23 to enable CP10 and CP11 coprocessors
	VMOV s2,r2 // copy z from r0 to s2,
	VCVT.F32.U32 s2,s2// convert z from U32 to float representation F32 in s2
	MOVS R3,#0xffffffff
	VMOV s3,r3 // copy z from r0 to s2,
	VCVT.F32.U32 s3,s3// convert z from U32 to float representation F32 in s2
	VDIV.f32 s1,s1,s3
	VDIV.f32 s2,s2,s3
	VMUL.f32 s1,s1,s1
	VMUL.f32 s2,s2,s2
	VADD.f32 s1,s1,s2
	VCMP.f32 s1,s4
	VMRS APSR_nzcv, FPSCR
	bge L3
	blt L1
L1: VADD.f32 s5,s5,s4
	b L3
L4: VMOV.f32 s4,#4.0
	VMUL.f32 s5,s5,s4
	ldr r3, =SAMPLE
	VMOV s3,r3 // copy z from r0 to s2,
	VCVT.F32.U32 s3,s3// convert z from U32 to float representation F32 in s2
	VDIV.F32 s5,s5,s3
	//Map x,y in unit range [0,1] using FPU

	//Calculate the z=sqrt(x^2+y^2) using FPU
	//Show the result of z in your report
L: 	b L
