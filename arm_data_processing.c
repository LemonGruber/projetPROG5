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
    
    uint8_t bit_4 = (ins >> 4) &1;          //Imediateter de la valeur de shift ou non
    uint8_t bit_5_6 = (ins >> 5) & 0b11;    //Type de shift
    uint8_t S = (ins >> 20) & 1;            //Bit de Sauvegarde 
    
    uint32_t val_1; //*Valeurs utile pour les calcule
    uint32_t val_2; //*
    
    //permet de caster
    uint32_t buff;
    
    int Rn = (ins >> 16) & 0xf;        //Premier operande, registre
    int Rm = ins & 0xf;                //Deuxieme operande (celui qui est shifter), registre
    int Rd = (ins >> 12) & 0xf;        //Registre de retour
    int Rs = (ins >> 8) & 0xf;         //Registe du decalage (si non imediat)
    int shift_imm = (ins >> 7) & 0x1f; //Decalage (si immediat)
    
    int decalage;
    
    char est_comparaison = 0;
    
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
    
    switch(bit_5_6)
    {
        case 0 :
            //LSL
            val_2 = val_2 << decalage;
        break;
        case 2 :
           //ASR
            val_2 = asr(val_2,(uint8_t)decalage);
        break;
        case 1 :
            //LSR
            
            val_2 = val_2  >> decalage;
        break;
        case 3 :
            //ROR
            val_2 = ror(val_2, (uint8_t)decalage);
        break;
        //Tout les cas sont traiter donc le default n'est pas obligatoire
    }
    
    val_1 = arm_read_register(p,Rn);
    opcode(p,val_2,val_1,ins,&value,&flags,&est_comparaison );
    
    if (!est_comparaison)
    {
        arm_write_register(p,Rd,value);      
    }
    //Si la sauvegarde est activer alors
    if (S == 1)
    {
        //Alors :
        //on prepart les flags et on les sauvegardes
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
    
    uint8_t S = (ins >> 20) & 1;           //Bit de sauvegarde 
    uint8_t Rn = (ins >> 16) & 0xF;        //Operande gauche
    uint8_t Rd = (ins >> 12) & 0xF;        //Registre de sauvegarde
    uint8_t rotate_imm = (ins >> 8) & 0xF; //Valeur de rotation
    uint8_t immed_8 = (ins >> 0) & 0xFF;   //Valeur immediate pour le calcul (operande droite)d
    
    
    uint32_t value_1;    //*valeur pour les opérande
    uint32_t value_2;    //*
    uint8_t flags;
    
    uint32_t buff_flags; //*buffeurs de sauvegarde
    uint32_t buff;       //*
    
    char est_comparaison = 0;
    //on recupere les valeurs
    value_1 = immed_8;
    value_2 = arm_read_register(p,Rn);
    
    //On effectue la rotation
    immed_8 = immed_8 >> rotate_imm * 2;
    value_1 = value_1 << (32-rotate_imm*2);
    value_1 = immed_8 | value_1;        
    
    //On effectue l'operation
    opcode(p,value_1, value_2, ins, &value_1, &flags, &est_comparaison);

    //On enregistre la valeur dans le registre de sauvegarde
    if (!est_comparaison)
    {
        arm_write_register(p,Rd,value_1);
    }
    //Si la sauvegarde est activer
    if (S == 1)
    {
        //Alors:
        //On sauvegarde les flags dans cpsr
        buff_flags = flags;
        buff = arm_read_cpsr(p);
        buff = (buff & ~(UserMask)) | (buff_flags << 28);
        arm_write_cpsr(p,buff);
    }
    return 0;
}

