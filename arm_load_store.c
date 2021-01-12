/*
Armator - simulateur de jeu d'instruction ARMv5T ? but p?dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G?n?rale GNU publi?e par la Free Software
Foundation (version 2 ou bien toute autre version ult?rieure choisie par vous).
Ce programme est distribu? car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp?cifique. Reportez-vous ? la
Licence Publique G?n?rale GNU pour plus de d?tails.
Vous devez avoir re?u une copie de la Licence Publique G?n?rale GNU en m?me
temps que ce programme ; si ce n'est pas le cas, ?crivez ? la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
?tats-Unis.
Contact: Guillaume.Huard@imag.fr
	 B?timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H?res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "registers.h"

int arm_load_store(arm_core p, uint32_t ins) {
    int B, H, I, L, P, S, U, W;
    int action, index, adresse;
    int Rm, Rd, Rn;    
    
    int shift_imm;
    int shift;
    
    uint32_t value_word;
    uint16_t value_half;
    uint8_t value_byte;
    uint8_t offset_8;
   
    Rd = (ins >> 12) & 0b1111;
    Rn = (ins >> 16) & 0b1111;
    W = (ins >> 21) & 1;
    U = (ins >> 23) & 1;
    P = (ins >> 24) & 1;
    action = ins >> 25 & 0b111;
    if (action == 0) // demi et double mots
    {
        S = (ins >> 6) & 1;
        H = (ins >> 5) & 1;
        L = (ins >> 20) & 1;
        I = (ins >> 22) & 1;

        if (I == 1)
        {
            offset_8 = ((ins >> 8) << 4) | (ins & 0b1111);
        }
        else
        {
            offset_8 = ins & 0b1111; // offset au lie de Rm
        }
        
        if (P == 0)
        {
            if (W == 1)
            {
                return UNDEFINED_INSTRUCTION; //UNPREDICTABLE;
            }
        }
        
        if (U == 1)
        {
            adresse = arm_read_register(p, Rn) + offset_8;
        }
        else
        {
            adresse = arm_read_register(p, Rn) - offset_8;
        }
        
        if (L == 0 && S == 0 && H == 1)
        {
            //store halfword
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            value_half = arm_read_register(p, Rd);
            arm_write_half(p, arm_read_register(p, Rn), value_half);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if(L == 0 && S == 1 && H == 0 )
        {
            //load double
            // Non pris en compte. #TODO
        }
        else if (L == 0 && S == 1 && H == 1)
        {
            //store double
            // Non pris en compte. #TODO
        }
        else if (L == 1 && S == 0 && H == 1)
        {
            //load halfword unsigned
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_half);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if (L == 1 && S == 1 && H == 0)
        {
            //load byte signed
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_byte(p,arm_read_register(p, Rn), &value_byte);
            arm_write_register(p, Rd, value_byte);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if (L == 1 && S == 1 && H == 1)
        {
            //load halfword signed
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_byte);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else
        {
            //couille
        }
       
        if (P == 1)
        {
            if (W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
        }

    }
    else
    {
        L = (ins >> 20) & 1;
        B = (ins >> 22) & 1;
        I = (ins >> 25) & 1;

        if (I == 1)
        {
            Rm = ins & 0b1111;
            shift_imm = (ins >> 7 ) &0b11111;
            shift = (ins >> 5) & 0b11;
            //Alors:
            //Traitement pour un mot
            if (shift == 0b00)
            {
                //LSL (declage a gauche)
                index = arm_read_register(p, Rm) << shift_imm;
            }
            else if (shift == 0b01)
            {
                //LSR (decalage a droite)
                if (shift_imm == 0)
                {
                    index = 0;
                }
                else
                {
                    index = arm_read_register(p, Rm) >> shift_imm;
                }
            }
            else if (shift == 0b10)
            {
                //ASR (decalage arithmetique a droite)
                if (shift_imm == 0)
                {
                    if (((Rm >> 31) & 1) == 1)
                    {
                        index = 0xFFFFFFFF;
                    }
                    else
                    {
                        index = 0;
                    }
                }
                else
                {
                    index = arm_read_register(p, Rm) >> shift_imm;
                }
                
            }
            else if (shift == 0b11)
            {
                //ROR ou RRX
                if (shift_imm == 0)
                {
                    index = ((arm_read_register(p, CPSR) >> 29) &0b1) || Rm >> 1;
                }
                else
                {
                    index = (arm_read_register(p, Rm) >> shift_imm) | (arm_read_register(p, Rm) << (32-shift_imm));
                }                
            }
        }
        else // I == 0
        {
            //Si non :
            //On recupere le offset dans index
            index = ins & 0xFFF;
        }

        if (U == 1)
        {
            adresse = arm_read_register(p, Rn) + index;
        }
        else
        {
            adresse = arm_read_register(p, Rn) - index;
        }
        
        if (P == 0)
        {  
            if (W == 0)
            {
                if (L == 1)
                {
                    if (B == 1) // Load unsigned byte
                    {
                        arm_read_byte(p, arm_read_register(p, Rn), &value_byte);
                        arm_write_register(p, Rd, value_byte);
                    }
                    else       //Load word
                    {
                        arm_read_word(p, arm_read_register(p, Rn), &value_word);
                        arm_write_register(p, Rd, value_word);
                    }
                }
                else
                {
                    if (B == 1) // Store unsigned byte
                    {
                        value_byte = arm_read_register(p, Rd);
                        arm_write_byte(p, arm_read_register(p, Rn), value_byte);
                    }
                    else        // Store word
                    {
                        value_word = arm_read_register(p, Rd);
                        arm_write_word(p, arm_read_register(p, Rn), value_word);
                    }
                }
            }
            else // W == 1
            {
                if (L == 1)
                {
                    if (B == 1) //Load unsigned byte
                    {
                        arm_read_byte(p, arm_read_register(p, Rn), &value_byte);
                        arm_write_usr_register(p, Rd, value_byte);
                    }
                    else
                    {
                        //Load word
                        arm_read_word(p, arm_read_register(p, Rn), &value_word);
                        arm_write_usr_register(p, Rd, value_word);
                    }
                }
                else
                {
                    if (B == 1) // Store unsigned byte
                    {
                        value_byte = arm_read_usr_register(p, Rd);
                        arm_write_byte(p, arm_read_register(p, Rn), value_byte);
                    }
                    else        //Store word
                    {
                        value_word = arm_read_usr_register(p, Rd);
                        arm_write_word(p, Rn, value_word);
                    }
                }
            }
            arm_write_register(p, Rn, adresse);
        }
        else // P == 1
        {
            if (W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            if (L == 1)
            {
                //Load word
                arm_read_word(p, adresse, &value_word);
                arm_write_register(p, Rd, value_word);
            }
            else
            {
                //Store word
                value_word = arm_read_register(p, Rd);
                arm_write_word(p, adresse, value_word);
            }
        }
    }
    
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    
    int S, W, L, P, U, PC, CondValid, j, bit_reg, sys;
    uint32_t cond, Rn, start_address, end_address, addr_Rn;
    int sum = 0, i;
    
    // Bit récupéré dans l'instruction
    P = ins >> 24 & 1;
    U = ins >> 23 & 1;
    S = ins >> 22 & 1;
    W = ins >> 21 & 1;
    L = ins >> 20 & 1;
    PC = ins >> 15 & 1;
    cond = ins >> 28 & 0xF;
    Rn = ins >> 16 & 0xF;
    
    //On regarde si on est en mode system ou user
    sys = Est_Sys_Ou_User(p);
    if (L && sys && S)
    {
        return UNDEFINED_INSTRUCTION;
    }
    
    //vérification si la condition est passé ou non
    CondValid = ConditionPassed(p, cond);
    
    //nombre de registre de la liste
    for (i = 0; i<=15; i++)
    {
        sum = sum + (ins >> i & 1);
    }
    
    addr_Rn = arm_read_register(p,Rn);
    
    if (!P && U) // 01 - increment after 
    {
        start_address = addr_Rn;
        end_address = addr_Rn + (sum * 4) - 4;
    }
    else if (P && U)  // 11 - increment before
    {
        start_address = addr_Rn + 4;
        end_address = addr_Rn + (sum * 4);
    }
    else if (!U && !P) // 00 - decrement after
    {
        start_address = addr_Rn - (sum * 4);
        end_address = addr_Rn - 4;
    }
    else // 10 - decrement before
    {
        start_address = addr_Rn - (sum * 4) + 4;
        end_address = addr_Rn;
    }
    j=0;
    
    if (CondValid && W)
    {
        if (U)  // U différencie le mode increment et decrement et donc la valeur de modification de Rn
        {
            arm_write_register(p, Rn,addr_Rn + (sum*4));
        }
        else
        {
            arm_write_register(p, Rn,addr_Rn - (sum*4));
        }
    }
    
    while (start_address <= end_address)
    {
     
        //Ici bit_reg sert à savoir si le registre j fait partie des registres à charger
        //et j est le numéro du registre   
        
        bit_reg = ins >> j & 1;
        
        if (bit_reg) //Le registre fait partie de la liste
        {
            if (L) //Load
            {
                if (S && !PC) // Si S et PC pas dans la liste
                {
                    if (arm_in_a_privileged_mode(p)) // Si mode privilégié
                    {
                        Execution_Load_Usr(p, start_address, j);
                    }
                    else
                    {
                        Execution_Load(p, start_address, j);
                    }
                }
                else
                {
                    Execution_Load(p, start_address, j);
                }
            }
            else // Store 
            {
                if (S) // Si S
                {
                    if (arm_in_a_privileged_mode(p)) // Si mode privilégié
                    {
                        Execution_Store_Usr(p, start_address, j);
                    }
                    else
                    {
                        Execution_Store(p, start_address, j);
                    }
                }
                else
                {
                    Execution_Store(p, start_address, j);
                }
            }
            
            start_address += 4;
        }
        j++;
    }
    
    
    //Si on a L, PC chargé et S alors CPSR est chargé depuis SPSR après avoir chargé les registes
    if (L && PC && S)
    {
        arm_write_cpsr(p,arm_read_spsr(p));
    }
    
    return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

int ConditionPassed(arm_core p, uint32_t cond) {
    
    int bit_Z, bit_N, bit_C, bit_V;
    uint32_t Reg_CPSR = arm_read_cpsr(p);
    
    bit_N = Reg_CPSR >> 31 & 1;
    bit_Z = Reg_CPSR >> 30 & 1;
    bit_C = Reg_CPSR >> 29 & 1;
    bit_V = Reg_CPSR >> 28 & 1;
    
    switch(cond){
      case 0x0:
        return bit_Z;
      case 0x1:
        return !bit_Z;
      case 0x2:
        return bit_C;
      case 0x3:
        return !bit_C;
      case 0x4:
        return bit_N;
      case 0x5:
        return !bit_N;
      case 0x6:
        return bit_V;
      case 0x7:
        return !bit_V;
      case 0x8:
        return (bit_C && !bit_Z); 
      case 0x9:
        return (!bit_C || bit_Z);
      case 0xA:
        return bit_N == bit_V;
      case 0xB:
        return bit_N != bit_V;
      case 0xC:
        return (!bit_Z && (bit_N == bit_V));
      case 0xD:
        return (bit_Z && (bit_N != bit_V));
      case 0xE:
        return 1;
      case 0xF:
        return 0;
    }
    return -1;   
}

int Est_Sys_Ou_User(arm_core p){
    uint32_t reg_CPSR = arm_read_cpsr(p);
    return (((reg_CPSR & 0x1F) == 0x1F) || ((reg_CPSR & 0x1F) == 0x10));
}

void Execution_Load(arm_core p, uint32_t addr, int reg){
    
    uint32_t value;
    
    arm_read_word(p,addr,&value);
    arm_write_register(p, reg, value);
}

void Execution_Load_Usr(arm_core p, uint32_t addr, int reg){
    
    uint32_t value;
    
    arm_read_word(p,addr,&value);
    arm_write_usr_register(p, reg, value);
}

void Execution_Store(arm_core p, uint32_t addr, int reg){
    
    uint32_t value;
    
    value = arm_read_register(p,reg);
    arm_write_word(p, addr, value);
}

void Execution_Store_Usr(arm_core p, uint32_t addr, int reg){
    
    uint32_t value;
    
    value = arm_read_usr_register(p,reg);
    arm_write_word(p, addr, value);
}











