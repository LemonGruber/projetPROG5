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
 * @brief gere les registres
 */

#include <stdint.h>

typedef struct registers_data *registers;

/**
 * @return un registre vide de type registers
 * @brief alloue la memoire pour les registres et defini le mode d'utilisation
 */
registers registers_create();

/**
 * @param r un registre de la forme de structure registers 
 * @brief libere la memoire prise par les registres
 */
void registers_destroy(registers r);

/**
 * @param r un registre de la forme de structure registers
 * @return le mode du regsitre de type uint8_t
 * @brief renvoie le mode actuel du registre
 */
uint8_t get_mode(registers r);

/**
 * @param r un registre de la forme de structure registers
 * @return 1 si le mode peut lire le registre spsr 0 sinon de type int
 * @brief permet de savoir si on peut lire le registre spsr 
 */
int current_mode_has_spsr(registers r);

/**
 * @param r un registre de la forme de structure registers
 * @return 1 si le mode est un mode privilegier 0 sinon de type int
 * @brief permet de savoir si on est en mode privilegier
 */
int in_a_privileged_mode(registers r);

/**
 * @param r un registre de la forme de structure registers
 * @param reg l'indice d'un registre de type uint8_t
 * @return la valeur du registre reg
 * @brief permet de lire la valeur d'un registre donner 
 */
uint32_t read_register(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg un numero de registre de type uint8_t
 * @return la valeur d'un registre reg dans r
 * @brief lit la valeur d'un registre donner utilisable par user
 */
uint32_t read_usr_register(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @return la valeur du registre cpsr
 * @brief lit dans les registre le registe CPSR et renvoie sa valeur
 */
uint32_t read_cpsr(registers r);

/**
 * @param r un registre de la forme registers
 * @return la valeur du registre spsr
 * @brief lit dans les registre le registe SPSR et renvoie sa valeur
 */
uint32_t read_spsr(registers r);

/**
 * @param r un registre de la forme registers
 * @param reg un numero de registre de type uint8_t
 * @param value la valeur a mettre dans le registe de type uint32_t
 * @brief met la valeur donner dans le registre voulu
 */
void write_register(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg un numero de registre
 * @param value la valeur a mettre dans le registe de type uint32_t
 * @brief met la valeur donner dans le registre user voulu 
 */
void write_usr_register(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param value la valeur a mettre dans le registe de type uint32_t
 * @brief modifie la valeur de cpsr
 */
void write_cpsr(registers r, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param value la valeur a mettre dans le registe de type uint32_t
 * @brief modifie la valeur de spsr
 */
void write_spsr(registers r, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @brief lit le registre demande dans le mode actuelle
 */
uint32_t acces_lecture_registre_SVC(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @brief lit le registre demande dans le mode actuelle
 */
uint32_t acces_lecture_registre_ABT(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @brief lit le registre demande dans le mode actuelle
 */
uint32_t acces_lecture_registre_UND(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @brief lit le registre demande dans le mode actuelle
 */
uint32_t acces_lecture_registre_IRQ(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @brief lit le registre demande dans le mode actuelle
 */
uint32_t acces_lecture_registre_FIQ(registers r, uint8_t reg);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @param value valeur a ecrire dans le registre de type uint32_t
 * @brief ecrit la valeur dans le registre demande dans le mode actuelle
 */
void acces_ecriture_registre_FIQ(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @param value valeur a ecrire dans le registre de type uint32_t
 * @brief ecrit la valeur dans le registre demande dans le mode actuelle
 */
void acces_ecriture_registre_IRQ(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @param value valeur a ecrire dans le registre de type uint32_t
 * @brief ecrit la valeur dans le registre demande dans le mode actuelle
 */
void acces_ecriture_registre_UND(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @param value valeur a ecrire dans le registre de type uint32_t
 * @brief ecrit la valeur dans le registre demande dans le mode actuelle
 */
void acces_ecriture_registre_ABT(registers r, uint8_t reg, uint32_t value);

/**
 * @param r un registre de la forme registers
 * @param reg numéros du registre de type uint8_t
 * @param value valeur a ecrire dans le registre de type uint32_t
 * @brief ecrit la valeur dans le registre demande dans le mode actuelle
 */
void acces_ecriture_registre_SVC(registers r, uint8_t reg, uint32_t value);

#endif