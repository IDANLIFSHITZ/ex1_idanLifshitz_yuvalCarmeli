//
// Created by magshimim on 4/18/2023.
//

#include "IsraeliQueue.h"
#include <stdlib.h>

typedef struct Node
{
    void* student;
    void* friends[5];
    void* enemies [3];
    int friendNum;
    int enemyNum;
    struct Node* next;
}*Node;

struct IsraeliQueue_t
{
    Node head;
    Node tail;
    int friend_th;
    int enemy_th;
    FriendshipFunction* friendFunc;
    ComparisonFunction* compFunc;
};


int checkFriend(IsraeliQueue q, void* f1, void* f2);
void addFunc(funcArray funcArr, FriendshipFunction func2Add);
FriendshipFunction* createFriendFuncArray(FriendshipFunction* friendFunc);
ComparisonFunction* createCompFuncArray(ComparisonFunction* friendFunc);

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
    newQueue->friendFunc = createFriendFuncArray(friendFunc);
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

    newQueue->friendFunc = createFriendFuncArray(q->friendFunc);
    newQueue->compFunc = createCompFuncArray(q->compFunc);
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
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friendFunc2Add)
{
    int size = 0;
    for (size = 0; q->friendFunc[size] != NULL; size++) {}
    q->friendFunc = (FriendshipFunction*)realloc(q->friendFunc ,sizeof(FriendshipFunction)*(size+1));
    if (q->friendFunc == NULL) return ISRAELIQUEUE_ALLOC_FAILED;

    q->friendFunc[size] = friendFunc2Add;
    return ISRAELIQUEUE_SUCCESS;
}


/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue, int)
{

}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue, int)
{

}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue)
{

}

/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue)
{

}

/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue, void *)
{

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

int checkFriend(IsraeliQueue q, void* f1, void* f2)
{

}

FriendshipFunction* createFriendFuncArray(FriendshipFunction* friendFunc)
{
    int size = 0;
    for (size = 0; friendFunc[size] != NULL; size++){}

    FriendshipFunction* newFriendFunc = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*size);
    if (newFriendFunc==NULL) return NULL;

    for (int i = 0; i < size; i++)
    {
        newFriendFunc[i] = friendFunc[i];
    }
    return newFriendFunc;
}

ComparisonFunction* createCompFuncArray(ComparisonFunction* friendFunc)
{
    int size = 0;
    for (size = 0; friendFunc[size] != NULL; size++){}

    FriendshipFunction* newFriendFunc = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*size);
    if (newFriendFunc==NULL) return NULL;

    for (int i = 0; i < size; i++)
    {
        newFriendFunc[i] = friendFunc[i];
    }
    return newFriendFunc;
}
