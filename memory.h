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
#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * @file memory.h
 * @author G.Huard, Vincent, Damien
 * @date 13/01/2021
 * @brief Gestion de la memoire (lecture, creation, modif).
 */

#include <stdint.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>

/**
 * @enum type_shift
 * @brief Le type de shift a faire LSL decalage a gauche, 
 * LSR decalage a droite.
 */
typedef enum {LSL, LSR} type_shift; 


typedef struct memory_data *memory;

/**
 * @param size La taille de la memoire; de type size_t
 * @param is_big_endian Si les valeurs de la memoire sont en 
 * big endian ou pas
 * @return Une memoire cree et initialise.
 * @brief Permet d'allouer la taille necessaire pour la memoire.
 */
memory memory_create(size_t size, int is_big_endian);

/**
 * @param mem La memoire de la forme de structure memory
 * @return La taille de la memoire.
 * @brief Permet de recuperer la taille de la memoire.
 */
size_t memory_get_size(memory mem); 

/**
 * @param mem La memoire de la forme de structure memory
 * @brief Libere la memoire prise par mem.
 */
void memory_destroy(memory mem);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value La valeur a l'addresse memoire; de type uint8_t 
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Recupere une valeur sur 8 bytes dans la memoire.
 */
int memory_read_byte(memory mem, uint32_t address, uint8_t *value);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value La valeur a l'addresse memoire; de type uint16_t 
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Recupere une valeur sur 16 bytes dans la memoire.
 */
int memory_read_half(memory mem, uint32_t address, uint16_t *value);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value La valeur a l'addresse memoire; de type uint32_t 
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Recupere une valeur sur 32 bytes dans la memoire.
 */
int memory_read_word(memory mem, uint32_t address, uint32_t *value);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value Une valeur; de type uint8_t
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Ecrire dans la memoire la valeur donnee a l'adresse donnee.
 */
int memory_write_byte(memory mem, uint32_t address, uint8_t value);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value Une valeur; de type uint16_t
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Ecrire dans la memoire la valeur donnee a l'adresse donnee.
 */
int memory_write_half(memory mem, uint32_t address, uint16_t value);

/**
 * @param mem La memoire de la forme de structure memory
 * @param address Une adresse memoire; de type uint32_t
 * @param value Une valeur; de type uint32_t
 * @return Retourne 1 si la valeur n'a pas pu etre recuperer, et 
 * 0 sinon.
 * @brief Ecrire dans la memoire la valeur donnee a l'adresse donnee.
 */
int memory_write_word(memory mem, uint32_t address, uint32_t value);

/**
 * @param m La memoire sous forme de structure memory
 * @brief Fonction qui permet d'afficher la memoire non vide (debug).
 */
void afficher_memoire(memory m);

/**
 * @param m La memoire sous forme de structure memory
 * @param address L'addresse de la memoire pour laquelle on veux 
 * acceder; de type uint32_t
 * @param retour La valeur de retour, soit la valeur a l'address 
 * memoire avec le decalage; de type pointeur de uint32_t
 * @param deccalage La valeur de decalage; de type int
 * @param LS Le type de decalage (a gauche ou a droite); de type 
 * type_shift
 * @brief Permet d'acceder a la valeur de la memoire a une addresse 
 * donnee avec le decalage requis.
 */
void acces_mem_address (memory m,uint32_t address, uint32_t *retour,int deccalage, type_shift LS);

/**
 * @param m La memoire sous forme de structure memory
 * @param address L'addresse de la memoire pour laquelle on veux 
 * ecrire; de type uint32_t
 * @param value La valeur que l'on veux ecrire en memoire
 * @brief Permet d'ecrire en memoire a une adresse donnee.
 */
void ecriture_mem_address (memory m, uint32_t address, uint32_t value);

/**
 * @param m La memoire sous forme de structure memory
 * @param mask Un masque; de type uint32_t
 * @param address L'adresse memoire a laquelle apliquer le masque 
 * sur sa valeur.
 * @brief Permet d'apliquer un masque a une valeur dans la memoire a 
 * une adresse donnee.
 */
void appliquer_mask (memory m, uint32_t mask, uint32_t address);
#endif
