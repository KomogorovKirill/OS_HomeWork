#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "sched.h"

typedef struct pInfo
{
	int id;            
    	int endT;           
    	struct pInfo *next;
}pinfo_t;

static int searchFlag;

static pinfo_t *pFirst, *pNext, *pPrev, *pTmp, *pLast;

static int GetQuickProcess()
{
    	if(NULL == pFirst)
        	return -1;
	else
	{
   		pNext = (pinfo_t *) pFirst;
		pTmp = (pinfo_t *) pPrev;
	}

    	int flag = 1;

    	while(NULL != pNext || flag)
	{
		flag = 0;
		if (pTmp -> endT >= pNext-> endT)
	            pTmp = (pinfo_t *) pNext;
	        pNext = pNext -> next;
	} 
    
    	if (pTmp -> endT == 0 || pTmp -> id != pPrev -> id)
        	searchFlag = 1;
    	else
        	searchFlag = 0;
        
    	pPrev = (pinfo_t *) pTmp;
    	return pTmp -> id;
}

int sched_init(void)
{
	searchFlag = 0;
    	pFirst = NULL;
    	pNext = NULL;
    	pPrev = NULL;
    	pTmp = NULL;
    	pLast = NULL;
    	return 0;
}

void sched_clean(void)
{
    	pTmp = (pinfo_t *) pFirst;
    
    	while(pTmp != NULL)
    	{
        	pPrev = (pinfo_t *) pTmp;
        	pTmp = pTmp -> next;
        	free(pPrev);
    	}
        
}

int add_proc(void)
{
    	if (NULL == pFirst)
    	{
        	pFirst = (pinfo_t *) malloc(sizeof(pinfo_t));
        	pFirst -> id = 0;
        	pPrev = pFirst;
      	  	pLast = pFirst;
        	return 0;
    	}

    	pTmp = (pinfo_t *) malloc(sizeof(pinfo_t));
    	pTmp -> id = pLast -> id + 1;
    	pTmp -> endT = 0;
    	pTmp -> next = NULL;
    	pLast -> next = pTmp;
    	pLast = pTmp;

    	return pTmp -> id;

}

int sched(int time, int cont)
{
  	if(NULL == pFirst)
        	return -1;

  	if (time != 0 && cont == 0) 
  	{
    		if(pPrev == pFirst)
    		{
        		pTmp = (pinfo_t *) pFirst;
        		pFirst = (pinfo_t *) pFirst -> next;
        		free(pTmp);
       			pPrev = pFirst;
    			return GetQuickProcess();
    		}

    		pTmp = (pinfo_t *) pFirst;
    
    		while(pTmp -> next != pPrev)
        		pTmp = (pinfo_t *) pTmp -> next;
   		pTmp -> next = pPrev -> next;
    		pPrev = pFirst;
    		return GetQuickProcess();
  	}
  
  	pPrev -> endT = cont;
  
  	if (searchFlag)
		 return GetQuickProcess();
  	else
		 return pPrev -> id; 
}

int sched_fin(void)
{
    	sched_clean();
    	return 0;
}
