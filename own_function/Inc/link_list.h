//
// Created by watermalen on 25-3-1.
//


#ifndef LINK_LIST_H
#define LINK_LIST_H
/* Includes ------------------------------------------------------------------*/
#include "link_list.h"
#include <stdlib.h>
#include <sys/_stdint.h>
/* typedef ------------------------------------------------------------------*/

typedef struct int8_single_chain {
  uint8_t data;
  struct int8_single_chain *next;
} int8_single_chain;

typedef struct int8_double_chain {
  uint8_t data;
  struct int8_double_chain *next;
  struct int8_double_chain *prev;
} int8_double_chain;

/* function ------------------------------------------------------------------*/
uint8_t create_int8_single_chain_list(uint8_t *data, uint32_t length, int8_single_chain **head,
                                      int8_single_chain **tail);

uint8_t create_int8_single_chain_loop(uint8_t *data, uint32_t length, int8_single_chain **head,
                                      int8_single_chain **tail);

uint8_t change_int8_single_list(uint8_t data, int8_single_chain *head, uint32_t location);

uint32_t combine_int8_single_list(int8_single_chain *tail1, uint32_t length1, int8_single_chain *head2,
                                  uint32_t length2);

uint8_t create_int8_double_chain_list(uint8_t *data, uint32_t length, int8_double_chain **head,
                                      int8_double_chain **tail);

uint8_t create_int8_double_chain_loop(uint8_t *data, uint32_t length, int8_double_chain **head,
                                      int8_double_chain **tail);


#endif //LINK_LIST_H
