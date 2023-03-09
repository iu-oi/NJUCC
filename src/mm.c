#include "mm.h"
#include <stdlib.h>
#include <string.h>

void array_list_init(ArrayList *al, u4 lim) {
  al->buf = (void **)malloc(lim << 3);
  al->lim = lim;
  al->size = 0;
}

void array_list_free(ArrayList *al) {
  while (!array_list_empty(al))
    free(al->buf[--al->size]);
  free(al->buf);
}

void _array_list_extend(ArrayList *al) {
  al->lim <<= 1;
  al->buf = (void **)realloc(al->buf, al->lim << 3);
}

void array_list_push(ArrayList *al, void *val) {
  if (_array_list_full(al))
    _array_list_extend(al);
  al->buf[al->size++] = val;
}

void *array_list_pop(ArrayList *al) { return al->buf[--al->size]; }

void linked_list_init(LinkedList *ll) {
  ll->head.prev = ll->head.next = NULL;
  ll->tail = &ll->head;
}

void linked_list_free(LinkedList *ll) {
  while (ll->tail != &ll->head) {
    ll->tail = ll->tail->prev;
    free(ll->tail->next);
  }
}

void linked_list_push(LinkedList *ll, LinkedStub *val) {
  ll->tail->next = val;
  val->prev = ll->tail;
  ll->tail = val;
}

LinkedStub *linked_list_pop(LinkedList *ll) {
  LinkedStub *val = ll->tail;
  ll->tail = ll->tail->prev;
  ll->tail->next = NULL;
  val->prev = val->next = NULL;
  return val;
}

void linked_list_add(LinkedList *ll, LinkedStub *prev, LinkedStub *val) {
  val->prev = prev;
  if ((val->next = prev->next) != NULL)
    val->next->prev = val;
  prev->next = val;
}

void linked_list_remove(LinkedList *ll, LinkedStub *pos) {
  if ((pos->prev->next = pos->next) != NULL)
    pos->next->prev = pos->prev;
  pos->next = pos->prev = NULL;
}

void _binary_stub_free(BinaryTreeStub *btrh) {
  if (btrh->right)
    _binary_stub_free(btrh->right);
  if (btrh->left)
    _binary_stub_free(btrh->left);
  free(btrh);
}

void binary_tree_remove(BinaryTreeStub *par, BinaryTreeStub *pos) {
  if (par->left == pos)
    par->left = NULL;
  else if (par->right == pos)
    par->right = NULL;
}

u4 hash_elf(char *key) {
  u4 hash = 0, high_4 = 0;

  while (*key != '\0') {
    hash = (hash << 4) + (u1)*key++;
    high_4 = hash & (0xf << 28);
    if (high_4 != 0) {
      hash ^= high_4 >> 24;
      hash &= ~high_4;
    }
  }

  return hash;
}

void hash_set_init(HashSet *hset, Hash hash, u4 size) {
  hset->keys = (char **)malloc(size << 3);
  memset(hset->keys, 0, size << 3);

  hset->hash = hash;
  hset->size = size;
  hset->ctr = 0;
}

void hash_set_free(HashSet *hset) {
  u4 index;
  for (index = 0; index < hset->size && !_hash_set_empty(hset); index++) {
    if (hset->keys[index] != NULL) {
      free(hset->keys[index]);
      hset->ctr--;
    }
  }
  free(hset->keys);
}

void _hash_set_extend(HashSet *hset) {
  HashSet old_set = *hset;
  hash_set_init(hset, hset->hash, hset->size << 1);

  u4 index = 0;
  while (index < old_set.size) {
    char *key = old_set.keys[index++];
    if (key != NULL) {
      hash_set_put(hset, key);
      free(key);
    }
  }
  free(old_set.keys);
}

u4 hash_set_put(HashSet *hset, char *key) {
  if (_hash_set_full(hset))
    _hash_set_extend(hset);
  hset->ctr++;

  u4 index = hset->hash(key) % hset->size;
  u4 off = 0;
  while (off++ < hset->size) {
    if (hset->keys[index] == NULL) {
      hset->keys[index] = (char *)malloc(strlen(key) + 1);
      strcpy(hset->keys[index], key);
      return index;
    }
    index = ++index % hset->size;
  }

  return -1;
}

u4 hash_set_lookup(HashSet *hset, char *key) {
  if (_hash_set_empty(hset))
    return -1;

  u4 index = hset->hash(key) % hset->size;
  u4 off = 0;
  while (off++ < hset->size) {
    if (hset->keys[index] == NULL)
      return -1;
    else if (strcmp(hset->keys[index], key) == 0)
      return index;
    index = ++index % hset->size;
  }

  return -1;
}

HashSet strings;

void string_set_init() { hash_set_init(&strings, hash_elf, LARGE_HSH_TBL); }

void string_set_free() { hash_set_free(&strings); }

char *new_string(char *req) {
  u4 index = hash_set_lookup(&strings, req);

  if (index == -1) {
    char *new_str = (char *)malloc(strlen(req) + 1);
    strcpy(new_str, req);
    hash_set_put(&strings, new_str);
    return new_str;
  }

  return strings.keys[index];
}

void hash_map_init(HashMap *hmap, Hash hash, u4 size) {
  hash_set_init(&hmap->key_set, hash, size);
  hmap->values = (void **)malloc(size << 3);
  memset(hmap->values, 0, size << 3);
}

void hash_map_free(HashMap *hmap) {
  HashSet old_key_set = hmap->key_set;
  hash_set_free(&hmap->key_set);
  old_key_set.keys = (char **)hmap->values;
  hash_set_free(&old_key_set);
}

void _hash_map_extend(HashMap *hmap) {
  HashMap old_map = *hmap;
  hash_map_init(hmap, hmap->key_set.hash, hmap->key_set.size << 1);

  u4 index = 0;
  while (index < old_map.key_set.size) {
    char *key = old_map.key_set.keys[index];
    if (key != NULL) {
      u4 new_index = hash_set_put(&hmap->key_set, key);
      hmap->values[new_index] = old_map.values[index];
      free(key);
    }
  }
  free(old_map.key_set.keys);
  free(old_map.values);
}

void hash_map_put(HashMap *hmap, char *key, void *val) {
  if (_hash_set_full(&hmap->key_set))
    _hash_map_extend(hmap);

  u4 index = hash_set_put(&hmap->key_set, key);
  if (index != -1)
    hmap->values[index] = val;
}

void *hash_map_get(HashMap *hmap, char *key) {
  u4 index = hash_set_lookup(&hmap->key_set, key);
  if (index != -1)
    return hmap->values[index];

  return NULL;
}