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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

/**
 * @struct memoire
 * @brief stock les valeur des memoires ainsi que son adresse
 */
struct memoire {
 uint32_t adresse;
 uint32_t valeur;
}memoire;

/**
 * @struct memory_data
 * @brief stock la taille, le type et la valeur de la memoire
 */
struct memory_data {
    size_t nb_elem;         //Nombre d'element charger dans la memoire
    size_t taille;          //Taille de la memoire
    int is_big_endian;
    struct memoire *data;
}memory_data; 

memory memory_create(size_t size, int is_big_endian) {
    memory mem = NULL;
    mem = malloc(sizeof(memory_data));
    if (mem != NULL)
    {
        mem->taille = size;
        mem->is_big_endian = is_big_endian;
        mem->nb_elem = 0;
        mem->data = malloc(sizeof(memoire)*size);
    }
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem->taille;
}

void memory_destroy(memory mem) {
    free(mem->data);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {

    int i = 0;
    int retour = 0;
    printf("\nlire : adresse : %d\n",address);
    afficher_memoire(mem);   
    while (i <= mem->nb_elem && mem->data[i].adresse != address)
    {
        printf("adresse : %d, valeur : %d \n",mem->data[i].adresse,mem->data[i].valeur);
        i++;
    }
    if (mem->data[i].adresse == address)
    {
        printf("val lue adresse : %d, valeur : %d \n",mem->data[i].adresse,mem->data[i].valeur);
        *value = (uint8_t)(mem->data[i].valeur);
        retour = 0;
    }
    else
    {
        retour = 1;
    }
        
 
    return retour;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    int i = 0;
    int retour = 0;
    
    while (i < mem->nb_elem && mem->data[i].adresse != address)
    {
        i++;
        
    }
    if (mem->data[i].adresse == address)
    {
        *value = (uint16_t)mem->data[i].valeur;
        retour = 0;
    }
    else
    {
        retour = 1;
    }
        
    
    return retour;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    int i = 0;
    int retour = 0;
    
    while (i < mem->nb_elem && mem->data[i].adresse != address)
    {
        i++;
    }
    if (mem->data[i].adresse == address)
    {
        *value = (uint32_t)mem->data[i].valeur;
        retour = 0;
    }
    else
    {
        retour = 1;
    }
        
    
    return retour;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    printf("\necrire : adresse : %d, value : %d\n",address,value);
    afficher_memoire(mem); 
    int i = 0;
    int retour = 0;
    while (i < mem->nb_elem && address != mem->data[i].adresse)
    {
        printf("lire_ecrit : adresse : %d, value : %d\n", mem->data[i].adresse,mem->data[i].valeur);
        i++;
    }
    if (i < mem->nb_elem)
    {
        mem->data[i].valeur = (uint32_t)value;
        retour = 0;
    }
    else
    {
        if (mem->nb_elem+1 <= mem->taille)
        {
            printf("nb_elem : %ld ",mem->nb_elem);
            mem->data[mem->nb_elem].valeur = (uint32_t)value;
            mem->data[mem->nb_elem].adresse = address;
            mem->nb_elem = mem->nb_elem + 1;
            retour = 0;
        }
        else
        {
            retour = 1;
        }
    }
     return retour;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
        int i = 0;
    int retour = 0;
    while (i < mem->nb_elem && address != mem->data[i].adresse)
    {
        i++;
    }
    if (address == mem->data[i].adresse)
    {
        mem->data[i].valeur = (uint32_t)value;
        retour = 0;
    }
    else
    {
        if (mem->nb_elem+1 < mem->taille)
        {
            mem->data[mem->nb_elem].valeur = (uint32_t)value;
            mem->data[mem->nb_elem].adresse = address;
            mem->nb_elem = mem->nb_elem + 1;
            retour = 0;
        }
        else
        {
            retour = 1;
        }
    }
     return retour;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    int i = 0;
    int retour = 0;
    while (i < mem->nb_elem && address != mem->data[i].adresse)
    {
        i++;
    }
    if (address == mem->data[i].adresse)
    {
        mem->data[i].valeur = (uint32_t)value;
        retour = 0;
    }
    else
    {
        if (mem->nb_elem+1 < mem->taille)
        {
            mem->data[mem->nb_elem].valeur = (uint32_t)value;
            mem->data[mem->nb_elem].adresse = address;
            mem->nb_elem = mem->nb_elem + 1;
            retour = 0;
        }
        else
        {
            retour = 1;
        }
    }
     return retour;
}


void afficher_memoire(memory m)
{
    printf("\n valeur memoire : \n");
    int i = 0;
    for (i = 0; i < m->nb_elem; i++)
    {
        printf("indice : %d, valeur : %d, adresse : %d \n",i,m->data[i].valeur,m->data[i].adresse);
    }
    printf("\n");
}