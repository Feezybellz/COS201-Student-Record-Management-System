#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define INITIAL_CAPACITY 10
#define PASSING_THRESHOLD 40.0

// Define the Student structure.
typedef struct {
    char name[MAX_NAME_LENGTH];
    int roll;
    float marks;
} Student;

// Function prototypes.
void clearInputBuffer(void);
void addStudent(Student **students, int *count, int *capacity);
void modifyStudent(Student *students, int count);
void deleteStudent(Student *students, int *count);
void displayStudents(Student *students, int count);
int searchStudent(Student *students, int count, int roll);
void calculateAverage(Student *students, int count);
void sortStudents(Student *students, int count, int ascending);
void saveToFile(Student *students, int count, const char *filename);
void loadFromFile(Student **students, int *count, int *capacity, const char *filename);

int main() {
    // 1. Initial Setup: Welcome message and user greeting.
    char userName[50];
    printf("Welcome to the Student Record System!\n");
    printf("Please enter your name: ");
    fgets(userName, sizeof(userName), stdin);
    userName[strcspn(userName, "\n")] = '\0';
    printf("Hello, %s! Let's manage some student records.\n", userName);

    // 7. Memory Management: Create a dynamic array of students.
    Student *students = malloc(INITIAL_CAPACITY * sizeof(Student));
    if (students == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    int count = 0;
    int capacity = INITIAL_CAPACITY;

    int choice;
    do {
        // Display the menu.
        printf("\n--- Menu ---\n");
        printf("1. Add Student\n");
        printf("2. Modify Student\n");
        printf("3. Delete Student\n");
        printf("4. Display All Students\n");
        printf("5. Search Student by Roll Number\n");
        printf("6. Calculate Average Marks\n");
        printf("7. Sort Students by Marks\n");
        printf("8. Save Records to File\n");
        printf("9. Load Records from File\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addStudent(&students, &count, &capacity);
                break;
            case 2:
                modifyStudent(students, count);
                break;
            case 3:
                deleteStudent(students, &count);
                break;
            case 4:
                displayStudents(students, count);
                break;
            case 5: {
                int roll;
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                clearInputBuffer();
                int index = searchStudent(students, count, roll);
                if (index != -1) {
                    printf("Student Found:\n");
                    printf("Roll: %d, Name: %s, Marks: %.2f - %s\n",
                           students[index].roll,
                           students[index].name,
                           students[index].marks,
                           (students[index].marks >= PASSING_THRESHOLD) ? "Pass" : "Fail");
                } else {
                    printf("Student with roll number %d not found.\n", roll);
                }
                break;
            }
            case 6:
                calculateAverage(students, count);
                break;
            case 7: {
                int sortChoice;
                printf("Sort by marks:\n1. Ascending\n2. Descending\n");
                printf("Enter your choice: ");
                scanf("%d", &sortChoice);
                clearInputBuffer();
                sortStudents(students, count, (sortChoice == 1));
                printf("Students sorted successfully.\n");
                break;
            }
            case 8:
                saveToFile(students, count, "student_records.txt");
                break;
            case 9:
                loadFromFile(&students, &count, &capacity, "student_records.txt");
                break;
            case 10:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 10);

    // Free dynamically allocated memory.
    free(students);
    return 0;
}

// Clear any extra input (for example, leftover newline characters).
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Add a student record.
void addStudent(Student **students, int *count, int *capacity) {
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        if (*count >= *capacity) {
            *capacity *= 2;
            Student *temp = realloc(*students, *capacity * sizeof(Student));
            if (temp == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return;
            }
            *students = temp;
        }

        // Validate integer input for roll number and check for uniqueness.
        int validRoll = 0;
        int tempRoll;
        while (!validRoll) {
            printf("Enter student roll number: ");
            if (scanf("%d", &tempRoll) == 1) {
                // Check if the roll number is already used.
                int duplicate = 0;
                for (int i = 0; i < *count; i++) {
                    if ((*students)[i].roll == tempRoll) {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate) {
                    printf("Error: Roll number %d already exists. Please enter a unique roll number.\n", tempRoll);
                    clearInputBuffer();
                } else {
                    (*students)[*count].roll = tempRoll;
                    validRoll = 1;
                }
            } else {
                printf("Error: Please enter a valid integer for the roll number.\n");
                clearInputBuffer();
            }
        }
        clearInputBuffer();

        // Get student name.
        printf("Enter student name: ");
        fgets((*students)[*count].name, MAX_NAME_LENGTH, stdin);
        (*students)[*count].name[strcspn((*students)[*count].name, "\n")] = '\0';

        // Validate float input for student marks.
        int validMarks = 0;
        while (!validMarks) {
            printf("Enter student marks (0 - 100): ");

            if (scanf("%f", &(*students)[*count].marks) == 1) {
                if ((*students)[*count].marks >= 0 && (*students)[*count].marks <= 100) {
                    validMarks = 1;  // Valid input
                } else {
                    printf("Error: Marks must be between 0 and 100.\n");
                    clearInputBuffer();  // Clear invalid input
                }
            } else {
                printf("Error: Please enter a valid number.\n");
                clearInputBuffer();  // Clear invalid input
            }
        }

        clearInputBuffer();

        // Display pass/fail status based on the marks.
        if ((*students)[*count].marks >= PASSING_THRESHOLD) {
            printf("Status: Pass\n");
        } else {
            printf("Status: Fail\n");
        }
        (*count)++;

        // Prompt user to see if they want to add another student record.
        printf("Do you want to add another student? (y/n): ");
        scanf(" %c", &choice);
        clearInputBuffer();
    }
}


// Modify an existing student record.
void modifyStudent(Student *students, int count) {
    int roll;
    printf("Enter roll number of student to modify: ");
    if (scanf("%d", &roll) != 1) {
        printf("Error: Please enter a valid integer for the roll number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = searchStudent(students, count, roll);
    if (index == -1) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    // Display the current student record.
    printf("Current record for roll number %d:\n", students[index].roll);
    printf("Name: %s\n", students[index].name);
    printf("Marks: %.2f\n", students[index].marks);

    // Prompt for new values.
    printf("Enter new student name: ");
    fgets(students[index].name, MAX_NAME_LENGTH, stdin);
    students[index].name[strcspn(students[index].name, "\n")] = '\0';  // Remove newline character

    // Validate float input for student marks.
    int validMarks = 0;
    while (!validMarks) {
        printf("Enter new student marks (0 - 100): ");
        if (scanf("%f", &students[index].marks) == 1) {
            if (students[index].marks >= 0 && students[index].marks <= 100) {
                validMarks = 1;  // Valid input
            } else {
                printf("Error: Marks must be between 0 and 100.\n");
                clearInputBuffer();  // Clear invalid input
            }
        } else {
            printf("Error: Please enter a valid number.\n");
            clearInputBuffer();  // Clear invalid input
        }
    }

    printf("Record updated successfully.\n");
}


// Delete a student record.
void deleteStudent(Student *students, int *count) {
    int roll;
    printf("Enter roll number of student to delete: ");
    scanf("%d", &roll);
    clearInputBuffer();
    int index = searchStudent(students, *count, roll);
    if (index == -1) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }
    // Shift subsequent records left.
    for (int i = index; i < *count - 1; i++) {
        students[i] = students[i + 1];
    }
    (*count)--;
    printf("Student record deleted successfully.\n");
}

// Display all student records.
void displayStudents(Student *students, int count) {
    if (count == 0) {
        printf("No student records to display.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("Student %d:\n", i + 1);
        printf("Roll: %d\n", students[i].roll);
        printf("Name: %s\n", students[i].name);
        printf("Marks: %.2f - %s\n\n", students[i].marks,
               (students[i].marks >= PASSING_THRESHOLD) ? "Pass" : "Fail");
    }
}

// Search for a student by roll number. Returns the index or -1 if not found.
int searchStudent(Student *students, int count, int roll) {
    for (int i = 0; i < count; i++) {
        if (students[i].roll == roll) {
            return i;
        }
    }
    return -1;
}

// Calculate and display the average marks of all students.
void calculateAverage(Student *students, int count) {
    if (count == 0) {
        printf("No student records available.\n");
        return;
    }
    float sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += students[i].marks;
    }
    printf("Average Marks: %.2f\n", sum / count);
}

// Sort student records by marks using bubble sort.
void sortStudents(Student *students, int count, int ascending) {
    // Bubble sort algorithm to sort the students based on marks.
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if ((ascending && students[j].marks > students[j + 1].marks) ||
                (!ascending && students[j].marks < students[j + 1].marks)) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // Call your displayStudent function to display the sorted list.
    displayStudents(students, count);
}

// Save student records to a text file.
void saveToFile(Student *students, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%.2f\n", students[i].roll, students[i].name, students[i].marks);
    }
    fclose(fp);
    printf("Records saved to %s successfully.\n", filename);
}

// Load student records from a text file.
void loadFromFile(Student **students, int *count, int *capacity, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    *count = 0; // Reset student count
    char buffer[128]; // Buffer to read lines

    while (fgets(buffer, sizeof(buffer), fp) != NULL) { // Ensure fgets reads correctly
        if (strchr(buffer, ',') == NULL) continue; // Skip invalid lines

        // Resize memory if needed
        if (*count >= *capacity) {
            *capacity = (*capacity == 0) ? 10 : (*capacity * 2); // Dynamic growth
            Student *temp = realloc(*students, *capacity * sizeof(Student));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                fclose(fp);
                return;
            }
            *students = temp;
        }

        // Read student data correctly
        if (sscanf(buffer, "%d,%49[^,],%f", &(*students)[*count].roll, (*students)[*count].name, &(*students)[*count].marks) == 3) {
            (*count)++;
        } else {
            printf("Warning: Invalid line format -> %s", buffer); // Debugging message
        }
    }

    fclose(fp);
    printf("Records loaded from %s successfully. Total records: %d\n", filename, *count);
}
