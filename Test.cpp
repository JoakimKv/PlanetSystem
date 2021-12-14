
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>



int main(int argc, char** argv)
{
	int** ppiTal ;
	int   iCount, iAntal = 6 ;

	ppiTal = NULL ;
	//ppiTal = (int**) malloc(sizeof(int*) * iAntal) ;
	ppiTal = (int**) realloc((int**) ppiTal, sizeof(int*) * iAntal) ;
	for (iCount = 0 ; iCount < iAntal ; ++iCount)
	{
		ppiTal[iCount] = (int*) malloc(sizeof(int)) ;
		*(ppiTal[iCount]) = iCount % iAntal ;
		printf("The index is %d and the number is %d.\n", iCount, *(ppiTal[iCount])) ;  
	}

	ppiTal = (int**) realloc(ppiTal, sizeof(int*) * (iAntal + 6)) ;

	printf("\n\n\n") ;

	for (iCount = 0 ; iCount < iAntal ; ++iCount)
	{
		printf("The index is %d and the number is %d.\n", iCount, *(ppiTal[iCount])) ; 
	}

	for (iCount = iAntal ; iCount < (iAntal + 6) ; ++iCount)
	{
		ppiTal[iCount] = NULL ;
	}

	for (iCount = iAntal ; iCount < (iAntal + 6) ; ++iCount)
	{
		ppiTal[iCount] = (int*) malloc(sizeof(int)) ;
		*(ppiTal[iCount]) = (iCount % iAntal) * 2  ;
	}

	printf("\n\n\n") ;

	for (iCount = 0 ; iCount < (iAntal + 6) ; ++iCount)
	{
		printf("The index is %d and the number is %d.\n", iCount, *(ppiTal[iCount])) ; 
	}

	for (iCount = 0 ; iCount < iAntal ; ++iCount)
	{
		free(ppiTal[iCount]) ;
	}

	free(ppiTal) ;
	return 0 ;
}
