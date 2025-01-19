#ifndef BTREE_H
#define BTREE_H

#include <stddef.h>
#include "row.h"

typedef struct Node {
    Row data;          
    struct Node* left;
    struct Node* right;
} Node;

// Structure principale de l'arbre binaire
typedef struct {
    Node* root;
    size_t size;
} BinaryTree;

// Fonctions pour gérer l'arbre binaire
BinaryTree* new_binary_tree();
void delete_binary_tree(BinaryTree* tree);

void insert_into_tree(BinaryTree* tree, Row data);
Row* search_tree(BinaryTree* tree, int id);
void delete_from_tree(BinaryTree* tree, int id);
void print_tree(BinaryTree* tree);

#endif

