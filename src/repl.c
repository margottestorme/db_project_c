#include "repl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }




void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}


void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}


MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    //TODO  here implement handling of other input as .exit
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer* input_buffer,
                                Statement* statement) {

  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;

    int id;
    char name[32];
    int args_assigned = sscanf(input_buffer->buffer, "insert %d %31s", &id, name);
    if (args_assigned < 2) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    // Remplir les champs de statement
    statement->insert_row.id = id;
    strncpy(statement->insert_row.name, name, sizeof(statement->insert_row.name) - 1);
    statement->insert_row.name[sizeof(statement->insert_row.name) - 1] = '\0'; // Null-terminate

    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
    //TODO Implement the command here
      if (table->num_rows >= TABLE_MAX_ROWS) {
        printf("La table est pleine!\n");
        return;
      }

      // Ajouter la ligne à la table
      Row* row = &(table->rows[table->num_rows]);
      *row = statement->insert_row; // Copie des données de la déclaration vers la table
      table->num_rows++;

      printf("Ligne insérée avec sucès. Nombre total de lignes: %zu\n", table->num_rows);
      break;
    case (STATEMENT_SELECT):
      //TODO implement the command here
      for (size_t i = 0; i < table->num_rows; i++) {
        Row* row = &(table->rows[i]);
        printf("ID: %d, Nom: %s\n", row->id, row->name);
      } 
      break;
  }
}


void repl(void){
  InputBuffer* input_buffer = new_input_buffer();
  Table* table = new_table();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        printf("recognized statement\n");
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }
     execute_statement(&statement, table);
     printf("Executed.\n");
  }
}

Table* new_table() {
  Table* table = (Table*)malloc(sizeof(Table));
  if (table == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for table.\n");
        exit(EXIT_FAILURE);
    }     
  table->num_rows = 0;
  return table;
}

void free_table(Table* table) {
  free(table);
}

