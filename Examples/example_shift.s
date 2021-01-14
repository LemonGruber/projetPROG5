.global main
.text
main:
    mov r0,#0b0001

    lsl r1,r0,#1
    lsr r1,#1
    asr r1,#2

    mov r1,#0b0001
    ror r1,#2

    add r0, r1, r0, lsl #8
    sub r0, r0, r1, lsr #8

fin:
    swi 0x123456
