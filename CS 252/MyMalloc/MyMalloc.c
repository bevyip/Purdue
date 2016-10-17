/*
 * CS252: MyMalloc Project
 *
 * The current implementation gets memory from the OS
 * every time memory is requested and never frees memory.
 *
 * You will implement the allocator as indicated in the handout,
 * as well as the deallocator.
 *
 * You will also need to add the necessary locking mechanisms to
 * support multi-threaded programs.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "MyMalloc.h"

static pthread_mutex_t mutex;

const int arenaSize = 2097152;

void increaseMallocCalls()  { _mallocCalls++; }

void increaseReallocCalls() { _reallocCalls++; }

void increaseCallocCalls()  { _callocCalls++; }

void increaseFreeCalls()    { _freeCalls++; }

extern void atExitHandlerInC()
{
    atExitHandler();
}

/*
 * Initial setup of allocator. First chunk is retrieved from the OS,
 * and the fence posts and freeList are initialized.
 */
void initialize()
{
    // Environment var VERBOSE prints stats at end and turns on debugging
    // Default is on
    _verbose = 1;
    const char *envverbose = getenv("MALLOCVERBOSE");
    if (envverbose && !strcmp(envverbose, "NO")) {
        _verbose = 0;
    }

    pthread_mutex_init(&mutex, NULL);
    void *_mem = getMemoryFromOS(arenaSize);

    // In verbose mode register also printing statistics at exit
    atexit(atExitHandlerInC);

    // establish fence posts
    ObjectHeader * fencePostHead = (ObjectHeader *)_mem;
    fencePostHead->_allocated = 1;
    fencePostHead->_objectSize = 0;

    char *temp = (char *)_mem + arenaSize - sizeof(ObjectHeader);
    ObjectHeader * fencePostFoot = (ObjectHeader *)temp;
    fencePostFoot->_allocated = 1;
    fencePostFoot->_objectSize = 0;
    fencePostFoot->_leftObjectSize = arenaSize - (2*sizeof(ObjectHeader));

    // Set up the sentinel as the start of the freeList
    _freeList = &_freeListSentinel;

    // Initialize the list to point to the _mem
    temp = (char *)_mem + sizeof(ObjectHeader);
    ObjectHeader *currentHeader = (ObjectHeader *)temp;
    currentHeader->_objectSize = arenaSize - (2*sizeof(ObjectHeader)); // ~2MB
    currentHeader->_leftObjectSize = 0;
    currentHeader->_allocated = 0;
    currentHeader->_listNext = _freeList;
    currentHeader->_listPrev = _freeList;
    _freeList->_listNext = currentHeader;
    _freeList->_listPrev = currentHeader;

    // Set the start of the allocated memory
    _memStart = (char *)currentHeader;

    _initialized = 1;
}

void * initializeNew (size_t size) {

	//Get a chunk of 2MB memory from the OS
	void * _mem = getMemoryFromOS(arenaSize);

	//Establish fence posts
	ObjectHeader * fencePostHead = (ObjectHeader *) _mem;
	fencePostHead->_allocated = 1;
	fencePostHead->_objectSize = 0;

	char * temp = (char *) _mem + arenaSize - sizeof(ObjectHeader);
	ObjectHeader * fencePostTail = (ObjectHeader *) temp;
	fencePostTail->_allocated = 1;
	fencePostTail->_objectSize = 0;
    fencePostTail->_leftObjectSize = arenaSize - (2*sizeof(ObjectHeader));

	//Initialize the block by making the first header
	temp = (char *) _mem + sizeof(ObjectHeader);
	ObjectHeader * currentHeader = (ObjectHeader *) temp;
	currentHeader->_objectSize = arenaSize - (2 * sizeof(ObjectHeader));
	currentHeader->_allocated = 0;
	currentHeader->_leftObjectSize = 0;

	//Put this free block in the free list
    currentHeader->_listNext = _freeList->_listNext;
    currentHeader->_listPrev = _freeList;

    _freeList->_listNext->_listPrev = currentHeader;
    _freeList->_listNext = currentHeader;

    //Call the allocate function now that new memory has been initialized
    return allocateObject(size);

}

