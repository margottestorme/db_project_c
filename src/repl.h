#ifndef REPL_H
#define REPL_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include "row.h"
#include "btree.h"

// Enums
typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

// Structs
#define TABLE_MAX_ROWS 10

typedef struct {
    BinaryTree* tree;
} Table;

Table* new_table(void);

// Structure de la base de données
typedef struct {
    Table* tables[32];
    char table_names[32][32];
    size_t num_tables;
    Table* active_table;
    char active_table_name[32];
} Database;

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  StatementType type;
  Row insert_row;
} Statement;

// Function declarations
InputBuffer* new_input_buffer(void);
void print_prompt(void);
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Database* db);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement, Database* db);
void execute_statement(Statement* statement, Table* table);
void repl(void);
void free_table(Table* table);

#endif

