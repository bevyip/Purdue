//
// CS252: MyMalloc Project
//
// The current implementation gets memory from the OS
// every time memory is requested and never frees memory.
//
// You will implement the allocator as indicated in the handout.
//
// Also you will need to add the necessary locking mechanisms to
// support multi-threaded programs.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include "MyMalloc.h"

static pthread_mutex_t mutex;

const int ArenaSize = 2097152;
const int NumberOfFreeLists = 1;

// Header of an object. Used both when the object is allocated and freed
struct ObjectHeader {
    size_t _objectSize;         // Real size of the object.
    int _allocated;             // 1 = yes, 0 = no 2 = sentinel
    struct ObjectHeader * _next;       // Points to the next object in the freelist (if free).
    struct ObjectHeader * _prev;       // Points to the previous object.
};

struct ObjectFooter {
    size_t _objectSize;
    int _allocated;
};

  //STATE of the allocator

  // Size of the heap
  static size_t _heapSize;

  // initial memory pool
  static void * _memStart;

  // number of chunks request from OS
  static int _numChunks;

  // True if heap has been initialized
  static int _initialized;

  // Verbose mode
  static int _verbose;

  // # malloc calls
  static int _mallocCalls;

  // # free calls
  static int _freeCalls;

  // # realloc calls
  static int _reallocCalls;

  // # realloc calls
  static int _callocCalls;

  // Free list is a sentinel
  static struct ObjectHeader _freeListSentinel; // Sentinel is used to simplify list operations
  static struct ObjectHeader *_freeList;


  //FUNCTIONS

  //Initializes the heap
  void initialize();

  // Allocates an object
  void * allocateObject( size_t size );

  // Frees an object
  void freeObject( void * ptr );

  // Returns the size of an object
  size_t objectSize( void * ptr );

  // At exit handler
  void atExitHandler();

  //Prints the heap size and other information about the allocator
  void print();
  void print_list();

  // Gets memory from the OS
  void * getMemoryFromOS( size_t size );

  void increaseMallocCalls() { _mallocCalls++; }

  void increaseReallocCalls() { _reallocCalls++; }

  void increaseCallocCalls() { _callocCalls++; }

  void increaseFreeCalls() { _freeCalls++; }

extern void
atExitHandlerInC()
{
  atExitHandler();
}

void initialize()
{
  // Environment var VERBOSE prints stats at end and turns on debugging
  // Default is on
  _verbose = 1;
  const char * envverbose = getenv( "MALLOCVERBOSE" );
  if ( envverbose && !strcmp( envverbose, "NO") ) {
    _verbose = 0;
  }

  pthread_mutex_init(&mutex, NULL);
  void * _mem = getMemoryFromOS( ArenaSize + (2*sizeof(struct ObjectHeader)) + (2*sizeof(struct ObjectFooter)) );

  // In verbose mode register also printing statistics at exit
  atexit( atExitHandlerInC );

  //establish fence posts
  struct ObjectFooter * fencepost1 = (struct ObjectFooter *)_mem;
  fencepost1->_allocated = 1;
  fencepost1->_objectSize = 123456789;
  char * temp =
      (char *)_mem + (2*sizeof(struct ObjectFooter)) + sizeof(struct ObjectHeader) + ArenaSize;
  struct ObjectHeader * fencepost2 = (struct ObjectHeader *)temp;
  fencepost2->_allocated = 1;
  fencepost2->_objectSize = 123456789;
  fencepost2->_next = NULL;
  fencepost2->_prev = NULL;

  //initialize the list to point to the _mem
  temp = (char *) _mem + sizeof(struct ObjectFooter);
  struct ObjectHeader * currentHeader = (struct ObjectHeader *) temp;
  temp = (char *)_mem + sizeof(struct ObjectFooter) + sizeof(struct ObjectHeader) + ArenaSize;
  struct ObjectFooter * currentFooter = (struct ObjectFooter *) temp;
  _freeList = &_freeListSentinel;
  currentHeader->_objectSize = ArenaSize + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter); //2MB
  currentHeader->_allocated = 0;
  currentHeader->_next = _freeList;
  currentHeader->_prev = _freeList;
  currentFooter->_allocated = 0;
  currentFooter->_objectSize = currentHeader->_objectSize;
  _freeList->_prev = currentHeader;
  _freeList->_next = currentHeader;
  _freeList->_allocated = 2; // sentinel. no coalescing.
  _freeList->_objectSize = 0;
  _memStart = (char*) currentHeader;
}

