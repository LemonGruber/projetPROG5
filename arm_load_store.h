/*
Armator - simulateur de jeu d'instruction ARMv5T ï¿½ but pï¿½dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Gï¿½nï¿½rale GNU publiï¿½e par la Free Software
Foundation (version 2 ou bien toute autre version ultï¿½rieure choisie par vous).

Ce programme est distribuï¿½ car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spï¿½cifique. Reportez-vous ï¿½ la
Licence Publique Gï¿½nï¿½rale GNU pour plus de dï¿½tails.

Vous devez avoir reï¿½u une copie de la Licence Publique Gï¿½nï¿½rale GNU en mï¿½me
temps que ce programme ; si ce n'est pas le cas, ï¿½crivez ï¿½ la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
ï¿½tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bï¿½timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hï¿½res
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

void write_load_reg_mem(arm_core p, int adresse, int Rd, int L, int B);

/*
 *permet de donné le code condition et le core pour obtenir si la condition est validé
 */
int ConditionPassed(arm_core p, uint32_t cond);

/*
 *permet de savoir si le core est en mode system
 */
int Est_Sys_Ou_User(arm_core p);

/*
 *Fonction basique permettant de stocker ou charger des données
 */
void Execution_Load(arm_core p, uint32_t addr, int reg);

void Execution_Load_Usr(arm_core p, uint32_t addr, int reg);

void Execution_Store(arm_core p, uint32_t addr, int reg);

void Execution_Store_Usr(arm_core p, uint32_t addr, int reg);

#endif





















