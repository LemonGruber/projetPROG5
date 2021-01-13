/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).
Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.
Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.
Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/


#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

/**
 * @struct registers_data
 * @brief stock tous les registres en brut
 */
struct registers_data {
    
    uint8_t mode;

    uint32_t R0, R1, R2, R3, R4, R5, R6, R7; // Tutti
/*    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;*/
    uint32_t R8, R9, R10, R11, R12; // USR
/*    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t R12;*/
    uint32_t R13, R13_svc, R13_abt, R13_und, R13_irq, R13_fiq; 
    uint32_t R14, R14_svc, R14_abt, R14_und, R14_irq, R14_fiq; ; // LR
    uint32_t R15; // PC
    uint32_t SPSR_svc, SPSR_abt, SPSR_und, SPSR_irq, SPSR_fiq;
    uint32_t CPSR;
    uint32_t R8_fiq, R9_fiq, R10_fiq, R11_fiq, R12_fiq; // FIQ

//usr, svc (for Supervisor mode), abt, und, irq and fiq
}registers_data;


registers registers_create() {
    registers r = NULL;
    r = malloc(sizeof(registers_data));
    r->mode = SVC;
    return r;
}


void registers_destroy(registers r) {
    free(r);
}


uint8_t get_mode(registers r) {
    return r->mode;
} 


int current_mode_has_spsr(registers r) {
    int retour = 0;
    switch (get_mode(r)) {
        case SVC:
        case ABT:
        case UND:
        case IRQ:
        case FIQ:
            retour = 1;
        break;
        default:
            retour = 0;
    }
    
    return retour;
}

int in_a_privileged_mode(registers r) {
    int retour = 0;
    switch (get_mode(r)) {
        case SVC:
        case ABT:
        case UND:
        case IRQ:
        case FIQ:
        case SYS:
            retour = 1;
        break;
        default:
            retour = 0;
    }
    
    return retour;
}

uint32_t read_register(registers r, uint8_t reg) {
    uint32_t value=0;

    switch(get_mode(r)){
        case FIQ:
            if ((8 <= reg && reg < 14) || reg == 17){
                value = acces_lecture_registre_FIQ(r, reg);
                break;
            }
        case IRQ:
            if (reg == 13 || reg == 14 || reg == 17){
                value = acces_lecture_registre_IRQ(r, reg);
                break;
            }
        case UND:
            if (reg == 13 || reg == 14 || reg == 17){
                value = acces_lecture_registre_UND(r, reg);
                break;
            }
        case ABT:
            if (reg == 13 || reg == 14 || reg == 17){
                value = acces_lecture_registre_ABT(r, reg);
                break;
            }
        case SVC:
            if (reg == 13 || reg == 14 || reg == 17){
                value = acces_lecture_registre_SVC(r, reg);
                break;
            }
        case SYS:
        case USR:
            value = read_usr_register(r, reg);
    }
    return value;
}


uint32_t read_usr_register(registers r, uint8_t reg) {
    uint32_t value=0;
    switch (reg)
    {
        case 0:
            value = r->R0;
        break;
        case 1:
            value = r->R1;
        break;
        case 2:
            value = r->R2;
        break;
        case 3:
            value = r->R3;
        break;
        case 4:
            value = r->R4;
        break;
        case 5:
            value = r->R5;
        break;
        case 6:
            value = r->R6;
        break;
        case 7:
            value = r->R7;
        break;
        case 8:
            value = r->R8;
        break;
        case 9:
            value = r->R9;
        break;
        case 10:
            value = r->R10;
        break;
        case 11:
            value = r->R11;
        break;
        case 12:
            value = r->R12;
        break;
         case 13:
            value = r->R13;
        break;
         case 14:
            value = r->R14;
        break;
        case 15:
            value = r->R15;
        break;
        case 16:
            value = r->CPSR;
    }
    return value;
}


uint32_t read_cpsr(registers r) {
    uint32_t value=0;
    value = r->CPSR;
    return value;
}


uint32_t read_spsr(registers r) {
    uint32_t value=0;
    switch (get_mode(r)){
        case FIQ:
            value = r->SPSR_fiq;
            break;
        case IRQ:
            value = r->SPSR_irq;
            break;
        case SVC:
            value = r->SPSR_svc;
            break;
        case ABT:
            value = r->SPSR_abt;
            break;
        case UND:
            value = r->SPSR_und;
            break;
    default:
        value = UNDEFINED_INSTRUCTION;
        break;
    }
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
    switch(get_mode(r)){
        case FIQ:
            if ((8 <= reg && reg < 14) || reg == 17){
                acces_ecriture_registre_FIQ(r, reg, value);
                break;
            }
        case IRQ:
            if (reg == 13 || reg == 14 || reg == 17){
                acces_ecriture_registre_IRQ(r, reg, value);
                break;
            }
        case UND:
            if (reg == 13 || reg == 14 || reg == 17){
                acces_ecriture_registre_UND(r, reg, value);
                break;
            }
        case ABT:
            if (reg == 13 || reg == 14 || reg == 17){
                acces_ecriture_registre_ABT(r, reg, value);
                break;
            }
        case SVC:
            if (reg == 13 || reg == 14 || reg == 17){
                acces_ecriture_registre_SVC(r, reg, value);
                break;
            }
        case SYS:
        case USR:
            write_usr_register(r, reg, value);
    }
}


