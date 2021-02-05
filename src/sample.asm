	.org $2000

reset:
	ldx #$42
	stx $01

loop:
	nop

	.org $fffc
	.word reset
	.word $0000