int opcode (arm_core p,uint32_t val_1, uint32_t val_2, uint32_t ins,uint32_t *val, uint8_t *flags, char *est_comparaison)
{
    //on recupere l'operation
    uint32_t op = (ins >> 21) & 0xF;
    
    //bits de verif
    /*uint8_t bit_0 = (op >> 0) & 1;
    uint8_t bit_1 = (op >> 1) & 1;
    uint8_t bit_2 = (op >> 2) & 1;
    uint8_t bit_3 = (op >> 3) & 1;*/
    
    //relatif cpsr/flag
    uint32_t cpsr_read = arm_read_cpsr(p);
    
    uint8_t Z_flag = 0;
    uint8_t N_flag = 0;
    uint8_t C_flag = 0;
    uint8_t V_flag = 0;
    
    //valeur calculer, buffer
    long int valeur_theorique;
    uint32_t valeur_reel;
    
    //caste des valeurs de calcules pour verif (carry)
    long int val_1_int = val_1;
    long int val_2_int = val_2;
    
    //verifier si le flag a ete deja cree
    char ok_flag  = 0;

    //initilaises les flags a 0
    *flags = 0;
    
    char est_sub = 0;
    
    if (cpsr_read)
    {
        C_flag = 0;
    }
    
    switch (op)
    {
        case 0:
           //AND (et bit a bit)
           valeur_reel = val_1 & val_2;
           valeur_theorique = valeur_reel;
        break;
        case 1 :
            //EOR (ou exclusif logique bit a bit)
            valeur_reel = val_1 ^ val_2;
            valeur_theorique = valeur_reel;
        break;
        case 2 :
            //SUB (soustration)
            est_sub = 1;
            valeur_reel = val_2 + (~val_1+1);
            valeur_theorique = val_2_int + (~val_1_int+1);
            
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 3:   
            //RSB (soustraction inverser)
            est_sub = 1;
            valeur_reel = val_1 + (~val_2+1);
            valeur_theorique = val_1_int + (~val_2_int+1);

            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 4 :
            //ADD (addition)
            valeur_reel = val_1 + val_2;
            valeur_theorique = val_1_int + val_2_int;

            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 5 :
            //ADC (addition avec le carry)
            valeur_reel = val_1 + val_2 + C_flag;
            valeur_theorique = val_1_int + val_2_int + C_flag;

            different(valeur_reel,valeur_theorique,&C_flag);
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 6 :
            //SBC (soutraction avec carry)
            est_sub = 1;
            valeur_reel = val_2 + (~val_1+1);
            valeur_theorique = val_2_int + (~val_1_int+1) - (~(C_flag & 1));

            different(valeur_reel,valeur_theorique,&C_flag);
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 7 :
            //RSC (soustraction inverse avec carry)
            est_sub = 1;
            valeur_reel = val_1 + (~val_2+1);
            valeur_theorique = val_1_int + (~val_2_int+1) - (~(C_flag & 1));

            different(valeur_reel,valeur_theorique,&C_flag);
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
        break;
        case 8 :
            //TST (test ...
            valeur_reel = val_1 & val_2;
            valeur_theorique = val_1_int & val_2_int;
            
             *est_comparaison = 1;
        break;
        case 9 :
            //TEQ (test equivalence ...
            valeur_reel = val_1 ^ val_2;
            valeur_theorique = val_1_int ^ val_2_int;
            
            *est_comparaison = 1;
        break;
        case 10 :
            //CMP (comparer ...
            est_sub = 1;
            valeur_reel = val_1 + (~val_2+1);
            valeur_theorique = val_1_int + (~val_2_int+1);

            different(valeur_reel,valeur_theorique,&C_flag);
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
            
            *est_comparaison = 1;
        break;
        case 11 :
            //CMN (compare negative ...
            valeur_reel = val_1 + val_2;
            valeur_theorique = val_1_int + val_2_int;

            different(valeur_reel,valeur_theorique,&C_flag);
            v_flag_add (valeur_reel,val_1,val_2,&V_flag,C_flag);
            
            *est_comparaison = 1;
        break;
        case 12 :
            //ORR (ou logique)
            valeur_reel = val_1 | val_2;
            valeur_theorique = val_1 | val_2;
        break;
        case 13 :
            //MOV (mouve)
            valeur_reel = val_1;
            valeur_theorique = val_1;

            verif_zero(valeur_reel,&Z_flag);
            ok_flag = 1;
        break;
        case 14 :
            //BIC (bit claire ...
            valeur_reel = val_1 & ~(val_2);
            valeur_theorique = val_1 & ~(val_2);

            verif_zero(valeur_reel,&Z_flag);
        break;
        case 15 :
            //MVN (mouve not)
            valeur_reel = ~val_1;
            valeur_theorique = ~val_2;

            verif_zero(valeur_reel,&Z_flag);
            ok_flag = 1;
        break;
        //pas besoin de default tout les cas sont traiter
    }
    
   
    //Si les flags n'on pas ete calculer  
    if (ok_flag == 0)
    {
        //Alors :
        //on calcul les flags restant (autre que le v qui depends des operation)
        C_flag = 0;                                      
        different(valeur_reel,valeur_theorique,&C_flag); //Calcul de C
        verif_zero(valeur_reel,&Z_flag);                 //Calcul de Z
        negatif(valeur_reel,&N_flag);                    //Calcul de N
        
        if (est_sub)
        {
            if (C_flag == 1)
            {
                C_flag = 0;
            }
            else
            {
                C_flag = 1;
            }
        }
        
        //Ecriture des flags
        *flags = *flags | ((V_flag) & 1);
        *flags = *flags | ((C_flag & 1) << 1);
        *flags = *flags | ((Z_flag & 1) << 2);
        *flags = *flags | ((N_flag & 1) << 3);
    }
    
    
    //Preparation de la valeur de retour
    *val = valeur_reel;
    
    return 0;
}


void verif_zero (uint32_t val,uint8_t *Z_flag)
{
    //Si la valeur est nul (egale a zero)
    if (val == 0)
    {
        //Si la valeur est nul le flag Z passe a 1
        *Z_flag = 1;
    }
}

void different (uint32_t val_1,long int val_2, uint8_t *C_flag)
{
    //Si la valeur theorique et la valeur reel calculer sont differente
    if (val_1 != val_2)
    {
        //Alors :
        //Il y a un carry, le flag C passe a 1
        *C_flag = 1;
    }
}

void negatif (uint32_t val, uint8_t *N_flag)
{
    //Si la valeur est negative
    if (((val >> 31) & 1) == 1)
    {
        //Alors : 
        //Le flag N passe a 1
        *N_flag = 1;
    }
}

void v_flag_add (uint32_t valeur_reel, uint32_t val_1, uint32_t val_2, uint8_t *V_flag, uint8_t C_flag)
{
    //Si le bit de poid fort est different entre les deux operande et que le bit de poid fort du resultat est a 0
    if ((((val_1 >> 31) & 1) != ((val_2 >> 31) & 1)) && (((valeur_reel >> 31) & 1) == 0)  ) //V
    {
        //On fait un ou exclusif entre la dernier retenu (1) et la retenue sortante (carry)
        *V_flag = *V_flag | (1 ^ (C_flag & 1)); 
    }
    //si non Si le bit de poid fort est pareil entre les deux operande et que le bit de poid fort du resultat est a 1
    else if ((((val_1 >> 31) & 1) == ((val_2 >> 31) & 1)) && (((valeur_reel>> 31) & 1) == 1)  ) 
    {
        //On fait un ou exclusif entre la dernier retenu (1) et la retenue sortante (carry)
        *V_flag = *V_flag | (1 ^ (C_flag & 1)); 
    }
    else
    {
        //Si non :
        //On fait un ou exclusif entre la dernier retenu (0) et la retenue sortante (carry)
        *V_flag = 0 ^ (C_flag & 1);
    }
}
