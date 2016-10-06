#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "phonebook_opt.h"
#define TABLE_SIZE 5553

typedef struct __OPT_ENTRY {
    char lastName[16];
    struct __OPT_ENTRY *pNext;
} opt_entry;

typedef struct _HASH_TABLE {
    opt_entry **table;
} hash_table;

int hash(char *str)
{
    unsigned int hash_value = 0;
    while(*str)
        hash_value = (hash_value << 5) - hash_value + (*str++);
    return (hash_value % TABLE_SIZE);
}

hash_table *create_hash_table()
{
    hash_table *my_table;

    /* Allocate memory for hashtable*/
    my_table = malloc(sizeof(hash_table));
    my_table->table = malloc(sizeof(opt_entry *) * TABLE_SIZE);

    /* Initialize the elements of the table */
    for(int i=0; i<TABLE_SIZE; i++)
        my_table->table[i] = NULL;

    return my_table;
}

void find_lastname(char *lastName, hash_table *hashtable)
{
    opt_entry *list;
    int hash_value = hash(lastName);

    /*   Searching from hash_table   */
    for(list = hashtable->table[hash_value] ; list!=NULL ; list = list->pNext) {
        if(strcmp(lastName,list->lastName)==0) {
            printf("%s FOUND!\n",lastName);
            return;
        }
    }
    printf("%s DOES NOT EXIST!\n",lastName);
    /* FAIL */
    return;
}

void append_ori(char *lastName,hash_table *hashtable)
{
    opt_entry *new_opt_entry;
    int hash_value = hash(lastName);

    printf("appending\n");
    new_opt_entry = (opt_entry *) malloc(sizeof(char) * MAX_LAST_NAME_SIZE);
    printf("appended\n");

    /* Creating opt_entry list by hash table */
    strcpy(new_opt_entry->lastName , lastName);
    new_opt_entry->pNext = hashtable->table[hash_value];
    hashtable->table[hash_value] = new_opt_entry;
}

void compare(char *a, char *b)
{
    int i = 0;

    /* fp2 is orginal file */
    FILE *fp1 = fopen(a, "r");
    FILE *fp2 = fopen(b, "r");

    char line[16];
    opt_entry *pHead,*e;
    pHead = (opt_entry *) malloc (sizeof(opt_entry));
    e = pHead;
    e->pNext = NULL;

    hash_table *table;
    table = create_hash_table();

    while(fgets(line,sizeof(line),fp1)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append_ori(line, table);
    }

    while(fgets(line,sizeof(line),fp2)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        find_lastname(line, table);
    }
    fclose(fp1);
    fclose(fp2);
}
