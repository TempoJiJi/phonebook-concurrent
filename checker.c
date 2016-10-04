#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TABLE_SIZE 5553

typedef struct __ENTRY {
    char lastName[16];
    struct __ENTRY *pNext;
} entry_c;

typedef struct _TABLE {
    entry_c **table;
} hash_t;

int hash_c(char *str)
{
    unsigned int hash_value = 0;
    while(*str)
        hash_value = (hash_value << 5) - hash_value + (*str++);
    return (hash_value % TABLE_SIZE);
}

hash_t *create_hash_table_c()
{
    hash_t *my_table;

    /* Allocate memory for hashtable*/
    my_table = malloc(sizeof(hash_t));
    my_table->table = malloc(sizeof(entry_c *) * TABLE_SIZE);

    /* Initialize the elements of the table */
    for(int i=0; i<TABLE_SIZE; i++)
        my_table->table[i] = NULL;

    return my_table;
}

entry_c *findName_c(char *lastName, hash_t *hashtable)
{
    entry_c *list;
    int hash_value = hash_c(lastName);

    /*   Searching from hash_table   */
    for(list = hashtable->table[hash_value] ; list!=NULL ; list = list->pNext) {
        if(strcmp(lastName,list->lastName)==0) {
            printf("%s FOUND!\n",lastName);
            return list;
        }
    }
    printf("%s DOES NOT EXIST!\n",lastName);
    /* FAIL */
    return NULL;
}

void append_c(char *lastName,hash_t *hashtable)
{
    entry_c *new_entry_c;
    int hash_value = hash_c(lastName);

    new_entry_c = (entry_c *) malloc(sizeof(entry_c));

    /* Creating entry_c list by hash table */
    strcpy(new_entry_c->lastName , lastName);
    new_entry_c->pNext = hashtable->table[hash_value];
    hashtable->table[hash_value] = new_entry_c;
}

void compare(char *a, char *b)
{
    int i = 0;

    /* fp2 is orginal file */
    FILE *fp1 = fopen(a, "r");
    FILE *fp2 = fopen(b, "r");

    char line[16];
    entry_c *pHead,*e;
    pHead = (entry_c *) malloc (sizeof(entry_c));
    e = pHead;
    e->pNext = NULL;

    hash_t *hash_table;
    hash_table = create_hash_table_c();

    while(fgets(line,sizeof(line),fp1)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append_c(line, hash_table);
    }

    while(fgets(line,sizeof(line),fp2)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        findName_c(line, hash_table);
    }
}
