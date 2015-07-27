#include "machinel.h"
 
#include <pebble.h>
 
// see .h file
struct aos *create_aos(int max) {
  struct aos * new_aos = malloc(sizeof (struct aos)); //allocates space for the structure
  new_aos->strings = malloc(sizeof (char *) * max); // allocates space for the array of strings
  new_aos->max = max;
  for (int i = 0; i < max; i++){
    new_aos->strings[i] = NULL;  //allocates space for each string
  }
  return new_aos;
 
}
 
 
 
 
// see .h file
void destroy_aos(struct aos *a) {
  int max = a->max;
  for (int i = 0 ;i < max; i++){
    free((a->strings)[i]);
  }
  free(a->strings);
  free(a);
}
 
 
 
// see .h file
char *aos_get(struct aos *a, int i) {
 
  if (i < 0){
    return NULL;
  }
  else if (i >= a->max){
    return NULL;
  }
  else if (a->strings[i] == NULL){
    return NULL;
  }
  else{
    return a->strings[i];    
  }                
}
 
// my_strdup(s) makes a duplicate of s
//  returns NULL if there is not enough memory
// effects: allocates memory: caller must free
// TIME: O(1)
char *my_strdup(const char *s) {
  char *new = malloc(sizeof(char) * strlen(s) + 1);
  strcpy(new,s);
  return new;
}
 
// see .h file
void aos_set(struct aos *a, const char *s, int i) {
 
  if (s == NULL){
    if (a->strings[i] != NULL){
      free(a->strings[i]);
    }
    a->strings[i] = NULL;
    return;
  }  
 
  if (a->strings[i] == NULL){
    a->strings[i] = malloc(sizeof(char) * (strlen(s) + 1));
    strcpy((a->strings)[i],s);    
  }
  else{
    free(a->strings[i]);
    (a->strings)[i] = my_strdup(s);  
  }
}
 
// see .h file
int aos_add(struct aos *a, const char *s) {
  int max = a->max;
  int found = 0;
  int k = 0;
  for (int i = 0; i < max;i++){
    if (a->strings[i] == NULL){
      found = 1;
      k = i;
      break;
    }
  }
  if (found == 0){
    return -1;
  }
  else{
    aos_set(a,s,k);
    return k;
  }
 
}
 
 
//finds the length of the array of strings
int aos_length(struct aos *a){
  char **s = a->strings;
  int max = a->max;
  int count = 0;
  for(int i = 0; i < max && s[i]!= NULL; i++){
    count++;
  }
  return count;
}
