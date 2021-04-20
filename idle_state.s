/*
 * idle_state.s
 *
 * author: Adem Yildiz
 *
 *
 * displaying idle state
 */


.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOC base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOC ODR register offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOC base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOC ODR register offset
.equ wait,             (2000)

/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text

/* main function */
.section .text
main:
	ldr r3, =RCC_IOPENR
	ldr r2, [r3]
	movs r1, 0x3
	orrs r2, r2, r1
	str r2, [r3]

/* making PB port output for SSD's numbers: A, B, C, D, E, F, G */
	ldr r3, =GPIOB_MODER
	ldr r2, [r3]
	ldr r1, =0x1555
	ands r2, r2, r1
	str r2, [r3]

/* making PA port output for SSD's digits: D1, D2, D3 D4 */
	ldr r3, =GPIOA_MODER
	ldr r2, [r3]
	ldr r1, =0xFF /* represents 11111111 */
	lsls r1, r1, #10
	bics r2, r2, r1

	ldr r1, =0x55 /* 01 01 01 01 -> pa5-6-7-8 */
	lsls r1, r1, #10
	orrs r2, r2, r1
	str r2, [r3]

/*------------------------------------------------------------------------------*/
/* displaying number on SSD digits one by one  */
display:

	/* enable first digit */
	ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #5 /* first digit (D1) is specified */
    orrs r2, r2, r1
    str r2, [r3]
	/* enable first digit's leds */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x24 /* number (2) that will be written is specified  */
    ands r2, r2, r1
    str r2, [r3]

	/* let the digit1 wait */
    ldr r0, =wait /*holds the number for a while*/
    delay_d1:
    subs r0, r0, #1
    bne delay_d1

	/* disable first digit */
    ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #5 /* cleaning first digit */
    bics r2, r2, r1
    str r2, [r3]
	/* disable first digit's led */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x7F /* cleaning the number */
    orrs r2, r2, r1
    str r2, [r3]
/*------------------------------------------------------------------------------*/

	/* enable second digit */
	ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #6 /* second digit (D2) is specified */
    orrs r2, r2, r1
    str r2, [r3]
	/* enable second digit's leds */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x40 /* number (0) that will be written is specified  */
    ands r2, r2, r1
    str r2, [r3]

	/* let the digit2 wait */
    ldr r0, =wait
    delay_d2:
    subs r0, r0, #1
    bne delay_d2

	/* disable second digit */
    ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #6 /* cleaning second digit */
    bics r2, r2, r1
    str r2, [r3]
	/* disable second digit's led */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x7F /* cleaning the number in second digit */
    orrs r2, r2, r1
    str r2, [r3]
/*------------------------------------------------------------------------------*/

	/* enable third digit */
	ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #7 /* third digit (D3) is specified */
    orrs r2, r2, r1
    str r2, [r3]
	/* enable third digit's leds */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x12 /* number (5) that will be written is specified  */
    ands r2, r2, r1
    str r2, [r3]

	/* let the digit3 wait */
    ldr r0, =wait
    delay_d3:
    subs r0, r0, #1
    bne delay_d3

	/* disable third digit */
    ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #7 /* cleaning third digit */
    bics r2, r2, r1
    str r2, [r3]
	/* disable third digit's led */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x7F /* cleaning the number in third digit */
    orrs r2, r2, r1
    str r2, [r3]
/*------------------------------------------------------------------------------*/

	/* enable fourth digit */
	ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #8 /* second digit (D2) is specified */
    orrs r2, r2, r1
    str r2, [r3]
	/* enable fourth digit's leds */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x0 /* number (8) that will be written is specified  */
    ands r2, r2, r1
    str r2, [r3]

	/* let the digit4 wait */
    ldr r0, =wait
    delay_d4:
    subs r0, r0, #1
    bne delay_d4

	/* disable fourth digit */
    ldr r3, =GPIOA_ODR
    ldr r2, [r3]
    movs r1, 0x1
    lsls r1, r1, #8 /* cleaning fourth digit */
    bics r2, r2, r1
    str r2, [r3]
	/* disable fourth digit's led */
    ldr r3, =GPIOB_ODR
    ldr r2, [r3]
    movs r1, 0x7F /* cleaning the number in fourth digit */
    orrs r2, r2, r1
    str r2, [r3]

    b display
    b .
    nop
