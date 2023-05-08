//
// Created by magshimim on 4/18/2023.
//

#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



enum {STRANGER, FRIEND, ENEMY};

typedef struct Node_t
{
    void* student;
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
    ComparisonFunction compFunc;
};


int checkFriend(IsraeliQueue q, void* f1, void* f2);
FriendshipFunction* createFriendFuncArray(FriendshipFunction* friendFunc);
int getFriendshipThresholdForMerged(IsraeliQueue* , int);
int getEnemyThresholdForMerged(IsraeliQueue* , int);

bool checkInArr(Node* arr, int size, Node Node2check);
Node createNewNode(void* student, int friendNum, int enemyNum);
void enqueueNode(IsraeliQueue q, Node node2Add);


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
    if (newQueue->friendFunc == NULL){
        free(newQueue);
        return NULL;
    }
    newQueue->compFunc = compFunc;
    newQueue->friend_th = friend_th;
    newQueue->enemy_th = enemy_th;
    newQueue->head = NULL;
    newQueue->tail = NULL;
    return newQueue;
}

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL,
 * NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue q) {
    if (q == NULL) return NULL;
    IsraeliQueue newQueue = (IsraeliQueue) malloc(sizeof(IsraeliQueue));
    if (newQueue == NULL) return NULL;

    newQueue->friendFunc = createFriendFuncArray(q->friendFunc);
    if (newQueue->friendFunc == NULL)
    {
        IsraeliQueueDestroy(newQueue);
        return NULL;
    }
    newQueue->compFunc = q->compFunc;
    newQueue->friend_th = q->friend_th;
    newQueue->enemy_th = q->enemy_th;

    newQueue->head = createNewNode(q->head->student, q->head->friendNum, q->head->enemyNum); // create new Head.
    if (newQueue->head == NULL)
    {
        IsraeliQueueDestroy(newQueue);
        return NULL;
    }
    Node currNode = newQueue->head;
    for (Node temp = q->head->next; temp != NULL; temp = temp->next) //for Nodes in queue
    {
        Node newNode = createNewNode(temp->student, temp->friendNum, temp->enemyNum); // creates new Node and returns it.
        if (newNode == NULL)
        {
            IsraeliQueueDestroy(newQueue);
            return NULL;
        }

        currNode->next = newNode;
        currNode = newNode;
    }
    newQueue->tail = currNode;
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
    free(q->friendFunc);
    free(q);

}


/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void * item)
{
    Node newNode = createNewNode(item, 0, 0); // creates new Node and returns it.
    if (newNode==NULL) return ISRAELIQUEUE_ALLOC_FAILED; // if allocation failed.

    enqueueNode(q, newNode);
    return ISRAELIQUEUE_SUCCESS;
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
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int FriendshipThreshold)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->friend_th = FriendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int EnemyThreshold)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->enemy_th = EnemyThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue q){
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
void* IsraeliQueueDequeue(IsraeliQueue q){
    if (q == NULL || q->head == NULL) return NULL;

    Node temp = q->head;
    void* item = temp->student;

    q->head = temp->next;
    free(temp);
    return item;
}

/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue q, void * item)
{
    if (q == NULL) return false;
    Node temp = q->head;
    while (temp != NULL){
        if (q->compFunc(temp->student, item) == 0) return true;
        temp = temp->next;
    }
    return false;
}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    int size = IsraeliQueueSize(q);
    Node* NodesArr = (Node*)malloc(sizeof(Node)*size);
    if (NodesArr==NULL) return ISRAELIQUEUE_ALLOC_FAILED;

    Node currNode = q->head;
    for(int i = 0; i < size; i++, currNode = currNode->next) // fills the array with the Nodes in their current positions.
    {
        NodesArr[i] = currNode;
    }

    for (int i = 0; i < size; i++)
    {
        Node Node2Advance = NodesArr[i];
        if (Node2Advance == q->head) //if current Node is the head.
        {
            Node temp = q->head;
            q->head = q->head->next;
            enqueueNode(q, temp); // re-enqueue the head.
        }
        else
        {
            for (currNode = q->head; currNode->next != Node2Advance; currNode = currNode->next) {} //finds the Node before the current Node.
            Node temp = currNode->next;
            currNode->next = temp->next;
            enqueueNode(q, temp);
        }
    }
}

