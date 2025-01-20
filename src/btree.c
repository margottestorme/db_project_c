#include <stdio.h>
#include <stdlib.h>
#include "btree.h"


Node* new_node(Row data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire pour le nœud.\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BinaryTree* new_binary_tree() {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (tree == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire pour l'arbre.\n");
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

// Suppression de l'arbre 
void delete_binary_tree(BinaryTree* tree) {
    free_tree(tree->root);
    free(tree);
}

// Insertion dans l'arbre
Node* insert_node(Node* node, Row data) {
    if (node == NULL) {
        return new_node(data);
    }

    if (data.id < node->data.id) {
        node->left = insert_node(node->left, data);
    } else if (data.id > node->data.id) {
        node->right = insert_node(node->right, data);
    } else {
        printf("Erreur : Une entrée avec l'ID %d existe déjà.\n", data.id);
    }

    return node;
}

void insert_into_tree(BinaryTree* tree, Row data) {
    tree->root = insert_node(tree->root, data);
    tree->size++;
}

// Recherche dans l'arbre
Row* search_node(Node* node, int id) {
    if (node == NULL) {
        printf("Recherche de l'ID %d : nœud actuel NULL, ID non trouvé.\n", id);
        return NULL;
    }

    printf("Recherche de l'ID %d : nœud actuel ID %d.\n", id, node->data.id);

    if (id == node->data.id) {
        printf("Recherche réussie : ID %d trouvé avec le nom %s.\n", id, node->data.name);
        return &node->data;
    } else if (id < node->data.id) {
        printf("Recherche : ID %d est plus petit que %d, descente à gauche.\n", id, node->data.id);
        return search_node(node->left, id);
    } else {
        printf("Recherche : ID %d est plus grand que %d, descente à droite.\n", id, node->data.id);
        return search_node(node->right, id);
    }
}

Row* search_tree(BinaryTree* tree, int id) {
    printf("Démarrage de la recherche de l'ID %d dans l'arbre.\n", id);
    return search_node(tree->root, id);
}


// Suppression dans l'arbre
Node* delete_node(Node* node, int id) {
    if (node == NULL) return NULL;

    if (id < node->data.id) {
        node->left = delete_node(node->left, id);
    } else if (id > node->data.id) {
        node->right = delete_node(node->right, id);
    } else {
        if (node->left == NULL) {
            Node* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node* temp = node->left;
            free(node);
            return temp;
        }

        Node* temp = node->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        node->data = temp->data;
        node->right = delete_node(node->right, temp->data.id);
    }

    return node;
}

void delete_from_tree(BinaryTree* tree, int id) {
    tree->root = delete_node(tree->root, id);
    tree->size--;
}

// Parcours en ordre (In-order traversal)
void in_order_traversal(Node* node) {
    if (node == NULL) return;
    in_order_traversal(node->left);
    printf("ID: %d, Nom: %s\n", node->data.id, node->data.name);
    in_order_traversal(node->right);
}

void print_tree(BinaryTree* tree) {
    in_order_traversal(tree->root);
}


// Écrit les données de l'arbre binaire dans un fichier en parcours en ordre
void write_tree_to_file(FILE* file, Node* node) {
    if (node == NULL) return;
    write_tree_to_file(file, node->left);
    fprintf(file, "%d,%s\n", node->data.id, node->data.name);
    write_tree_to_file(file, node->right);
}

