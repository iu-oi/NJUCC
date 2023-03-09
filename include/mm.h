#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;

typedef struct ArrayList {
  void **buf;
  u4 lim;
  u4 size;
} ArrayList;

#define SHORT_ARR_LST 8
#define LONG_ARR_LST 128

void array_list_init(ArrayList *al, u4 lim);
void array_list_free(ArrayList *al);
void _array_list_extend(ArrayList *al);
#define _array_list_full(AL) ((AL)->size == (AL)->lim)
#define array_list_empty(AL) ((AL)->size == 0)
#define array_list_get(AL, IDX) ((AL)->buf[IDX])
void array_list_push(ArrayList *al, void *val);
void *array_list_pop(ArrayList *al);

typedef ArrayList ArrayStack;

#define array_stack_init array_list_init
#define array_stack_free array_list_free
#define array_stack_empty array_list_empty
#define array_stack_get array_list_get
#define array_stack_push array_list_push
#define array_stack_pop array_list_pop
#define array_stack_top(AS) array_list_get(AS, (AS)->size - 1)

typedef struct LinkedStub {
  struct LinkedStub *prev;
  struct LinkedStub *next;
} LinkedStub;

typedef struct LinkedList {
  LinkedStub head;
  LinkedStub *tail;
} LinkedList;

void linked_list_init(LinkedList *ll);
void linked_list_free(LinkedList *ll);
#define linked_list_empty(LL) ((LL)->tail == &(LL)->head)
void linked_list_push(LinkedList *ll, LinkedStub *val);
LinkedStub *linked_list_pop(LinkedList *ll);
void linked_list_add(LinkedList *ll, LinkedStub *prev, LinkedStub *val);
void linked_list_remove(LinkedList *ll, LinkedStub *pos);

typedef LinkedList LinkedStack;

#define linked_stack_init linked_list_init
#define linked_stack_free linked_list_free
#define linked_stack_empty linked_list_empty
#define linked_stack_push linked_list_push
#define linked_stack_pop linked_list_pop
#define linked_stack_top(LS) ((LS)->tail)

typedef struct BinaryTreeStub {
  struct BinaryTreeStub *left;
  struct BinaryTreeStub *right;
} BinaryTreeStub;

typedef struct BinaryTree {
  BinaryTreeStub *root;
} BinaryTree;

void _binary_stub_free(BinaryTreeStub *btrh);
#define binary_tree_init(BTR) (BTR)->root = NULL
#define binary_tree_free(BTR) _binary_stub_free((BTR)->root)
void binary_tree_remove(BinaryTreeStub *par, BinaryTreeStub *pos);

typedef u4 (*Hash)(char *);

u4 hash_elf(char *key);
/* more hash funtions here... */

#define SMALL_HSH_TBL 512
#define LARGE_HSH_TBL 4096

typedef struct HashSet {
  char **keys;
  Hash hash;
  u4 size;
  u4 ctr;
} HashSet;

void hash_set_init(HashSet *hset, Hash hash, u4 size);
void hash_set_free(HashSet *hset);
#define _hash_set_full(HSET) ((HSET)->ctr == (HSET)->size)
#define _hash_set_empty(HSET) ((HSET)->ctr == 0)
void _hash_set_extend(HashSet *hset);
u4 hash_set_put(HashSet *hset, char *key);
u4 hash_set_lookup(HashSet *hset, char *key);

void string_set_init();
void string_set_free();
char *new_string(char *text);

typedef struct HashMap {
  HashSet key_set;
  void **values;
} HashMap;

void hash_map_init(HashMap *hmap, Hash hash, u4 size);
void hash_map_free(HashMap *hmap);
void _hash_map_extend(HashMap *hmap);
void hash_map_put(HashMap *hmap, char *key, void *val);
void *hash_map_get(HashMap *hmap, char *key);

#endif