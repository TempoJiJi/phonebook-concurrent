#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

append_a *new_append_a(char *ptr, char *eptr, int tid, int ntd, entry *start)
{
    append_a *app = (append_a *) malloc(sizeof(append_a));

    /* 每個thread的起點 */    
    app->ptr = ptr;	//map + MAX_LAST_NAME_SIZE * i

    /* mapping 的終點 */
    app->eptr = eptr;	// map + fs

    /* DEBUG 用的，確認是哪個thread */
    app->tid = tid;	// i

    /* Thread的數量 */
    app->nthread = ntd;	    //THREAD_NUM

    /* 從entry pool的某個起點開始 */
    app->entryStart = start;	//entry_pool + i

    /* 將entry_pool的起點(已分爲4個）設爲app的起點 */
    app->pHead = (app->pLast = app->entryStart);

    return app;
}

void append(void *arg)
{
    append_a *app = (append_a *) arg;

    char *i = app->ptr;	
    entry *j = app->entryStart;	    

    while(i < app->eptr){
	app->pLast->pNext = j;	//Current last->j 
	app->pLast = app->pLast->pNext;	    // Current last become j

        app->pLast->lastName = i;   
        app->pLast->pNext = NULL;   
	
        i += MAX_LAST_NAME_SIZE * app->nthread;
        j += app->nthread;	
    }

    pthread_exit(NULL);
}

void show_entry(entry *pHead)
{
    while (pHead != NULL) {
        printf("%s", pHead->lastName);
        pHead = pHead->pNext;
    }
}
