#include <stdio.h>
#include <stdlib.h>

struct list_item {
  int value;

  struct list_item *next;
};

void append(struct list_item *first, int x); /* puts x at the end of the list */
void prepend(struct list_item *first, int x); /* puts x at the beginning of the list */
void print(struct list_item *first);  /* prints all elements in the list */
/* input_sorted: find the first element in the list larger than x
   and input x right before that element */
void input_sorted(struct list_item *first, int x);
void clear(struct list_item *first); /* free everything dynamically allocated */

struct list_item * generate(int x);


void append(struct list_item *first, int x) {
  if (first->next == NULL) {
    struct list_item *itm = generate(x);
    first->next = itm;
  } else {
    append(first->next, x);
  }
}


void prepend(struct list_item *first, int x) {
  struct list_item *itm = generate(x);

  itm->next = first->next;
  first->next = itm;  
}

void print(struct list_item *first) {
  if (first->next == NULL) {
    if (first->value == -1) {
      printf("list is empty.\n");
      return;
    }

    printf("%d\n",first->value);
  } else {
    if (first->value != -1) {
      printf("%d, ", first->value);
      
    }
    print(first->next);
  }
}


void input_sorted(struct list_item *first, int x) {
  struct list_item *ptr = first;

  while (ptr->next != NULL) {
    if (ptr->next->value > x) {
      struct list_item *n = generate(x);
      n->next = ptr->next;
      ptr->next = n;
      return;
    } else {
      ptr = ptr->next;
    }
  }
  struct list_item *n = generate(x);
  ptr->next = n;
}

void clear(struct list_item *first) {
  while (first->next != NULL) {
    struct list_item *tmp = first->next;
    first->next = first->next->next;
    free(tmp);
  }
}

struct list_item * generate(int x) {
  struct list_item *itm = (struct list_item *) malloc(sizeof(struct list_item));
  itm->value = x;
  itm->next = NULL;
  return itm;
}


/* main */
int main( int argc, char ** argv ) {
  struct list_item root;
  root.value = -1; /* Ignore this value */
  root.next = NULL;

  // adding items.
  append(&root, 1);
  append(&root, 2);
  append(&root, 4);
  input_sorted(&root, 3);
  input_sorted(&root, 0);
  input_sorted(&root, 5);
  prepend(&root, 4);

  print(&root);
  clear(&root);
  print(&root);
}
