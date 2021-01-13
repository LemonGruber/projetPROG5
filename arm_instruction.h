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
#ifndef __ARM_INSTRUCTION_H__
#define __ARM_INSTRUCTION_H__

/**
 * @file arm_instruction.c
 * @author G.Huard, Vincent, Damien, Yann
 * @date 13/01/2021
 * @brief decode les instructions pas a pas (sans calculer)
 */
#include "arm_core.h"

/**
 * @param p Les informations machine sous forme de structure de arm_core
 * @return Retourne 0 si tout va bien sinon 1 ou une constante d'erreur en int.
 * @brief Permet d'avancer d'un pas dans les instructions du programme.
 */
int arm_step(arm_core p);

/**
 * @param p Les informations machine sous forme de structure de arm_core
 * @param ins Instruction en cours d'exécution
 * @param retour Booleen de type char (0 si la condition est fausse 1 si elle est vrai et 2 si c'est une condition special) de type pointeur de char
 * @return Retourne 0 si tout va bien, sinon 1 ou une constante d'erreur en int.
 * @brief Verifie si la condition est respectee ou non.
 */
int cond_fonct (arm_core p, uint32_t ins, char *retour);

#endif
