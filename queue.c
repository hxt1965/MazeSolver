#include <stdlib.h>
#include "queue.h"
 
//chnage variable names
queue *create(void)
{
        queue *q = malloc(sizeof(*q));
        if (!q)
                return NULL;
        q->back = NULL;
        q->front = NULL;
        return q;
}
 
int destroy(queue *q)
{
        if (!q)
                return 0;
        while (!isempty(q))
            pop(q);
        free(q);
        return 1;
}
 
int isempty(queue *q)
{
        if (!q)
                return 1;
        if (!q->front)
                return 1;
        else
                return 0;
}
 
int push(queue *q, eleP elem, int d)
{
        node *new = malloc(sizeof(*new));
        if (!new)
                return 0;
        if (!q || !elem)
        {
                free(new);
                return 0;
        }
        new->ele = elem;
        new->dist = d;
        new->next = q->back;
        new->prev = NULL;
        if (q->back)
                q->back->prev = new;
        q->back = new;
        if (!q->front)
                q->front = new;
        return 1;
}
 
eleP pop(queue *q)
{
        node *prev;
        eleP *elem;
        if (isempty(q))
                return NULL;
        prev = q->front->prev;
        elem = q->front->ele;
        if(q->front!=NULL)
            free(q->front);
        q->front = prev;
        return elem;
}

eleP popLast(queue *q)
{
    if(isempty(q))
        return NULL;
    eleP *last;// = malloc(sizeof(node*));
    node *prevN;// = malloc(sizeof(node*));
    last =  q->back->ele;
    prevN = q->back->next;
    free(q->back);
    q->back = prevN;
    return last;
}

node* top( queue *q)
{
    //eleP *elem;
    if(isempty(q))
        return NULL;
    //elem = q->front->ele;
    return q->front;
}

node* back (queue *q )
{
    if(isempty(q))
        return NULL;
    return q->back;
}
