#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TABLE_SIZE 5553

typedef struct __ENTRY {
    char lastName[16];
    struct __ENTRY *pNext;
} entry;

typedef struct _TABLE {
    entry **table;
} hash_t;

int hash(char *str)
{
    unsigned int hash_value = 0;
    while(*str)
        hash_value = (hash_value << 5) - hash_value + (*str++);
    return (hash_value % TABLE_SIZE);
}

hash_t *create_hash_table()
{
    hash_t *my_table;

    /* Allocate memory for hashtable*/
    my_table = malloc(sizeof(hash_t));
    my_table->table = malloc(sizeof(entry *) * TABLE_SIZE);

    /* Initialize the elements of the table */
    for(int i=0; i<TABLE_SIZE; i++)
        my_table->table[i] = NULL;

    return my_table;
}

entry *findName(char *lastName, hash_t *hashtable)
{
    entry *list;
    int hash_value = hash(lastName);

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

void append(char *lastName,hash_t *hashtable)
{
    entry *new_entry;
    int hash_value = hash(lastName);

    new_entry = (entry *) malloc(sizeof(entry));

    /* Creating entry list by hash table */
    memcpy(new_entry->lastName , lastName,strlen(lastName));
    new_entry->pNext = hashtable->table[hash_value];
    hashtable->table[hash_value] = new_entry;
}

int main()
{
    int i = 0;
    FILE *fp1 = fopen("opt_entry","r");
    FILE *fp2 = fopen("./dictionary/words.txt","r");
    char line[16];
    entry *pHead,*e;
    pHead = (entry *) malloc (sizeof(entry));
    e = pHead;
    e->pNext = NULL;

    hash_t *hash_table;
    hash_table = create_hash_table();

    while(fgets(line,sizeof(line),fp1)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append(line, hash_table);
    }

    while(fgets(line,sizeof(line),fp2)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        findName(line, hash_table);
    }
    printf("Check Done!\n");
}
