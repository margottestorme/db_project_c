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


MetaCommandResult do_meta_command(InputBuffer* input_buffer, Database* db) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } 
  if (strcmp(input_buffer->buffer, ".status") == 0) {
    if (db->active_table != NULL) {
        printf("Table active : '%s'\n", db->active_table_name);
    } else {
        printf("Aucune table active.\n");
    }
    printf("Nombre total de tables : %zu\n", db->num_tables);
    return META_COMMAND_SUCCESS;
  } 
  else {
    //TODO  here implement handling of other input as .exit
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

Table* find_table(Database* db, const char* name) {
  for (size_t i = 0; i < db->num_tables; i++) {
    if (strcmp(db->table_names[i], name) == 0) {
      return db->tables[i];
    }
  }
  return NULL;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement, Database* db) {
  if (strncmp(input_buffer->buffer, "create table", 12) == 0) {
    char table_name[32];
    if (sscanf(input_buffer->buffer, "create table %31s", table_name) != 1) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    if (db->num_tables >= 32) {
      printf("Erreur : Nombre maximum de tables atteint.\n");
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    if (find_table(db, table_name) != NULL) {
      printf("Erreur : Une table avec ce nom existe déjà.\n");
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    Table* new_table_instance = new_table();
    db->tables[db->num_tables] = new_table_instance;
    strncpy(db->table_names[db->num_tables], table_name, 31);
    db->table_names[db->num_tables][31] = '\0';
    db->num_tables++;

    printf("Table '%s' créée avec succès.\n", table_name);
    return PREPARE_SUCCESS;
  }

  if (strncmp(input_buffer->buffer, "use", 3) == 0) {
    char table_name[32];
    if (sscanf(input_buffer->buffer, "use %31s", table_name) != 1) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    Table* table = find_table(db, table_name);
    if (table == NULL) {
      printf("Erreur : Table '%s' introuvable.\n", table_name);
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    db->active_table = table;
    strncpy(db->active_table_name, table_name, sizeof(db->active_table_name));
    printf("Table active : '%s'.\n", table_name);
    return PREPARE_SUCCESS;
  }

  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    if (db->active_table == NULL) {
      printf("Erreur : Aucune table active. Utilisez 'use <nom_table>'.\n");
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    statement->type = STATEMENT_INSERT;

    int id;
    char name[32];
    int args_assigned = sscanf(input_buffer->buffer, "insert %d %31s", &id, name);
    if (args_assigned < 2) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

    statement->insert_row.id = id;
    strncpy(statement->insert_row.name, name, sizeof(statement->insert_row.name) - 1);
    statement->insert_row.name[sizeof(statement->insert_row.name) - 1] = '\0';

    return PREPARE_SUCCESS;
  }

  if (strcmp(input_buffer->buffer, "select") == 0) {
    if (db->active_table == NULL) {
      printf("Erreur : Aucune table active. Utilisez 'use <nom_table>'.\n");
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }

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

Database* new_database() {
  Database* db = (Database*)malloc(sizeof(Database));
  if (db == NULL) {
    fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour la base de données.\n");
    exit(EXIT_FAILURE);
  }
  db->num_tables = 0;
  db->active_table = NULL;
  return db;
}


void repl(void){
  InputBuffer* input_buffer = new_input_buffer();
  Database* db = new_database();

  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer, db)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement, db)) {
      case (PREPARE_SUCCESS):
        printf("recognized statement\n");
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }
    if (db->active_table != NULL) {
      execute_statement(&statement, db->active_table);
      printf("Executed.\n");
    }
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

