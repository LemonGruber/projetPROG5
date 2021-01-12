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

    uint32_t buff;
    int decalage = 0;

    switch (address % 4)
    {
        case 0 :
            decalage = 24;
        break;
        case 3 :
            decalage = 0;
        break;
        case 2 :
            decalage = 8;
        break;
        case 1 : 
            decalage = 16;
        break;
    }
    if (!mem->is_big_endian)
    {
        decalage = 24 - decalage;
    }
        acces_mem_address (mem,address,&buff,decalage,LSR);
    
    *value = buff;           
    return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    *value = 0;
    
    uint32_t buff;
    int retour = 0;
    
    uint32_t mask;
    int decalage;
    
     if (address%4 == 0)
    {
        decalage = 0;
        mask = 16;
    }
    else if (address %4 == 2)
    {
        decalage = 16;
        mask = 0;
    }
    if(mem->is_big_endian)
    {
        decalage = 16 - decalage;
    }
        
    acces_mem_address (mem,address,&buff,decalage,LSR);
    buff = buff & ~(0xFFFF << mask);
    *value = buff;
   
    return retour;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    *value = 0;
    int retour = 0;
    acces_mem_address (mem,address,value,0,LSL);
    return retour;
    
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    uint32_t buff;
    
    int decalage;
    switch (address % 4)
    {
        case 0:
            decalage = 0;  
        break;
        case 3 :
            decalage = 24;  
        break;
        case 1 :
            decalage = 8;  
        break;
        case 2 :
            decalage = 16;
        break;
    }
    if (mem->is_big_endian)
    {
        decalage = 24-decalage;
    }
 
    buff = value;
    appliquer_mask (mem,~(0xff << decalage),address);
    ecriture_mem_address (mem,address,buff << decalage);
    
    return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
    
    uint32_t buff;
    
    int decalage;
    
  
    if (address  % 4 == 0)
    {
        decalage = 16;
    }
    else if (address % 4 == 2)
    {
        decalage = 0;
    }
    if (!mem->is_big_endian)
    {
        decalage = 16 - decalage; 
    }
    
    buff = value;
    appliquer_mask (mem,~(0xFFFF<<decalage),address);
    ecriture_mem_address (mem,address,buff << decalage);
   
    return 0;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    int retour = 0;
    appliquer_mask (mem,~(0xFFFFFFFF),address);
    ecriture_mem_address (mem,address,value);
    return retour;
}

void afficher_memoire(memory m)
{
    printf("\n valeur memoire : \n");
    int i = 0;
    for (i = 0; i < m->taille; i++)
    {
        if (m->data[i] != 0)
        {
            printf("indice : %d, valeur : %08x \n",i,m->data[i]);
        }
    }
    printf("\n");
}

void acces_mem_address (memory m,uint32_t address, uint32_t *retour,int deccalage, type_shift LS)
{
    if (LS == LSL)
    {
        *retour = m->data[(address * 8)/32] << deccalage;
    }
    else
    {
        *retour = m->data[(address * 8)/32] >> deccalage;
    }
}

void ecriture_mem_address (memory m, uint32_t address, uint32_t value)
{
    int retour;
    if (address *8 < memory_get_size(m))
    {
        m->data[(address * 8)/32] =  m->data[(address * 8)/32] | value;
        retour = 0;
    }
    else 
    {
       retour = 1;
    }
    if (retour)
    {
        printf("ok");
    }
}

void appliquer_mask (memory m, uint32_t mask, uint32_t address)
{
    m->data[(address * 8)/32] = m->data[(address * 8)/32] & mask;
}