void initializeNew() {
    void * _mem = getMemoryFromOS( ArenaSize + (2*sizeof(struct ObjectHeader)) + (2*sizeof(struct ObjectFooter)) );
    //establish fence posts
    struct ObjectFooter * fencepost1 = (struct ObjectFooter *)_mem;
    fencepost1->_allocated = 1;
    fencepost1->_objectSize = 123456789;
    char * temp =
        (char *)_mem + (2*sizeof(struct ObjectFooter)) + sizeof(struct ObjectHeader) + ArenaSize;
    struct ObjectHeader * fencepost2 = (struct ObjectHeader *)temp;
    fencepost2->_allocated = 1;
    fencepost2->_objectSize = 123456789;
    fencepost2->_next = NULL;
    fencepost2->_prev = NULL;

    //initialize the list to point to the _mem
    temp = (char *) _mem + sizeof(struct ObjectFooter);
    struct ObjectHeader * currentHeader = (struct ObjectHeader *) temp;
    temp = (char *)_mem + sizeof(struct ObjectFooter) + sizeof(struct ObjectHeader) + ArenaSize;
    struct ObjectFooter * currentFooter = (struct ObjectFooter *) temp;
    currentHeader->_objectSize = ArenaSize + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter); //2MB
    currentHeader->_allocated = 0;
    currentHeader->_next = _freeList;
    currentFooter->_allocated = 0;
    currentFooter->_objectSize = currentHeader->_objectSize;
    currentHeader->_prev = _freeList->_prev->_next;

    _freeList->_prev->_next = currentHeader;
}

void * allocateObject( size_t size )
{
  //Make sure that allocator is initialized
  if ( !_initialized ) {
    _initialized = 1;
    initialize();
  }

  // Add the ObjectHeader/Footer to the size and round the total size up to a multiple of
  // 8 bytes for alignment.
  size_t roundedSize = (size + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter) + 7) & ~7;

    //Make a pointer to the freelist
    struct ObjectHeader * p = _freeList->_next;

    char * temp;

    struct ObjectHeader * oldHeader;
    struct ObjectHeader * removeHeader;

    int flag = 0;

    while (p != _freeList) {
        flag = 0;
        //Case 1: When we don't need to break the chunk into two parts
        if (p->_objectSize >= roundedSize && p->_objectSize < roundedSize + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter) + 8) {
            //You found the right sized
            temp = (char *) p;
            removeHeader = p;

            //Set the head's allocated = 1 and remove it from the FreeList
            removeHeader->_allocated = 1;

            //Removing the header from the FreeList
            removeHeader->_prev->_next = removeHeader->_next;
            removeHeader->_next->_prev = removeHeader->_prev;

            //Going to make changes to the Footer
            temp = temp + removeHeader->_objectSize - sizeof(struct ObjectFooter);
            struct ObjectFooter * removeFooter = (struct ObjectFooter *) temp;

            //Change the variable in Footer
            removeFooter->_allocated = 1;
            removeFooter->_objectSize = removeHeader->_objectSize;

            //Change the header variable to what you want to return
            oldHeader = removeHeader;

            //Break out of the loop because you're done
            break;

        } //Case 2: When you can break the memory chunk into two parts
        else if (p->_objectSize >= roundedSize + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter) + 8) {

            temp = (char *) p;
            oldHeader = p;

            //Create a new Footer at the end of the requested size
            temp = temp + roundedSize - sizeof(struct ObjectFooter);
            struct ObjectFooter * oldFooter = (struct ObjectFooter *) temp;

            //Allocate the variables in the Old Footer
            oldFooter->_allocated = 1;
            oldFooter->_objectSize = roundedSize;

            //Create a new Header
            temp = temp + sizeof(struct ObjectFooter);
            struct ObjectHeader * newHeader = (struct ObjectHeader *) temp;

            //Allocate the variables in the new header
            newHeader->_allocated = 0;
            newHeader->_objectSize = oldHeader->_objectSize - roundedSize;
            newHeader->_next = oldHeader->_next;
            newHeader->_prev = oldHeader->_prev;

            //Double linked list operations (adding the new broken block to the list and removing the old block)
            oldHeader->_prev->_next = newHeader;
            oldHeader->_next->_prev = newHeader;
            oldHeader->_objectSize = roundedSize;

            //Change the variables in Old Header
            oldHeader->_allocated = 1;
            oldHeader->_next = newHeader;


            //Allocate the variables in New Footer
            temp = temp + newHeader->_objectSize - sizeof(struct ObjectFooter);
            struct ObjectFooter * newFooter = (struct ObjectFooter *) temp;
            newFooter->_allocated = 0;
            newFooter->_objectSize = newHeader->_objectSize;

            //Break out of the loop because the job is done
            break;
        } //Case 3: All the memory chunk in the initial 2MB block is full/or not enough memory remaining to return for the requested size.
        else {
            flag = 1;

        }
        //Move on to the next node in the FreeList
        p = p->_next;
    }

    if (flag == 1) {
        initializeNew();
        allocateObject(size);
    }
  // Store the size in the header
  //struct ObjectHeader * o = (struct ObjectHeader *) _mem;

  //o->_objectSize = roundedSize;

  pthread_mutex_unlock(&mutex);

  // Return a pointer to usable memory
  return (void *) (oldHeader + 1);
}

