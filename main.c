#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"


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
    if (studentsFile == NULL)
    {
        return 0;
    }
    FILE* coursesFile = fopen(argv[2+flag], "r");
    if (coursesFile == NULL)
    {
        fclose(studentsFile);
        return 0;
    }
    FILE* hackersFile = fopen(argv[3+flag], "r");
    if (hackersFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        return 0;
    }
    FILE* queuesFile = fopen(argv[4+flag], "r");
    if (queuesFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        return 0;
    }
    FILE* outputFile = fopen(argv[5+flag], "w");
    if (outputFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        return 0;
    }
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

    updateCapLettersFlag(system, !flag);
    system = readEnrollment(system, queuesFile);
    if (system == NULL) // if readEnrollment failed.
    {
        destroyEnrollmentSystem(system);
        return 0;
    }

    hackEnrollment(system, outputFile);
    destroyEnrollmentSystem(system);
    return 0;
}








