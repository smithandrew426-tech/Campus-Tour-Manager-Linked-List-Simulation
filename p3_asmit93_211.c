#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Struct for a single stop
typedef struct Stop {
    char name[26];
    char description[101];
    int stayTime;
} Stop;

// Struct for a node in the linked list
typedef struct Tour {
    Stop data;
    struct Tour* next;
} Tour;

// Function prototypes
void printTour(Tour *head);
void addStop(Tour **head, int sort);
void deleteStop(Tour **head);
void saveTour(Tour **head);
void restoreTour(Tour **head);
void free_list(Tour *head);
void print_total_stay_time(Tour *head);

int main() {
    char buffer[50];
    int inProgress = 1;
    int choice = -1;
    Tour *tourStops = NULL;  // Head pointer for the tour linked list
    int sort = 0;
    int sortChosen = 0;

    printf("Welcome to the GMUTT Campus Tour!\n");


    // Ask how to insert (alphabetically or after a stop)
    printf("Choose 1 to create the tour in alphabetical order\n");
    printf("Choose 2 to customize the order of your Tour stops\n");
    while(!sortChosen){
        printf("Choice-> ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            sscanf(buffer, "%i", &sort);
            sortChosen = 1;
        } else{
            printf("Invalid input!!\n");
        }
        
    }

    if(sort == 1){
        printf("New tour stops will be inserted into the list in an alphabetical order.\n\n");
    } else{
        printf("New tour stops will be inserted into the list in an order chosen by the user.\n\n");
    }

    // Main menu loop
    while (inProgress) {
        printf("Please choose from the following options:\n");
        printf("1) Print the Current Campus Tour\n");
        printf("2) Add a tour stop to the tour\n");
        printf("3) Delete a tour stop from the tour\n");
        printf("4) Save the current tour to a file\n");
        printf("5) Load a tour from a file\n");
        printf("6) Exit the program\n");
        printf("Choice-> ");
        
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            sscanf(buffer, "%i", &choice);
        }
        printf("\n\n");

        // Handle user input
        switch (tolower(choice)) {
            case 1: 
                printf("----------------------------------------------\n");
                printTour(tourStops);
                printf("----------------------------------------------\n");
                break;
            case 2: 
                addStop(&tourStops, sort);
                break;
            case 3: 
                deleteStop(&tourStops);
                break;
            case 4: 
                saveTour(&tourStops);
                break;
            case 5: 
                restoreTour(&tourStops);
                break;
            case 6: 
                free_list(tourStops);
                inProgress = 0;
                printf("Thank you for visiting GMU!\n");
                break;
            default: 
                printf("Invalid input!!\n");
                break;
        }
    }

    return 0;
}

// Prints all tour stops
void printTour(Tour *head) {
    Tour *tempTour = head;
    int count = 1;
    if (tempTour == NULL) {
        printf("Sorry, there are no stops on the tour yet.\n");
        return;
    }
    while (tempTour != NULL) {
        printf("Tour Stop %i: %s - %s\n", count, tempTour->data.name, tempTour->data.description);
        printf("Stay Time: %i minutes\n", tempTour->data.stayTime);
        tempTour = tempTour->next;
        count++;
    }
    print_total_stay_time(head);
}

