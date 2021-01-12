.global main
.text
main:
    mov r0,#8
    mov r1,#10

    subs r0,r1
    
    mrs r3,CPSR
    mrs r4,SPSR

end:
    swi #0x123456
