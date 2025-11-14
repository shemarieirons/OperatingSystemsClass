// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) {
    if (l == NULL) return;
    node_t *curr = l->head;
    while (curr != NULL) {
        node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    free(l);
}

void list_print(list_t *l) {
    if (l == NULL || l->head == NULL) {
        printf("List is empty\n");
        return;
    }

    node_t *curr = l->head;
    while (curr != NULL) {
        printf("%d -> ", curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
}

char * listToString(list_t *l) {
    char* buf = malloc(10024);
    buf[0] = '\0';
    char tbuf[20];
    node_t* curr = l->head;
    while (curr != NULL) {
        sprintf(tbuf, "%d->", curr->value);
        strcat(buf, tbuf);
        curr = curr->next;
    }
    strcat(buf, "NULL");
    return buf;
}

int list_length(list_t *l) {
    node_t *curr = l->head;
    int count = 0;

    while (curr != NULL) {
        curr = curr->next;
        count+=1;
    }
        
    return count; }


void list_add_to_back(list_t *l, elem value) {
     node_t *new = getNode(value);
     if (l->head == NULL) {
        l->head = new;
        return;
    }
     node_t *curr = l->head;
     while (curr->next != NULL)
        curr = curr->next;
     curr->next = new;
}
void list_add_to_front(list_t *l, elem value) {
     node_t *cur_node = (node_t *) getNode(value);

     /* Insert to front */
     node_t *head = l->head;  // get head of list

     cur_node->next = head;
     l->head = cur_node;
}

node_t * getNode(elem value) {
  node_t *mynode;

  mynode = (node_t *) malloc(sizeof(node_t));
  mynode->value = value;
  mynode->next = NULL;

  return mynode;
}

void list_add_at_index(list_t *l, elem value, int index) {
    if (l == NULL || index <= 0) return;
    node_t *new_node = getNode(value);
    if (l->head == NULL) {
        if (index == 1) {
            l->head = new_node;
        } else {
            free(new_node);
        }
        return;
    }
    int len = list_length(l);
    if (index > len + 1) {
        free(new_node);
        return;
    }
    node_t *curr = l->head;
    node_t *prev = NULL;
    int i;
    for (i = 1; i < index; i++) {
        prev = curr;
        curr = curr->next;
        if (curr == NULL) break;
    }
    new_node->next = curr;
    if (prev == NULL) {
        l->head = new_node;
    } else {
        prev->next = new_node;
    }
}

elem list_remove_from_back(list_t *l) {
    
    if (l->head == NULL) {
        return -1;
    }
    node_t *curr = l->head;

    if (curr->next == NULL) {
        int del_val = curr->value;
        free(curr);
        l->head = NULL;
        return del_val;
    }
    
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    node_t *del = curr->next;
    curr->next = NULL;

    int del_val = del->value;
    free(del);
     
    return del_val; }

elem list_remove_from_front(list_t *l) {
    
    if (l->head == NULL) {
        return -1;
    }
    node_t *old_head = l->head;
    l->head = l->head->next;

    int val = old_head->value;
    free(old_head);
    return val;
}
elem list_remove_at_index(list_t *l, int index) {
    if (l == NULL || l->head == NULL || index < 0) {
        return -1;
    }

    node_t *curr = l->head;
    int del_val;

    if (index == 1) {
        del_val = curr->value;
        l->head = curr->next;
        free(curr);
        return del_val;
    }   

    node_t *prev = NULL;
    int j;
    for (j = 1; j < index && curr != NULL; j++) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        return -1;
    }

    prev->next = curr->next;
    del_val = curr->value;
    free(curr);

    return del_val;
}

bool list_is_in(list_t *l, elem value) { 
    node_t *curr = l->head;
    while (curr!=NULL) {
        if (curr->value == value) {
            return true;
        }
        curr = curr->next;
    }  
    
    return false; }
    
elem list_get_elem_at(list_t *l, int index) {
    if (l == NULL || l->head == NULL || index <= 0) {
        return -1;
    }
    node_t *curr = l->head;
    int s;
    for (s=1; s<index; s++) {
        curr = curr->next;
        if (curr == NULL) {
            return -1;
        }
    }
    return curr->value;
}

int list_get_index_of(list_t *l, elem value) { 
    if (l == NULL || l->head == NULL) return -1;
    node_t *curr = l->head;
    int count = 1;
    while (curr != NULL) {
        if (curr->value == value) {
            return count;
        }
        curr = curr->next;
        count++;
    }  
    return -1; 
}
