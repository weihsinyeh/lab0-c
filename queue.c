#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
element_t *create_new_element(char *s)
{
    element_t *new_element = (element_t *) test_malloc(sizeof(element_t));

    if (!new_element)
        return NULL;

    new_element->value = test_malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_element->value) {
        test_free(new_element);
        return NULL;
    }

    strlcpy(new_element->value, s, strlen(s) + 1);

    return new_element;
}

void remove_element(element_t *element, char *sp, size_t bufsize)
{
    if (sp != NULL) {
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
        (struct list_head *) test_malloc(sizeof(struct list_head));

    if (!head || !head->prev)
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
        test_free(entry->value);
        test_free(entry);
    }
    list_del_init(l);
    test_free(l);
    return;
}
>>>>>>> Implementation of q_new, q_size and q_free

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
    test_free(middle->value);
    test_free(middle);
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
            test_free(entry->value);
            free(entry);

            hasduplicates = true;
        } else if (hasduplicates) {
            list_del_init(&(entry->list));
            test_free(entry->value);
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
    if (head == NULL || list_empty(head) || list_is_singular(head))
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
    if ((head == NULL || list_empty(head)) || k <= 1)
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
struct list_head *merge(struct list_head *l1,
                        struct list_head *l2,
                        bool descend)
{
    // merge with recursive
    if (!l2)
        return l1;
    if (!l1)
        return l2;
    if (descend == false) {
        if (strcmp(list_entry(l1, element_t, list)->value,
                   list_entry(l2, element_t, list)->value) < 0) {
            l1->next = merge(l1->next, l2, descend);
            return l1;
        } else {
            l2->next = merge(l1, l2->next, descend);
            return l2;
        }
    } else {
        if (strcmp(list_entry(l1, element_t, list)->value,
                   list_entry(l2, element_t, list)->value) > 0) {
            l1->next = merge(l1->next, l2, descend);
            return l1;
        } else {
            l2->next = merge(l1, l2->next, descend);
            return l2;
        }
    }
}
struct list_head *mergeSortList(struct list_head *head, bool descend)
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
    struct list_head *l1 = mergeSortList(head, descend);
    struct list_head *l2 = mergeSortList(fast, descend);

    // merge sorted l1 and sorted l2
    return merge(l1, l2, descend);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head == NULL || list_empty(head))
        return;
    if (list_is_singular(head))
        return;
    head->prev->next = NULL;

    head->next = mergeSortList(head->next, descend);
    head->next->prev = head;

    struct list_head *iter = head->next;
    struct list_head *pre = head;
    while (iter->next) {
        iter->prev = pre;
        iter = iter->next;
        pre = pre->next;
    }
    iter->prev = pre;
    iter->next = head;
    head->prev = iter;

    return;
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
                test_free(first->value);
                test_free(first);
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
    if ((head == NULL || list_empty(head)) || list_is_singular(head))
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
                test_free(first->value);
                test_free(first);
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
void create_used_head(struct list_head *head, struct list_head *used_head)
{
    struct list_head *node;
    for (node = head->next; node != (head); node = node->next) {
        char *used = "unused";
        q_insert_tail(used_head, used);
    }
}
struct list_head *find_old(struct list_head *head,
                           int old,
                           struct list_head *used_head)
{
    int count = 0;
    struct list_head *node;
    struct list_head *update_iter = used_head->next;
    for (node = head->next; node != (head); node = node->next) {
        char *used = "used";
        list_entry(update_iter, element_t, list)->value = used;
        if (old == count)
            return node;
        count += 1;
    }
    return node;
}

struct list_head *find_new(struct list_head *head, struct list_head *used_head)
{
    struct list_head *node, *new = head;
    char *not_used = "unused";
    for (node = used_head->next; node != (used_head); node = node->prev) {
        new = new->next;
        if (!strcmp(list_entry(node, element_t, list)->value, not_used)) {
            // find a node that not used;
            return new;
        }
    }
    return new;
}
/* Fisher-Yates shuffle Algorithm */
void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    srand(time(NULL));
    /*
    int len = q_size(head);
    LIST_HEAD(used_head);
    create_used_head(head, used_head);
    while (len) {
        int random = rand() % len;

        struct list_head *old = find_old(head, random, used_head);
        struct list_head *new = find_new(head, used_head);
        // exchange the old and new
        struct list_head *old_prev = old->prev;
        struct list_head *old_next = old->next;
        old->prev->next = new;
        old->next->prev = new;

        new->prev->next = old;
        new->next->prev = old;

        old->prev = new->prev;
        old->next = new->next;

        new->prev = old_prev;
        new->next = old_next;

        len -= 1;
    }*/
}