void freeObject( void * ptr )
{

	// Add your code here
	//Trying to write freeeeeeeeeeeeee

	//Warnings: Ptr object given just below the header of the object, so you have to traverse up by sizeof(struct ObjectHeader) to gain access to know the upper and lower node

	//Case 1: Coalese with the upper node
	//Case 2: Coalese with the lower node
	//Case 3: Coalese with both nodes

	//Check the Upper and Lower nodes and check the appropriate flags.

	int flagFooter, flagHeader = 0;

	//Make a temp pointer
	char * temp = (char *) ptr;

	//Go to the header of the given pointer
	temp = temp - sizeof(struct ObjectHeader);

	struct ObjectHeader * checkHeader = (struct ObjectHeader *) temp;

    printf("checkHeader->_allocated = %d\n", checkHeader->_allocated);
    printf("checkHeader->_objectSize = %d\n", (int) checkHeader->_objectSize);

	//Check the upper footer
	temp = temp - sizeof(struct ObjectFooter);

	struct ObjectFooter * checkFooter = (struct ObjectFooter *) temp;
    //printf("YOLO\n");

    printf("checkFooter->_allocated = %d\n", checkFooter->_allocated);
    printf("checkFooter->_objectSize = %d\n", (int) checkFooter->_objectSize);

	if (checkFooter->_allocated == 1) {
        flagFooter = 1;
        printf("YOLO\n");
    }
    //printf("YOLO\n");
	//Check the lower header
	temp = temp + sizeof(struct ObjectFooter) + checkHeader->_objectSize;


	checkHeader = (struct ObjectHeader *) temp;

    printf("checkHeader->_objectSize = %d\n", (int) checkHeader->_objectSize);
    printf("checkHeader->_allocated = %d\n", checkHeader->_allocated);

    if (checkHeader->_allocated == 1) {
        printf("YOLO1\n");
		flagHeader = 1;
    }

    temp = temp + checkHeader->_objectSize - sizeof(struct ObjectFooter);
    checkFooter = (struct ObjectFooter *) temp;

    printf("checkFooter->_allocated = %d\n", checkFooter->_allocated);
    printf("checkFooter->_objectSize = %d\n", (int) checkFooter->_objectSize);

	//Start the entire procedure:
	if (flagHeader && flagFooter) {
		//Coalese both the nodes with the nodes which needs to be freed
        //raise(SIGSEGV);
	} else if (flagHeader) {
		//Coalese with the lower node
        //raise(SIGSEGV);
	} else if (flagFooter) {
		//Coalese with th higher node
        //raise(SIGSEGV);
	} else {
		//Just remove the requested node without any coalesing
        //raise(SIGSEGV);
	}

	return;

}

size_t objectSize( void * ptr )
{
  // Return the size of the object pointed by ptr. We assume that ptr is a valid obejct.
  struct ObjectHeader * o =
    (struct ObjectHeader *) ( (char *) ptr - sizeof(struct ObjectHeader) );

  // Substract the size of the header
  return o->_objectSize;
}

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

void print_list()
{
  printf("FreeList: ");
  if ( !_initialized ) {
    _initialized = 1;
    initialize();
  }
  struct ObjectHeader * ptr = _freeList->_next;
  while(ptr != _freeList){
      long offset = (long)ptr - (long)_memStart;

      //Check this immediately
      printf("[offset:%ld,size:%zd]",offset,ptr->_objectSize);
      ptr = ptr->_next;
      if(ptr != NULL){
          printf("->");
      }
  }
  printf("\n");
}

void * getMemoryFromOS( size_t size )
{
  // Use sbrk() to get memory from OS
  _heapSize += size;

  void * _mem = sbrk( size );

  if(!_initialized){
      _memStart = _mem;
  }

  _numChunks++;

  return _mem;
}

void atExitHandler()
{
  // Print statistics when exit
  if ( _verbose ) {
    print();
  }
}

//
// C interface
//

extern void *
malloc(size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseMallocCalls();

  return allocateObject( size );
}

extern void
free(void *ptr)
{
  pthread_mutex_lock(&mutex);
  increaseFreeCalls();

  if ( ptr == 0 ) {
    // No object to free
    pthread_mutex_unlock(&mutex);
    return;
  }

  freeObject( ptr );
}

extern void *
realloc(void *ptr, size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseReallocCalls();

  // Allocate new object
  void * newptr = allocateObject( size );

  // Copy old object only if ptr != 0
  if ( ptr != 0 ) {

    // copy only the minimum number of bytes
    size_t sizeToCopy =  objectSize( ptr );
    if ( sizeToCopy > size ) {
      sizeToCopy = size;
    }

    memcpy( newptr, ptr, sizeToCopy );

    //Free old object
    freeObject( ptr );
  }

  return newptr;
}

extern void *
calloc(size_t nelem, size_t elsize)
{
  pthread_mutex_lock(&mutex);
  increaseCallocCalls();

  // calloc allocates and initializes
  size_t size = nelem * elsize;

  void * ptr = allocateObject( size );

  if ( ptr ) {
    // No error
    // Initialize chunk with 0s
    memset( ptr, 0, size );
  }

  return ptr;
}
