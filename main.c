#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"


#define MIN_ARGC 6
#define MAX_ARGC 7


enum {STUDENTS_FILE = 1, COURSES_FILE, HACKERS_FILE, QUEUES_FILE, TARGET_FILE};


// argc is of size 6/7.
// argv is ./HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>.

int main(int argc, char** argv)
{
    // if invalid number of arguments.
    if (argc < MIN_ARGC || argc > MAX_ARGC)
    {
        return 0;
    }

    int flag = 0;
    if (argc == MAX_ARGC && strcmp(argv[1], "-i") == 0)
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