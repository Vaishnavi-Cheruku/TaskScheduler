#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 100
#define MAX_NAME_LENGTH 50

// Structure to represent a task (assignment or record submission)
struct Task
{
    int id;
    char name[MAX_NAME_LENGTH];
    int duration;
    struct Task *dependencies[MAX_TASKS];
    int numDependencies;
    int startTime;
    int deadline;
    int submitted; // Flag to track submission status (0: not submitted, 1: submitted)
};

// Function to perform topological sorting using Depth-First Search
// Function to perform topological sorting using Depth-First Search
void topologicalSortUtil(struct Task* task, int visited[], int* currentTime) {
    visited[task->id] = 1;

    // Recur for all the dependencies of the task
    for (int i = 0; i < task->numDependencies; ++i) {
        if (!visited[task->dependencies[i]->id]) {
            topologicalSortUtil(task->dependencies[i], visited, currentTime);
        }
    }

    // Assign start time based on the completion time of dependencies
    int maxDependencyCompletionTime = 0;
    for (int i = 0; i < task->numDependencies; ++i) {
        int dependencyCompletionTime = task->dependencies[i]->startTime + task->dependencies[i]->duration;
        if (dependencyCompletionTime > maxDependencyCompletionTime) {
            maxDependencyCompletionTime = dependencyCompletionTime;
        }
    }

    // Assign start time based on the maximum completion time of dependencies
    task->startTime = (maxDependencyCompletionTime > *currentTime) ? maxDependencyCompletionTime : *currentTime;

    // Update the current time
    *currentTime = task->startTime + task->duration;
}


// Function to perform topological sorting and print the schedule
void scheduleTasks(struct Task tasks[], int numTasks)
{
    int visited[MAX_TASKS] = {0};
    int currentTime = 0;

    // Perform topological sort for each task
    for (int i = 0; i < numTasks; ++i)
    {
        if (!visited[tasks[i].id])
        {
            topologicalSortUtil(&tasks[i], visited, &currentTime);
        }
    }

    // Print the optimal schedule
    printf("Optimal Task Schedule:\n");
    for (int i = 0; i < numTasks; ++i)
    {
        printf("Task %d: %s\n", tasks[i].id, tasks[i].name);
        printf("  Start Time: %d\n", tasks[i].startTime);
        printf("  Deadline: %d\n", tasks[i].deadline);
        printf("  Submission Status: %s\n", (tasks[i].submitted) ? "Submitted" : "Not Submitted");
        printf("\n");
    }
}

// Function to submit a task
void submitTask(struct Task *task)
{
    task->submitted = 1;
    printf("Task %d (%s) has been submitted!\n", task->id, task->name);
}

// Function to display task details
void displayTaskDetails(struct Task *task)
{
    printf("Task Details for Task %d (%s):\n", task->id, task->name);
    printf("  Duration: %d\n", task->duration);
    printf("  Start Time: %d\n", task->startTime);
    printf("  Deadline: %d\n", task->deadline);
    printf("  Submission Status: %s\n", (task->submitted) ? "Submitted" : "Not Submitted");
}

// Function to take input for tasks and their dependencies
int takeInput(struct Task tasks[], int *numTasks)
{
    printf("Enter the number of tasks: ");
    if (scanf("%d", numTasks) != 1 || *numTasks <= 0 || *numTasks > MAX_TASKS)
    {
        printf("Invalid input. Please enter a positive number of tasks (up to %d).\n", MAX_TASKS);
        return 0; // Return 0 to indicate failure
    }

    printf("Enter details for each task:\n");

    for (int i = 0; i < *numTasks; ++i)
    {
        tasks[i].id = i;
        printf("Task %d - Enter name: ", i);
        scanf("%s", tasks[i].name);

        printf("Enter duration for %s: ", tasks[i].name);
        if (scanf("%d", &tasks[i].duration) != 1 || tasks[i].duration <= 0)
        {
            printf("Invalid duration. Please enter a positive number.\n");
            return 0; // Return 0 to indicate failure
        }

        tasks[i].numDependencies = 0;
        tasks[i].submitted = 0;

        printf("Enter the number of dependencies for %s: ", tasks[i].name);
        if (scanf("%d", &tasks[i].numDependencies) != 1 || tasks[i].numDependencies < 0 || tasks[i].numDependencies > MAX_TASKS)
        {
            printf("Invalid number of dependencies. Please enter a non-negative number (up to %d).\n", MAX_TASKS);
            return 0; // Return 0 to indicate failure
        }

        // Take input for dependencies only if the number of dependencies is greater than 0
        if (tasks[i].numDependencies > 0)
        {
            printf("Enter the IDs of dependencies for %s (space-separated): ", tasks[i].name);
            for (int j = 0; j < tasks[i].numDependencies; ++j)
            {
                int dependencyId;
                if (scanf("%d", &dependencyId) != 1 || dependencyId < 0 || dependencyId >= i)
                {
                    printf("Invalid dependency ID. Please enter a non-negative integer less than the current task ID.\n");
                    return 0; // Return 0 to indicate failure
                }
                tasks[i].dependencies[j] = &tasks[dependencyId];
            }
        }

        printf("Enter the deadline for %s: ", tasks[i].name);
        if (scanf("%d", &tasks[i].deadline) != 1 || tasks[i].deadline <= 0)
        {
            printf("Invalid deadline. Please enter a positive number.\n");
            return 0; // Return 0 to indicate failure
        }
    }

    return 1; // Return 1 to indicate success
}

