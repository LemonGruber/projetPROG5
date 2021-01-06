/*
armator - simulateur de jeu d'instruction armv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
teRmes de la Licence Publique G�n�rale GNU publi�e par la Free Software
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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "registers.h"
#include <stdio.h>

int arm_load_store(arm_core p, uint32_t ins) {
    
    printf("\ndebut\n%u\n",ins);
    
    int Rm, Rd, Rn;
    int L, S, H, I, U, W, P;
    int action;
    int shift, shift_imm;
    
    int valeur;
    
    uint32_t address;
    uint32_t index;
    uint32_t value_word;
    uint16_t value_half;
    uint8_t value_byte;
    uint8_t offset_8;
   
   offset_8 = 1;
   offset_8 = offset_8;

   
    action = ins >> 25 & 0b111;
    P = ins >> 24 & 1;
    W = ins >> 21 & 1;
    L = (ins >> 20) & 1;
    U = (ins >> 23) & 1;
    
    Rn = (ins >> 16) & 0b1111;
    Rd = (ins >> 12) & 0b1111;
    
    if (action == 0)
    {
        S = (ins >> 6) & 1;
        H = (ins >> 5) & 1;
        I = (ins >> 22) & 1;
        
        if (I == 1)
        {
            offset_8 = ((ins >> 8) << 4) | (ins & 0b1111);
        }
        else
        {
            //offset_8 prend la valeur de Rm
            offset_8 = ins & 0xF;
        }
            //Si non :
            //On recupere la valeur dans le registre
        valeur = arm_read_register(p,(ins >> 12) & 0b1111);
        
        
        if (L == 0 && S == 0 && H == 1)
        {
            //store halfword
            arm_write_half(p,Rn,valeur);
        }
        else if(L == 0 && S == 1 && H == 0 )
        {
            //load double
        }
        else if (L == 0 && S == 1 && H == 1)
        {
            //store double
        }
        else if (L == 1 && S == 0 && H == 1)
        {
            //load halfword unsigned
            arm_read_half(p,Rn,&value_half);
            
        }
        else if (L == 1 && S == 1 && H == 0)
        {
            //load byte signed
            arm_read_byte(p,Rn,&value_byte);
        }
        else if (L == 1 && S == 1 && H == 1)
        {
            //load halfword signed
            arm_read_half(p,Rn,&value_half);
        }
        else
        {
            //couille
        }
        //Traitement pour un demi mot
       
    }
    else // action == 1
    {
        
        I = (ins >> 25) & 1;
        
        if (I == 1)
        {
            Rm = ins & 0b1111;
            shift_imm = (ins >> 7 ) & 0b11111;
            shift = (ins >> 5) & 0b11;
            
            if (shift == 0b00)
            {
                //LSL (declage a gauche)
                index = arm_read_register(p,Rm) << shift_imm;
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
                    index = arm_read_register(p,Rm) >> shift_imm;
                }
            }   
            else if (shift == 0b10)
            {
                //ASR (decalage arithmetique a droite)
                
                if (shift_imm == 0)
                {
                    int bit_sign = arm_read_register(p,Rm) >> 31 & 0b1;
                    if (bit_sign)
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
                    index = arm_read_register(p,Rm) >> shift_imm;
                }
            }
            else if (shift == 0b11)
            {
                //ROR ou RRX
                uint32_t C_flag = arm_read_register(p,CPSR);
                C_flag = (C_flag >> 29) & 0b1;
                if (shift_imm == 0)
                {
                    index = (C_flag << 31) | (arm_read_register(p,Rm) >> 1);
                }
                else 
                {
                    index = arm_read_register(p,Rm) >> (shift_imm) & (arm_read_register(p,Rm) << (32-shift_imm));
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
            address = arm_read_register(p,Rn) + index;
        }
        else
        {
            address = arm_read_register(p,Rn) + index;
        }
        if (P == 0)
        {  
            if (W == 0)
            {
                if (L == 1)
                {
                    //Load word
                    arm_read_word(p, Rn, &value_word);
                    arm_write_register(p, Rd, value_word);
                }
                else
                {
                    //Store word
                    value_word = arm_read_register(p, Rd);
                    arm_write_word(p, Rn, value_word);
                }
            }
            else // W == 1
            {
                if (L == 1)
                {
                    //Load word
                    arm_read_word(p, Rn, &value_word);
                    arm_write_usr_register(p, Rd, value_word);
                }
                else
                {
                    //Store word
                    value_word = arm_read_usr_register(p, Rd);
                    arm_write_word(p, Rn, value_word);
                }
            }
            arm_write_register(p, Rn, address);
        }
        else // P == 1
        {
            if (W == 1)
            {
                arm_write_register(p, Rn, address);
            }
            if (L == 1)
            {
                //Load word
                arm_read_word(p, Rn, &value_word);
                arm_write_usr_register(p, Rd, value_word);
            }
            else
            {
                //Store word
                value_word = arm_read_usr_register(p, Rd);
                arm_write_word(p, Rn, value_word);
            }
        }
        printf("\nfin\n");
    }
    arm_afficher_mem (p);
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}