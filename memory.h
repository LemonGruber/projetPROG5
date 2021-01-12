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
#include <stdint.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>

typedef enum {LSL, LSR} type_shift; 
typedef enum {OR, AND} logique; 
/**
 * @file memory.h
 * @author Guillame, Vincent
 * @date 18/12/2020
 * @brief gestion de la memoire (lecture, creation, modif)
 */
typedef struct memory_data *memory;

/**
 * @param size la taille de la memoire de type size_t
 * @param is_big_endian si les valeurs de la memoire sont en big endian ou pas
 * @return une memoire cree et initialiser
 * @brief permet d'allouer la taille necessaire pour la memoire
 */
memory memory_create(size_t size, int is_big_endian);

/**
 * @param mem la memoire de la forme de structure memory
 * @return la taille de la memoire
 * @brief permet de recuperer la taille de la memoire
 */
size_t memory_get_size(memory mem); 

/**
 * @param mem la memoire de la forme de structure memory
 * @brief libere la memoire prise par mem
 */
void memory_destroy(memory mem);

/* All these functions perform a read/write access to a byte/half/word data at
 * address a in mem. The result is respectively taken from or stored to the
 * parameter value. The access is made using the given endianess (be == 1 for a
 * big endian access and be == 0 for a little endian access).
 * The return value indicates a succes (0) or a failure (-1).
 */

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint8_t 
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief recuper une valeur sur 8 bytes dans la memoire
 */
int memory_read_byte(memory mem, uint32_t address, uint8_t *value);

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint16_t 
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief recuper une valeur sur 16 bytes dans la memoire
 */
int memory_read_half(memory mem, uint32_t address, uint16_t *value);

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint32_t 
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief recuper une valeur sur 32 bytes dans la memoire
 */
int memory_read_word(memory mem, uint32_t address, uint32_t *value);

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint8_t
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief ecrire dans la memoire la valeur donner a l'adresse donner
 */
int memory_write_byte(memory mem, uint32_t address, uint8_t value);

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint16_t
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief ecrire dans la memoire la valeur donner a l'adresse donner
 */
int memory_write_half(memory mem, uint32_t address, uint16_t value);

/**
 * @param mem la memoire de la forme de structure memory
 * @param address une adresse memoire de type uint32_t
 * @param value une valeur de type uint32_t
 * @return 1 si la valeur n'a pas pu etre recuperer 0 si non
 * @brief ecrire dans la memoire la valeur donner a l'adresse donner
 */
int memory_write_word(memory mem, uint32_t address, uint32_t value);

/**
 * @param m la memoire sous forme de structure memory
 * @brief fonction qui peremet d'afficher la memoire (debug)
 */
void afficher_memoire(memory m);

/**
 * @param m la memoire sous forme de structure memory
 * @param address l'addresse de la memoire pour laquelle on veux acceder de type uint32_t
 * @param retour la valeur de retour, soit la valeur a l'address memoire avec le decalage de type uint21_t
 * @param deccalage la valeur de decalage de type int
 * @param LS le type de decalage (a gauche ou a droite) de type type_shift
 * @brief permet d'acceder a la valeur de la memoire a une addresse donner avec le decalage requis
 */
void acces_mem_address (memory m,uint32_t address, uint32_t *retour,int deccalage, type_shift LS);

/**
 * @param m la memoire sous forme de structure memory
 * @param address l'addresse de la memoire pour laquelle on veux ecrire de type uint32_t
 * @param value la valeur que l'on veux ecrire en memoire
 * @brief permet d'ecrire en memoire a une adresse donner
 */
void ecriture_mem_address (memory m, uint32_t address, uint32_t value);

/**
 * @param m la memoire sous forme de structure memory
 * @param mask un masque de type uint32_t
 * @param address l'adresse memoire a laquelle apliquer le masque sur sa valeur.
 * @brief permet d'apliquer un masque a une valeur dans la memoire a une adresse donnee
 */
void appliquer_mask (memory m, uint32_t mask, uint32_t address);
#endif
