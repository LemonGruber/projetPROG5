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

/**
 * @file arm_load_store.h
 * @date 04/01/2021
 * @author G.Huard 
 * @brief Gere les instrcution d'acces memoire
 */
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"

/**
 * @param p les differents parametre utile pour la memoire sous forme de structure arm_core
 * @param ins l'instrcution en bits sur 32 bit de type uint32_t
 * @return permet de faire un load ou un store sur la memoire d'une valeur donner celon l'instruction
 */
int arm_load_store(arm_core p, uint32_t ins);

/**
 * @param p les differents parametre utile pour la memoire sous forme de structure arm_core
 * @param ins l'instrcution en bits sur 32 bit de type uint32_t
 * @return permet de faire des load et/ou des store sur la memoire d'une valeur donner celon l'instruction
 * soit dupliquer des valeurs
 */
int arm_load_store_multiple(arm_core p, uint32_t ins);

/**
 * @param p les differents parametre utile pour la memoire sous forme de structure arm_core
 * @param ins l'instrcution en bits sur 32 bit de type uint32_t
 * @return 
 * @todo trouver a quoi ca sert
 */
int arm_coprocessor_load_store(arm_core p, uint32_t ins);

#endif