// Adds a new stop to the tour
void addStop(Tour **head, int sort) {
    char buffer[128];
    Stop newStop;

    // Get stop name
    printf("Please enter the name of this new stop: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%25[^\n]", newStop.name);
    } else {
        printf("Error reading stop name.\n");
        return;
    }

    // Check for duplicate stop
    Tour *temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->data.name, newStop.name) == 0) {
            printf("A stop with that name already exists.\n");
            return;
        }
        temp = temp->next;
    }

    // Get description
    printf("Enter an interesting fact about this stop: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%100[^\n]", newStop.description);
    } else {
        printf("Error reading description.\n");
        return;
    }

    // Get stay time
    printf("Please enter the stay time (in minutes): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &newStop.stayTime) != 1) {
            printf("Invalid stay time entered.\n");
            return;
        }
    }

    // Allocate new node
    Tour *newNode = malloc(sizeof(Tour));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }
    newNode->data = newStop;
    newNode->next = NULL;

    ////////////////////

    if (sort == 1) {
        // Insert alphabetically
        if (*head == NULL || strcmp(newStop.name, (*head)->data.name) < 0) {
            newNode->next = *head;
            *head = newNode;
        } else {
            Tour *current = *head;
            while (current->next != NULL && strcmp(current->next->data.name, newStop.name) < 0) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        printf("New stop inserted in alphabetical order.\n");

    } else {
        // Insert after a specific stop or at end
        if (*head == NULL) {
            *head = newNode;
            printf("First stop added!\n");
        } else {
            printf("Enter the name of the stop to insert after (or leave blank to insert at the end): ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
            }

            if (strlen(buffer) == 0) {
                // Insert at end
                Tour *current = *head;
                while (current->next != NULL) current = current->next;
                current->next = newNode;
            } else {
                // Insert after specific stop
                Tour *current = *head;
                while (current != NULL && strcmp(current->data.name, buffer) != 0) {
                    current = current->next;
                }

                if (current == NULL) {
                    printf("Stop not found. Inserting at the end.\n");
                    current = *head;
                    while (current->next != NULL) current = current->next;
                    current->next = newNode;
                } else {
                    newNode->next = current->next;
                    current->next = newNode;
                }
            }
        }
        printf("New stop added successfully!\n");
    }
}

// Deletes a stop by name
void deleteStop(Tour **head) {
    Tour *current = *head;
    Tour *prev = NULL;
    char buffer[50];
    char name[50];

    if (*head == NULL) {
        printf("The Tour list is empty!\n");
        return;
    }

    printf("Please enter the name of the stop: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%49[^\n]", name);
    } else {
        printf("NULL entry!\n");
        return;
    }
    printf("\n");

    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            if (prev == NULL) {
                // Deleting the head node
                *head = current->next;
            } else {
                // Deleting after head
                prev->next = current->next;
            }
            free(current);
            printf("Stop '%s' deleted successfully.\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Stop '%s' not found.\n", name);
}

// Saves the current tour to a file
void saveTour(Tour **head) {
    FILE *fp;
    char buffer[50];
    char fileName[50];

    printf("Please enter the file name to where you want to save the tour: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%s", fileName);
    }
    printf("\n");

    // Attempt to open the file in write-binary mode. This will create a new file or overwrite an existing one.
    fp = fopen(fileName, "wb");

    // Check if file creation was successful
    if (fp == NULL) {
        printf("Error creating or opening file.\n");
    } else {
        Tour *temp = *head;

        // Write each stop to the file
        while (temp != NULL) {
            fwrite(&(temp->data), sizeof(Stop), 1, fp);
            temp = temp->next;
        }

        printf("Tour has been saved!\n");
        fclose(fp);  // Close the file after writing
    }
}

void print_total_stay_time(Tour *head) {
    int totalStayTime = 0;
    Tour *temp = head;
    while (temp != NULL) {
        totalStayTime += temp->data.stayTime;
        temp = temp->next;
    }
    printf("Total time for the tour: %d minutes\n", totalStayTime);
}

// Restores a tour from a file
void restoreTour(Tour **head) {
    if (*head != NULL) {
        free_list(*head);
        *head = NULL;
    }

    FILE *fp;
    char buffer[50];
    char fileName[50];

    printf("Please enter the file name to restore the tour from: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%s", fileName);
    }
    printf("\n");

    

    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Tour *newHead = NULL;
    Tour *current = NULL;

    Stop tempStop;
    while (fread(&tempStop, sizeof(Stop), 1, fp) == 1) {
        Tour *newNode = malloc(sizeof(Tour));
        if (newNode == NULL) {
            printf("Memory allocation failed.\n");
            fclose(fp);
            return;
        }
        newNode->data = tempStop;
        newNode->next = NULL;

        if (newHead == NULL) {
            newHead = newNode;
            current = newHead;
        } else {
            current->next = newNode;
            current = current->next;
        }
    }

    *head = newHead;
    fclose(fp);

    printf("Tour restored successfully!\n");
}

// Frees all memory used by the tour list
void free_list(Tour *head) {
    Tour *current = head;
    Tour *next_node = NULL;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}
