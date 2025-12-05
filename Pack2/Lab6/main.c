#include "student.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void handleFoundStudents(StatusCode status, Student *foundStudents, int foundCount);

void printMenu() {
    printf("\nStudent Database\n");
    printf("1. Find student by ID\n");
    printf("2. Find students by surname\n");
    printf("3. Find students by name\n");
    printf("4. Find students by group\n");
    printf("5. Sort students by ID\n");
    printf("6. Sort students by surname\n");
    printf("7. Sort students by name\n");
    printf("8. Sort students by group\n");
    printf("9. Write student by ID to trace file\n");
    printf("10. Write top students to trace file\n");
    printf("0. Exit\n");
    printf("Choose option: ");
}


double calculateOverallAverage(const Student *students, int count) {
    double total = 0;
    for (int i = 0; i < count; i++) {
        total += calculateAverage(students[i].exams, EXAMS_COUNT);
    }
    return count > 0 ? total / count : 0;
}


void handleFoundStudents(StatusCode status, Student *foundStudents, int foundCount) {
    if (status == OK) {
        printf("Found %d student(s):\n", foundCount);
        for (int i = 0; i < foundCount; i++) {
            printf("%d. %s %s, Group: %s\n",
                   i + 1, foundStudents[i].surname,
                   foundStudents[i].name, foundStudents[i].group);
        }
    } else if (status == ERR_NOT_FOUND) {
        printf("No students found\n");
    } else {
        printf("Error occurred\n");
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <trace_file>\n", argv[0]);
        return 1;
    }

    Student *students = NULL;
    int studentCount = 0;

    StatusCode status = readStudentsFromFile(argv[1], &students, &studentCount);
    if (status != OK) {
        printf("Error reading file. Status code: %d\n", status);
        return 1;
    }

    printf("Loaded %d students\n", studentCount);

    int choice;
    char buffer[NAME_LEN];
    unsigned int id;
    Student foundStudent;
    Student *foundStudents = NULL;
    int foundCount = 0;

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter ID: ");
                if (scanf("%u", &id) == 1) {
                    status = findStudentById(students, studentCount, id, &foundStudent);
                    if (status == OK) {
                        printf("Found: %s %s, Group: %s\n",
                               foundStudent.surname, foundStudent.name,
                               foundStudent.group);
                    } else {
                        printf("Student not found\n");
                    }
                } else {
                    printf("Invalid ID format\n");
                    while (getchar() != '\n');
                }
                break;

            case 2:
                printf("Enter surname: ");
                if (scanf("%49s", buffer) == 1) {
                    status = findStudentsByField(students, studentCount, buffer, 1,
                                                 &foundStudents, &foundCount);
                    handleFoundStudents(status, foundStudents, foundCount);
                }
                break;

            case 3:
                printf("Enter name: ");
                if (scanf("%49s", buffer) == 1) {
                    status = findStudentsByField(students, studentCount, buffer, 2,
                                                 &foundStudents, &foundCount);
                    handleFoundStudents(status, foundStudents, foundCount);
                }
                break;

            case 4:
                printf("Enter group: ");
                if (scanf("%19s", buffer) == 1) {
                    status = findStudentsByField(students, studentCount, buffer, 3,
                                                 &foundStudents, &foundCount);
                    handleFoundStudents(status, foundStudents, foundCount);
                }
                break;

            case 5:
                status = sortStudents(students, studentCount, compareById);
                if (status == OK) {
                    printf("Sorted by ID\n");
                    printf("Sorted list:\n");
                    for (int i = 0; i < studentCount; i++) {
                        printf("%d. ID: %u, %s %s, Group: %s\n",
                               i + 1, students[i].id,
                               students[i].surname, students[i].name,
                               students[i].group);
                    }
                } else {
                    printf("Sorting failed. Status code: %d\n", status);
                }
                break;

            case 6:
                status = sortStudents(students, studentCount, compareBySurname);
                if (status == OK) {
                    printf("Sorted by surname\n");
                    printf("Sorted list:\n");
                    for (int i = 0; i < studentCount; i++) {
                        printf("%d. %s %s, Group: %s\n",
                               i + 1, students[i].surname, students[i].name,
                               students[i].group);
                    }
                } else {
                    printf("Sorting failed. Status code: %d\n", status);
                }
                break;

            case 7:
                status = sortStudents(students, studentCount, compareByName);
                if (status == OK) {
                    printf("Sorted by name\n");
                    printf("Sorted list:\n");
                    for (int i = 0; i < studentCount; i++) {
                        printf("%d. %s %s, Group: %s\n",
                               i + 1, students[i].name, students[i].surname,
                               students[i].group);
                    }
                } else {
                    printf("Sorting failed. Status code: %d\n", status);
                }
                break;

            case 8:
                status = sortStudents(students, studentCount, compareByGroup);
                if (status == OK) {
                    printf("Sorted by group\n");
                    printf("Sorted list:\n");
                    for (int i = 0; i < studentCount; i++) {
                        printf("%d. Group: %s, %s %s\n",
                               i + 1, students[i].group,
                               students[i].surname, students[i].name);
                    }
                } else {
                    printf("Sorting failed. Status code: %d\n", status);
                }
                break;

            case 9:
                printf("Enter ID: ");
                if (scanf("%u", &id) == 1) {
                    status = findStudentById(students, studentCount, id, &foundStudent);
                    if (status == OK) {
                        status = writeStudentToTraceFile(&foundStudent, argv[2]);
                        if (status == OK) {
                            printf("Written to trace file\n");
                        } else {
                            printf("Error writing to trace file. Status code: %d\n", status);
                        }
                    } else {
                        printf("Student not found\n");
                    }
                } else {
                    printf("Invalid ID format\n");
                    while (getchar() != '\n');
                }
                break;

            case 10:
                {
                    double overallAvg = calculateOverallAverage(students, studentCount);
                    printf("Overall average: %.2f\n", overallAvg);
                    status = writeTopStudentsToTraceFile(students, studentCount,
                                                         overallAvg, argv[2]);
                    if (status == OK) {
                        printf("Top students written to trace file (threshold: %.2f)\n",
                               overallAvg);
                    } else {
                        printf("Error writing to trace file. Status code: %d\n", status);
                    }
                }
                break;

            case 0:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid option. Please choose 0-10.\n");
        }

        if (foundStudents) {
            free(foundStudents);
            foundStudents = NULL;
            foundCount = 0;
        }

        while (getchar() != '\n');

    } while (choice != 0);

    if (students) {
        free(students);
    }

    return 0;
}