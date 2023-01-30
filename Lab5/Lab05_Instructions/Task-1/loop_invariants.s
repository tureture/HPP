	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 1
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3                               ; -- Begin function main
lCPI0_0:
	.long	0                               ; 0x0
	.long	1                               ; 0x1
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	w20, #2048
	movk	w20, #12207, lsl #16
	mov	w0, #1
	mov	w1, #2048
	movk	w1, #12207, lsl #16
	bl	_calloc
	mov	x19, x0
Lloh0:
	adrp	x0, l_str@PAGE
Lloh1:
	add	x0, x0, l_str@PAGEOFF
	bl	_puts
	mov	x8, #0
	add	x9, x19, #32
	mov	w10, #10000
Lloh2:
	adrp	x11, lCPI0_0@PAGE
Lloh3:
	ldr	d0, [x11, lCPI0_0@PAGEOFF]
	movi.2s	v1, #2
	mov	x11, #-3689348814741910324
	movk	x11, #52429
	movk	x11, #16414, lsl #48
	movi.2s	v2, #4
	movi.2s	v3, #6
	movi.2s	v4, #8
	mov	w12, #14464
	movk	w12, #1, lsl #16
LBB0_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_2 Depth 2
	mul	w13, w8, w10
	scvtf	d5, w13
	fmov	d6, x11
	fdiv	d5, d5, d6
	dup.2d	v5, v5[0]
	mov	x13, x9
	mov	w14, #10000
	fmov	d6, d0
LBB0_2:                                 ;   Parent Loop BB0_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	add.2s	v7, v6, v1
	add.2s	v16, v6, v2
	add.2s	v17, v6, v3
	sshll.2d	v18, v6, #0
	scvtf.2d	v18, v18
	sshll.2d	v7, v7, #0
	scvtf.2d	v7, v7
	sshll.2d	v16, v16, #0
	scvtf.2d	v16, v16
	sshll.2d	v17, v17, #0
	scvtf.2d	v17, v17
	fadd.2d	v18, v5, v18
	fadd.2d	v7, v5, v7
	fadd.2d	v16, v5, v16
	fadd.2d	v17, v5, v17
	ldp	q19, q20, [x13, #-32]
	ldp	q21, q22, [x13]
	fadd.2d	v18, v18, v19
	fadd.2d	v7, v7, v20
	fadd.2d	v16, v16, v21
	fadd.2d	v17, v17, v22
	stp	q18, q7, [x13, #-32]
	stp	q16, q17, [x13], #64
	add.2s	v6, v6, v4
	subs	x14, x14, #8
	b.ne	LBB0_2
; %bb.3:                                ;   in Loop: Header=BB0_1 Depth=1
	add	x8, x8, #1
	add	x9, x9, x12
	cmp	x8, x10
	b.ne	LBB0_1
; %bb.4:
	mov	x8, #0
	movi	d0, #0000000000000000
LBB0_5:                                 ; =>This Inner Loop Header: Depth=1
	add	x9, x19, x8
	ldp	d1, d2, [x9]
	ldp	d3, d4, [x9, #16]
	fadd	d0, d0, d1
	fadd	d0, d0, d2
	fadd	d0, d0, d3
	fadd	d0, d0, d4
	add	x8, x8, #32
	cmp	x20, x8
	b.ne	LBB0_5
; %bb.6:
	str	d0, [sp]
Lloh4:
	adrp	x0, l_.str.1@PAGE
Lloh5:
	add	x0, x0, l_.str.1@PAGEOFF
	bl	_printf
	mov	x0, x19
	bl	_free
	mov	w0, #0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpLdr	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.loh AdrpAdd	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str.1:                               ; @.str.1
	.asciz	"Done. sum = %15.3f\n"

l_str:                                  ; @str
	.asciz	"fast"

.subsections_via_symbols
