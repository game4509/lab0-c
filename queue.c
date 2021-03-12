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
    if (q == NULL)
        return NULL;
    q->q_size = 0;
    q->head = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;

    while (q->head != NULL) {
        list_ele_t *cur = q->head;
        q->head = q->head->next;
        free(cur->value);
        free(cur);
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
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = NULL;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    if (q->q_size == 0)
        q->tail = newh;
    q->q_size += 1;
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
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    newh->next = NULL;
    strncpy(newh->value, s, strlen(s) + 1);
    newh->value[strlen(s)] = '\0';

    if (q->q_size == 0)
        q->head = newh;
    else
        q->tail->next = newh;
    q->tail = newh;
    q->q_size += 1;

    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
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
    if (q == NULL || q->q_size == 0)
        return false;
    if (sp != NULL) {
        int min_ = strlen(q->head->value) < bufsize - 1 ? strlen(q->head->value)
                                                        : bufsize - 1;
        strncpy(sp, q->head->value, min_);
        sp[min_] = '\0';
    }
    free(q->head->value);
    list_ele_t *newh = q->head;
    q->head = q->head->next;
    q->q_size--;
    free(newh);
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
    return q->q_size;
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
    if (q == NULL || q->q_size == 0 || q->q_size == 1)
        return;
    list_ele_t *cur1 = q->head, *cur2 = q->head->next, *cur3;
    q->tail = cur1;
    cur1->next = NULL;
    while (1) {
        if (cur2->next == NULL) {
            cur2->next = cur1;
            q->head = cur2;
            break;
        }
        cur3 = cur2->next;
        cur2->next = cur1;
        cur1 = cur2;
        cur2 = cur3;
    };
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
static list_ele_t *merge_sort(list_ele_t *head, int indx, int indy)
{
    if (indy - indx >= 1) {
        // printf("1 %d %d\n",indx,indy);
        list_ele_t *back_head = head;
        int block = (indx + indy) / 2;
        for (int i = indx; i <= block; i++) {
            back_head = back_head->next;
        }
        head = merge_sort(head, indx, block);
        back_head = merge_sort(back_head, block + 1, indy);

        // two list situation
        int left_rest = block - indx + 1;
        int right_rest = indy - block;
        list_ele_t *left = head, *right = back_head, *newlist = NULL,
                   *newhead = NULL;
        if (strcmp(left->value, right->value) <= 0) {
            newlist = left;
            newhead = left;
            left = left->next;
            left_rest--;
        } else {  // insert head
            newlist = right;
            newhead = right;
            right = right->next;
            right_rest--;
        }
        while (left_rest > 0 && right_rest > 0) {
            if (strcmp(left->value, right->value) <= 0) {
                newlist->next = left;
                newlist = newlist->next;
                left = left->next;
                left_rest--;
            } else {  // insert head
                list_ele_t *lost = right->next;
                newlist->next = right;
                newlist = newlist->next;
                right = lost;
                right_rest--;
            }
        }
        if (right_rest > 0) {
            newlist->next = right;
        } else {
            newlist->next = left;
        }
        return newhead;
    }
    return head;
}
void q_sort(queue_t *q)
{
    if (q == NULL || q->q_size == 0)
        return;
    q->head = merge_sort(q->head, 0, q->q_size - 1);
    q->tail = q->head;
    for (int i = 0; i < q->q_size - 1; i++) {
        q->tail = q->tail->next;
    }
    q->tail->next = NULL;
    return;
}
