.global main
.text
debut:
    cmp r0,r1
    sub r0,#1
    ble debut
    mov pc, lr

main:
    mov r0,#0x12
    mov r1,#0x34
    add r0, r1, r0, lsl #8
    mov r1,#0x56
    add r0, r1, r0, lsl #8
    mov r1,#0x78
    add r0, r1, r0, lsl #8

    mov r1,#0x2000
    str r0,[r1]

    mov r0,#0
    ldrb r0,[r1]
    mov r0,#0
    ldrh r0,[r1]

    mov r0,#0
    mov r1,#0

    mov r0, #0b1001
    mov r1, #0b1000
    
    and r2,r0,r1

    mov r0,#0
    mov r1,#0
    mov r2,#0

    mov r0,#7
    mov r1,#5

test_boucle:
    cmp r0,r1
    ble debut

fin:
    swi 0x123456
