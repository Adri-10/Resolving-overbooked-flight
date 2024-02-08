#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
typedef struct
{
   void** heapAry;
   int    last;
   int    size;
   int    (*compare) (void* argu1, void* argu2);
   int    maxSize;
} HEAP;

//	Prototype Definitions
HEAP* heapCreate (int maxSize,
            int (*compare) (void* arg1, void* arg2));
bool  heapInsert  (HEAP* heap, void*  dataPtr);
bool  heapDelete  (HEAP* heap, void** dataOutPtr);
int   heapCount   (HEAP* heap);
bool  heapFull    (HEAP* heap);
bool  heapEmpty   (HEAP* heap);
void  heapDestroy (HEAP* heap);

static void _reheapUp   (HEAP* heap, int childLoc);
static void _reheapDown (HEAP* heap, int root);

//  Functions not developed in text
int  heapCount (HEAP* heap);
bool heapFull  (HEAP* heap);
bool heapEmpty (HEAP* heap);


HEAP* heapCreate (int maxSize,
                  int  (*compare) (void* argu1, void* argu2))
{
//	Local Definitions
	HEAP* heap;

//	Statements
	heap = (HEAP*)malloc(sizeof (HEAP));
	if (!heap)
	   return NULL;

	heap->last    = -1;
	heap->compare = compare;

	// Force heap size to power of 2 -1
	heap->maxSize = (int) pow (2, ceil(log2(maxSize))) - 1;
	heap->heapAry = (void**)
	                 calloc(heap->maxSize, sizeof(void*));
	return heap;
}	// createHeap

bool heapInsert (HEAP* heap, void* dataPtr)
{
//	Statements
	if (heap->size == 0)                 // Heap empty
	   {
	    heap->size                = 1;
	    heap->last                = 0;
	    heap->heapAry[heap->last] = dataPtr;
	    return true;
	   } // if
	if (heap->last == heap->maxSize - 1)
	  return false;
	++(heap->last);
	++(heap->size);
	heap->heapAry[heap->last] = dataPtr;
	_reheapUp (heap, heap->last);
	return true;
}	// heapInsert

void  _reheapUp  (HEAP* heap, int childLoc)
{
//	Local Definitions
	int    parent;
	void** heapAry;
	void*  hold;

//	Statements
	// if not at root of heap -- index 0
	if (childLoc)
	   {
	    heapAry = heap->heapAry;
	    parent = (childLoc - 1)/ 2;
	    if (heap->compare(heapAry[childLoc],
	                      heapAry[parent]) > 0)
	        // child is greater than parent -- swap
	        {
	         hold             = heapAry[parent];
	         heapAry[parent]  = heapAry[childLoc];
	         heapAry[childLoc] = hold;
	        _reheapUp (heap, parent);
	       } // if heap[]
	   } // if newNode
	return;
}	// reheapUp

bool heapDelete (HEAP* heap, void** dataOutPtr)
{
//	Statements
	if (heap->size == 0)
	    // heap empty
	    return false;
	*dataOutPtr = heap->heapAry[0];
	heap->heapAry[0]  = heap->heapAry[heap->last];
	(heap->last)--;
	(heap->size)--;
	_reheapDown (heap, 0);
	return true;
}	// heapDelete

void _reheapDown (HEAP* heap, int root)
{
//	Local Definitions
	void* hold;
	void* leftData;
	void* rightData;
	int   largeLoc;
	int   last;

//	Statements
	last = heap->last;
	if ((root * 2 + 1) <= last)         // left subtree
	    // There is at least one child
	   {
	    leftData   = heap->heapAry[root * 2 + 1];
	    if ((root * 2 + 2) <= last)  // right subtree
	       rightData = heap->heapAry[root * 2 + 2];
	    else
	       rightData = NULL;

	    // Determine which child is larger
	    if ((!rightData)
	         || heap->compare (leftData, rightData) > 0)
	       {
	        largeLoc = root * 2 + 1;
	       } // if no right key or leftKey greater
	    else
	       {
	        largeLoc = root * 2 + 2;
	       } // else
	       //summer2020, 2019-1-60-093
	    // Test if root > larger subtree
	    if (heap->compare (heap->heapAry[root],
	        heap->heapAry[largeLoc]) < 0)
	        {
	         // parent < children
	         hold                = heap->heapAry[root];
	         heap->heapAry[root] = heap->heapAry[largeLoc];
	         heap->heapAry[largeLoc] = hold;
	         _reheapDown (heap, largeLoc);
	        } // if root <
	} // if root
	return;
}	// reheapDown



typedef struct data
{
        char name[100];
        int priority;
}DATA;

int compare( void* p1, void* p2  )
{
    int x1,x2;
    x1 = ((DATA*)p1)->priority;
    x2 = ((DATA*)p2)->priority;

    if(x1>x2) return 1;
    else if(x1==x2) return 0;
    else return -1;
}

int main()
{
    char name1[50],name2[50];
    int A,B,C;
    DATA* datap;
    HEAP* heap = heapCreate(100,compare);
    FILE* fp;

    /////////////////////////////////////////////////////////////////////////// build heap
    fp = fopen("207.txt","r");
    while( fscanf( fp,"%s %s %d %d %d", &name1, &name2, &A, &B, &C ) != EOF )
    {
           strcat(name1," ");
           strcat(name1,name2);
           printf("%-20s %-5d %d %2d -->  ",name1,A,B,C);
           datap = (DATA*)malloc( sizeof(DATA) );
           strcpy( datap->name, name1 );
           datap->priority = A/1000+B-C;
           heapInsert(heap, datap);
           printf("%d\n", datap->priority);
    }

    ////////////////////////////////////////////////////////////////////////// heap sort
    int last = heap->last;
    int count = 1;
    printf("\n\n");
    while( last > 0 )
    {
           datap = (DATA*)(heap->heapAry[0]);
           printf("%2d. %-20s    %d\n",count, datap->name, datap->priority);

           last = heap->last;
           heap->heapAry[0] = (DATA*)(heap->heapAry[last]);
           heap->heapAry[last] = datap;
           heap->last--;
           _reheapDown (heap, 0);
           count++;
    }


    getch();
    return 0;
}
