#ifndef __DATA_H__
#define __DATA_H__

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;

#define size2b(x) ((x) << 3)

typedef struct ArrayList {
  void **buf;
  u4 lim;
  u4 size;
} ArrayList;

void array_list_init(ArrayList *, u4);
void _array_list_extend(ArrayList *);
void array_list_push(ArrayList *, void *);
void *array_list_pop(ArrayList *);
void array_list_free(ArrayList *);

#define array_list_get(ARR_LST, INX) ((ARR_LST)->buf[INX])
#define _array_list_full(ARR_LST) ((ARR_LST)->size == (ARR_LST)->lim)
#define array_list_empty(ARR_LST) ((ARR_LST)->size == 0)

typedef ArrayList ArrayStack;

#define array_stack_init array_list_init
#define array_stack_push array_list_push
#define array_stack_pop array_list_pop
#define array_stack_free array_list_free
#define array_stack_get array_list_get
#define array_stack_empty array_list_empty
#define array_stack_top(ARR_STK) array_stack_get(ARR_STK, (ARR_STK)->size - 1)

typedef struct LinkedStub {
  struct LinkedStub *prev;
  struct LinkedStub *next;
} LinkedStub;

typedef struct LinkedList {
  LinkedStub head;
  LinkedStub *tail;
} LinkedList;

void linked_list_init(LinkedList *);
void linked_list_push(LinkedList *, LinkedStub *);
LinkedStub *linked_list_pop(LinkedList *);
void linked_list_add(LinkedList *, LinkedStub *, LinkedStub *);
void linked_list_remove(LinkedList *, LinkedStub *);
void linked_list_free(LinkedList *);

#define linked_list_first(LNK_LST) ((LNK_LST)->head.next)
#define linked_list_last(LNK_LST) ((LNK_LST)->tail)
#define linked_list_empty(LNK_LST) ((LNK_LST)->tail == &(LNK_LST).head)

typedef LinkedList LinkedStack;

#define linked_stack_init linked_list_init
#define linked_stack_push linked_list_push
#define linked_stack_pop linked_list_pop
#define linked_stack_free linked_list_free
#define linked_stack_top linked_list_last
#define linked_stack_empty linked_list_empty

typedef struct BinaryTreeStub {
  struct BinaryTreeStub *left;
  struct BinaryTreeStub *right;
} BinaryTreeStub;

typedef struct BinaryTree {
  BinaryTreeStub *root;
} BinaryTree;

void binary_tree_remove(BinaryTreeStub *, BinaryTreeStub *);
void _binary_stub_free(BinaryTreeStub *);

#define binary_tree_init(BTR) (BTR)->root = NULL
#define binary_tree_free(BTR) _binary_stub_free((BTR)->root)
#define binary_tree_addl(BTR, LEFT) (BTR)->left = LEFT
#define binary_tree_addr(BTR, RIGHT) (BTR)->right = RIGHT

typedef struct KeyPair {
  char *key;
  void *value;
} KeyPair;

typedef u4 (*Hash)(char *);
u4 hash_elf(char *);
/* more hash funtions here... */

typedef struct HashMap {
  KeyPair *pairs;
  Hash hash;
  u4 scale;
  u4 ctr;
} HashMap;

void hash_map_init(HashMap *, Hash, u4);
void _hash_map_extend(HashMap *);
void hash_map_put(HashMap *, char *, void *);
void *hash_map_get(HashMap *, char *);
void hash_map_free(HashMap *);

#define _hash_map_full(HSH_MP) ((HSH_MP)->ctr == (HSH_MP)->scale)
#define _hash_map_empty(HSH_MP) ((HSH_MP)->ctr == 0)

void string_cache_init();
void string_cache_free();

char *new_string(char *);

#endif