#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"

/*
// argc is of size 7.
// argv is ./HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>.

int main(int argc, char** argv)
{
    int flag = 0;
    if (argc == 7 && strcmp(argv[1], "-i"))
    {
        flag = 1;
    }
    FILE* studentsFile = fopen(argv[1+flag], "r");
    FILE* coursesFile = fopen(argv[2+flag], "r");
    FILE* hackersFile = fopen(argv[3+flag], "r");
    FILE* queuesFile = fopen(argv[4+flag], "r");
    FILE* outputFile = fopen(argv[5+flag], "w");
    // if one of the files is NULL (didn't open right).
    if (studentsFile == NULL || coursesFile == NULL || hackersFile == NULL || queuesFile == NULL || outputFile == NULL)
    {
        return 0;
    }

    EnrollmentSystem system = createEnrollment(studentsFile, coursesFile, hackersFile);
    if (system == NULL) // if createEnrollment failed.
    {
        return 0;
    }

    updateCapLettersFlag(system, !flag);
    system = readEnrollment(system, queuesFile);
    if (system == NULL) // if readEnrollment failed.
    {
        destroySys(system);
        return 0;
    }

    hackEnrollment(system, outputFile);
    destroySys(system);
    return 0;
}



*/












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
    FILE *studentsFile = fopen("C:\\Users\\yuval\\CLionProjects\\ex1_idanLifshitz_yuvalCarmeli\\ExampleTest\\students.txt", "r");
    FILE *hackersFile = fopen("C:\\Users\\yuval\\CLionProjects\\ex1_idanLifshitz_yuvalCarmeli\\ExampleTest\\hackers.txt", "r");
    FILE *coursesFile = fopen("C:\\Users\\yuval\\CLionProjects\\ex1_idanLifshitz_yuvalCarmeli\\ExampleTest\\courses.txt", "r");
    FILE *queuesFile = fopen("C:\\Users\\yuval\\CLionProjects\\ex1_idanLifshitz_yuvalCarmeli\\ExampleTest\\queues.txt", "r");
    FILE *out = fopen("C:\\Users\\yuval\\CLionProjects\\ex1_idanLifshitz_yuvalCarmeli\\ExampleTest\\myOut.txt", "w");

   EnrollmentSystem ee = readEnrollment(createEnrollment(studentsFile, coursesFile, hackersFile), queuesFile);
    hackEnrollment(ee, out);



    FriendshipFunction *funcs = malloc(3 * sizeof(FriendshipFunction));


    *funcs = friendships_function1;
    funcs[1] = friendships_function2;
    funcs[2] = NULL;

    ComparisonFunction f2 = comparison_function;

    IsraeliQueue q1 = IsraeliQueueCreate(funcs, f2, 200, 100);
    IsraeliQueue q2 = IsraeliQueueCreate(funcs, f2, 100, 100);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 0;
    int e = 0;

    int *item1 = &a;
    int *item2 = &b;
    int *item3 = &c;
    int *item4 = &d;
    int *item5 = &e;

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

    fclose(studentsFile);
    fclose(hackersFile);
    fclose(coursesFile);
    fclose(queuesFile);
    fclose(out);


    return 0;
}