// Main menu function
void main_menu(struct Task tasks[], int numTasks)
{
    int choice;
    do
    {
        // Displaying menu options
        printf("\n\n");
        printf("\n\t\t\t*************************************************\n");
        printf("\n\t\t\t\t      MAIN MENU: ");
        printf("\n\t\t\t\t     1. VIEW OPTIMAL TASK SCHEDULE ");
        printf("\n\t\t\t\t     2. SUBMIT TASK ");
        printf("\n\t\t\t\t     3. DISPLAY TASK DETAILS ");
        printf("\n\t\t\t\t     4. EXIT\n ");
        printf("\n\t\t\t*************************************************\n");
        printf("\n\t\t\t\t      Enter your choice: ");

        // Reading user choice
        if (scanf("%d", &choice) != 1)
        {
            // Handling non-integer input
            printf("Invalid input. Please enter a number.\n");
            // Clearing the input buffer
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            scheduleTasks(tasks, numTasks);
            break;
        case 2:
            // Logic for submitting task
            printf("Enter the ID of the task to submit: ");
            int taskId;
            if (scanf("%d", &taskId) != 1 || taskId < 0 || taskId >= numTasks)
            {
                printf("Invalid task ID.\n");
            }
            else
            {
                submitTask(&tasks[taskId]);
            }
            break;
        case 3:
            // Logic for displaying task details
            printf("Enter the ID of the task to display details: ");
            int taskIdDetails;
            if (scanf("%d", &taskIdDetails) != 1 || taskIdDetails < 0 || taskIdDetails >= numTasks)
            {
                printf("Invalid task ID.\n");
            }
            else
            {
                displayTaskDetails(&tasks[taskIdDetails]);
            }
            break;
        case 4:
            exit(0);
        default:
            printf("\n\t\t\t\t      ...Invalid Option!...\n");
        }
    } while (choice != 4);
}

// Greetings function
void greetings()
{
    printf("\n\n");
    printf("\t\t\t     ****************************************\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *     ----------------------------     *\n");
    printf("\t\t\t     *      WELCOME TO TASK SCHEDULER        *\n");
    printf("\t\t\t     *     ----------------------------     *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     ****************************************\n");
    printf("\n\n");
    printf("\t\t\t     ****************************************\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *       ------------------------       *\n");
    printf("\t\t\t     *      MANAGE YOUR TASKS EFFICIENTLY    *\n");
    printf("\t\t\t     *       ------------------------       *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *     Stay organized, stay on track!    *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     ****************************************\n");
    printf("\n\n\t\t\t             Press Enter to continue: ");

    // To wait for the user to press Enter
    getchar();
}

int main()
{
    // Greet the user
    greetings();

    // Take input for tasks and their dependencies
    struct Task tasks[MAX_TASKS];
    int numTasks;

    // Loop until valid input is received
    while (!takeInput(tasks, &numTasks))
    {
        // Clearing the input buffer
        while (getchar() != '\n');
    }

    // Call the function to schedule tasks optimally
    scheduleTasks(tasks, numTasks);

    // Enter the main menu
    main_menu(tasks, numTasks);

    return 0;
}