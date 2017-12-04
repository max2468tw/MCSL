	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	x: .float 0.123
	y: .float 0.456
	z: .word 20

.text
	.global main

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

main:
	bl enable_fpu
	ldr r0,=x
	vldr.f32 s0,[r0]
	ldr r0,=y
	vldr s1,[r0]
	vadd.f32 s2,s0,s1
	// Your code start from here
	//Calculate the following values using FPU instructions
	//and show the register result in your report

	vsub.f32 s2,s0,s1// s2=x-y
	vmul.f32 s2,s0,s1// s2=x*y
	vdiv.f32 s2,s0,s1// s2=x/y

	ldr r0,=z // load z into r0,
	ldr r0,[r0]
	vmov s2,r0 // copy z from r0 to s2,
	VCVT.F32.U32 s2,s2// convert z from U32 to float representation F32 in s2
	vadd.f32 s3,s1,s0
	vadd.f32 s3,s3,s2// calculate s3=z+x+y

L: b L
