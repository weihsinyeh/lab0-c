#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an new element */
element_t *create_new_element(char *s)
{
    element_t *new_element = (element_t *) malloc(sizeof(element_t));
    if (new_element == NULL || new_element->value == NULL)
        return NULL;
    // strncpy(new_element->value ,s);
    size_t dest_size = strlen(new_element->value);
    if (dest_size > 0) {
        *(new_element->value) = '\0';
        strncat(new_element->value, s, dest_size - 1);
    }
    return new_element;
}

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));

    if (head == NULL)
        return NULL;

    INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
<<<<<<< HEAD
void q_free(struct list_head *head) {}
=======
void q_free(struct list_head *l)
{
    if (l == NULL)
        return;
    element_t *entry = NULL, *safe = NULL;
    list_for_each_entry_safe (entry, safe, l, list) {
        free(entry->value);
        free(entry);
    }
    free(l);
    return;
}
>>>>>>> Implementation of q_new, q_size and q_free

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    // (head->prev)  new_element  head
    element_t *new_element = create_new_element(s);
    // cannot allocate space for new element
    if (new_element == NULL)
        return false;
    // INIT_LIST_HEAD(&(new_element->list));
    list_add(&(new_element->list), head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    // (head->prev)  new_element  head
    if (!head)
        return false;

    element_t *new_element = create_new_element(s);
    if (new_element == NULL)
        return false;

    list_add_tail(&(new_element->list), head->prev);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    // NULL if queue is NULL or empty
    if (head == NULL || list_empty(head))
        return NULL;

    struct list_head *node = head->next;

    // find node's address
    element_t *element = list_entry(node, element_t, list);

    if (sp != NULL) {
        memcpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(node);
    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;

    struct list_head *node = head->prev;
    element_t *element = list_entry(node, element_t, list);
    if (element->value == NULL)
        return NULL;
    if (sp != NULL) {
        memcpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(node);
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL)
        return 0;
    if (list_empty(head))
        return 0;
    long count = 0;
    list_for_each (head->next, head)
        count += 1;

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return false;
    struct list_head *slowptr = head;
    struct list_head *fastptr = head;
    while (fastptr && fastptr->next) {
        slowptr = slowptr->next;
        fastptr = fastptr->next->next;
        if (fastptr == head)
            break;
    }
    list_move(slowptr, head);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    // if asending;
    // insertion sort

    // if descend;
    q_reverse(head);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