void * allocateObject(size_t size)
{
    // Make sure that allocator is initialized
    if (!_initialized)
        initialize();

    /* Add the ObjectHeader to the size and round the total size up to a
     * multiple of 8 bytes for alignment.
     */
    size_t roundedSize = (size + sizeof(ObjectHeader) + 7) & ~7;

    //Make a pointer to the free list
    ObjectHeader * p = _freeList->_listNext;

    //Flag for Case 3:
    int flag = 0;

    ObjectHeader * t = _freeList->_listNext;

    //Traverse the free list to find a chunk of the requested or larger size
    while (p != _freeList) {

        flag = 0;

        //Case 1: Where you don't need to break the chunk into two parts
        if (p->_objectSize >= roundedSize && p->_objectSize < roundedSize + sizeof(ObjectHeader) + 8) {

            //You can allocate this chunk without any problems

            //Change the header settings
            p->_allocated = 1;

            //Remove the block from the free list
            p->_listNext->_listPrev = p->_listPrev;
            p->_listPrev->_listNext = p->_listNext;

            //Get out of the loop
            break;

        } //Case 2: Where you do need to break the memory into two parts
        else if (p->_objectSize >= roundedSize + sizeof(ObjectHeader) + 8) {

            //Update the current block's _objectSize
            p->_objectSize -= roundedSize;

            //Get the pointer to the point of split
            char * temp = (char *) p + p->_objectSize;

            //Make a header here
            struct ObjectHeader * newHeader = (ObjectHeader *) temp;

            //Assign values to the header
            newHeader->_objectSize = roundedSize;
            newHeader->_leftObjectSize = p->_objectSize;
            newHeader->_allocated = 1;

            //Update the proceeding header's _leftObjectSize
            temp += newHeader->_objectSize;
            ObjectHeader * rightHeader = (ObjectHeader *) temp;
            rightHeader->_leftObjectSize = newHeader->_objectSize;

            //Set the return value
            p = newHeader;

            //Break out of the loop cause you done!
            break;

        }  //Case 3: Where the memory request cannot be fulfilled by the current free list and so you
           //			 need to get more memory from the OS and then add it to the free list.
	else {

            //Set a flag so that you know if there was no chunk of memory that could satisfy
            // the request.
            flag = 1;

		}

        //Move on to the next chunk of memory in the free list
        p = p->_listNext;
    }

    //Check for case 3 flag
    if (flag == 1)
        return initializeNew(size);

    pthread_mutex_unlock(&mutex);

    // Return a pointer to usable memory
    return (void *) (p + 1);
}

void freeObject(void *ptr)
{

    //Assign a pointer to the start of the free list
    ObjectHeader * p = _freeList->_listNext;

    //Make flags
    int leftFlag = 0;
    int rightFlag = 0;

    //Make a pointer to the currentHeader
    char * temp = (char *) ptr - sizeof(ObjectHeader);
    ObjectHeader * currentHeader = (ObjectHeader *) temp;

    //Go to the left header
    temp -= currentHeader->_leftObjectSize;
    ObjectHeader * leftHeader = (ObjectHeader *) temp;

    //Setting the flag
    if (!leftHeader->_allocated)
        leftFlag = 1;

    //Go to the right header this time
    temp += leftHeader->_objectSize + currentHeader->_objectSize;
    ObjectHeader * rightHeader = (ObjectHeader *) temp;

    //Set the flag again
    if (!rightHeader->_allocated)
        rightFlag = 1;

    //Set the big check condition now

    //Case 1: Coalesce with both sides
    if (leftFlag && rightFlag) {

        //Update left node's object size
        leftHeader->_objectSize += currentHeader->_objectSize + rightHeader->_objectSize;

        //Remove right node from the free list
        while (p->_listNext != rightHeader) {
            p = p->_listNext;
        }

        p->_listNext->_listNext->_listPrev = p;
        p->_listNext = p->_listNext->_listNext;

        //Update the right right node's objectSize
        char * temp = (char *) rightHeader + rightHeader->_objectSize;
        ObjectHeader * rightRightNode = (ObjectHeader *) temp;
        rightRightNode->_leftObjectSize = leftHeader->_objectSize;

    } //Case 2: Coalesce with left
    else if (leftFlag) {

        //Go to the left header and increase its _objectSize
        leftHeader->_objectSize += currentHeader->_objectSize;

        //Go to the right header and update the _leftObjectSize
        rightHeader->_leftObjectSize = leftHeader->_objectSize;

    } //Case 3: Coalesce with right
    else if (rightFlag) {

        //Increase the currentHeader's _objectSize by adding the right header's _objectSize
        currentHeader->_objectSize += rightHeader->_objectSize;

        //Change the pointers to fix the free list
        currentHeader->_listNext = rightHeader->_listNext;
        currentHeader->_listPrev = rightHeader->_listPrev;

        rightHeader->_listPrev->_listNext = currentHeader;
        rightHeader->_listNext->_listPrev = currentHeader;

        //Change the allocated value in the currentHeader
        currentHeader->_allocated = 0;

        //Go to the header right of the rightHeader to update the _leftObjectSize
        char * temp = (char *) rightHeader + rightHeader->_objectSize;
        ObjectHeader * rightRightHeader = (ObjectHeader *) temp;

        //Update the _leftObjectSize
        rightRightHeader->_leftObjectSize = currentHeader->_objectSize;

    } //Case 4: No coalescing
    else {

        //Just add the block to the head of the free list
        currentHeader->_listNext = _freeList->_listNext;
        currentHeader->_listPrev = _freeList;
        _freeList->_listNext = currentHeader;

        //Change the allocated value in the currentHeader
        currentHeader->_allocated = 0;

    }

    return;
}

