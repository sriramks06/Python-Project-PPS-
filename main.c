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
int is_valid_date(int day, int month, int year);

int main() {
    int choice;
    printf("Calendar Scheduling and Alarm System\n");

    while (1) {
        printf("\n1. Add Event\n2. View Events\n3. Check Alarms\n4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

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

    Event new_event;
    printf("Enter event details:\n");

    printf("Day (1-31): ");
    scanf("%d", &new_event.day);

    printf("Month (1-12): ");
    scanf("%d", &new_event.month);

    printf("Year: ");
    scanf("%d", &new_event.year);

    if (!is_valid_date(new_event.day, new_event.month, new_event.year)) {
        printf("Invalid date! Please try again.\n");
        return;
    }

    printf("Hour (0-23): ");
    scanf("%d", &new_event.hour);

    printf("Minute (0-59): ");
    scanf("%d", &new_event.minute);

    if (new_event.hour < 0 || new_event.hour > 23 || new_event.minute < 0 || new_event.minute > 59) {
        printf("Invalid time! Please try again.\n");
        return;
    }

    printf("Description: ");
    getchar(); // Consume the leftover newline
    fgets(new_event.description, sizeof(new_event.description), stdin);
    new_event.description[strcspn(new_event.description, "\n")] = '\0'; // Remove newline

    events[event_count++] = new_event;
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
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    int curr_day = current_time->tm_mday;
    int curr_month = current_time->tm_mon + 1;
    int curr_year = current_time->tm_year + 1900;
    int curr_hour = current_time->tm_hour;
    int curr_minute = current_time->tm_min;

    int alarm_triggered = 0;

    printf("\nChecking alarms for current time: %02d/%02d/%04d %02d:%02d\n",
           curr_day, curr_month, curr_year, curr_hour, curr_minute);

    for (int i = 0; i < event_count; i++) {
        if (events[i].day == curr_day &&
            events[i].month == curr_month &&
            events[i].year == curr_year &&
            events[i].hour == curr_hour &&
            (events[i].minute == curr_minute || events[i].minute == curr_minute - 1 || events[i].minute == curr_minute + 1)) {
            printf("\nALARM! Event happening now or nearby: %s\n", events[i].description);
            alarm_triggered = 1;
        }
    }

    if (!alarm_triggered) {
        printf("No alarms at this time.\n");
    }

    printf("Alarm check complete.\n");
}

int is_valid_date(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1) return 0;

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29; // Leap year
    }

    return day <= days_in_month[month - 1];
}
