#include "IsraeliQueue.h"
#include <stdlib.h>
#include <math.h>



enum {STRANGER, FRIEND, ENEMY};

typedef struct Node_t
{
    void* student;
    int friendNum;
    int enemyNum;
    struct Node_t* next;
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


int checkFriend(IsraeliQueue queue, void* item1, void* item2);

FriendshipFunction* createFriendFuncArray(FriendshipFunction* friendFunc);

int getFriendshipThresholdForMerged(IsraeliQueue* , int);

int getEnemyThresholdForMerged(IsraeliQueue* , int);

bool checkInArr(Node* arr, int size, Node Node2check);

Node createNewNode(void* student, int friendNum, int enemyNum);

void enqueueNode(IsraeliQueue queue, Node node2Add);


/**Error clarification:
 * ISRAELIQUEUE_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * ISRAELIQUEUE_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * ISRAELIQUEUE_BAD_PARAM: Indicates an illegal parameter was passed.
 * ISRAELI_QUEUE_ERROR: Indicates any error beyond the above.
 * */

/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/
IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendFunc, ComparisonFunction compFunc, int friend_th, int enemy_th)
{
    IsraeliQueue newQueue = (IsraeliQueue)malloc(sizeof(struct IsraeliQueue_t));
    if (newQueue == NULL)
    {
        return NULL;
    }
    newQueue->friendFunc = createFriendFuncArray(friendFunc);
    if (newQueue->friendFunc == NULL)
    {
        free(newQueue);
        return NULL;
    }
    free(friendFunc);
    newQueue->compFunc = compFunc;
    newQueue->friend_th = friend_th;
    newQueue->enemy_th = enemy_th;
    newQueue->head = NULL;
    newQueue->tail = NULL;
    return newQueue;
}

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL,
 * NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue queue) {
    if (queue == NULL)
    {
        return NULL;
    }
    IsraeliQueue newQueue = (IsraeliQueue) malloc(sizeof(struct IsraeliQueue_t));
    if (newQueue == NULL)
    {
        return NULL;
    }

    newQueue->friendFunc = createFriendFuncArray(queue->friendFunc);
    if (newQueue->friendFunc == NULL)
    {
        IsraeliQueueDestroy(newQueue);
        return NULL;
    }
    newQueue->compFunc = queue->compFunc;
    newQueue->friend_th = queue->friend_th;
    newQueue->enemy_th = queue->enemy_th;

    // create new Head.
    newQueue->head = createNewNode(queue->head->student, queue->head->friendNum, queue->head->enemyNum);
    if (newQueue->head == NULL)
    {
        IsraeliQueueDestroy(newQueue);
        return NULL;
    }

    Node currNode = newQueue->head;
    for (Node tempNode = queue->head->next; tempNode != NULL; tempNode = tempNode->next) //for Nodes in queue
    {
        // creates new Node and returns it.
        Node newNode = createNewNode(tempNode->student, tempNode->friendNum, tempNode->enemyNum);
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
void IsraeliQueueDestroy(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return;
    }
    Node tempNode = queue->head;
    while (tempNode != NULL)
    {
        Node nextNode = tempNode->next;
        free(tempNode);
        tempNode = nextNode;
    }
    free(queue->friendFunc);
    free(queue);

}


/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * item)
{
    Node newNode = createNewNode(item, 0, 0); // creates new Node and returns it.
    if (newNode==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED; // if allocation failed.
    }
    enqueueNode(queue, newNode);
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction friendFunc2Add)
{
    int size;
    for (size = 0; queue->friendFunc[size] != NULL; size++) {
    }

    FriendshipFunction* tempFuncArray = (FriendshipFunction*)realloc(queue->friendFunc ,sizeof(FriendshipFunction)*(size+2));
    if (tempFuncArray == NULL)
    {
        IsraeliQueueDestroy(queue);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    queue->friendFunc = tempFuncArray;

    queue->friendFunc[size] = friendFunc2Add;
    queue->friendFunc[size+1] = NULL;
    return ISRAELIQUEUE_SUCCESS;
}


/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int FriendshipThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->friend_th = FriendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int EnemyThreshold)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queue->enemy_th = EnemyThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    int size = 0;
    Node tempNode = queue->head;
    while (tempNode != NULL)
    {
        size++;
        tempNode = tempNode->next;
    }
    return size;
}

/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue queue)
{
    if (queue == NULL || queue->head == NULL)
    {
        return NULL;
    }

    Node tempNode = queue->head;
    void* item = tempNode->student;

    queue->head = tempNode->next;
    free(tempNode);
    return item;
}

