#include <pebble.h>
// a module for handling an array of strings
struct aos {
   char **strings; //an array of strings
   int max; //the max number of strings that can be held in the array
};
// TIP: the field strings should be a dynamically allocated array of (char *)
//      where each (char *) is a pointer to a string (initialized to NULL)
//      for example, to access the first string, use a->strings[0]
//      to create it, you should be using: malloc(sizeof(char *) * max)
// create_aos(max) returns a new aos structure with enough capacity to store max strings
//     initially, no strings exist, so each entry is a NULL pointer
//     (which is not the same as an empty string)
//   requires:  max > 0
//   effect: allocates a new struct aos, caller must eventually call destroy_aos()
//   TIME: O(n), where n = max
struct aos *create_aos(int max);
// destroy_aos(a) will first free all of the (existing) strings in a
//     and then will free the array of strings in a
//     and then free the structure a itself
//   requires:  a was previously returned from create_aos
//   effect: a is deallocated (and no longer valid)
//   TIME: O(n), where n = a->max
void destroy_aos(struct aos *a);
//void search_fun(struct aos *a, const char *s);
// aos_get(a, i) will return the string at index i
//     or NULL if no string exists at index i
//     or NULL if i < 0 or i >= a->max
//   requires:  a is valid
//   TIME: O(1)
char *aos_get(struct aos *a, int i);
// aos_set(a, s, i) will set the string at index i to s
//     if a string previously existed at index i it will be freed
//     if s is NULL, then no string will exist at i (set to NULL)
//     otherwise, a duplicate copy of s will be stored
//     [NOTE: use your my_strdup function from P0a]
//   requires:  a is valid
//         0 <= i < a->max
//   effects: string at index i will be a duplicate of s
//         or no string will exist at i if s is NULL
//   TIME: O(n), where n is strlen(s)
void aos_set(struct aos *a, const char *s, int i);
// aos_add(a, s) is equivalent to aos_set(a, s, k)
//     where k is the first (lowest) index in the array
//     where a string does not exist (is a NULL pointer)
//     k is the value returned by aos_add.
//
//     if a is full (all elements in a exist),
//       aos_add does not change a and returns -1
//   requires:  a is valid
//         s != NULL
//   effects: a duplicate of s is stored in a, if a was not full
//   TIME: O(n), where n = a->max
int aos_add(struct aos *a, const char *s);
int aos_length(struct aos *a);
