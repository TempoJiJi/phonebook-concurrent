#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#include "phonebook_opt.h"
#include "debug.h"

entry *findName(char lastname[], entry *pHead)
{
    size_t len = strlen( lastname);
    while (pHead != NULL) {
        if (strncasecmp(lastname, pHead->lastName, len) == 0
                && (pHead->lastName[len] == '\n' ||
                    pHead->lastName[len] == '\0')) {
            pHead->lastName = (char *) malloc( sizeof(char) * MAX_LAST_NAME_SIZE);
            memset(pHead->lastName, '\0', MAX_LAST_NAME_SIZE);
            strcpy(pHead->lastName, lastname);
            pHead->dtl = (pdetail) malloc( sizeof( detail));
            return pHead;
        }
        dprintf("find string = %s", pHead->lastName);
        pHead = pHead->pNext;
    }
    return NULL;
}

args *new_args(char *ptr, char *eptr, entry *start,int i)
{
    args *app = (args *) malloc(sizeof(args));

    app->ptr = ptr;	//map + MAX_LAST_NAME_SIZE * i
    app->eptr = eptr;	// map + fs
    app->entryStart = start;	//entry_pool + i
    app->pHead = (app->pLast = app->entryStart);

    return app;
}

void append(void *arg)
{
    args *app = (args *) arg;

    char *i = app->ptr;
    entry *j = app->entryStart;

    while (i < app->eptr) {
        app->pLast->pNext = j;	//Current last->j
        app->pLast = app->pLast->pNext;	    // Current last become j

        app->pLast->lastName = i;
        app->pLast->pNext = NULL;

        i += MAX_LAST_NAME_SIZE * THREAD_NUM;
        j += THREAD_NUM;
    }
}

void show_entry(entry *pHead)
{
    while (pHead) {
        printf("%s", pHead->lastName);
        pHead = pHead->pNext;
    }
}

