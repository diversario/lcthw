#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
  int id;
  int set;
  char *name;
  char *email;
};

struct Database {
  int max_data;
  int max_rows;
  struct Address *rows;
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *messages, struct Connection *conn) {
  if (errno) {
    perror(messages);
  } else {
    printf("ERROR: %s\n", messages);
  }

  if (conn) Database_close(conn);

  exit(1);
}

void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, *addr->name, *addr->email);
}

void Database_load(struct Connection *conn) {
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);

  if (rc != 1) {
    die("Failed to load database", conn);
  }
}

struct Connection *Database_open(const char *filename, char mode, int max_data, int max_rows) {
  struct Connection *conn = malloc(sizeof(struct Connection));
  if (!conn) die("Memory error", conn);

  conn->db = malloc(sizeof(struct Database));
  if (!conn->db) die("Memory error", conn);

  if (mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if (conn->file) {
      Database_load(conn);
    }
  }

  if (!conn->file) die("Failed to open the file", conn);

  return conn;
}

void Database_close(struct Connection *conn) {
  if (conn) {
    if (conn->file) fclose(conn->file);
    if (conn->db) free(conn->db);
    free(conn);
  }
}

void Database_write(struct Connection *conn) {
  rewind(conn->file);

  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
  if (rc != 1) die("Failed to write database", conn);

  rc = fflush(conn->file);
  if (rc == -1) die("Cannot flush database", conn);
}

void Database_create(struct Connection *conn) {
  for (int i = 0; i < MAX_ROWS; i++) {
    struct Address addr = {.id = i, .set = 0};
    conn->db->rows[i] = addr;
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
  struct Address *addr = &conn->db->rows[id];
  if (addr->set) die("Already set, delete it first", conn);

  addr->set = 1;

  char *res = strncpy(addr->name, name, MAX_DATA);
  addr->name[sizeof(addr->name) - 1] = '\0';
  if (!res) die("Name copy failed", conn);

  res = strncpy(addr->email, email, MAX_DATA);
  addr->email[sizeof(addr->email) - 1] = '\0';
  if (!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id) {
  struct Address *addr = &conn->db->rows[id];

  if (addr->set) Address_print(addr);
  else die("ID is not set", conn);
}

void Database_delete(struct Connection *conn, int id) {
  struct Address addr = {.id = id, .set = 0};
  conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
  int i = 0;
  struct Database *db = conn->db;

  for (i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = &db->rows[i];

    if (cur->set) {
      Address_print(cur);
    }
  }
}

int main (int argc, char *argv[]) {
  if (argc < 5) die("USAGE: ex17 <max data> <max rows> <dbfile> <action> [action params]", NULL);

  int max_data = argv[1];
  int max_rows = argv[2];

  char *filename = argv[3];
  char action = argv[4][0];
  struct Connection *conn = Database_open(filename, action, max_data, max_rows);
  int id = 0;

  if (argc > 3) id = atoi(argv[5]);
  if (id >= MAX_ROWS) die("There's not that many records.", conn);

  switch(action) {
    case 'c':
      Database_create(conn);
      Database_write(conn);
      break;
    case 'g':
      if (argc != 4) die("Need an ID to get", conn);
      Database_get(conn, id);
      break;
    case 's':
      if(argc != 6) die("Need id, name, email to set", conn);

      Database_set(conn, id, argv[6], argv[7]);
      Database_write(conn);
      break;
    case 'd':
      if(argc != 4) die("Need id to delete", conn);

      Database_delete(conn, id);
      Database_write(conn);
      break;
    case 'l':
      Database_list(conn);
      break;
    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
  }

  Database_close(conn);

  return 0;
}
