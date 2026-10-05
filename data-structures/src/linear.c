#include "ds.h"

#include <stdlib.h>

void sqlist_init(SqList *list)
{
    if (list != NULL) {
        list->length = 0;
    }
}

bool sqlist_insert(SqList *list, size_t index, int value)
{
    size_t i;

    if (list == NULL || index > list->length || list->length >= DS_MAX_SIZE) {
        return false;
    }
    /* 从后往前挪动，避免覆盖还未移动的元素。 */
    for (i = list->length; i > index; --i) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    ++list->length;
    return true;
}

bool sqlist_delete(SqList *list, size_t index, int *deleted)
{
    size_t i;

    if (list == NULL || deleted == NULL || index >= list->length) {
        return false;
    }
    *deleted = list->data[index];
    for (i = index; i + 1 < list->length; ++i) {
        list->data[i] = list->data[i + 1];
    }
    --list->length;
    return true;
}

bool sqlist_get(const SqList *list, size_t index, int *value)
{
    if (list == NULL || value == NULL || index >= list->length) {
        return false;
    }
    *value = list->data[index];
    return true;
}

bool linklist_init(LinkList *list)
{
    if (list == NULL) {
        return false;
    }
    list->head = (LinkNode *)malloc(sizeof(*list->head));
    if (list->head == NULL) {
        list->length = 0;
        return false;
    }
    list->head->next = NULL;
    list->length = 0;
    return true;
}

void linklist_destroy(LinkList *list)
{
    LinkNode *node;

    if (list == NULL) {
        return;
    }
    node = list->head;
    while (node != NULL) {
        LinkNode *next = node->next;
        free(node);
        node = next;
    }
    list->head = NULL;
    list->length = 0;
}

bool linklist_insert(LinkList *list, size_t index, int value)
{
    LinkNode *previous;
    LinkNode *node;
    size_t i;

    if (list == NULL || list->head == NULL || index > list->length) {
        return false;
    }
    node = (LinkNode *)malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    previous = list->head;
    for (i = 0; i < index; ++i) {
        previous = previous->next;
    }
    node->data = value;
    node->next = previous->next;
    previous->next = node;
    ++list->length;
    return true;
}

bool linklist_delete(LinkList *list, size_t index, int *deleted)
{
    LinkNode *previous;
    LinkNode *removed;
    size_t i;

    if (list == NULL || list->head == NULL || deleted == NULL ||
        index >= list->length) {
        return false;
    }
    previous = list->head;
    for (i = 0; i < index; ++i) {
        previous = previous->next;
    }
    removed = previous->next;
    *deleted = removed->data;
    previous->next = removed->next;
    free(removed);
    --list->length;
    return true;
}

bool linklist_get(const LinkList *list, size_t index, int *value)
{
    const LinkNode *node;
    size_t i;

    if (list == NULL || list->head == NULL || value == NULL ||
        index >= list->length) {
        return false;
    }
    node = list->head->next;
    for (i = 0; i < index; ++i) {
        node = node->next;
    }
    *value = node->data;
    return true;
}
