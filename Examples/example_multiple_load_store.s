.global main
.text
main:
    ldr r1, =donnee
    

    ldmia r1!, {r6,r7,r8}

    mov r6,#0
    mov r7,#0
    mov r8,#0
    ldr r1, =donnee

    ldmib r1!, {r6,r7}

    mov r6,#0
    mov r7,#0
    mov r8,#0


    ldmda r1, {r6,r7,r8}

    mov r6,#0
    mov r7,#0
    mov r8,#0


    ldmdb r1, {r6,r7,r8}

    ldr r1, =nouv_donnee
    stmia r1, {r6,r7,r8}
    ldmia r1, {r9,r10,r11}
    
    mov r9,#0
    mov r10,#0
    mov r11,#0

    stmia r1, {r9,r10,r11}

    
    stmib r1, {r6,r7,r8}
    ldmia r1, {r9,r10,r11}
    
    mov r9,#0
    mov r10,#0
    mov r11,#0

    stmib r1!, {r9,r10,r11}


    stmda r1, {r6,r7,r8}
    ldmda r1, {r9,r10,r11}
    
    mov r9,#0
    mov r10,#0
    mov r11,#0
 
    stmda r1, {r9,r10,r11}





    stmdb r1, {r6,r7,r8}
    ldmda r1, {r9,r10,r11}

    mov r9,#0
    mov r10,#0
    mov r11,#0

    stmdb r1, {r9,r10,r11}




fin:
    swi 0x123456

.data
donnee:
    .word 0x11223344
    .word 0x55667788
    .word 0x22446688

nouv_donnee: