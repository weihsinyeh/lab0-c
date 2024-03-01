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

    if (new_element == NULL)
        return NULL;
    INIT_LIST_HEAD(&(new_element->list));

    if (new_element->value == NULL) {
        free(new_element);
        return NULL;
    }
    new_element->value = strdup(s);
    if (new_element->value == NULL) {
        free(new_element);
        return NULL;
    }
    // INIT_LIST_HEAD(&(new_element->list));
    return new_element;
}

void remove_element(element_t *element, char *sp, size_t bufsize)
{
    if (sp != NULL) {
        memcpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del_init(&(element->list));
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
        list_del(&(entry->list));
        free(entry->value);
        free(entry);
    }
    list_del_init(l);
    free(l);
    return;
}
>>>>>>> Implementation of q_new, q_size and q_free

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;

    element_t *new_element = create_new_element(s);
    if (new_element == NULL)
        return false;

    list_add(&(new_element->list), head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;

    element_t *new_element = create_new_element(s);
    if (new_element == NULL)
        return false;

    // list_add_tail(&(new_element->list), head->prev); //another way
    list_add_tail(&(new_element->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;

    struct list_head *node = head->next;
    element_t *element = list_entry(node, element_t, list);
    remove_element(element, sp, bufsize);

    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;

    struct list_head *node = head->prev;
    element_t *element = list_entry(node, element_t, list);
    remove_element(element, sp, bufsize);

    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return 0;

    long count = 0;
    struct list_head *node;
    list_for_each (node, head)
        count += 1;

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return false;

    struct list_head *posptr = head->next;
    struct list_head *negptr = head->prev;
    while (posptr != negptr && posptr->next != negptr) {
        posptr = posptr->next;
        negptr = negptr->prev;
    }
    element_t *middle = list_entry(posptr, element_t, list);
    list_del_init(posptr);
    free(middle->value);
    free(middle);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || list_empty(head))
        return false;

    element_t *entry, *safe;
    bool hasduplicates = false;

    list_for_each_entry_safe (entry, safe, head, list) {
        if (&(safe->list) != head && strcmp(entry->value, safe->value) == 0) {
            list_del_init(&(entry->list));
            free(entry->value);
            free(entry);
            hasduplicates = true;
        } else if (hasduplicates) {
            list_del_init(&(entry->list));
            free(entry->value);
            free(entry);
            hasduplicates = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    /*struct list_head *node, *safe;

    for (node = (head)->next, safe = node->next;
         node != (head) && safe != (head);
         node = node->next, safe = node->next) {
        node->prev->next = safe;
        safe->next->prev = node;

        safe->prev = node->prev;
        node->next = safe->next;
        safe->next = node;
        node->prev = safe;
    }*/
    struct list_head *first, *second;
    list_for_each_safe (first, second, head) {
        if (second == head)
            break;
        first->prev->next = second;
        second->prev = first->prev;
        first->next = second->next;
        first->prev = second;
        second->next->prev = first;
        second->next = first;

        second = first->next;
    }
    return;
}


/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return;
    // change every two pointer
    struct list_head *node, *tmp;
    list_for_each_safe (node, tmp, head) {
        node->next = node->prev;
        node->prev = tmp;
    }
    node->next = node->prev;
    node->prev = tmp;

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (head == NULL || list_empty(head))
        return;
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    struct list_head *node, *safe;
    if (k <= 1)
        return;

    list_for_each_safe (node, safe, head) {
        int count = 0;
        while (count < k) {
            count += 1;
            safe = safe->next;
            if (safe == head)
                break;
        }
        struct list_head *pre_node = node->prev;
        struct list_head *post_node = safe->next;
        pre_node->next = post_node;
        post_node->prev = pre_node;

        struct list_head *next_head = malloc(sizeof(struct list_head));
        next_head->next = node;
        next_head->prev = safe;
        node->prev = next_head;
        safe->next = next_head;

        q_reverse(next_head);
        list_cut_position(head->next, head->prev, next_head);
        free(next_head);
    }
}

struct list_head *merge(struct list_head *l1, struct list_head *l2)
{
    // merge with recursive
    if (!l2)
        return l1;
    if (!l1)
        return l2;
    if (strcmp(list_entry(l1, element_t, list)->value,
               list_entry(l2, element_t, list)->value) < 0) {
        l1->next = merge(l1->next, l2);
        return l1;
    } else {
        l2->next = merge(l1, l2->next);
        return l2;
    }
}
struct list_head *mergeSortList(struct list_head *head)
{
    // merge sort
    if (!head || !head->next)
        return head;

    struct list_head *fast = head->next;
    struct list_head *slow = head;

    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    struct list_head *l1 = mergeSortList(head);
    struct list_head *l2 = mergeSortList(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head == NULL || list_empty(head))
        return;
    head->prev->next = NULL;

    head->next = mergeSortList(head->next);
    head->next->prev = head;
    struct list_head *iter = head;
    while (iter->next != NULL)
        iter = iter->next;
    iter->next = head;
    head->prev = iter;
    if (descend)
        q_reverse(head);
    return;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return 0;

    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (strcmp(entry->value, safe->value) <= 0) {
            list_del_init(&(safe->list));
            free(safe->value);
            free(safe);
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return 0;

    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        if (strcmp(entry->value, safe->value) >= 0) {
            list_del_init(&(safe->list));
            free(safe->value);
            free(safe);
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (head == NULL || list_empty(head))
        return 0;

    queue_contex_t *headList = list_entry(head, queue_contex_t, chain);
    int listsSize = headList->size;
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (listsSize == 0)
        return 0;

    struct list_head *list = &(headList->chain);
    for (int i = 1; i < listsSize; i++)
        list = merge(list, &list[i]);

    return q_size(list);
}
