#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

#define error(args...) do { fprintf (stderr, args); exit (1); } while (0)

char* readline (FILE* f);

void dict_app_fun(const char* key, const char* val, void* arg){
  printf("%s:%s\n", key, val);
}

void save_file_fun(const char* key, const char* val, void* arg) {
  FILE* fp = arg;
  fprintf (fp, "%s:%s\n", key, val);
}

int main (int argc, char** argv) {
  dict_t* dict = dict_create ();

  for(int i = 1; i < argc; i++) {
      FILE* softArg = fopen(argv[i], "r");
      if (!softArg) {
        error("File does not exist!");
        break;
      }
      char line[4096];
      while(fgets(line, sizeof(line), softArg)) {
        char* val;
        for (val = line; *val && *val != ':'; ++val){
          ;
        }
        *(val++) = 0;
        size_t lastIndx = strlen(val)-1;
        if (val[lastIndx] == '\n') {
          val[lastIndx] = '\0';
        }
        dict_put(dict, line, val);
      }
      fclose(softArg);
  }

  printf("> ");

  //main input loop
  char* cmd = NULL;
  while((cmd = readline(stdin)) != NULL){
    if ( strncmp(cmd, "get", 3) == 0 && cmd[3] == ' '){
      char* arg = cmd + 4;
      char* val = dict_get(dict, arg);
      printf("%s\n", val ? val : "");
      free(cmd);
      printf("> ");
    }    
    else if (strncmp (cmd, "put", 3) == 0 && cmd[3] == ' ') {
      char* key = cmd +4;
      char* val;
      for(val = key; *val && *val != ':'; ++val){
        ;
      }
      if (*val == 0) {
          error("put without colon in %s\n", cmd);
          break;
      }
      else {
        *(val++) = 0; //sets colon equal to 0
        dict_put(dict, key, val);
        free(cmd);
        printf("> ");
      }
    }
    else if ( strncmp(cmd, "del", 3) == 0 && cmd[3] == ' '){
      char* arg = cmd + 4; //this should give us the key fed by the cmd
      dict_del(dict, arg);
      free(cmd);
      printf("> ");
    }  
    else if (strcmp(cmd, "clr") == 0) {
      dict_clear(dict);
      free(cmd);
      printf("> ");
    }
    else if (strcmp(cmd, "siz") == 0) {
      printf("%lu\n", dict_size(dict));
      free(cmd);
      printf("> ");
    }
    else if (strcmp(cmd, "dmp") == 0){
      dict_apply_fun_t fun = dict_app_fun;
      void* arg = NULL;
      printf("BEGIN_DUMP\n");
      dict_apply(dict, fun, arg);
      printf("END_DUMP\n");
      free(cmd);
      printf("> ");
    }
    else if (strncmp(cmd, "svf", 3) == 0 && cmd[3] == ' ') {
      dict_apply_fun_t fun = save_file_fun;
      char* filePath = cmd + 4;
      FILE* f = fopen(filePath, "w");
      if (!f){
        error("File does not exist!");
        free(cmd);
        break;
      }
      dict_apply(dict, fun, f);
      fclose(f);
      printf("SAVED\n");
      free(cmd);
      printf("> ");
    }
    else if (strncmp(cmd, "ldf", 3) == 0 && cmd[3] == ' ') { //the hard one
      char* filePath = cmd + 4;
      FILE* f = fopen(filePath, "r");
      if (!f) {
        error("File does not exist!");
        break;
      }
      char line[10000];
      while(fgets(line, sizeof(line), f)) {
        char* val;
        for (val = line; *val && *val != ':'; ++val){
          ;
        }
        *(val++) = 0;
        size_t lastIndx = strlen(val)-1;
        if (val[lastIndx] == '\n') {
          val[lastIndx] = '\0';
        }
        dict_put(dict, line, val);
      }
      fclose(f);
      printf("LOADED\n");
      free(cmd);
      printf("> ");
    }
    else {
      error("Syntax error: %s\n", cmd);
      free(cmd);
      break;
    }
  }
  printf ("Process ended");
  dict_destroy(dict);
}

#define BUFLEN 1024

char* readline (FILE* f) {
  char*  buf = NULL;
  size_t alloc_len = 0;
  ssize_t pos = -1;

  while (1) {
    int c = fgetc (f);

    if (c == EOF)
      return buf;
    ++pos;
    if (alloc_len <= pos) { // Reading one more char, and no more space.
      alloc_len += BUFLEN;
      buf = realloc (buf, alloc_len);
    }
    if (c == (int) '\n') {
      buf[pos] = 0;
      return buf;
    }
    buf[pos] = (char) c;
  }

  return buf;
}