/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* q ,ComparisonFunction compFunc) {
    //gets size of array.
    if (q == NULL) return NULL;
    int size = 0;
    for (size = 0; q[size] != NULL; size++) {}

    //creates friendship function array for merged queue.
    FriendshipFunction *mergedFunctions = createFriendFuncArray(q[0]->friendFunc);
    if (mergedFunctions == NULL) return NULL;

    //creates merged queue.
    IsraeliQueue mergedQueue = IsraeliQueueCreate(mergedFunctions, compFunc,
                                                  getFriendshipThresholdForMerged(q, size),
                                                  getEnemyThresholdForMerged(q, size));
    if (mergedQueue == NULL){
        free(mergedFunctions);
        return NULL;
    }

    //adds friendship functions to merged queue.
    int j = 0;
    for (int i = 1; i < size; i++) {
        while (q[i]->friendFunc[j] != NULL) {
            IsraeliQueueAddFriendshipMeasure(mergedQueue,q[i]->friendFunc[j]);
            j++;
        }
    }

    //merge queues.
    bool notEmpty = true;
    void* temp = NULL;
    while(notEmpty){
        notEmpty = false;
        for (int i = 0; i < size; i++) {
            if (q[i]->head != NULL) {
                temp = IsraeliQueueDequeue(q[i]);
                IsraeliQueueEnqueue(mergedQueue, temp);
                notEmpty = true;
            }
        }
    }
    return mergedQueue;


}

int checkFriend(IsraeliQueue q, void* f1, void* f2)
{
    int sum = 0, sumCount = 0, curr = 0;
    for (int i = 0; q->friendFunc[i] != NULL; i++) //for friend function in queue.
    {
        curr = q->friendFunc[i](f1, f2);
        if (curr > q->friend_th) return FRIEND; // if friend.
        sum += curr;
        sumCount++;
    }
    if (sum/sumCount < q->enemy_th) return ENEMY; //if enemy.
    return STRANGER; //else stranger.
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

int getFriendshipThresholdForMerged(IsraeliQueue* q, int size){
    int friend_th = 0;
    for (int i = 0; i < size; i++) {
        friend_th += q[i]->friend_th;
    }
    return friend_th / size;
}

int getEnemyThresholdForMerged(IsraeliQueue* q, int size){
    int enemy_th, root = 0;
    for (int i = 0; i < size; i++) {
        enemy_th *= abs(q[i]->friend_th);
    }
    root = 1/size;
    return pow(enemy_th, root);
}
 FriendshipFunction* createFriendFuncForMerged(IsraeliQueue* q, int size) {

 }

bool checkInArr(Node* arr, int size, Node Node2check)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == Node2check) return true;
    }
    return false;
}

Node createNewNode(void* student, int friendNum, int enemyNum)
{
    Node newNode = (Node) malloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    newNode->student = student;
    newNode->friendNum = friendNum;
    newNode->enemyNum = enemyNum;
    newNode->next = NULL;
    return newNode;
}

void enqueueNode(IsraeliQueue q, Node node2Add)
{
    Node bestFriend = NULL;
    int relation = 0;
    bool firstEnemy = true;

    for (Node currNode = q->head; currNode != q->tail; currNode = currNode->next) // for not reached tail of back of queue.
    {
        relation = checkFriend(q, node2Add, currNode); //gets current node relation to new Node.
        if (relation == FRIEND && bestFriend == NULL && currNode->friendNum < FRIEND_QUOTA) bestFriend = currNode; // if friends and closest friend to front of the line.
        else if (relation == ENEMY && currNode->enemyNum < RIVAL_QUOTA)  // if enemy.
        {
            if (firstEnemy) //if first enemy in queue.
            {
                firstEnemy = false;
                currNode->enemyNum++;
            }
            bestFriend = NULL;
        }
    }
    if (bestFriend != NULL) // if found a friend to skip the queue to.
    {
        int i = 0;
        bestFriend->friendNum++;
        node2Add->next = bestFriend->next;
        bestFriend->next = node2Add;
        if (q->tail == bestFriend) q->tail = node2Add;
    }
    else // else goes to back of the queue
    {
        q->tail->next = node2Add;
        q->tail = node2Add;
    }
}

