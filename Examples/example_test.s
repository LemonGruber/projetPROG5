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

    mov r2,#2000
   
    strb r0,[r2]
    ldr r5,[r2]

    mov r5,#0
    
    str r0,[r2]
    ldrb r5,[r2]



end:
    swi 0x123456
