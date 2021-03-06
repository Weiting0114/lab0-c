#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;

    while (q->head) {
        list_ele_t *temp;
        temp = q->head;
        q->head = q->head->next;
        free(temp->value);
        free(temp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    /* Don't forget to allocate space for the string and copy it */
    /* Use error function  "sizeof(strlen(s)=1)"*/
    newh->value = malloc(sizeof(char) * strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    /*Didn't use memset cause
    strncpy will fill /0 in the end of string*/
    // memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s) + 1);

    newh->next = q->head;
    q->head = newh;
    q->size += 1;
    if (q->tail == NULL) {
        q->tail = newh;
    }

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;

    newt->value = malloc(sizeof(char) * strlen(s) + 1);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s) + 1);
    q->size += 1;

    /*If Queue is empty head=tail=newt, else  normaly
    insert tail.*/
    if (q->tail == NULL) {
        q->head = q->tail = newt;
        newt->next = NULL;
    } else {
        q->tail->next = newt;
        q->tail = newt;
        newt->next = NULL;
    }
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL || q->size == 0)
        return false;

    list_ele_t *rm_node = q->head;
    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, rm_node->value, bufsize - 1);
    }

    /*
     *Method as same as free function
     *Free node's value and then free value
     */
    q->head = q->head->next;
    q->size -= 1;
    free(rm_node->value);
    free(rm_node);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size <= 1)
        return;

    list_ele_t *cur, *curnxt;
    q->tail = q->head;
    cur = q->head;
    curnxt = q->head->next;
    while (curnxt != NULL) {
        q->head = curnxt;
        curnxt = q->head->next;
        q->head->next = cur;
        cur = q->head;
    }
    q->tail->next = NULL;
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->size <= 1)
        return;

    q->head = split(q->head);

    while (q->tail) {
        q->tail = q->tail->next;
    }
}

list_ele_t *split(list_ele_t *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    /*Split list*/
    list_ele_t *slow = head, *fast;
    for (fast = head->next; fast && fast->next; fast = fast->next->next) {
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    list_ele_t *left = split(head);
    list_ele_t *right = split(fast);

    // merge sorted left and sorted right
    return merge(left, right);
}

/*Merge two short lists */
list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;

    while (left && right) {
        if (strcmp(left->value, right->value) < 0) {
            *tmp = left;
            left = left->next;
        } else {
            *tmp = right;
            right = right->next;
        }
        tmp = &((*tmp)->next);
    }

    if (left)
        *tmp = left;
    if (right)
        *tmp = right;
    return head;
}