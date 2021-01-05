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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "registers.h"

int arm_load_store(arm_core p, uint32_t ins) {
    int L;
    int action;
    int Rm;
    int S;
    int H;
    int I;
    int Rd;
    int U;
    
    int shift_imm;
    int shift;
    
    int valeur;
    int Rn;
    
    uint32_t value_word;
    uint16_t value_half;
    uint8_t value_byte;
   
   
    action = ins >> 25 & 0b111;
    if (action == 0)
    {
        S = (ins >> 6) & 1;
        H = (ins >> 5) & 1;
        L = (ins >> 20) & 1;
        I = (ins >> 22) & 1;
        
        Rm = ins & 0b1111;
        Rn = (ins >> 16) & 0b1111;
        
        
        shift_imm = (ins >> 7 ) &0b11111;
        shift = (ins >> 5) & 0b11;
        
        if (shift == 0b00)
        {
            //LSL (declage a gauche)
            Rm = Rm << shift_imm;
        }
        else if (shift == 0b01)
        {
            //LSR (decalage a droite)
            Rm = Rm >> shift_imm;
        }
        else if (shift == 0b10)
        {
            //ASR (decalage arithmetique a droite)
        }
        else if (shift == 0b11)
        {
            //ROR ou RRX
        }
        
        Rn = Rn + Rm;
        //Si valeur brut
        if (I == 1)
        {
            //Alors:
            //On recupere la valeur
            valeur = (ins >> 12) & 0b1111;
        }
        else
        {
            //Si non :
            //On recupere la valeur dans le registre
            valeur = arm_read_register(p,(ins >> 12) & 0b1111);
        }
        
        
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
    else
    {
        L = (ins >> 20) & 1;
        U = (ins >> 23) & 1;
        I = (ins >> 25) & 1;
        Rd = (ins >> 12) & 0b1111;
        Rn = arm_read_register(p ,(ins >> 16) & 0b1111);


        if (I == 1)
        {
            Rm = arm_read_register(p, ins & 0b1111);
            shift_imm = (ins >> 7 ) &0b11111;
            shift = (ins >> 5) & 0b11;
            //Alors:
            //Traitement pour un mot
            if (shift == 0b00)
            {
                //LSL (declage a gauche)
                Rm = Rm << shift_imm;
            }
            else if (shift == 0b01)
            {
                //LSR (decalage a droite)
                if (shift_imm == 0)
                {
                    Rm = 0;
                }
                else
                {
                    Rm = Rm >> shift_imm;
                }
            }
            else if (shift == 0b10)
            {
                //ASR (decalage arithmetique a droite)
                if (shift_imm == 0)
                {
                    if (((Rm >> 31) & 0b1) == 1)
                    {
                        Rm = 0xFFFFFFFF;
                    }
                    else
                    {
                        Rm = 0;
                    }
                }
                else
                {
                    Rm = Rm >> shift_imm;
                }
                
            }
            else if (shift == 0b11)
            {
                //ROR ou RRX
                if (shift_imm == 0)
                {
                    Rm = ((arm_read_register(p, CPSR) >> 29) &0b1) || Rm >> 1;
                }
                else
                {
                    Rm = (Rm >> shift_imm) & (Rm << (32-shift_imm));
                }
                
            }
        }
        else
        {
            //Si non :
            //On recupere le offset dans Rm pour factoriser le code
            Rm = ins & 0xFFF;
        }

        if (U == 1)
        {
            Rn = Rn + Rm;
        }
        else
        {
            Rn = Rn - Rm;
        }
        
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
    
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}