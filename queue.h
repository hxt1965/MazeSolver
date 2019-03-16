#ifndef QUEUE_H
#define QUEUE_H
 
//a pointer to the element
typedef void *eleP;
 
typedef struct node
{
        eleP ele;
        int dist;
        struct node *next;
        struct node *prev;
}node;
 
typedef struct
{
        node *back;
        node *front;
}queue;
 
/*
 * Create an queue.
 */
queue *create(void);
 
/*
 * Destroy an queue.
 * Returns 1 on success, and otherwise 0.
 */
int destroy(queue *q);
 
/*
 * Check if queue is empty.
 * Returns 1 if the queue is empty, and otherwise 0.
 */
int isempty(queue *q);
 
/*
 * Add element to queue.
 * Returns 1 on success, and otherwise 0.
 */
int push(queue *q, eleP elem, int d);
 
/*
 * Remove element from queue, and return it.
 */
eleP pop(queue *q);

/*
 * Returns the first element on the queue
 */
node* top( queue *q);

/*
 * returns the last node on this queue
 */
node* back (queue *q);

eleP popLast ( queue *q );
#endif
