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
 * @date 13/01/2021
 * @author G.Huard, Damien, Adrien, Vincent 
 * @brief Gere les instructions d'acces memoire
 */
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"

/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @param ins L'instrcution en bits sur 32 bit de type uint32_t
 * @return Retourne 0 si tout va bien, sinon 1 ou une erreur; de type int.
 * @brief Permet de faire un load ou un store sur la memoire d'une
 * valeur donnee selon l'instruction.
 */
int arm_load_store(arm_core p, uint32_t ins);

/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @param ins L'instrcution en bits sur 32 bit de type uint32_t
 * @return Retourne 0 si tout va bien, sinon 1 ou une erreur; de type int.
 * @brief Permet de faire des load ou des store sur la memoire 
 * d'un ou des registres donnee selon l'instruction.
 */
int arm_load_store_multiple(arm_core p, uint32_t ins);

/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @param ins L'instrcution en bits sur 32 bit de type uint32_t
 * @return  Retourne 0 si tout va bien, sinon 1 ou une erreur; 
 * de type int.
 * @brief Permet de faire les load store au niveau coprocesseur 
 * (Non modifé, a faire disparaitre si on ne trouve ce que c'est 
 * sensé faire).
 */
int arm_coprocessor_load_store(arm_core p, uint32_t ins);


/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @param cond La condition a verifier de type uint32_t
 * @return Retourne 0 si la condition est fausse sinon 1; de type int.
 * @brief Permet de donne le code condition et le core pour obtenir 
 * si la condition est valide.
 */
int ConditionPassed(arm_core p, uint32_t cond);


/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @return Retourne 1 si le core est en mode systeme 0 sinon.
 * @brief Permet de savoir si le core est en mode system.
 */
int Est_Sys_Ou_User(arm_core p);

/**
 * @param p Les differents parametres utiles pour la memoire sous 
 * forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le load de type int
 * @brief Permet de faire un load pour un word dans le mode courant.
 */
void Execution_Load(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le load de type int
 * @brief Permet de faire un load pour un word pour le mode utilisateur.
 */
void Execution_Load_Usr(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le load de type int
 * @brief Permet de faire un load pour un byte dans le mode utilisateur.
 */
void Execution_Load_Usr_Byte(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le load de type int
 * @brief Permet de faire un load pour un byte dans le mode courant.
 */
void Execution_Load_Byte(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le store de type int
 * @brief Permet de faire un store de word dans le mode courant.
 */
void Execution_Store(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le store de type int
 * @brief Permet de faire un store de word dans le mode utilisateur.
 */
void Execution_Store_Usr(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le store de type int
 * @brief Permet de faire un store de byte dans le mode utilisateur.
 */
void Execution_Store_Usr_Byte(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param addr Addresse memoire utiliser de type uint32_t
 * @param reg Registre pour le store de type int
 * @brief Permet de faire un store de byte dans le mode courant.
 */
void Execution_Store_Byte(arm_core p, uint32_t addr, int reg);

/**
 * @param p Les differents parametres utiles pour la memoire 
 * sous forme de structure arm_core
 * @param adresse Adresse a la quelle acceder dans la memoire
 * @param Rd Registre a utiliser pour l'opération en cours
 * @param L Bit L de l'instruction
 * @param B Bit B de l'instruction
 * @brief Fait le load ou le store sans rien regarder d'autre que 
 * le format (byte ou word) (factorisation du code).
 */
void write_load_reg_mem(arm_core p, int adresse, int Rd, int L, int B);

#endif





















