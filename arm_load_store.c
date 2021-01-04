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
    int rm;
    int S;
    int H;
    int I;
    
    int shift_imm;
    int shift;
    
    
    int valeur;
    int address;
    
    uint16_t value;
    
   
    action = ins >> 24 & 0b111;
    if (action == 0)
    {
        S = (ins >> 6) & 1;
        H = (ins >> 5) & 1;
        L = (ins >> 20) & 1;
        I = (ins >> 22) & 1;
        
        rm = ins & 0b1111;
        address = (ins >> 16) & 0b1111;
        
        
        shift_imm = (ins >> 7 ) &0b11111;
        shift = (ins >> 5) & 0b11;
        
        if (shift == 0b00)
        {
            //LSL (declage a gauche)
            rm = rm << shift_imm;
        }
        else if (shift == 0b01)
        {
            //LSR (decalage a droite)
            rm = rm >> shift_imm;
        }
        else if (shift == 0b10)
        {
            //ASR (decalage arithmetique a droite)
        }
        else if (shift == 0b11)
        {
            //ROR ou RRX
        }
        
        address = address + rm;
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
            arm_write_half(p,address,valeur);
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
            arm_read_half(p,address,*value);
            
        }
        else if (L == 1 && S == 1 && H == 0)
        {
            //load byte signed
            arm_read_byte(p,address,*value);
        }
        else if (L == 1 && S == 1 && H == 1)
        {
            //load halfword signed
            arm_read_half(p,address,*value);
        }
        else
        {
            //couille
        }
        //Traitement pour un demi mot
       
    }
    else
    {
        //Traitement pour un mot
        if (L == 1)
        {

            //Load word
        }
        else
        {
            //store word
        }
    }
    
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