void write_usr_register(registers r, uint8_t reg, uint32_t value) {
    switch (reg)
    {
        case 0:
            r->R0=value;
        break;
        case 1:
            r->R1=value;
        break;
        case 2:
            r->R2=value;
        break;
        case 3:
            r->R3=value;
        break;
        case 4:
            r->R4=value;
        break;
        case 5:
            r->R5=value;
        break;
        case 6:
            r->R6=value;
        break;
        case 7:
            r->R7=value;
        break;
        case 8:
            r->R8=value;
        break;
        case 9:
            r->R9=value;
        break;
        case 10:
            r->R10=value;
        break;
        case 11:
            r->R11=value;
        break;
        case 12:
            r->R12=value;
        break;
         case 13:
            r->R13=value;
        break;
         case 14:
            r->R14=value;
        break;
        case 15:
            r->R15=value;
        break;
    }
}


void write_cpsr(registers r, uint32_t value) {
    r->CPSR = value;
}

void write_spsr(registers r, uint32_t value) {
    switch (get_mode(r)){
        case FIQ:
            r->SPSR_fiq = value;
            break;
        case IRQ:
            r->SPSR_irq = value;
            break;
        case SVC:
            r->SPSR_svc = value;
            break;
        case ABT:
            r->SPSR_abt = value;
            break;
        case UND:
            r->SPSR_und = value;
            break;
        default:
            break;
    }
}

uint32_t acces_lecture_registre_FIQ(registers r, uint8_t reg){
    uint32_t value;
    switch (reg){
        case 8:
            value = r->R8_fiq;
            break;
        case 9:
            value = r->R9_fiq;
            break;
        case 10:
            value = r->R10_fiq;
            break;
        case 11:
            value = r->R11_fiq;
            break;
        case 12:
            value = r->R12_fiq;
            break;
         case 13:
            value = r->R13_fiq;
            break;
         case 14:
            value = r->R14_fiq;
            break;
        case 17:
            value = r->SPSR_fiq;
            break;
    }
    return value;
}

uint32_t acces_lecture_registre_IRQ(registers r, uint8_t reg){
    uint32_t value;
    switch (reg){
    case 13:
        value = r->R13_irq;
        break;
    case 14:
        value = r->R14_irq;
        break;
    case 17:
        value = r->SPSR_irq;
        break;
    }
    return value;
}

uint32_t acces_lecture_registre_UND(registers r, uint8_t reg){
    uint32_t value;
    switch (reg){
    case 13:
        value = r->R13_und;
        break;
    case 14:
        value = r->R14_und;
        break;
    case 17:
        value = r->SPSR_und;
        break;
    }
    return value;
}

uint32_t acces_lecture_registre_ABT(registers r, uint8_t reg){
    uint32_t value;
    switch (reg){
    case 13:
        value = r->R13_abt;
        break;
    case 14:
        value = r->R14_abt;
        break;
    case 17:
        value = r->SPSR_abt;
        break;
    }
    return value;
}

uint32_t acces_lecture_registre_SVC(registers r, uint8_t reg){
    uint32_t value;
    switch (reg){
    case 13:
        value = r->R13_svc;
        break;
    case 14:
        value = r->R14_svc;
        break;
    case 17:
        value = r->SPSR_svc;
        break;
    }
    return value;
}


void acces_ecriture_registre_FIQ(registers r, uint8_t reg, uint32_t value){
    switch (reg){
        case 8:
            r->R8_fiq = value;
            break;
        case 9:
            r->R9_fiq = value;
            break;
        case 10:
            r->R10_fiq = value;
            break;
        case 11:
            r->R11_fiq = value;
            break;
        case 12:
            r->R12_fiq = value;
            break;
         case 13:
            r->R13_fiq = value;
            break;
         case 14:
            r->R14_fiq = value;
            break;
        case 17:
            r->SPSR_fiq = value;
            break;
    }
}

void acces_ecriture_registre_IRQ(registers r, uint8_t reg, uint32_t value){
    switch (reg){
    case 13:
        r->R13_irq = value;
        break;
    case 14:
        r->R14_irq = value;
        break;
    case 17:
        r->SPSR_irq = value;
        break;
    }
}

void acces_ecriture_registre_UND(registers r, uint8_t reg, uint32_t value){
    switch (reg){
    case 13:
        r->R13_und = value;
        break;
    case 14:
        r->R14_und = value;
        break;
    case 17:
        r->SPSR_und = value;
        break;
    }
}

void acces_ecriture_registre_ABT(registers r, uint8_t reg, uint32_t value){
    switch (reg){
    case 13:
        r->R13_abt = value;
        break;
    case 14:
        r->R14_abt = value;
        break;
    case 17:
        r->SPSR_abt = value;
        break;
    }
}

void acces_ecriture_registre_SVC(registers r, uint8_t reg, uint32_t value){
    switch (reg){
    case 13:
        r->R13_svc = value;
        break;
    case 14:
        r->R14_svc = value;
        break;
    case 17:
        r->SPSR_svc = value;
        break;
    }
}