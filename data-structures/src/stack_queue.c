#include "ds.h"

#include <stdlib.h>

void seqstack_init(SeqStack *stack)
{
    if (stack != NULL) {
        stack->top = 0;
    }
}

bool seqstack_push(SeqStack *stack, int value)
{
    if (stack == NULL || stack->top >= DS_MAX_SIZE) {
        return false;
    }
    stack->data[stack->top++] = value;
    return true;
}

bool seqstack_pop(SeqStack *stack, int *value)
{
    if (stack == NULL || value == NULL || stack->top == 0) {
        return false;
    }
    *value = stack->data[--stack->top];
    return true;
}

bool seqstack_peek(const SeqStack *stack, int *value)
{
    if (stack == NULL || value == NULL || stack->top == 0) {
        return false;
    }
    *value = stack->data[stack->top - 1];
    return true;
}

void linkstack_init(LinkStack *stack)
{
    if (stack != NULL) {
        stack->top = NULL;
        stack->size = 0;
    }
}

void linkstack_destroy(LinkStack *stack)
{
    LinkNode *node;

    if (stack == NULL) {
        return;
    }
    node = stack->top;
    while (node != NULL) {
        LinkNode *next = node->next;
        free(node);
        node = next;
    }
    stack->top = NULL;
    stack->size = 0;
}

bool linkstack_push(LinkStack *stack, int value)
{
    LinkNode *node;

    if (stack == NULL) {
        return false;
    }
    node = (LinkNode *)malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->data = value;
    node->next = stack->top;
    stack->top = node;
    ++stack->size;
    return true;
}

bool linkstack_pop(LinkStack *stack, int *value)
{
    LinkNode *node;

    if (stack == NULL || value == NULL || stack->top == NULL) {
        return false;
    }
    node = stack->top;
    *value = node->data;
    stack->top = node->next;
    free(node);
    --stack->size;
    return true;
}

void cirqueue_init(CirQueue *queue)
{
    if (queue != NULL) {
        queue->front = 0;
        queue->rear = 0;
    }
}

bool cirqueue_enqueue(CirQueue *queue, int value)
{
    size_t next_rear;

    if (queue == NULL) {
        return false;
    }
    next_rear = (queue->rear + 1) % (DS_MAX_SIZE + 1);
    if (next_rear == queue->front) {
        return false;
    }
    queue->data[queue->rear] = value;
    queue->rear = next_rear;
    return true;
}

bool cirqueue_dequeue(CirQueue *queue, int *value)
{
    if (queue == NULL || value == NULL || cirqueue_is_empty(queue)) {
        return false;
    }
    *value = queue->data[queue->front];
    queue->front = (queue->front + 1) % (DS_MAX_SIZE + 1);
    return true;
}

bool cirqueue_is_empty(const CirQueue *queue)
{
    return queue == NULL || queue->front == queue->rear;
}