/*
 * Prints the current state of the heap.
 */
void print()
{
    printf("\n-------------------\n");

    printf("HeapSize:\t%zd bytes\n", _heapSize );
    printf("# mallocs:\t%d\n", _mallocCalls );
    printf("# reallocs:\t%d\n", _reallocCalls );
    printf("# callocs:\t%d\n", _callocCalls );
    printf("# frees:\t%d\n", _freeCalls );

    printf("\n-------------------\n");
}

/*
 * Prints the current state of the freeList
 */
void print_list() {
    printf("FreeList: ");
    if (!_initialized)
        initialize();

    ObjectHeader * ptr = _freeList->_listNext;

    while (ptr != _freeList) {
        long offset = (long)ptr - (long)_memStart;
        printf("[offset:%ld,size:%zd]", offset, ptr->_objectSize);
        ptr = ptr->_listNext;
        if (ptr != NULL)
            printf("->");
    }
    printf("\n");
}

/*
 * This function employs the actual system call, sbrk, that retrieves memory
 * from the OS.
 *
 * @param: the chunk size that is requested from the OS
 * @return: pointer to the beginning of the chunk retrieved from the OS
 */
void * getMemoryFromOS(size_t size)
{
    _heapSize += size;

    // Use sbrk() to get memory from OS
    void *_mem = sbrk(size);

    // if the list hasn't been initialized, initialize memStart to mem
    if (!_initialized)
        _memStart = _mem;

    return _mem;
}

void atExitHandler()
{
    // Print statistics when exit
    if (_verbose)
        print();
}

/*
 * C interface
 */

extern void * malloc(size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseMallocCalls();

    return allocateObject(size);
}

extern void free(void *ptr)
{
    pthread_mutex_lock(&mutex);
    increaseFreeCalls();

    if (ptr == 0) {
        // No object to free
        pthread_mutex_unlock(&mutex);
        return;
    }

    freeObject(ptr);
}

extern void * realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseReallocCalls();

    // Allocate new object
    void *newptr = allocateObject(size);

    // Copy old object only if ptr != 0
    if (ptr != 0) {

        // copy only the minimum number of bytes
        ObjectHeader* hdr = (ObjectHeader *)((char *) ptr - sizeof(ObjectHeader));
        size_t sizeToCopy =  hdr->_objectSize;
        if (sizeToCopy > size)
            sizeToCopy = size;

        memcpy(newptr, ptr, sizeToCopy);

        //Free old object
        freeObject(ptr);
    }

    return newptr;
}

extern void * calloc(size_t nelem, size_t elsize)
{
    pthread_mutex_lock(&mutex);
    increaseCallocCalls();

    // calloc allocates and initializes
    size_t size = nelem *elsize;

    void *ptr = allocateObject(size);

    if (ptr) {
        // No error; initialize chunk with 0s
        memset(ptr, 0, size);
    }

    return ptr;
}
