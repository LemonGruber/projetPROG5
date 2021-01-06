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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {
    uint32_t ins;
    int result;
    
    result = arm_fetch( p, &ins);
    
    uint8_t bit_31 = (ins >> 31) & 1;
    uint8_t bit_30 = (ins >> 30) & 1;
    uint8_t bit_29 = (ins >> 29) & 1;
    uint8_t bit_28 = (ins >> 28) & 1;
    uint8_t bit_27 = (ins >> 27) & 1;
    uint8_t bit_26 = (ins >> 26) & 1;
    uint8_t bit_25 = (ins >> 25) & 1;
    uint8_t bit_24 = (ins >> 24) & 1;
    uint8_t bit_23 = (ins >> 23) & 1;
    uint8_t bit_22 = (ins >> 22) & 1;
    uint8_t bit_21 = (ins >> 21) & 1;
    uint8_t bit_20 = (ins >> 20) & 1;
    uint8_t bit_7 = (ins >> 7) & 1;
    uint8_t bit_6 = (ins >> 6) & 1;
    uint8_t bit_5 = (ins >> 5) & 1;
    uint8_t bit_4 = (ins >> 4) & 1;
    
    if (bit_31 == 1 && bit_30 == 1 && bit_29 == 1 && bit_28 == 1)
    {
        //unconditional instruction
    }
    else if(bit_24 == 1 && bit_25 == 1 && bit_26 == 1 && bit_27 == 1)
    {
        //software interuption
        result = SOFTWARE_INTERRUPT ;
    }
    else if (bit_4 == 1 && bit_24 == 0 && bit_25 == 1 && bit_26 == 1 && bit_27 == 1)
    {
        //coprossesor register transfere
        result = arm_coprocessor_others_swi(p,ins);
    }
    else if (bit_4 == 0 && bit_24 == 0 && bit_25 == 1 && bit_26 == 1 && bit_27 == 1)
    {
        //coprossesor data processing
        result = arm_coprocessor_others_swi(p,ins);
    }
    else if (bit_25 == 0 && bit_26 == 1 && bit_27 == 1)
    {
        //copros load/store
        result = arm_coprocessor_load_store(p,ins);
    }
    else if (bit_25 == 1 && bit_26 == 0 && bit_27 == 1)
    {
        //branchement link
        result = arm_branch(p,ins);
    }
    else if (bit_25 == 0 && bit_26 == 0 && bit_27 == 1)
    {
        //multiple load/store
        result = arm_load_store_multiple(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 1 && bit_25 == 1 && bit_20 == 1 && bit_21 == 1 && bit_22 == 1 && bit_23 == 1 && bit_24 == 1 && bit_4 == 1 && bit_5 == 1 && bit_6 == 1 && bit_7 == 1)
    {
        //Architecturaly undifined
        result = 1;
    }
    else if (bit_27 == 0 && bit_26 == 1 && bit_25 == 1 && bit_4 == 1)
    {
        //media instruction
    }
    else if (bit_27 == 0 && bit_26 == 1 && bit_25 == 1 && bit_4 == 0)
    {
        //register offset load/store
        result = arm_load_store(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 1 && bit_25 == 0)
    {
        //load store immadate offset
        result = arm_load_store(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 == 1 && bit_24 == 1 && bit_23 == 0 && bit_21 == 1 && bit_20 == 0)
    {
        //move immediate to statue register
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==1 && bit_24 == 1 && bit_23 == 0 && bit_21 == 0 && bit_20 == 0)
    {
        //undifinded instruction
        result = UNDEFINED_INSTRUCTION ;
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==1)
    {
        //data processing imediant
        result = arm_data_processing_immediate_msr(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==0 && bit_7 == 1 && bit_4 == 1)
    {
        //extra ou multiple load/store 
        result = arm_load_store(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==0 && bit_24 == 1 && bit_23 == 0 && bit_20 == 0 && bit_7 == 0 && bit_4 == 1)
    {
        //miscellinious instruction
        result = arm_miscellaneous(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==0 && bit_7 == 0 && bit_4 == 1)
    {
        //data processing register shift
        result = arm_data_processing_shift(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==0 && bit_4 == 1)
    {
        //miscellinious instruction
        result = arm_miscellaneous(p,ins);
    }
    else if (bit_27 == 0 && bit_26 == 0 && bit_25 ==0 && bit_4 == 0)
    {
        //data processing immediate shift
        result = arm_data_processing_shift(p,ins);
    }
    else
    {
        
    }
    return result;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
