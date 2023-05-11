#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"


#define MIN_ARGC 6
#define MAX_ARGC 7

#define NUM_OF_FILES 5

enum {STUDENTS_FILE, COURSES_FILE, HACKERS_FILE, QUEUES_FILE, TARGET_FILE};

void openSystemFiles(FILE* systemFiles[NUM_OF_FILES],
                       char* studentsFilePath,
                       char* coursesFilePath,
                       char* hackersFilePath,
                       char* queuesFilePath,
                       char* outputFilePath);

void closeSystemFiles(FILE** systemFiles);

// argc is of size 6/7.
// argv is ./HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>.

int main(int argc, char** argv)
{
    // if invalid number of arguments.
    if (argc < MIN_ARGC || argc > MAX_ARGC)
    {
        return 0;
    }

    int flag = 1;
    if (argc == MAX_ARGC && strcmp(argv[1], "-i") == 0)
    {
        flag = 2;
    }

    FILE* systemFiles[NUM_OF_FILES] = {};
    openSystemFiles(systemFiles,
                    argv[STUDENTS_FILE+flag],
                    argv[COURSES_FILE+flag],
                    argv[HACKERS_FILE+flag],
                    argv[QUEUES_FILE+flag],
                    argv[TARGET_FILE+flag]);

    if (systemFiles[NUM_OF_FILES-1] == NULL)
    {
        return 0;
    }

    // Gets enrollment system, already initialized.
    EnrollmentSystem system = createEnrollment(systemFiles[STUDENTS_FILE],
                                               systemFiles[COURSES_FILE],
                                               systemFiles[HACKERS_FILE]);
    if (system == NULL) // if createEnrollment failed.
    {
        closeSystemFiles(systemFiles);
        return 0;
    }

    // Update program to address capital letters according to flag.
    updateCapLettersFlag(system, flag-1);

    // Gets updated system with filled queues according to queues file.
    EnrollmentSystem tempSystem = readEnrollment(system, systemFiles[QUEUES_FILE]);
    if (tempSystem == NULL) // if readEnrollment failed.
    {
        destroyEnrollmentSystem(system);
        closeSystemFiles(systemFiles);
        return 0;
    }
    system = tempSystem;

    // Enqueues Hackers to desired courses and writes output to output file.
    hackEnrollment(system, systemFiles[TARGET_FILE]);

    destroyEnrollmentSystem(system);
    closeSystemFiles(systemFiles);

    return 0;
}

void openSystemFiles(FILE* systemFiles[NUM_OF_FILES],
                       char* studentsFilePath,
                       char* coursesFilePath,
                       char* hackersFilePath,
                       char* queuesFilePath,
                       char* outputFilePath)
{
    FILE* studentsFile = fopen(studentsFilePath, "r");
    if (studentsFile == NULL)
    {
        return;
    }
    FILE* coursesFile = fopen(coursesFilePath, "r");
    if (coursesFile == NULL)
    {
        fclose(studentsFile);
        return;
    }
    FILE* hackersFile = fopen(hackersFilePath, "r");
    if (hackersFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        return;
    }
    FILE* queuesFile = fopen(queuesFilePath, "r");
    if (queuesFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        return;
    }
    FILE* outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL)
    {
        fclose(studentsFile);
        fclose(coursesFile);
        fclose(hackersFile);
        fclose(queuesFile);
        return;
    }

    systemFiles[STUDENTS_FILE] = studentsFile;
    systemFiles[COURSES_FILE] = coursesFile;
    systemFiles[HACKERS_FILE] = hackersFile;
    systemFiles[QUEUES_FILE] = queuesFile;
    systemFiles[TARGET_FILE] = outputFile;
}

void closeSystemFiles(FILE** systemFiles)
{
    for (int i = 0; i < NUM_OF_FILES; i++)
    {
        fclose(systemFiles[i]);
    }
}
