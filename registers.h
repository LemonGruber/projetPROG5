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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__

/**
 * @file registers.h
 * @author G.Huard, Vincent, Damien
 * @date 13/01/2021
 * @brief Gere les registres.
 */

#include <stdint.h>

typedef struct registers_data *registers;

/**
 * @return Un registre vide de type registers.
 * @brief Alloue la memoire pour les registres et defini le mode 
 * d'utilisation.
 */
registers registers_create();

/**
 * @param r Un registre de la forme de structure registers 
 * @brief Libere la memoire prise par les registres.
 */
void registers_destroy(registers r);

/**
 * @param r Un registre de la forme de structure registers
 * @return Le mode du regsitre; de type uint8_t.
 * @brief Renvoie le mode actuel du registre.
 */
uint8_t get_mode(registers r);

/**
 * @param r Un registre de la forme de structure registers
 * @return Retourne 1 si le mode peut lire le registre spsr, 
 * 0 sinon; de type int.
 * @brief Permet de savoir si on peut lire le registre spsr. 
 */
int current_mode_has_spsr(registers r);

/**
 * @param r Un registre de la forme de structure registers
 * @return Retourne 1 si le mode est un mode privilegier, 
 * 0 sinon; de type int.
 * @brief Permet de savoir si on est en mode privilege.
 */
int in_a_privileged_mode(registers r);

/**
 * @param r un registre de la forme de structure registers
 * @param reg l'indice d'un registre de type uint8_t
 * @return la valeur du registre reg
 * @brief Permet de lire la valeur d'un registre donne. 
 */
uint32_t read_register(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Un numero de registre; de type uint8_t
 * @return La valeur d'un registre reg dans r.
 * @brief Lit la valeur d'un registre donne utilisable par user.
 */
uint32_t read_usr_register(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @return La valeur du registre cpsr.
 * @brief Lit dans les registres le registe CPSR et renvoie sa valeur.
 */
uint32_t read_cpsr(registers r);

/**
 * @param r Un registre de la forme registers
 * @return La valeur du registre spsr.
 * @brief Lit dans les registres le registe SPSR et renvoie sa valeur.
 */
uint32_t read_spsr(registers r);

/**
 * @param r Un registre de la forme registers
 * @param reg Un numero de registre; de type uint8_t
 * @param value La valeur a mettre dans le registe; de type uint32_t
 * @brief Met la valeur donnee dans le registre voulu.
 */
void write_register(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Un numero de registre
 * @param value La valeur a mettre dans le registe; de type uint32_t
 * @brief Met la valeur donnee dans le registre user voulu. 
 */
void write_usr_register(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param value La valeur a mettre dans le registe CPSR; 
 * de type uint32_t
 * @brief Modifie la valeur de cpsr.
 */
void write_cpsr(registers r, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param value La valeur a mettre dans le registe SPSR; 
 * de type uint32_t
 * @brief Modifie la valeur de spsr.
 */
void write_spsr(registers r, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @brief Lit le registre demande dans le mode SVC.
 */
uint32_t acces_lecture_registre_SVC(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @brief Lit le registre demande dans le mode ABT.
 */
uint32_t acces_lecture_registre_ABT(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @brief Lit le registre demande dans le mode UND.
 */
uint32_t acces_lecture_registre_UND(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @brief Lit le registre demande dans le mode IRQ.
 */
uint32_t acces_lecture_registre_IRQ(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @brief Lit le registre demande dans le mode FIQ.
 */
uint32_t acces_lecture_registre_FIQ(registers r, uint8_t reg);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @param value Valeur a ecrire dans le registre; de type uint32_t
 * @brief Ecrit la valeur dans le registre demande en mode FIQ.
 */
void acces_ecriture_registre_FIQ(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @param value Valeur a ecrire dans le registre; de type uint32_t
 * @brief Ecrit la valeur dans le registre demande en mode IRQ.
 */
void acces_ecriture_registre_IRQ(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @param value Valeur a ecrire dans le registre; de type uint32_t
 * @brief Ecrit la valeur dans le registre demande en mode UND.
 */
void acces_ecriture_registre_UND(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @param value Valeur a ecrire dans le registre; de type uint32_t
 * @brief Ecrit la valeur dans le registre demande en mode ABT.
 */
void acces_ecriture_registre_ABT(registers r, uint8_t reg, uint32_t value);

/**
 * @param r Un registre de la forme registers
 * @param reg Numeros du registre; de type uint8_t
 * @param value Valeur a ecrire dans le registre; de type uint32_t
 * @brief Ecrit la valeur dans le registre demande en mode SVC.
 */
void acces_ecriture_registre_SVC(registers r, uint8_t reg, uint32_t value);

#endif