#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q_head = malloc(sizeof(struct list_head));
    if (!q_head) {
        return NULL;
    }

    INIT_LIST_HEAD(q_head);
    return q_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *cur, *n;
    list_for_each_entry_safe (cur, n, l, list) {
        list_del(&cur->list);
        q_release_element(cur);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    element_t *node = malloc(sizeof(*node));
    node->value = strdup(s);
    list_add(&node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    element_t *node = malloc(sizeof(*node));
    node->value = strdup(s);
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;
    element_t *node = list_entry(head->next, element_t, list);
    list_del(head->next);
    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;
    element_t *node = list_entry(head->prev, element_t, list);
    list_del(head->prev);
    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *q_head)
{
    if (!q_head)
        return 0;

    int len = 0;
    struct list_head *cur;

    list_for_each (cur, q_head) {
        len++;
    }
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return false;
    struct list_head *a = head->next;
    struct list_head *b = head->prev;

    while ((a != b) && (a->next != b)) {
        a = a->next;
        b = b->prev;
    }

    list_del(a);
    element_t *node = list_entry(a, element_t, list);
    free(node->value);
    free(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;

    LIST_HEAD(trash);
    element_t *it, *safe;
    struct list_head *cut = head;

    list_for_each_entry_safe (it, safe, head, list) {
        if (&safe->list != head && strcmp(safe->value, it->value) == 0)
            continue;

        if (it->list.prev != cut) {
            LIST_HEAD(temp);
            list_cut_position(&temp, cut, &it->list);
            list_splice(&temp, &trash);
        }
        cut = safe->list.prev;
    }

    list_for_each_entry_safe (it, safe, &trash, list)
        q_release_element(it);

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_empty(head))
        return;

    struct list_head *a = head->next;
    struct list_head *b = head->next->next;

    while (a != head && b != head) {
        a->prev->next = b;
        a->next = b->next;
        b->prev = a->prev;
        a->prev = b;
        b->next->prev = a;
        b->next = a;

        a = a->next;
        b = a->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return;

    struct list_head *a = head;
    struct list_head *b = head->next;

    do {
        a->next = a->prev;
        a->prev = b;
        a = b;
        b = a->next;
    } while (a != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || k < 0)
        return;

    struct list_head *cur, *safe, *temp = head;
    LIST_HEAD(h);
    int cnt = 0;

    list_for_each_safe (cur, safe, head) {
        cnt++;
        if (cnt == k) {
            list_cut_position(&h, temp, cur);
            q_reverse(&h);
            list_splice_init(&h, temp);
            cnt = 0;
            temp = safe->prev;
        }
    }
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
