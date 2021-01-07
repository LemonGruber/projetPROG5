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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    
    int result = 0;
   
    printf("je suis ici pour une action \n");
    uint8_t bit_4 = (ins >> 4) &1;
    
    uint8_t bit_5 = (ins >> 5) &1;    //* Type de shift
    uint8_t bit_6 = (ins >> 6) &1;    //*
    
    uint8_t op = (ins >> 21) & 0xf;
    
    uint32_t val_1;
    uint32_t val_2;
    
    uint32_t buff;
    
    uint8_t S = (ins >> 20) & 1;
    
    int Rn = (ins >> 16) & 0xf;        //Premier operande, registre
    int Rm = ins & 0xf;                //Deuxieme operande (celui qui est shifter), registre
    int Rd = (ins >> 12) & 0xf;        //Registre de retour
    int Rs = (ins >> 8) & 0xf;         //Registe du decalage (si non imediat)
    int shift_imm = (ins >> 7) & 0x1f; //Decalage (si immediat)

    int decalage;
    uint32_t value = 0;
    
    uint8_t flags = 0;
    uint32_t buff_flags;
    
   
    
    if (bit_4 == 0)
    {
        //immediate shift
        decalage = shift_imm;
        
    }
    else
    {
        //register shift
        decalage = arm_read_register(p,Rs);
    }
    
    
    val_2 = arm_read_register(p,Rm);
    
    if (bit_5 == 0 && bit_6 == 0)
    {
        //LSL
        val_2 = val_2 << decalage;
    }
    else if (bit_5 == 1 && bit_6 == 0)
    {
        //LSR
        val_2 = val_2  >> decalage;
    }
    else if (bit_5 == 0 && bit_6 == 1)
    {
        //ASR
        buff = (~(1 << decalage)) << (32-decalage);
        val_2 = (val_2  >> decalage) | buff;
    }
    else if (bit_5 == 1 && bit_6 == 1 && decalage == 1)
    {
        //ROR (Rougeole Oreillons Rubéole)
        buff = val_2;
        val_2 = val_2  >> decalage;
        val_2 = val_2 | (buff << (32-decalage));
    }
   
    val_1 = arm_read_register(p,Rn);
    opcode(p,val_1,val_2,op,&value,&flags);
    arm_write_register(p,Rd,value);      
    
    if (S == 1)
    {
        buff_flags = flags;
        buff = arm_read_cpsr(p);
        buff = (buff & ~(UserMask)) | (buff_flags << 28);
        arm_write_cpsr(p,buff);
    }
    
            
    return result;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION ;
}

int arm_data_processing_immediate(arm_core p, uint32_t ins) {
    
    printf("\n ici pour le MOV \n");
    uint8_t op = (ins >> 21) & 0xF;
    uint8_t S = (ins >> 20) & 1;
    uint8_t Rn = (ins >> 16) & 0xF;
    uint8_t Rd = (ins >> 12) & 0xF;
    uint8_t rotate_imm = (ins >> 8) & 0xF;
    uint8_t immed_8 = (ins >> 0) & 0xFF;
    
    
    uint32_t value_1;
    uint32_t value_2;
    uint8_t flags;
    
    uint32_t buff_flags;
    uint32_t buff;
    
    value_1 = immed_8;
    
    value_2 = arm_read_register(p,Rn);
    
    
    immed_8 = immed_8 >> rotate_imm * 2;
    value_1 = value_1 << (32-rotate_imm*2);
    value_1 = immed_8 | value_1;        
    
    printf("value_1 : %8.8x \n",value_1);
    opcode(p,value_1, value_2, op, &value_1, &flags);
    
   // printf("\n Rd : %0x8",Rd)
    arm_write_register(p,Rd,value_1);
    
    
    if (S == 1)
    {
        buff_flags = flags;
        buff = arm_read_cpsr(p);
        buff = (buff & ~(UserMask)) | (buff_flags << 28);
        arm_write_cpsr(p,buff);
    }
    return 0;
}

