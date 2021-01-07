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
    
    char retour;
    
    int ruckkehr;
    
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
        ruckkehr = cond_fonct(p,cond,&retour);
        if (retour == 1)
        {
            printf("\n ecrire ici");
            val_pc = val_pc + ((immediat_signe | (0xFF << 24)) << 2);
            printf("\n val_pc : %8.8x \n",val_pc);
            arm_write_register(p,15,val_pc);
        }
    }
    return ruckkehr;
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
    return UNDEFINED_INSTRUCTION;
}

int cond_fonct (arm_core p, uint8_t cond, char *retour)
{
    uint32_t cpsr= arm_read_cpsr(p);
    uint8_t Z_flag = (cpsr >> Z) & 1;
    uint8_t N_flag = (cpsr >> N) & 1; 
    uint8_t C_flag = (cpsr >> C) & 1; 
    uint8_t V_flag = (cpsr >> V) & 1; 
    
    uint8_t bit_0 = (cond >> 0) &1;
    uint8_t bit_1 = (cond >> 1) &1;
    uint8_t bit_2 = (cond >> 2) &1;
    uint8_t bit_3 = (cond >> 3) &1;
    
    if (bit_3 == 0 && bit_2 == 0 && bit_1 == 0 && bit_0 == 0)
    {
        //EQ
        if (Z_flag == 1)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 0 && bit_1 == 0 && bit_0 == 1)
    {
        //NEQ
        if (Z_flag == 1)
        {
            *retour = 0;
        }
        else
        {
            *retour = 1;
        }
    }
    else if (bit_3 == 0 && bit_2 == 0 && bit_1 == 1 && bit_0 == 0)
    {
        //CS/HS
        if (C_flag == 1)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 0 && bit_1 == 1 && bit_0 == 1)
    {
        //CC/LO
        if (C_flag == 0)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 1 && bit_1 == 0 && bit_0 == 0)
    {
        //MI
        if (N_flag == 1)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 1 && bit_1 == 0 && bit_0 == 1)
    {
        //PL
        if (N_flag == 0)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 1 && bit_1 == 1 && bit_0 == 0)
    {
        //VS
        if (V_flag == 1)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 0 && bit_2 == 1 && bit_1 == 1 && bit_0 == 1)
    {
        //VC
        if (V_flag == 0)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 0 && bit_1 == 0 && bit_0 == 0)
    {
        //HI
        if (C_flag == 1 && Z_flag == 0)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 0 && bit_1 == 0 && bit_0 == 1)
    {
        //LS
        if (C_flag == 0 || Z_flag == 1)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 0 && bit_1 == 1 && bit_0 == 0)
    {
        //GE
        if (N_flag == V_flag)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 0 && bit_1 == 1 && bit_0 == 1)
    {
        //LT
        if (N_flag != V_flag)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 1 && bit_1 == 0 && bit_0 == 0)
    {
        //GT
        if (Z_flag == 0 && N_flag == V_flag)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 1 && bit_1 == 0 && bit_0 == 1)
    {
        //LE
        if (Z_flag == 1 || N_flag != V_flag)
        {
            *retour = 1;
        }
        else
        {
            *retour = 0;
        }
    }
    else if (bit_3 == 1 && bit_2 == 1 && bit_1 == 1 && bit_0 == 0)
    {
        *retour = 1;
    }
    
    return 0;
}
