#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EVENTS 100

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    char description[100];
} Event;

Event events[MAX_EVENTS];
int event_count = 0;

// Function prototypes
void add_event();
void view_events();
void check_alarms();

int main() {
    int choice;
    printf("Calendar Scheduling and Alarm System\n");

    while (1) {
        printf("\n1. Add Event\n2. View Events\n3. Check Alarms\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_event();
                break;
            case 2:
                view_events();
                break;
            case 3:
                check_alarms();
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void add_event() {
    if (event_count >= MAX_EVENTS) {
        printf("Event list is full! Cannot add more events.\n");
        return;
    }

    printf("Enter event details:\n");

    printf("Day (1-31): ");
    scanf("%d", &events[event_count].day);

    printf("Month (1-12): ");
    scanf("%d", &events[event_count].month);

    printf("Year: ");
    scanf("%d", &events[event_count].year);

    printf("Hour (0-23): ");
    scanf("%d", &events[event_count].hour);

    printf("Minute (0-59): ");
    scanf("%d", &events[event_count].minute);

    printf("Description: ");
    getchar(); // Consume the leftover newline
    fgets(events[event_count].description, sizeof(events[event_count].description), stdin);

    // Remove newline from the description
    events[event_count].description[strcspn(events[event_count].description, "\n")] = '\0';

    event_count++;
    printf("Event added successfully!\n");
}

void view_events() {
    if (event_count == 0) {
        printf("No events to show.\n");
        return;
    }

    printf("\nScheduled Events:\n");
    for (int i = 0; i < event_count; i++) {
        printf("%02d/%02d/%04d %02d:%02d - %s\n",
               events[i].day, events[i].month, events[i].year,
               events[i].hour, events[i].minute, events[i].description);
    }
}

void check_alarms() {
    time_t now;
    struct tm *current_time;

    // Get the current system time
    now = time(NULL);
    current_time = localtime(&now);

    int curr_day = current_time->tm_mday;
    int curr_month = current_time->tm_mon + 1;
    int curr_year = current_time->tm_year + 1900;
    int curr_hour = current_time->tm_hour;
    int curr_minute = current_time->tm_min;

    for (int i = 0; i < event_count; i++) {
        if (events[i].day == curr_day &&
            events[i].month == curr_month &&
            events[i].year == curr_year &&
            events[i].hour == curr_hour &&
            events[i].minute == curr_minute) {
            printf("\nALARM! Event happening now: %s\n", events[i].description);
        }
    }

    printf("Alarm check complete.\n");
}
