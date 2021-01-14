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

        if (I == 1) // type de offset
        {
            offset_8 = ((ins >> 8) << 4) | (ins & 0b1111);
        }
        else
        {
            offset_8 = ins & 0b1111; // offset au lie de Rm
        }
        
        if (U == 1) // operation +
        {
            adresse = arm_read_register(p, Rn) + offset_8;
        }
        else        // operation -
        {
            adresse = arm_read_register(p, Rn) - offset_8;
        }

        if (W == 1)
        {
            if (P == 0)
            {
                return UNDEFINED_INSTRUCTION; //UNPREDICTABLE;
            }
            else // P == 1
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        
        if (L == 0 && S == 0 && H == 1)
        {
            //store halfword
            value_half = arm_read_register(p, Rd);
            arm_write_half(p, arm_read_register(p, Rn), value_half);
        }
        else if(L == 0 && S == 1)
        {
            // load double
            // store double
            // Non pris en compte. #TODO
        }
        else if (L == 1 && S == 0 && H == 1)
        {
            //load halfword unsigned
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_half);
        }
        else if (L == 1 && S == 1 && H == 0)
        {
            //load byte signed
            arm_read_byte(p,arm_read_register(p, Rn), &value_byte);
            arm_write_register(p, Rd, value_byte);
        }
        else if (L == 1 && S == 1 && H == 1)
        {
            //load halfword signed
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_byte);
        }
        else
        {
            // On aurait pas du être ici
        }
       
        if (P == 0)
        {
            if (W == 0) // Mise à jour des registres
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

        if (U == 1) // operation +
        {
            adresse = arm_read_register(p, Rn) + index;
        }
        else        // operation -
        {
            adresse = arm_read_register(p, Rn) - index;
        }
        
        if (P == 0)
        {  
            if (W == 0)
            {
                write_load_reg_mem(p, arm_read_register(p, Rn), Rd, L, B);
            }
            else // W == 1
            {
                if (L == 1)
                {
                    if (B == 1) //Load unsigned byte user acces
                    {
                        Execution_Load_Usr_Byte(p, arm_read_usr_register(p, Rn), Rd);
                    }
                    else        // Load word user acces
                    {
                        Execution_Load_Usr(p, arm_read_usr_register(p, Rn), Rd);
                    }
                }
                else
                {
                    if (B == 1) // Store unsigned byte user acces
                    {
                        Execution_Store_Usr_Byte(p, arm_read_usr_register(p, Rn), Rd);
                    }
                    else        //Store word user acces
                    {
                        Execution_Store_Usr(p, arm_read_usr_register(p, Rn), Rd);
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
            write_load_reg_mem(p, adresse, Rd, L, B);
        }
    }
    
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    
    int S, W, L, P, U, PC, j, bit_reg, sys;
    uint32_t Rn, start_address, end_address, addr_Rn;
    int sum = 0, i;
    
    // Bit récupéré dans l'instruction
    P = ins >> 24 & 1;
    U = ins >> 23 & 1;
    S = ins >> 22 & 1;
    W = ins >> 21 & 1;
    L = ins >> 20 & 1;
    PC = ins >> 15 & 1;
    Rn = ins >> 16 & 0xF;
    
    //On regarde si on est en mode system ou user
    sys = Est_Sys_Ou_User(p);
    if (L && sys && S)
    {
        return UNDEFINED_INSTRUCTION;
    }
    
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
    else if (P && !U) // 10 - decrement after
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
    
    if (W)
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

void Execution_Load_Usr_Byte(arm_core p, uint32_t addr, int reg){
    
    uint8_t value;
    
    arm_read_byte(p,addr,&value);
    arm_write_usr_register(p, reg, value);
}

void Execution_Load_Byte(arm_core p, uint32_t addr, int reg){
    
    uint8_t value;
    
    arm_read_byte(p,addr,&value);
    arm_write_register(p, reg, value);
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

void Execution_Store_Usr_Byte(arm_core p, uint32_t addr, int reg){
    
    uint8_t value;
    
    value = arm_read_usr_register(p,reg);
    arm_write_byte(p, addr, value);
}

void Execution_Store_Byte(arm_core p, uint32_t addr, int reg){
    
    uint8_t value;
    
    value = arm_read_register(p,reg);
    arm_write_byte(p, addr, value);
}

void write_load_reg_mem(arm_core p, int adresse, int Rd, int L, int B){
    switch (L){
    case 0:
        switch (B){
            case 0: // Store word
                Execution_Store(p, adresse, Rd);
                break;
            case 1: // Store unsigned byte
                Execution_Store_Byte(p, adresse, Rd);
                break;
        }
        break;
    case 1:
        switch (B){
            case 0: //Load word
                Execution_Load(p, adresse, Rd);
                break;
            case 1: // Load unsigned byte
                Execution_Load_Byte(p, adresse, Rd);
                break;
        }
        break;
    }
}