.global main
.text
main:
    mov r0,#0b0101
    mov r1,#0b0001
    
    and r2,r0,r1
    and r3,r1,r0

    mov r3,#0
    mov r2,#0
    and r2,r0,#0b0001
    
    mov r2,#0
    eor r2,r0,r1

    mov r2,#0
    sub r2,r0,r1

    mov r2,#0
    sub r2,r1,r0

    mov r2,#0
    sub r2,r1,r0

    mov r2,#0
    rsb r2,r1,r0

    mov r2,#0
    add r2,r1,r0

    mov r2,#0
    adc r2,r1,r0

    mov r2,#0
    sbc r2,r1,r0

    mov r2,#0
    rsc r2,r1,r0

    mov r2,#0

    tst r1,r0
    teq r1,r0
    cmp r0,r1
    cmn r1,r0


    mov r2,#0
    bic r2,r1,r0

    mov r2,#0
    mvn r2,#0b1101

    mov r2,#0
    orr r2,r0,r1

    mov r2,#0

fin:
    swi 0x123456
