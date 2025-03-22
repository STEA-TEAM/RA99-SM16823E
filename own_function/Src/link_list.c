//
// Created by watermalen on 25-3-1.
//
#include "link_list.h"



uint8_t create_int8_single_chain_list(uint8_t *data, uint32_t length, int8_single_chain **head,
                                      int8_single_chain **tail) {
  if (length != 0) {
    int8_single_chain *p = (int8_single_chain *) malloc(sizeof(int8_single_chain));
    p->data = *data;
    p->next = NULL;
    *head = p;
    uint32_t i = 1;
    while (i < length) {
      i++;
      int8_single_chain *q = (int8_single_chain *) malloc(sizeof(int8_single_chain));
      q->data = *(data + i);
      q->next = NULL;
      p->next = q;
      p = p->next;
    }
    *tail = p;
    return 0;
  }
  return 1;
}

uint8_t create_int8_single_chain_loop(uint8_t *data, uint32_t length, int8_single_chain **head,
                                      int8_single_chain **tail) {
  if (length != 0) {
    int8_single_chain *p = (int8_single_chain *) malloc(sizeof(int8_single_chain));
    p->data = *data;
    p->next = NULL;
    *head = p;
    uint32_t i = 1;
    while (i < length) {
      i++;
      int8_single_chain *q = (int8_single_chain *) malloc(sizeof(int8_single_chain));
      q->data = *(data + i);
      q->next = *head;
      p->next = q;
      p = p->next;
    }
    *tail = p;
    return 0;
  }
  return 1;
}

uint8_t change_int8_single_list(uint8_t data, int8_single_chain *head, uint32_t location) {
  int8_single_chain *loca = head;
  uint32_t i = 1;
  while (i < location) {
    i++;
    if (loca->next == NULL) {
      return 1;
    }
    loca = loca->next;
  }
  loca->data = data;
  return 0;
}

uint32_t combine_int8_single_list(int8_single_chain *tail1, uint32_t length1, int8_single_chain *head2,
                                  uint32_t length2) {
  tail1->next = head2;
  return length1 + length2;
}


uint8_t create_int8_double_chain_list(uint8_t *data, uint32_t length, int8_double_chain **head,
                                      int8_double_chain **tail) {
  if (length != 0) {
    int8_double_chain *p = (int8_double_chain *) malloc(sizeof(int8_double_chain));
    p->data = *data;
    p->next = NULL;
    p->prev = NULL;
    *head = p;
    uint32_t i = 1;
    while (i < length) {
      i++;
      int8_double_chain *q = (int8_double_chain *) malloc(sizeof(int8_double_chain));
      q->data = *(data + i);
      q->prev = p;
      q->next = NULL;
      p->next = q;
      p = p->next;
    }
    *tail = p;
    return 0;
  }
  return 1;
}

uint8_t create_int8_double_chain_loop(uint8_t *data, uint32_t length, int8_double_chain **head,
                                      int8_double_chain **tail) {
  if (length != 0) {
    int8_double_chain *p = (int8_double_chain *) malloc(sizeof(int8_double_chain));
    p->data = *data;
    p->next = NULL;
    p->prev = NULL;
    *head = p;
    uint32_t i = 1;
    while (i < length) {
      i++;
      int8_double_chain *q = (int8_double_chain *) malloc(sizeof(int8_double_chain));
      q->data = *(data + i);
      q->prev = p;
      q->next = *head;
      p->next = q;
      p = p->next;
    }
    (*head)->prev = p;
    *tail = p;
    return 0;
  }
  return 1;
}
