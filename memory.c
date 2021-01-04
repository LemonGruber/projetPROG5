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
 * @struct memory_data
 * @brief stock la taille, le type et les valeur dans la memoire
 */
struct memory_data {
    size_t taille;          //Taille de la memoire
    int is_big_endian;
    uint32_t *data;
}memory_data; 

memory memory_create(size_t size, int is_big_endian) {
    memory mem = NULL;
    mem = (memory)malloc(sizeof(memory_data));
    if (mem != NULL)
    {
        mem->taille = size;
        mem->is_big_endian = is_big_endian;
        mem->data = malloc(sizeof(uint32_t)*size);
    }
    return mem;
}

size_t memory_get_size(memory mem) {
    return (mem->taille*32);
}

void memory_destroy(memory mem) {
    free(mem->data);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {

    if (mem->is_big_endian)
    {
        if (address %4   == 0)
        {
            *value = mem->data[(address*8) /32] >> 24;
        }
        else if (address % 4 == 3)
        {
            *value = mem->data[(address*8) /32] >> 0;
        }
        else if (address % 4 == 2)
        {
            *value = (mem->data[(address*8) /32]) >> 8;
        }
        else if (address % 4 == 1)
        {
            *value = mem->data[(address*8) /32] >> 16;
        }
    }
    else
    {
        if (address % 4 == 0)
        {
            *value = mem->data[(address*8) /32] >> 0;
        }
        else if (address % 4 == 3)
        {
            *value = (mem->data[(address*8) /32]) >> 24;
        }
        else if (address % 4 == 2)
        {
            *value = mem->data[(address*8) /32] >> 16;
        }
        else if (address % 4 == 1)
        {
            *value = mem->data[(address*8) /32] >> 8;
        }
    }
            
        
    return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    *value = 0;
    int retour = 0;
    if (!mem->is_big_endian)
    {
        if (address%4 == 0)
        {
            *value = (mem->data[(address*8)/32] << 16) >> 16;
        }
        else if (address %4 == 2)
        {
            *value = (mem->data[(address*8)/32] >> 16);
        }
    }
    else
    {
        if ( address % 4 == 0)
        {
            *value = (mem->data[(address*8)/32] >> 16);
        }
        else if (address % 4 == 2)
        {
            *value = (mem->data[(address*8)/32] << 16) >> 16;
        }
    }
        
    
    return retour;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    *value = 0;
    int retour = 0;
    *value = mem->data[(address*8)/32];
    return retour;
}



int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    uint32_t buff;
    if (!mem->is_big_endian)
    {
        if (address == 0)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff);
        }
        else if (address % 4 == 3)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 24);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 24);
        }
        else if (address % 4 == 0)
        { 
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff);
        }
        else if(address % 4 == 1)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 8);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 8);
        }
        else if(address % 4 == 2)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 16);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 16);
        }
    }
    else
    {
        if (address == 0)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 24);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 24);
        }
        else if (address % 4 == 3)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff);
        }
        else if (address % 4 == 0)
        { 
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 24);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 24);
        }
        else if(address % 4 == 1)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 16);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 16);
        }
        else if(address % 4 == 2)
        {
            buff = value;
            mem->data[(address*8)/32] = mem->data[(address*8)/32] & ~(0xff << 8);
            mem->data[(address*8)/32] = mem->data[(address*8)/32] | (buff << 8);
        }
    }
    
    return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
    
    uint32_t buff;
    if (mem->is_big_endian)
    {
        if (address % 4 == 0)
        {
            buff = value;
            buff = buff << 16;
            mem->data[(address * 8)/32] = buff | (mem->data[(address * 8)/32] & ~(1 << 16));
        }
        else if (address  % 4 == 0)
        {
            buff = value;
            buff = buff << 16;
            mem->data[(address * 8)/32] = buff | (mem->data[(address * 8)/32] & ~(1 << 16));
        }
        else if (address % 4 == 2)
        {
            mem->data[(address * 8)/32] = value | (mem->data[(address * 8)/32] & 0x00);
        }
    }
    else
    {
        if (address % 4 == 0)
        {
           
            mem->data[(address * 8)/32] = value | (mem->data[(address * 8)/32] & 0x00);
        }
        else if (address % 4 == 2)
        {
            buff = value;
            buff = buff << 16;
            mem->data[(address * 8)/32] = buff | (mem->data[(address * 8)/32] & ~(1 << 16));
        }
    }
     return 0;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    int retour = 0;
    mem->data[(address * 8)/32] = value;
    return retour;
}


void afficher_memoire(memory m)
{
    printf("\n valeur memoire : \n");
    int i = 0;
    for (i = 0; i < m->taille; i++)
    {
        printf("indice : %d, valeur : %08x \n",i,m->data[i]);
    }
    printf("\n");
}