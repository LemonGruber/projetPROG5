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
 * @brief stock tout les registe en brut sauf les registres que l'on ne connais pas (attention)
 */
struct registers_data {
    
    uint8_t mode;

    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;
    uint32_t R8;
    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t R12;
    uint32_t R13;
    uint32_t R14;
    uint32_t R15;
    uint32_t SPSR;
    uint32_t CPSR;
    uint32_t IR;
    uint32_t reg[15];
  
}registers_data;


registers registers_create() {
    registers r = NULL;
    r = malloc(sizeof(registers_data));
    return r;
}


void registers_destroy(registers r) {
    free(r);
}


uint8_t get_mode(registers r) {
    return r->mode;
} 

int current_mode_has_spsr(registers r) {
    return 0;
}

int in_a_privileged_mode(registers r) {
    return 0;
}

uint32_t read_register(registers r, uint8_t reg) {
    uint32_t value=0;
    return value;
}


uint32_t read_usr_register(registers r, uint8_t reg) {
    uint32_t value=0;
    switch (reg)
    {
        case 0:
            value=r->R0;
        break;
        case 1:
            value=r->R1;
        break;
        case 2:
            value=r->R2;
        break;
        case 3:
            value=r->R3;
        break;
        case 4:
            value=r->R4;
        break;
        case 5:
            value=r->R5;
        break;
        case 6:
            value=r->R6;
        break;
        case 7:
            value=r->R7;
        break;
        case 8:
            value=r->R8;
        break;
        case 9:
            value=r->R9;
        break;
        case 10:
            value=r->R10;
        break;
        case 11:
            value=r->R11;
        break;
        case 12:
            value=r->R12;
        break;
         case 13:
            value=r->R13;
        break;
         case 14:
            value=r->R14;
        break;
        case 15:
            value=r->R15;
        break;
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
    value = r->SPSR;
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
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
    r->SPSR = value;
}
