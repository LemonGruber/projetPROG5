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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>


int arm_branch(arm_core p, uint32_t ins) {
    
    uint8_t L = (ins >> 24) & 1;
    uint8_t cond = (ins >> 28) & 0xF ;
    uint32_t immediat_signe = ins & 0xFFFFFF;
    
    uint32_t val_pc;
    
    val_pc = arm_read_register(p,15);
    
    if (L == 1)
    {
        arm_write_register(p,14,val_pc-4);
    }
    
    if (cond == 0xF)
    {
        //BLX
        arm_write_register(p,15,val_pc + (immediat_signe & (0xFF << 24) << 2));
    }
    else
    {
        //B/BL
        val_pc = val_pc + ((immediat_signe | (0xFF << 24)) << 2);
        arm_write_register(p,15,val_pc);
    }
    return 0;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    } 
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    uint8_t bit_21 = get_bit(ins, 21);
    uint8_t Rd = get_bits(ins, 15, 12);
    uint32_t value;

    if (bit_21 == 1)
    {
        if (get_bit(ins, 22) == 1)
        {
            value = arm_read_register(p, SPSR);
            arm_write_register(p, Rd, value);
        }
        else
        {
            value = arm_read_register(p, CPSR);
            arm_write_register(p, Rd, value);
        }
        return value;
    }
    return UNDEFINED_INSTRUCTION;
}
