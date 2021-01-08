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
#ifndef __ARM_BRANCH_OTHER_H__
#define __ARM_BRANCH_OTHER_H__

/**
 * @file arm_branch_other.h
 * @author G.Huard, Vincent, Yann
 * @date 06/01/2021
 * @brief gere les fonctions de branchement, les miscellaneous, et les coprocessor other 
 */
#include <stdint.h>
#include "arm_core.h"

/**
 * @param p les information machine sous forme de structure de arm_core
 * @param ins l'instruction en type uint32_t
 * @return 0 si tout va bien 1 si non
 * @brief gere toute les fonctions de branchement "simple"
 */
int arm_branch(arm_core p, uint32_t ins);

/**
 * @param p les information machine sous forme de structure de arm_core
 * @param ins l'instruction en type uint32_t
 * @return 0 si tout va bien 1 si non
 * @brief gere toutes les fonction au niveau coprocesseur sauf, celle de load et store
 */
int arm_coprocessor_others_swi(arm_core p, uint32_t ins);

/**
 * @param p les information machine sous forme de structure de arm_core
 * @param ins l'instruction en type uint32_t
 * @return 0 si tout va bien 1 si non de type int
 * @brief gere les fonctions miscellaneous
 */
int arm_miscellaneous(arm_core p, uint32_t ins);


/**
 * @param p les information machine sous forme de structure de arm_core
 * @param cond la contion a tester avec son opcode de type uint8_t
 * @param retour booleen de type char (0 si la condition est fausse 1 si non) de type pointeur de char
 * @return 0 si tout vas bien 1 si non de type int
 * @brief verifier si la condition est respecter ou non
 */
int cond_fonct (arm_core p, uint8_t cond, char *retour);

#endif
