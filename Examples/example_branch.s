.global main
.text

fonction_2:
    add r0,r0,r0
    mov pc,lr


fonction_1:
    add r0,r0,r0
    stmfd r13!,{r14,r0}
    bl fonction_2
    ldmfd r13!,{r14,r5}
    mov pc,lr

main:
    mov r0,#8
    mov r1,#5
    mov r13,#0x2000

tant_que:
    sub r0,#1
    cmp r1,r0
    bne tant_que

fintantque:
    #sauvegarde des resgistre
    bl fonction_1
    #retour
    add r0,r0,r0

end:
    swi 0x123456
    