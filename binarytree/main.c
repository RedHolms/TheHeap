#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct _Node {
   int value;
   struct _Node* parent;
   struct _Node* left;
   struct _Node* right;
} Node;

void btree_create(Node** head, int value) {
   *head = malloc(sizeof(Node));
   memset(*head, 0, sizeof(Node));
   (*head)->value = value;
}

void btree_push(Node* head, int value) {
   if (head == NULL) return;

   Node* current = head;
   while (true) {
      if (current->value < value) {
         if (!current->right) {
            btree_create(&current->right, value);
            current->right->parent = current;
            return;
         } else {
            current = current->right;
         }
      } else if (current->value > value) {
         if (!current->left) {
            btree_create(&current->left, value);
            current->left->parent = current;
            return;
         } else {
            current = current->left;
         }
      } else return;
   }
}

bool btree_find(Node* head, int value) {
   if (head == NULL) return false;

   Node* current = head;
   while (current) {
      if (value < current->value)
         current = current->left;
      else if (value > current->value)
         current = current->right;
      else
         return true;
   }
   return false;
}

void btree_print(Node* node, int tabs) {
   printf("% *s%i\n", tabs, "", node->value);
   if (node->left) {
      printf("% *sleft\n", tabs, "");
      btree_print(node->left, tabs+2);
   }
   if (node->right) {
      printf("% *sright\n", tabs, "");
      btree_print(node->right, tabs+2);
   }
}

int main() {
   Node* btree = NULL;

   btree_create(&btree, 0);

   btree_push(btree, -1);
   btree_push(btree, -15);
   btree_push(btree, 23);
   btree_push(btree, 1);
   btree_push(btree, 0);

   btree_print(btree, 0);

   assert(btree_find(btree, -15));

   return 0;
}