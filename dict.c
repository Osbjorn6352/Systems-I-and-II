#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dict.h"

typedef struct dict_list {
    char* key;
    char* val;
    struct dict_list* next;
} dict_list_t;

typedef struct dict {
    dict_list_t* head;
    size_t size;
} dict_t;


dict_t* dict_create () {
  dict_t* ret = malloc(sizeof (dict_t));
  ret->size = 0;//default size is 0
  ret->head = NULL;//default head is null
  return ret;
}

void dict_put (dict_t* dict, const char* key, const char* val) {
  dict_list_t* curr;
  for (curr = dict->head; curr; curr = curr->next){
    if (strcmp(curr->key, key) == 0) {
      free (curr -> val);
      char* valCopy = strdup(val);
      curr -> val = valCopy;
      return;
    }
  }
  curr = malloc (sizeof (dict_list_t));
  curr -> key = strdup(key);
  curr -> val = strdup(val);
  curr -> next = dict -> head;
  dict -> head = curr;
  dict -> size++;
}

char* dict_get (const dict_t* dict, const char* key) {
  for (dict_list_t* curr = dict->head; curr; curr = curr->next){
    if (strcmp(curr->key, key) == 0){
        return curr->val; //stop the loop and return el->value if you find el->key in the list
    }
  }
  return NULL; //if key is not found returns NULL
}

void dict_del (dict_t* dict, const char* key) {
  if (dict->head == NULL){
    return;
  }
  dict_list_t* curr = dict -> head;
  if (strcmp (curr->key, key) == 0){       //special case first element marked for deletion
    char* delKey = dict->head->key;
    char* delVal = dict->head->val;
    dict_list_t* delEl = dict->head;       //mark the head for deletion
    dict -> head = dict->head->next;
    free(delKey);
    free(delVal);
    free(delEl);
    dict -> size--;
    return;                                 //free the marked head
  }
  while (curr -> next && (strcmp(curr->next->key,key))){
    curr = curr -> next;
  }
  if (curr->next == NULL){
    return;
  }
if (strcmp(curr->next->key, key) == 0){
    char* delKey = curr->next->key;
    char* delVal = curr->next->val;
    dict_list_t* del = curr->next; //mark the element for deletion
    curr -> next = curr->next->next;
    free(delKey);
    free(delVal);
    free(del);                     //delete element with all held values
    dict -> size--;
  }
}

size_t dict_size (const dict_t* dict) {
  return dict -> size;
}

void dict_clear (dict_t* dict) {
  dict -> size = 0;
  if (dict -> head == NULL){
    return;
  }
  else if (dict->head->next == NULL){
    dict_list_t* temp = dict -> head;
    free(dict->head->val);
    free(dict->head->key);
    dict -> head = dict->head->next;
    free(temp);
  }   
   else {
    dict_list_t* temp;
    char* tempKey;
    char* tempVal;
    while (dict -> head != NULL){
      temp = dict->head;
      tempKey = dict->head->key;
      tempVal = dict->head->val;
      dict -> head = dict->head->next;
      free(tempKey);
      free(tempVal);
      free(temp);
    }
  }
}

void dict_destroy (dict_t* dict) {
  dict -> size = 0;
  if (dict -> head == NULL){
    free(dict);  //added step of freeing the whole dict, in each and every case
  }
  else if (dict->head->next == NULL){
    free(dict->head->val);
    free(dict->head->key);
    free(dict -> head);
    free(dict);               //as opposed to dict_clear, this step is easier
  }                           //because we can directly free the dict
  else {
    dict_list_t* temp;
    char* tempKey;
    char* tempVal;
    while (dict -> head != NULL){
      temp = dict->head;
      tempKey = dict->head->key;
      tempVal = dict->head->val;
      dict -> head = dict->head->next;
      free(tempKey);
      free(tempVal);
      free(temp);
    }
    free(dict);              //here we add the step of freeing the dict
  }                          //after the completion of the while loop
}                            //the loop, however, is identical to that of
                             //dict_clear

typedef void (*dict_apply_fun_t) (const char* key, const char* val, void* arg);

void dict_apply (const dict_t* dict, const dict_apply_fun_t fun, void* arg) {
  dict_list_t* curr; //this will be our method of iterating through the list without changing d\
ict
  for (curr = dict->head; curr != NULL; curr = curr -> next) {
    fun(curr->key, curr->val, arg); //call our function for each item in our list
  }
}
