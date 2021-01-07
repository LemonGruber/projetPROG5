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
    /*  uint8_t bit_31 = (ins >> 31) & 1;
    uint8_t bit_30 = (ins >> 30) & 1;
    uint8_t bit_29 = (ins >> 29) & 1;
    uint8_t bit_28 = (ins >> 28) & 1;
    uint8_t bit_27 = (ins >> 27) & 1;
    uint8_t bit_26 = (ins >> 26) & 1;
    uint8_t bit_25 = (ins >> 25) & 1;*/
    uint8_t bit_24 = (ins >> 24) & 1;
    uint8_t bit_23 = (ins >> 23) & 1;
    uint8_t bit_22 = (ins >> 22) & 1;
    uint8_t bit_21 = (ins >> 21) & 1;
    uint8_t bit_20 = (ins >> 20) & 1;
    uint8_t bit_7 = (ins >> 7) & 1;
    uint8_t bit_6 = (ins >> 6) & 1;
    uint8_t bit_5 = (ins >> 5) & 1;
    uint8_t bit_4 = (ins >> 4) & 1;
    uint8_t champ;

    champ = (uint8_t) ((ins & 0xE000000) >> 25);
    switch (champ){
        case 0:
            switch (bit_4){
                case 0:
                    if (bit_24 == 1 && bit_23 == 0 && bit_20 == 0)
                    {             
                        // Miscellaneous instruction
                        result = arm_miscellaneous(p,ins);
                    }
                    else
                    {
                        // Data processing immed_shift
                        result = arm_data_processing_shift(p,ins);
                    }
                    break;
                case 1:
                    switch (bit_7){
                        case 0:
                            if (bit_24 == 1 && bit_23 == 0 && bit_20 == 0)
                            {
                                // Miscellaneous instruction
                                result = arm_miscellaneous(p,ins);
                            }
                            else 
                            {
                                // Data processing register shift
                                result = arm_data_processing_shift(p,ins);
                            }
                            break;
                        case 1: // A vérifier !!! Possiblement mauvaise fonction
                            // Multiplies Extra load/stores
//                            result = arm_multiplie_extra(p,ins);
                            break;
                    }
                    break;
            }
            break;
        case 1:
            if (bit_24 == 1 && bit_23 == 0 && bit_20 == 0)
            {
                switch (bit_21){
                    case 0:
                        // Endefined instruction
                        result = UNDEFINED_INSTRUCTION ;
                        break;
                    case 1:
                        // Move immeditate to status register
                        // #TODO créer la fonction ?
                        break;
                }
            }
            else
            {
                // Data processing immediat
                result = arm_data_processing_shift(p,ins);
            }
            break;
        case 2:
            // Load Sotre imme offset
            result = arm_load_store(p,ins);
            break;
        case 3:
            switch (bit_4){
                case 0:
                    // Load store register offset
                    result = arm_load_store(p,ins);
                    break;
                case 1:
                    if (bit_20 == 1 && bit_21 == 1 && bit_22 == 1 && bit_23 == 1 && bit_24 == 1 && bit_5 == 1 && bit_6 == 1 && bit_7 == 1)
                    {
                        // Architecture undefined
                        // #TODO bonus
                        result = 1;
                    }
                    else
                    {
                        // Media instruction
                        // #TODO question au prof
                    }
                break;
            }
            break;
        case 4:
            // Load Store multiple
            result = arm_load_store_multiple(p,ins);
            break;
        case 5:
            // Branch and branch link
            result = arm_branch(p,ins);
            break;
        case 6:
            // Coprocessor load store and double register transfer
            result = arm_coprocessor_load_store(p,ins);
            break;
        case 7:
            switch (bit_24){
                case 0:
                    switch (bit_4){
                        case 0:
                            // Coprocessor data processing
                            result = arm_data_processing_immediate_msr(p, ins);
                            break;
                        case 1:
                            // Coprocessor register transfer
                            // #TODO a vérifier
                            result = arm_coprocessor_others_swi(p, ins);
                            break;
                    }
                    break;
                case 1:
                    // Software interrupt
                    result = SOFTWARE_INTERRUPT ;
                    break;
            }
            break;
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
