#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"
#include "time.h"
#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif
/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */
/* Create an new element */

/* Create an new element */
element_t *create_new_element(char *s)
{
    element_t *new_element = (element_t *) malloc(sizeof(element_t));

    if (!new_element)
        return NULL;

    new_element->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_element->value) {
        free(new_element);
        return NULL;
    }

    strlcpy(new_element->value, s, strlen(s) + 1);

    return new_element;
}

void remove_element(element_t *element, char *sp, size_t bufsize)
{
    if (sp) {
        size_t len;
        if (strlen(element->value) < bufsize - 1)
            len = strlen(element->value);
        else
            len = bufsize - 1;
        memcpy(sp, element->value, len);
        sp[len] = '\0';
    }
    list_del_init(&(element->list));
}


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));

    if (!head || !head->prev)
        return NULL;

    INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
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

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new_element = create_new_element(s);

    if (!new_element)
        return false;


    list_add(&(new_element->list), head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new_element = create_new_element(s);
    if (!new_element)
        return false;

    // list_add_tail(&(new_element->list), head->prev); //another way
    list_add_tail(&(new_element->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->next;
    element_t *element = list_entry(node, element_t, list);
    remove_element(element, sp, bufsize);

    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->prev;
    element_t *element = list_entry(node, element_t, list);
    remove_element(element, sp, bufsize);

    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
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
    if (!head || list_empty(head))
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
    if (!head || list_empty(head))
        return false;

    if (list_is_singular(head))
        return true;

    element_t *entry = list_entry((head)->next, element_t, list);
    element_t *safe = list_entry((entry)->list.next, element_t, list);

    bool hasduplicates = false;

    while (&entry->list != (head)) {
        if (&safe->list != (head) && !strcmp(entry->value, safe->value)) {
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
        entry = safe;
        safe = list_entry(safe->list.next, element_t, list);
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

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
    if (!head || list_empty(head))
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
    if ((!head || list_empty(head)) || k <= 1)
        return;
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

    int times = q_size(head);
    struct list_head *tail;

    LIST_HEAD(tmp);
    LIST_HEAD(new_head);

    for (int i = 0; i < times; i += k) {
        int j = 0;
        list_for_each (tail, head) {
            if (j == k)
                break;
            j++;
        }
        list_cut_position(&tmp, head, tail->prev);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, &new_head);
    }
    list_splice_init(&new_head, head);
}

void mergeTwoLists(struct list_head *L1, struct list_head *L2, bool descend)
{
    if (!L1 || !L2)
        return;
    struct list_head head;
    INIT_LIST_HEAD(&head);
    while (!list_empty(L1) && !list_empty(L2)) {
        element_t *e1 = list_first_entry(L1, element_t, list);
        element_t *e2 = list_first_entry(L2, element_t, list);
        struct list_head *node = (descend ^ (strcmp(e1->value, e2->value) < 0))
                                     ? L1->next
                                     : L2->next;
        list_move_tail(node, &head);
    }
    list_splice_tail_init(list_empty(L1) ? L2 : L1, &head);
    list_splice(&head, L1);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *slow = head;
    struct list_head *fast = head->next;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(&left, descend);
    q_sort(head, descend);
    mergeTwoLists(head, &left, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    while (1) {
        bool nothandle = true;
        struct list_head *entry = head->next, *safe = entry->next;
        while (entry != head && safe != head) {
            element_t *first = list_entry(entry, element_t, list);
            element_t *second = list_entry(safe, element_t, list);
            if (strcmp(first->value, second->value) >= 0) {
                list_del_init(&(first->list));
                free(first->value);
                free(first);
                nothandle = false;
            }
            entry = safe;
            safe = entry->next;
        }
        if (nothandle == true)
            break;
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if ((!head || list_empty(head)) || list_is_singular(head))
        return 0;

    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    while (1) {
        bool nothandle = true;
        struct list_head *entry = head->next, *safe = entry->next;
        while (entry != head && safe != head) {
            element_t *first = list_entry(entry, element_t, list);
            element_t *second = list_entry(safe, element_t, list);
            if (strcmp(first->value, second->value) <= 0) {
                list_del_init(&(first->list));
                free(first->value);
                free(first);
                nothandle = false;
            }
            entry = safe;
            safe = entry->next;
        }
        if (nothandle == true)
            break;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int __merge(struct list_head *l1, struct list_head *l2)
{
    if (!l1 || !l2)
        return 0;
    LIST_HEAD(tmp_head);
    while (!list_empty(l1) && !list_empty(l2)) {
        element_t *ele_1 = list_first_entry(l1, element_t, list);
        element_t *ele_2 = list_first_entry(l2, element_t, list);
        element_t *ele_min =
            strcmp(ele_1->value, ele_2->value) < 0 ? ele_1 : ele_2;
        list_move_tail(&ele_min->list, &tmp_head);
    }
    list_splice_tail_init(l1, &tmp_head);
    list_splice_tail_init(l2, &tmp_head);
    list_splice(&tmp_head, l1);
    return q_size(l1);
}
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;
    else if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);
    int size = q_size(head);
    int count = (size % 2) ? size / 2 + 1 : size / 2;
    int queue_size = 0;
    for (int i = 0; i < count; i++) {
        queue_contex_t *first = list_first_entry(head, queue_contex_t, chain);
        queue_contex_t *second =
            list_entry(first->chain.next, queue_contex_t, chain);
        while (!list_empty(first->q) && !list_empty(second->q)) {
            queue_size = __merge(first->q, second->q);
            list_move_tail(&second->chain, head);
            first = list_entry(first->chain.next, queue_contex_t, chain);
            second = list_entry(first->chain.next, queue_contex_t, chain);
        }
    }
    return queue_size;
}
static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void list_swap(struct list_head *entry1, struct list_head *entry2)
{
    struct list_head *pos = entry2->prev;

    list_del(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1)
        pos = entry2;
    list_add(entry1, pos);
}
/* Fisher-Yates shuffle Algorithm */
void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    int len = q_size(head);
    struct list_head *tail = head->prev;
    struct list_head *safe = tail->prev;

    for (; len != 0; safe = safe->prev) {
        int random = rand() % (len--);
        struct list_head *iter = head;
        do {
            iter = iter->next;
        } while (random--);
        if (iter == tail)
            continue;
        list_swap(iter, tail);
        tail = safe;
    }
    return;
}