
struct SLEntry {
  int value;
  struct SLEntry * next;
};

typedef struct SLEntry SLEntry;

struct SLList {
  SLEntry * head;
};

typedef struct SLList SLList;

void sllist_init(SLList * list);
void sllist_add_end( SLList *list, int value );
int sllist_remove(SLList *list, int value);
void sllist_remove_interval(SLList *list, int min, int max);
SLList * sllist_intersection(SLList *a, SLList *b);
void sllist_print(SLList *list);

