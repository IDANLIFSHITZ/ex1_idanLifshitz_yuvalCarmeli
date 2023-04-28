//
// Created by magshimim on 4/18/2023.
//

#include "IsraeliQueue.h"
#include <stdlib.h>

typedef struct funcArray
{
    int size;
    FriendshipFunction functions[];
}*funcArray;

typedef struct Node
{
    void* student;
    void* friends[5];
    void* enemies [3];
    int num_of_friends;
    int num_of_enemies;
    struct Node* next;
}*Node;

struct IsraeliQueue_t
{
    Node head;
    Node tail;
    int friend_th;
    int enemy_th;
    FriendshipFunction* friendFunc;
    ComparisonFunction compFunc;
};




/**Error clarification:
 * ISRAELIQUEUE_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * ISRAELIQUEUE_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * ISRAELIQUEUE_BAD_PARAM: Indicates an illegal parameter was passed.
 * ISRAELI_QUEUE_ERROR: Indicates any error beyond the above.
 * */

/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/
IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendFunc, ComparisonFunction compFunc, int friend_th, int enemy_th){
    IsraeliQueue newQueue = (IsraeliQueue)malloc(sizeof(newQueue));
    if (newQueue == NULL){
        return NULL;
    }
    newQueue->friendFunc = friendFunc;
    newQueue->compFunc = compFunc;
    newQueue->friend_th = friend_th;
    newQueue->enemy_th = enemy_th;
    newQueue->head = NULL;
    return newQueue;
}

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL,
 * NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue q){
    if (q == NULL) return NULL;
    IsraeliQueue newQueue = (IsraeliQueue)malloc(sizeof(IsraeliQueue));
    if (newQueue == NULL) return NULL;

    newQueue->friendFunc = q->friendFunc;
    newQueue->compFunc = q->compFunc;
    newQueue->friend_th = q->friend_th;
    newQueue->enemy_th = q->enemy_th;
    newQueue->head = NULL;

    Node temp = q->head;
    while (temp != NULL){
        IsraeliQueueEnqueue(newQueue, temp->student);
        temp = temp->next;
    }
    return newQueue;

}

/**@param IsraeliQueue: an IsraeliQueue created by IsraeliQueueCreate
 *
 * Deallocates all memory allocated by IsraeliQueueCreate for the object pointed to by
 * the parameter.*/
void IsraeliQueueDestroy(IsraeliQueue q){
    if (q == NULL) return;
    Node temp = q->head;
    while (temp != NULL){
        Node next = temp->next;
        free(temp);
        temp = next;
    }
    free(q);

}


/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item)
{
    Node newNode = (Node)malloc(sizeof(Node));
    if (newNode==NULL) return NULL;

    newNode->student = item;
    Node bf = NULL;
    Node curr = NULL;
    bool firstEnemy = false;
    while (curr != q->tail)
    {
        if ()
        {

        }

    }



}

/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction func)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;

}


/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int fThreshold)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->friend_th = fThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int eThreshold)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->friend_th = eThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue q)
{
    if (q == NULL) return 0;
    int size = 0;
    Node temp = q->head;
    while (temp != NULL){
        size++;
        temp = temp->next;
    }
    return size;
}
/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue q)
{
    if (q == NULL || q->head == NULL) return NULL;

    void * item = q->head->student; //save the item to return

    q->head = q->head->next; //advance the head

    if(q->head == NULL) q->tail = NULL; //if the queue is empty after advancing the head, set the tail to NULL

    return item;



}

/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue q, void *item)
{
    while (q->head != null){
        if (q->compFunc(q->head->student, item) == 0 ) return true;
        q->head = q->head->next;
    }
}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue)
{

}

/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue*,ComparisonFunction)
{

}