int opcode (arm_core p,uint32_t val_1, uint32_t val_2, uint8_t op,uint32_t *val, uint8_t *flags)
{
    uint8_t bit_0 = (op >> 0) & 1;
    uint8_t bit_1 = (op >> 1) & 1;
    uint8_t bit_2 = (op >> 2) & 1;
    uint8_t bit_3 = (op >> 3) & 1;
    
    long int valeur_theorique;
    uint32_t valeur_reel;
    
    uint32_t buff_1 = val_1;
    uint32_t buff_2 = val_2;
    
    long int val_1_int = val_1;
    long int val_2_int = val_2;
    
    char ok_flag  = 0;
     
    uint32_t cpsr_read = arm_read_cpsr(p);
    uint8_t Z_flag = ((cpsr_read >> Z) & 1);
    uint8_t N_flag = ((cpsr_read >> N) & 1);
    uint8_t C_flag = ((cpsr_read >> C) & 1);
    uint8_t V_flag = ((cpsr_read >> V) & 1);
    
    *flags = 0;
    
    printf("valeur op : %d%d%d%d",bit_3,bit_2,bit_1,bit_0);
    
    if (bit_0 == 0 && bit_1 == 0 && bit_2 == 0 && bit_3 == 0)
    {
        //AND (et bit a bit)
        valeur_reel = val_1 & val_2;
        valeur_theorique = valeur_reel;
    }
    else if (bit_0 == 1 && bit_1 == 0 && bit_2 == 0 && bit_3 == 0)
    {
        //EOR (ou exclusif logique bit a bit)
        valeur_reel = val_1 ^ val_2;
        valeur_theorique = valeur_reel;
    }
    else if (bit_0 == 0 && bit_1 == 1 && bit_2 == 0 && bit_3 == 0)
    {
        //SUB (soustration)
        valeur_reel = val_2 - val_1;
        valeur_theorique = val_2_int - val_1_int;
        
        C_flag = 0;
        V_flag = 0;
        
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
        
    }
    else if (bit_0 == 1 && bit_1 == 1 && bit_2 == 0 && bit_3 == 0)
    {
        //RSB (soustraction inverser)
        valeur_reel = val_1 - val_2;
        valeur_theorique = val_1_int - val_2_int;
        
        C_flag = 0;
        V_flag = 0;
        
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
    }
    else if (bit_0 == 0 && bit_1 == 0 && bit_2 == 1 && bit_3 == 0)
    {
        //ADD (addition)
        valeur_reel = val_1 + val_2;
        valeur_theorique = val_1_int + val_2_int;
        
        C_flag = 0;
        V_flag = 0;
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
    }
    else if (bit_0 == 1 && bit_1 == 0 && bit_2 == 1 && bit_3 == 0)
    {
        //ADC (addition avec le carry)
        valeur_reel = val_1 + val_2;
        valeur_theorique = val_1_int + val_2_int + C_flag;
        
        C_flag = 0;
        V_flag = 0;
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
        
    }
    else if (bit_0 == 0 && bit_1 == 1 && bit_2 == 1 && bit_3 == 0)
    {
        //SBC (soutraction avec carry)
        valeur_reel = val_2 - val_1;
        valeur_theorique = val_2 - val_2 - (~(C_flag & 1));
        
        C_flag = 0;
        V_flag = 0;
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
    }
    else if (bit_0 == 1 && bit_1 == 1 && bit_2 == 1 && bit_3 == 0)
    {
        //RSC (soustraction inverse avec carry)
        valeur_reel = val_1 - val_2;
        valeur_theorique = val_1 - val_2 - (~(C_flag & 1));
        
        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if ((((val_2 >> 31) & 1) == ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else if ((((val_2 >> 31) & 1) != ((val_1 >> 31) & 1)) && (((valeur_reel >> 31) & 1) != 1))
        {
            V_flag = 1 ^ (C_flag & 1);
        }
        else
        {
            V_flag = 0 ^ (C_flag & 1);
        }
    }
    else if (bit_0 == 0 && bit_1 == 0 && bit_2 == 0 && bit_3 == 1)
    {
        //NZCV
        //TST (test ...
        val_1 = val_1 & val_2;
        if (val_1 == 0)
        {
            *flags = *flags | (1 << 3);
        }
        if ((val_1 >> 31) == 1)
        {
            *flags = *flags | (1 << 4);
        }
        ok_flag = 1;
        
        
    }
    else if (bit_0 == 1 && bit_1 == 0 && bit_2 == 0 && bit_3 == 1)
    {
        //TEQ (test equivalence ...
        val_1 = val_1 | val_2;
        if (val_1 == 0)
        {
            *flags = *flags | (1 << 3);
        }
        if ((val_1 >> 31) == 1)
        {
            *flags = *flags | (1 << 4);
        }
        ok_flag = 1;
        
    }
    else if (bit_0 == 0 && bit_1 == 1 && bit_2 == 0 && bit_3 == 1)
    {
        //CMP (comparer ...
        buff_2 = val_2;
        buff_1 = val_1;
        
        buff_2 = buff_2 - buff_1;
        val_1 = val_2 - val_1;
        if (val_1 == 0)                 //Z
        {
            *flags = *flags | (1 << 3);
        }
        if ((val_1 >> 31) == 1)         //N
        {
            *flags = *flags | (1 << 4);
        }
        if (val_1 != buff_2)            //C
        {
            *flags = *flags | (1 << 1);
        }
         if ((((buff_1 >> 31) & 1) != ((val_2 >> 31) & 1)) && (((val_1 >> 31) & 1) == 0)  ) //V
        {
            *flags = *flags | (((*flags >> 1) & 1) ^ 1); 
        }
        else if ((((buff_1 >> 31) & 1) == ((val_2 >> 31) & 1)) && (((val_1 >> 31) & 1) == 1)  ) 
        {
            *flags = *flags | (((*flags >> 1) & 1) ^ 1); 
        }
        ok_flag = 1;
    }
    else if (bit_0 == 1 && bit_1 == 1 && bit_2 == 0 && bit_3 == 1)
    {
        //CMN (compare negative ...
        buff_2 = val_2;
        buff_1 = val_1;
        
        buff_1 = buff_2 + buff_1;
        val_1 = val_2 + val_1;
        if (val_1 == 0)                 //Z
        {
            *flags = *flags | (1 << 3);
        }
        if ((val_1 >> 31) == 1)         //N
        {
            *flags = *flags | (1 << 4);
        }
        if (val_1 != buff_1)            //C
        {
            *flags = *flags | (1 << 1);
        }
        
        if (((((buff_1 >> 31) & 1) != ((val_2 >> 31) & 1))) && (((val_1 >> 31) & 1) == 0)  ) //V
        {
            *flags = *flags | (((*flags >> 1) & 1) ^ 1); 
        }
        else if (((((buff_1 >> 31) & 1) == (val_2 >> 31))) & 1 && (((val_1 >> 31) & 1) == 1)  ) 
        {
            *flags = *flags | (((*flags >> 1) & 1) ^ 1); 
        }
        ok_flag = 1;
    }
     else if (bit_0 == 0 && bit_1 == 0 && bit_2 == 1 && bit_3 == 1)
    {
        //ORR (ou logique)
         valeur_reel = val_1 | val_2;
         valeur_theorique = val_1 | val_2;
         
         C_flag = 0;
         V_flag = 0;
         ok_flag = 1;
    }
     else if (bit_0 == 1 && bit_1 == 0 && bit_2 == 1 && bit_3 == 1)
    {
        //MOV (mouve)
         printf ("je suis ici \n val_1 = %8.8x \n",val_1);
        valeur_reel = val_1;
        valeur_theorique = val_1;
        
        C_flag = 0;
        V_flag = 0;
        ok_flag = 1;
    }
     else if (bit_0 == 0 && bit_1 == 1 && bit_2 == 1 && bit_3 == 1)
    {
        //BIC (bit claire ...
         valeur_reel = val_1 & ~(val_2);
         valeur_theorique = val_1 & ~(val_2);
         
        C_flag = 0;
        V_flag = 0;
        ok_flag = 1;
    }
     else if (bit_0 == 1 && bit_1 == 1 && bit_2 == 1 && bit_3 == 1)
    {
        //MVN (mouve not)
        valeur_reel = ~val_1;
        valeur_theorique = ~val_2;
        
        C_flag = 0;
        V_flag = 0;
        ok_flag = 1;
    }
    
    if (ok_flag == 0)
    {
        C_flag = 0;
        Z_flag = 0;
        N_flag = 0;

        if (valeur_reel != valeur_theorique)
        {
            C_flag = 1;
        }
        if (valeur_reel == 0)
        {
            Z_flag = 1;
        }
        if (((valeur_reel >> 31) &1) == 1)
        {
           N_flag = 1;
        }
        
        *flags = 0;
        *flags = *flags | V_flag;
        *flags = *flags | (C_flag << 1);
        *flags = *flags | (Z_flag << 1);
        *flags = *flags | (N_flag << 1);
    }
    *val = valeur_reel;
    
    return 0;
}