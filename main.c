#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"

#define NUM_OF_ARGUMENTS_WITH_FLAG 7

enum {STUDENTS_FILE = 1, COURSES_FILE, HACKERS_FILE, QUEUES_FILE, TARGET_FILE};

// argc is of size 7.
// argv is ./HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>.

int main(int argc, char** argv)
{
    int flag = 0;
    if (argc == NUM_OF_ARGUMENTS_WITH_FLAG && strcmp(argv[1], "-i") == 0)
    {
        flag = 1;
    }

    FILE* studentsFile = fopen(argv[STUDENTS_FILE+flag], "r");
    if (studentsFile == NULL)
    {
        return 0;
    }
    FILE* coursesFile = fopen(argv[COURSES_FILE+flag], "r");
    if (coursesFile == NULL)
    {
        fclose(studentsFile);
        return 0;
    }
    FILE* hackersFile = fopen(argv[HACKERS_FILE+flag], "r");
    if (hackersFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        return 0;
    }
    FILE* queuesFile = fopen(argv[QUEUES_FILE+flag], "r");
    if (queuesFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        return 0;
    }
    FILE* outputFile = fopen(argv[TARGET_FILE+flag], "w");
    if (outputFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        return 0;
    }

    // Gets enrollment system, already initialized.
    EnrollmentSystem system = createEnrollment(studentsFile, coursesFile, hackersFile);
    if (system == NULL) // if createEnrollment failed.
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        fclose(outputFile);
        return 0;
    }

    // Update program to address capital letters according to flag.
    updateCapLettersFlag(system, flag);

    // Gets updated system with filled queues according to queues file.
    system = readEnrollment(system, queuesFile);
    if (system == NULL) // if readEnrollment failed.
    {
        destroyEnrollmentSystem(system);
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        fclose(outputFile);
        return 0;
    }

    // Enqueues Hackers to desired courses and writes output to output file.
    hackEnrollment(system, outputFile);

    destroyEnrollmentSystem(system);

    fclose(studentsFile);
    fclose(coursesFile);
    fclose(hackersFile);
    fclose(queuesFile);
    fclose(outputFile);

    return 0;
}


/*
 //5 if numbers are equal
int friendships_function1(void *item1, void *item2) {
    if (*((int *) item1) == *((int *) item2)) {
        return 5;
    }

    return 0;
}

// 5 if item1 -3 < item2< item1+3
int friendships_function2(void *item1, void *item2) {
    if (*((int *) item1) - 3 < *((int *) item2) && *((int *) item2) < *((int *) item1) + 3) {
        return 5;
    }

    return 0;
}

int comparison_function(void *item1, void *item2) {
    if (*((int *) item1) == *((int *) item2)) {
        return 1;
    }

    return 0;
}



int main() {
    FILE *studentsFile = fopen("C:\\Users\\User\\CLionProjects\\mtm_ex1_EricYoad\\students.txt", "r");
    FILE *hackersFile = fopen("C:\\Users\\User\\CLionProjects\\mtm_ex1_EricYoad\\hackers.txt", "r");
    FILE *coursesFile = fopen("C:\\Users\\User\\CLionProjects\\mtm_ex1_EricYoad\\courses.txt", "r");
    FILE *queuesFile = fopen("C:\\Users\\User\\CLionProjects\\mtm_ex1_EricYoad\\queues.txt", "r");


    EnrollmentSystem ee = readEnrollment(createEnrollment(studentsFile, coursesFile, hackersFile), queuesFile);
    FriendshipFunction* funcs = malloc(3 * sizeof(FriendshipFunction));
    *funcs = friendships_function1;
    *(funcs + 1) = friendships_function2;
    *(funcs + 2) = NULL;

    ComparisonFunction f2 = comparison_function;

    IsraeliQueue q1 = IsraeliQueueCreate(funcs, f2, 200, 100);
    IsraeliQueue q2 = IsraeliQueueCreate(funcs, f2, 100, 100);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 0;
    int e = 0;

    int* item1 = &a;
    int* item2 = &b;
    int* item3 = &c;
    int* item4 = &d;
    int* item5 = &e;

    IsraeliQueueEnqueue(q1, item1);
    IsraeliQueueEnqueue(q1, item2);
    IsraeliQueueEnqueue(q1, item3);
    IsraeliQueueEnqueue(q2, item4);
    IsraeliQueueEnqueue(q2, item5);

    IsraeliQueue qArr[3] = {q1, q2, NULL};

    IsraeliQueue q3 = IsraeliQueueMerge(qArr, f2);

    IsraeliQueueUpdateFriendshipThreshold(q1, 2);
    IsraeliQueueUpdateRivalryThreshold(q1, 5);

    int sizeQ1 = IsraeliQueueSize(q1);

    return 0;
 */





