#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "repl.h"

// Test pour prepare_statement
void test_prepare_statement() {
    InputBuffer input_buffer;
    Statement statement;

    input_buffer.buffer = "insert 1 mgt";
    assert(prepare_statement(&input_buffer, &statement) == PREPARE_SUCCESS);
    assert(statement.type == STATEMENT_INSERT);
    assert(statement.insert_row.id == 1);
    assert(strcmp(statement.insert_row.name, "mgt") == 0);

    input_buffer.buffer = "select";
    assert(prepare_statement(&input_buffer, &statement) == PREPARE_SUCCESS);
    assert(statement.type == STATEMENT_SELECT);

    input_buffer.buffer = "invalid";
    assert(prepare_statement(&input_buffer, &statement) == PREPARE_UNRECOGNIZED_STATEMENT);
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

    // Vérifier les données existantes dans la table
    for (size_t i = 0; i < table->num_rows; i++) {
        assert(table->rows[i].id == (int)(i + 1));
        char expected_name[32];
        sprintf(expected_name, "Name%zu", i + 1);
        assert(strcmp(table->rows[i].name, expected_name) == 0);
    }

    // Vérifier la commande select
    statement.type = STATEMENT_SELECT;
    execute_statement(&statement, table);

    // Libérer la mémoire
    free_table(table);
}

int main() {
    test_prepare_statement();
    test_execute_statement();
    printf("Ca marche!\n");
    return 0;
}

