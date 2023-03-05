#include "data.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

void array_list_init(ArrayList *al, u4 lim) {
  al->buf = (void **)malloc(size2b(lim));
  al->lim = lim;
  al->size = 0;
}

void _array_list_extend(ArrayList *al) {
  al->lim <<= 1;
  al->buf = (void **)realloc(al->buf, size2b(al->lim));
}

void array_list_push(ArrayList *al, void *val) {
  if (al->size == al->lim)
    _array_list_extend(al);
  al->buf[al->size++] = val;
}

void *array_list_pop(ArrayList *al) {
  al->size--;
  return al->buf[al->size];
}

void array_list_free(ArrayList *al) {
  while (al->size != 0)
    free(al->buf[--al->size]);
  free(al->buf);
}

void linked_list_init(LinkedList *ll) {
  ll->head.prev = ll->head.next = NULL;
  ll->tail = &ll->head;
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

void linked_list_free(LinkedList *ll) {
  while (ll->tail != &ll->head) {
    ll->tail = ll->tail->prev;
    free(ll->tail->next);
  }
}

void binary_tree_remove(BinaryTreeStub *par, BinaryTreeStub *pos) {
  if (par->left == pos)
    par->left = NULL;
  else if (par->right == pos)
    par->right = NULL;
}

void _binary_stub_free(BinaryTreeStub *btrh) {
  if (btrh->right)
    _binary_stub_free(btrh->right);
  if (btrh->left)
    _binary_stub_free(btrh->left);
  free(btrh);
}

u4 hash_elf(char *key) {
  u1 ch;
  u4 hash = 0, high_4 = 0;

  while ((ch = *key++) != 0) {
    hash = (hash << 4) + ch;
    high_4 = hash & (0xf << 28);
    if (high_4 != 0) {
      hash ^= high_4 >> 24;
      hash &= ~high_4;
    }
  }

  return hash;
}

void hash_map_init(HashMap *hmap, Hash hash, u4 scale) {
  hmap->keys = (char **)malloc(size2b(scale));
  memset(hmap->keys, 0, size2b(scale));

  hmap->values = (void **)malloc(size2b(scale));
  memset(hmap->values, 0, size2b(scale));

  hmap->scale = scale;
  hmap->ctr = 0;
  hmap->hash = hash;
}

void _hash_map_extend(HashMap *hmap) {
  char **old_keys = hmap->keys;
  void **old_values = hmap->values;

  hmap->scale <<= 1;

  hmap->keys = (char **)malloc(size2b(hmap->scale));
  memset(hmap->keys, 0, size2b(hmap->scale));

  hmap->values = (void **)malloc(size2b(hmap->scale));
  memset(hmap->values, 0, size2b(hmap->scale));

  u4 index;
  for (index = 0; index < hmap->scale >> 1; index++) {
    hash_map_put(hmap, hmap->keys[index], hmap->values[index]);
    free(hmap->keys[index]);
  }

  free(old_keys);
  free(old_values);
}

void hash_map_put(HashMap *hmap, char *key, void *val) {
  if (hmap->ctr == hmap->scale)
    _hash_map_extend(hmap);
  hmap->ctr++;

  u4 off = 0;
  u4 hashed = hmap->hash(key) % hmap->scale;

  while (off < hmap->scale) {
    u4 index = (hashed + off++) % hmap->scale;

    if (hmap->keys[index] == NULL) {
      char *key_copy = (char *)malloc(strlen(key) + 1);
      strcpy(key_copy, key);
      hmap->keys[index] = key_copy;
      hmap->values[index] = val;
      break;
    }
  }
}

void *hash_map_get(HashMap *hmap, char *key) {
  u4 off = 0;
  u4 hashed = hmap->hash(key) % hmap->scale;

  while (off < hmap->scale) {
    u4 index = (hashed + off++) % hmap->scale;

    if (hmap->keys[index] == NULL)
      return NULL;
    else if (strcmp(hmap->keys[index], key) == 0)
      return hmap->values[index];
  }

  return NULL;
}


void hash_map_free(HashMap *hmap) {
  u4 index;

  for (index = 0; index < hmap->scale && hmap->ctr > 0; index++) {
    if (hmap->keys[index] != NULL) {
      free(hmap->keys[index]);
      free(hmap->values[index]);
      hmap->ctr--;
    }
  }

  free(hmap->keys);
  free(hmap->values);
}