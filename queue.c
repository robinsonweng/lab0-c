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
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    /* Walk through and free linked list & char */
    /* Free should use on things that allocate by malloc */
    while (q->head) {
        free(q->head->value);
        list_ele_t *temp_list = q->head;
        q->head = q->head->next;
        free(temp_list);
    }
    q->size = 0;
    /* Free queue structure */
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
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    int slen = strlen(s);
    newh->value = (char *) malloc(sizeof(char) * slen + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, sizeof(char) * slen + 1);
    if (!q->tail)
        q->tail = newh;
    newh->next = q->head;
    q->head = newh;
    q->size++;
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
    if (!q || *s == '\0')
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    int slen = strlen(s);
    newt->value = (char *) malloc(sizeof(char) * slen + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, sizeof(char) * slen + 1);
    if (!q->tail) {
        q->head = newt;
        q->tail = newt;
        q->size++;
        return true;
    }
    q->tail->next = newt;
    q->tail = newt;
    q->tail->next = NULL;
    q->size++;
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
    if (!q || !q->head || !sp || (bufsize == 0))
        return false;
    strncpy(sp, q->head->value, bufsize - 1);
    *(sp += bufsize - 1) = '\0';
    free(q->head->value);
    list_ele_t *temp = q->head;
    q->head = q->head->next;
    free(temp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head)
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
    if (!q || (q->size <= 1) || !q->head)
        return;
    list_ele_t *previous, *current, *comming;
    previous = NULL, current = q->head, comming = q->head;
    while (current) {
        comming = comming->next;
        current->next = previous;
        previous = current;
        current = comming;
    }
    q->tail = q->head;
    q->head = previous;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 * some perfomance improvement could be done here
 */
list_ele_t *merge_sort(list_ele_t *head, queue_t *q)
{
    /* divide and conquer */
    if (!head || !head->next)
        return head;
    list_ele_t *left = head;
    list_ele_t *right = head->next;
    left->next = NULL;

    left = merge_sort(left, q);
    right = merge_sort(right, q);

    /* sort the node and link them together */
    list_ele_t *temp = NULL;
    while (left || right) {
        if (!left || (right && (strcmp(right->value, left->value) < 0))) {
            if (!temp) {
                head = temp = right;
            } else {
                temp->next = right;
                q->tail = temp = temp->next;
            }
            right = right->next;
        } else {
            if (!temp) {
                head = temp = left;
            } else {
                temp->next = left;
                q->tail = temp = temp->next;
            }
            left = left->next;
        }
    }
    return head;
}

void merge(list_ele_t **head1,
           list_ele_t **tail1,
           list_ele_t **head2,
           list_ele_t **tail2)
{
    list_ele_t *temp = NULL;
    if (strcmp((*head1)->value, (*head2)->value) > 0) {
        temp = *head1;
        *head1 = *head2;
        *head2 = temp;

        temp = *tail1;
        *tail1 = *tail2;
        *tail2 = temp;
    }

    list_ele_t *ahead = *head1, *atail = *tail1;
    list_ele_t *bhead = *head2, *btail = *tail2;
    list_ele_t *btailnext = btail->next;

    while (ahead != atail && bhead != btailnext) {
        if (strcmp(ahead->next->value, bhead->value) > 0) {
            temp = bhead->next;
            bhead->next = ahead->next;
            ahead->next = bhead;
            bhead = temp;
        }
        ahead = ahead->next;
    }
    if (ahead == atail)
        ahead->next = bhead;
    else
        *tail2 = *tail1;
}

void iter_merge_sort(list_ele_t **head, int size)
{
    if (*head == NULL)
        return;
    list_ele_t *head1 = NULL, *tail1 = NULL;
    list_ele_t *head2 = NULL, *tail2 = NULL;
    list_ele_t *prevend = NULL;

    for (int gap = 1; gap < size; gap = gap * 2) {
        head1 = *head;
        while (head1) {
            bool first_iter = false;
            if (head1 == *head)
                first_iter = true;

            int counter = gap;
            tail1 = head1;
            while (--counter && tail1->next) {
                tail1 = tail1->next;
            }

            head2 = tail1->next;
            if (!head2) {
                break;
            }
            counter = gap;
            tail2 = head2;
            while (--counter && tail2->next)
                tail2 = tail2->next;

            list_ele_t *temp = tail2->next;

            merge(&head1, &tail1, &head2, &tail2);

            if (first_iter)
                *head = head1;
            else
                prevend->next = head1;
            prevend = tail2;
            head1 = temp;
        }
        prevend->next = head1;
    }
}

void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;
    iter_merge_sort(&(q->head), q->size);
}
