#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct __ENTRY {
    char lastName[16];
    struct __ENTRY *pNext;
} entry;

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

void find(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return;
        pHead = pHead->pNext;
    }
    printf("%s does not exist!\n",lastname);
}

int main()
{
    int i = 0;
    FILE *fp1 = fopen("output","r");
    FILE *fp2 = fopen("./dictionary/words.txt","r");
    char line[16];
    entry *pHead,*e;
    pHead = (entry *) malloc (sizeof(entry));
    e = pHead;
    e->pNext = NULL;
    while(fgets(line,sizeof(line),fp1)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    e = pHead;
    while(fgets(line,sizeof(line),fp2)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        find(line, e);
        e = pHead;
    }
}