/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue queue, void * item)
{
    if (queue == NULL)
    {
        return false;
    }
    Node tempNode = queue->head;
    while (tempNode != NULL)
    {
        if (queue->compFunc(tempNode->student, item) == 0)
        {
            return true;
        }
        tempNode = tempNode->next;
    }
    return false;
}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue)
{
    if (queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    int size = IsraeliQueueSize(queue);
    Node* NodesArr = (Node*)malloc(sizeof(Node)*size);
    if (NodesArr==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    Node currNode = queue->head;

    // fills the array with the Nodes in their current positions.
    for(int i = 0; i < size; i++, currNode = currNode->next)
    {
        NodesArr[i] = currNode;
    }

    for (int i = 0; i < size; i++)
    {
        Node Node2Advance = NodesArr[i];
        if (Node2Advance == queue->head) //if current Node is the head.
        {
            Node temp = queue->head;
            queue->head = queue->head->next;
            enqueueNode(queue, temp); // re-enqueue the head.
        }
        else
        {
            for (currNode = queue->head; currNode->next != Node2Advance; currNode = currNode->next){
            } //finds the Node before the current Node.

            Node tempNode = currNode->next;
            currNode->next = tempNode->next;
            enqueueNode(queue, tempNode);
        }
    }

    free(NodesArr);
    return ISRAELIQUEUE_SUCCESS;
}

/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* queue ,ComparisonFunction compFunc)
{
    //gets size of array.
    if (queue == NULL)
    {
        return NULL;
    }

    int size;
    for (size = 0; queue[size] != NULL; size++){
    }

    //creates friendship function array for merged queue.
    FriendshipFunction *mergedFunctions = createFriendFuncArray(queue[0]->friendFunc);
    if (mergedFunctions == NULL)
    {
        return NULL;
    }

    //creates merged queue.
    IsraeliQueue mergedQueue = IsraeliQueueCreate(mergedFunctions, compFunc,
                                                  getFriendshipThresholdForMerged(queue, size),
                                                  getEnemyThresholdForMerged(queue, size));
    if (mergedQueue == NULL)
    {
        free(mergedFunctions);
        return NULL;
    }

    //adds friendship functions to merged queue.
    int j = 0;
    for (int i = 1; i < size; i++)
    {
        while (queue[i]->friendFunc[j] != NULL)
        {
            IsraeliQueueAddFriendshipMeasure(mergedQueue,queue[i]->friendFunc[j]);
            j++;
        }
    }

    //merge queues.
    bool notEmpty = true;
    void* tempItem = NULL;
    while(notEmpty)
    {
        notEmpty = false;
        for (int i = 0; i < size; i++)
        {
            if (queue[i]->head != NULL)
            {
                tempItem = IsraeliQueueDequeue(queue[i]);
                IsraeliQueueEnqueue(mergedQueue, tempItem);
                notEmpty = true;
            }
        }
    }
    return mergedQueue;
}

int checkFriend(IsraeliQueue queue, void* item1, void* item2)
{
    double sum = 0, sumCount = 0, currItem;
    bool flag = false;
    for (int i = 0; queue->friendFunc[i] != NULL; i++) //for friend function in queue.
    {
        flag = true;
        currItem = queue->friendFunc[i](item1, item2);
        if (currItem > queue->friend_th)
        {
            return FRIEND;
        }
        sum += currItem;
        sumCount++;
    }
    if(flag == false)
    {
        return STRANGER;
    }
    if (sum/sumCount < queue->enemy_th)
    {
        return ENEMY;
    }
    return STRANGER;
}

FriendshipFunction* createFriendFuncArray(FriendshipFunction* friendFunc)
{
    int size;
    for (size = 0; friendFunc[size] != NULL; size++){
    }

    FriendshipFunction* newFriendFunc = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(size+1));
    if (newFriendFunc==NULL)
    {
        return NULL;
    }
    for (int i = 0; i < size; i++)
    {
        newFriendFunc[i] = friendFunc[i];
    }
    newFriendFunc[size] = NULL;
    return newFriendFunc;
}

int getFriendshipThresholdForMerged(IsraeliQueue* queue, int size){
    int friend_th = 0;
    for (int i = 0; i < size; i++)
    {
        friend_th += queue[i]->friend_th;
    }
    return friend_th / size;
}

int getEnemyThresholdForMerged(IsraeliQueue* queue, int size){
    int enemy_th = 1;
    double root;
    for (int i = 0; i < size; i++)
    {
        enemy_th *= abs(queue[i]->friend_th);
    }
    root = 1/((double)size);
    return pow(enemy_th, root);
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
    Node newNode = (Node)malloc(sizeof(struct Node_t));
    if (newNode == NULL)
    {
        return NULL;
    }
    newNode->student = student;
    newNode->friendNum = friendNum;
    newNode->enemyNum = enemyNum;
    newNode->next = NULL;
    return newNode;
}

void enqueueNode(IsraeliQueue queue, Node node2Add)
{
    if (queue->head == NULL)
    {
        queue->head = node2Add;
        queue->tail = node2Add;
    }
    else
    {
        Node bestFriend = NULL;
        int relation;
        bool firstEnemy = true;

        for (Node currNode = queue->head;
            currNode != NULL; currNode = currNode->next) // for not reached tail of back of queue.

        {
            relation = checkFriend(queue, node2Add->student, currNode->student); //gets current node relation to new Node.
            if (relation == FRIEND && bestFriend == NULL && currNode->friendNum < FRIEND_QUOTA)
            {
                bestFriend = currNode; // if friends and closest friend to front of the line.
            }
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
            bestFriend->friendNum++;
            node2Add->next = bestFriend->next;
            bestFriend->next = node2Add;
            if (queue->tail == bestFriend)
            {
                queue->tail = node2Add;
            }
        }
        else // else goes to back of the queue
        {
            queue->tail->next = node2Add;
            queue->tail = node2Add;
        }
    }
}

