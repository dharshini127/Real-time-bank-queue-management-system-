#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================
   DATA STRUCTURES
   ========================= */

typedef struct Node {
    int token;
    char name[50];
    char purpose[100];
    time_t arrival_time;
    struct Node *next;
} Node;

typedef struct Record {
    int token;
    char name[50];
    char purpose[100];
    time_t arrival_time;
    time_t service_time;
    double waiting_seconds;
    struct Record *next;
} Record;

/* =========================
   QUEUE POINTERS
   ========================= */

Node *front = NULL;
Node *rear  = NULL;
Record *record_head = NULL;
int token_counter = 1;
int served_count = 0;
double total_waiting_time = 0.0;

/* =========================
   UTILITY FUNCTIONS
   ========================= */

void print_time(time_t t) {
    struct tm *tm_info = localtime(&t);
    char buffer[26];
    strftime(buffer, 26, "%d-%m-%Y %H:%M:%S", tm_info);
    printf("%s", buffer);
}

/* =========================
   ENQUEUE (JOIN QUEUE)
   ========================= */

void enqueue(char *name, char *purpose) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->token = token_counter++;
    strcpy(newNode->name, name);
    strcpy(newNode->purpose, purpose);
    newNode->arrival_time = time(NULL);
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    printf("\nPerson added to queue successfully!\n");
    printf("Token Number: %d\n", newNode->token);
}

/* =========================
   DEQUEUE (SERVE PERSON)
   ========================= */

void dequeue() {
    if (front == NULL) {
        printf("\nQueue is empty. No one to serve.\n");
        return;
    }

    Node *temp = front;
    front = front->next;
    if (front == NULL)
        rear = NULL;

    time_t service_time = time(NULL);
    double waiting = difftime(service_time, temp->arrival_time);

    /* Store record */
    Record *rec = (Record *)malloc(sizeof(Record));
    rec->token = temp->token;
    strcpy(rec->name, temp->name);
    strcpy(rec->purpose, temp->purpose);
    rec->arrival_time = temp->arrival_time;
    rec->service_time = service_time;
    rec->waiting_seconds = waiting;
    rec->next = record_head;
    record_head = rec;

    served_count++;
    total_waiting_time += waiting;

    printf("\nServing Token %d (%s)\n", temp->token, temp->name);
    printf("Purpose: %s\n", temp->purpose);
    printf("Arrival Time: "); print_time(temp->arrival_time);
    printf("\nService Time: "); print_time(service_time);
    printf("\nWaiting Time: %.2f seconds\n", waiting);

    free(temp);
}

/* =========================
   DISPLAY LIVE QUEUE
   ========================= */

void display_queue() {
    if (front == NULL) {
        printf("\nQueue is empty.\n");
        return;
    }

    Node *temp = front;
    printf("\n--- LIVE QUEUE STATUS ---\n");
    while (temp != NULL) {
        printf("Token: %d | Name: %s | Purpose: %s | Arrival: ",
               temp->token, temp->name, temp->purpose);
        print_time(temp->arrival_time);
        printf("\n");
        temp = temp->next;
    }
}

/* =========================
   DISPLAY VISIT RECORDS
   ========================= */

void display_records() {
    if (record_head == NULL) {
        printf("\nNo records available yet.\n");
        return;
    }

    Record *temp = record_head;
    printf("\n--- BANK VISIT RECORDS ---\n");
    while (temp != NULL) {
        printf("Token: %d | Name: %s\n", temp->token, temp->name);
        printf("Purpose: %s\n", temp->purpose);
        printf("Arrival: "); print_time(temp->arrival_time);
        printf(" | Served: "); print_time(temp->service_time);
        printf(" | Wait: %.2f sec\n", temp->waiting_seconds);
        printf("----------------------------------\n");
        temp = temp->next;
    }
}

/* =========================
   AVERAGE WAITING TIME
   ========================= */

void average_waiting_time() {
    if (served_count == 0) {
        printf("\nNo customers served yet.\n");
        return;
    }

    printf("\nAverage Waiting Time: %.2f seconds\n",
           total_waiting_time / served_count);
}

/* =========================
   MAIN MENU
   ========================= */

int main() {
    int choice;
    char name[50];
    char purpose[100];

    while (1) {
        printf("\n====== BANK QUEUE SYSTEM ======\n");
        printf("1. New Person Join Queue\n");
        printf("2. Serve Next Person\n");
        printf("3. Display Live Queue\n");
        printf("4. Display Visit Records\n");
        printf("5. Show Average Waiting Time\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
        case 1:
            printf("Enter Name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Enter Purpose of Visit: ");
            fgets(purpose, sizeof(purpose), stdin);
            purpose[strcspn(purpose, "\n")] = 0;

            enqueue(name, purpose);
            break;

        case 2:
            dequeue();
            break;

        case 3:
            display_queue();
            break;

        case 4:
            display_records();
            break;

        case 5:
            average_waiting_time();
            break;

        case 6:
            printf("Exiting system...\n");
            exit(0);

        default:
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
