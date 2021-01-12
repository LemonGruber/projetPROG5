.global main
.text
main:
    mov r0,#0x12
    mov r1,#0x34
    add r0, r1, r0, lsl #8
    mov r1,#0x56
    add r0, r1, r0, lsl #8
    mov r1,#0x78
    add r0, r1, r0, lsl #8

    mov r2,#0x2000
    str r0,[r2]

    add r2,r2,#32
    strh r0,[r2]

    add r2,r2,#16
    strb r0,[r2]

    mov r2,#0x2000
    ldrb r3,[r2]

    mov r3,#0
    ldrh r3,[r2]

    mov r3,#0
    ldr r3,[r2]

    mov r3,#0
    add r2,r2,#32
    ldrh r3,[r2]

    mov r3,#0
    ldrb r3,[r2]

    mov r3,#0
    add r2,r2,#16
    ldrb r3,[r2]



fin:
    swi 0x123456
