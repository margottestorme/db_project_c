#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "repl.h"
#include "btree.h"

// Test pour prepare_statement
void test_prepare_statement() {
    InputBuffer input_buffer;
    Statement statement;
    Database* db = new_database();
    Table* table = new_table(); 

    // Test de la création d'une table
    input_buffer.buffer = "create table pokemons";
    assert(prepare_statement(&input_buffer, &statement, db) == PREPARE_SUCCESS);

    // Vérification que la table a bien été ajoutée
    assert(db->num_tables == 1);
    assert(strcmp(db->table_names[0], "pokemons") == 0);

    // Test de la sélection de la table active avec "use"
    input_buffer.buffer = "use pokemons";
    assert(prepare_statement(&input_buffer, &statement, db) == PREPARE_SUCCESS);

    // Vérification que la table est bien sélectionnée comme active
    assert(db->active_table != NULL);
    assert(strcmp(db->active_table_name, "pokemons") == 0);

    // Test d'insertion après "use"
    input_buffer.buffer = "insert 2 john";
    assert(prepare_statement(&input_buffer, &statement, db) == PREPARE_SUCCESS);
    assert(statement.type == STATEMENT_INSERT);
    assert(statement.insert_row.id == 2);
    assert(strcmp(statement.insert_row.name, "john") == 0);

    // Test de la commande "select" après "use"
    input_buffer.buffer = "select";
    assert(prepare_statement(&input_buffer, &statement, db) == PREPARE_SUCCESS);
    assert(statement.type == STATEMENT_SELECT);

    free_database(db);
}



void test_execute_statement() {
    Table* table = new_table();
    Statement statement;

    // Insérer des lignes jusqu'à la limite
    statement.type = STATEMENT_INSERT;
    for (int i = 1; i <= TABLE_MAX_ROWS; i++) {
        statement.insert_row.id = i;
        sprintf(statement.insert_row.name, "Name%d", i);
        execute_statement(&statement, table);
    }

    // Vérifier que la table est pleine
    assert(table->num_rows == TABLE_MAX_ROWS);

    // Essayer d'insérer une ligne supplémentaire
    statement.insert_row.id = TABLE_MAX_ROWS + 1;
    sprintf(statement.insert_row.name, "Overflow");
    execute_statement(&statement, table);

    // Vérifier que la ligne supplémentaire n'a pas été insérée
    assert(table->num_rows == TABLE_MAX_ROWS);

    // Vérifier la commande select
    statement.type = STATEMENT_SELECT;
    execute_statement(&statement, table);

    // Libérer la mémoire
    free_table(table);
}

void test_binary_tree() {
    // Création d'un arbre
    BinaryTree* tree = new_binary_tree();
    assert(tree != NULL);
    assert(tree->root == NULL);
    assert(tree->size == 0);

    // Insertion de données
    Row row1 = {1, "mgt"};
    Row row2 = {2, "john"};
    Row row3 = {3, "doe"};
    insert_into_tree(tree, row1);
    insert_into_tree(tree, row2);
    insert_into_tree(tree, row3);
    printf("insertion dans btree :\n");
    print_tree(tree);

    // Vérification des insertions
    assert(tree->size == 3);
    assert(search_tree(tree, 1) != NULL);
    assert(search_tree(tree, 2) != NULL);
    assert(search_tree(tree, 3) != NULL);

    // Recherche d'un élément inexistant
    assert(search_tree(tree, 4) == NULL);

    // Suppression d'un élément
    delete_from_tree(tree, 2);
    assert(tree->size == 2);
    assert(search_tree(tree, 2) == NULL);

    // Vérification des éléments restants
    assert(search_tree(tree, 1) != NULL);
    assert(search_tree(tree, 3) != NULL);

    // Test du parcours et de l'affichage
    printf("Arbre après suppression (parcours en ordre) :\n");
    print_tree(tree);

    delete_binary_tree(tree);
    printf("Test du binary tree terminé\n");
}


int main() {
    test_prepare_statement();
    test_execute_statement();
    test_binary_tree();
    printf("Ca marche!\n");
    return 0;
}
