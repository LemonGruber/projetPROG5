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

#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__

/**
 * @file arm_data_processing.h
 * @author G.Huard, Vincent, Leandre, Yann, Damien
 * @date 13/01/2021
 * @brief gere les cacules sur les donnees
 */
#include <stdint.h>
#include "arm_core.h"

/**
 * @param p le systeme en structure arm_core
 * @param ins l'instruction en uint32_t
 * @return retourne 0 si tout va bien 1 si non
 * @brief effectue tout les shifts "compliquer", et "simple" avec 1 ou 2 opertion (shift et une operation)
 */
int arm_data_processing_shift(arm_core p, uint32_t ins);

/**
 * @param p le systeme en structure arm_core
 * @param ins l'instruction en uint32_t
 * @return retourne 0 si tout va bien 1 si non
 * @brief effectue les calculs lorsque les data sont en valeur immediate
 */
int arm_data_processing_immediate(arm_core p, uint32_t ins);

/**
 * @param p le systeme en structure arm_core
 * @param ins l'instruction en uint32_t
 * @return 0 si tout vas bien 1 si non
 * @brief effectue tout les calculs sur les data au niveau coprossesseur
 */
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

/**
 * @param p le systeme en structure arm_core
 * @param val_1 la premiere valeur (a utiliser par defaut si il y a pas de 2nd valeur)
 * @param val_2 la deuxieme valeur (0 si il n'y a pas d'autre valeurs)
 * @param ins l'instruction de type uint32_t
 * @param val la valeur de sortie (reste par defaut si on veux que les flags)
 * @param flag les flags des sorties d'opreration si desire (0 si non)
 * @param est_comparaion est a 1 si il a effectue une comparaison (TST,CMP etc..), 0 si non de type pointeur de char
 * @return retourne 0 si tout vas bien 1 si non
 * @brief calcul entre 2 valeurs la valeur de sortie et/ou les flags
 */
int opcode (arm_core p,uint32_t val_1, uint32_t val_2, uint32_t ins,uint32_t *val, uint8_t *flag, char *est_comparaison);

/**
 * @param val une valeur a tester en uint32_t
 * @param Z_flag revoie le flag calcule en uint8_t
 * @brief verifie si la valeur passe en argument est a zero 
 */
void verif_zero (uint32_t val,uint8_t *Z_flag);

/**
 * @param val_1 valeur calcule reel de type uint32_t
 * @param val_2 valeur calcule a trouver de type int
 * @param C_flag renvoie le flag calcule en uint8_t
 * @brief permet de verifier si la valeur reel et la valeur a trouve son differents ou non 
 */
void different (uint32_t val_1,long int val_2, uint8_t *C_flag);

/**
 * @param val valeur a verifier de type uint32_t
 * @param N_flag renvoie le flag calcule en uint8_t
 * @brief verifie si la valeur est negative ou non
 */
void negatif (uint32_t val, uint8_t *N_flag);

/**
 * @param valeur_reel la valeur reel caclule en uint32_t
 * @param val_1 la 1er operande de type uint32_t
 * @param val_2 la 2nd operande de type uint32_t
 * @param V_flag le flag calcule de type uint8_t
 * @param C_flag le carry deja calcule avant l'appel de la fonction de type uint8_t
 * @brief permet de calculer le flag V pour une addition
 */
void v_flag_add (uint32_t valeur_reel, uint32_t val_1, uint32_t val_2, uint8_t *V_flag, uint8_t C_flag);

/**
 * @param valeur_reel la valeur reel caclule en uint32_t
 * @param val_1 la 1er operande de type uint32_t
 * @param val_2 la 2nd operande de type uint32_t
 * @param V_flag le flag calcule de type uint8_t
 * @param C_flag le carry deja calcule avant l'appel de la fonction de type uint8_t
 * @brief permet de calculer le flag V pour une soustraction
 */
void v_flag_sub (uint32_t valeur_reel, uint32_t val_1, uint32_t val_2,uint8_t *V_flag, uint8_t C_flag);

#endif
