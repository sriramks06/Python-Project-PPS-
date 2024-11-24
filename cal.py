import datetime
import json
import os
import time
import tkinter as tk
from tkinter import messagebox, simpledialog, ttk
from plyer import notification

# Data storage (Using a JSON file to save events)
FILE_NAME = 'events.json'

# Load events from JSON file
def load_events():
    try:
        with open(FILE_NAME, 'r') as file:
            return json.load(file)
    except (FileNotFoundError, json.JSONDecodeError):
        return []

# Save events to JSON file
def save_events(events):
    with open(FILE_NAME, 'w') as file:
        json.dump(events, file, indent=4)

# Function to add a new event
def add_event_gui():
    title = simpledialog.askstring("Add Event", "Enter event title:")
    date_str = simpledialog.askstring("Add Event", "Enter event date (YYYY-MM-DD):")
    time_str = simpledialog.askstring("Add Event", "Enter event time (HH:MM):")
    recurring = simpledialog.askstring("Add Event", "Enter recurrence (none, daily, weekly, monthly):", initialvalue="none")

    try:
        event_datetime = datetime.datetime.strptime(date_str + ' ' + time_str, "%Y-%m-%d %H:%M")
        event = {
            'title': title,
            'datetime': event_datetime.strftime("%Y-%m-%d %H:%M"),
            'recurring': recurring.lower()
        }
        events = load_events()
        events.append(event)
        save_events(events)
        messagebox.showinfo("Success", "Event added successfully!")
        refresh_events()
    except ValueError:
        messagebox.showerror("Error", "Invalid date/time format. Please try again.")

# Function to view all scheduled events in a listbox
def refresh_events():
    events = load_events()
    event_listbox.delete(0, tk.END)
    for idx, event in enumerate(events):
        event_info = f"{idx + 1}. {event['title']} - {event['datetime']} - {event['recurring']}"
        event_listbox.insert(tk.END, event_info)

# Function to delete an event
def delete_event_gui():
    selected_index = event_listbox.curselection()
    if not selected_index:
        messagebox.showwarning("Warning", "Please select an event to delete.")
        return
    
    events = load_events()
    events.pop(selected_index[0])
    save_events(events)
    refresh_events()
    messagebox.showinfo("Success", "Event deleted successfully.")

# Function to search for events by title or date
def search_event_gui():
    search_term = simpledialog.askstring("Search Event", "Enter event title or date (YYYY-MM-DD):")
    if not search_term:
        return
    
    events = load_events()
    result = [f"{idx + 1}. {e['title']} - {e['datetime']} - {e['recurring']}" 
              for idx, e in enumerate(events) if search_term.lower() in e['title'].lower() or search_term in e['datetime']]
    
    if result:
        messagebox.showinfo("Search Results", "\n".join(result))
    else:
        messagebox.showinfo("Search Results", "No matching events found.")

# Function to check and trigger alarms
def check_alarms():
    events = load_events()
    current_time = datetime.datetime.now()
    
    # Debug statement to show current time
    print(f"Checking alarms at: {current_time.strftime('%Y-%m-%d %H:%M')}")

    updated_events = []
    for event in events:
        event_time = datetime.datetime.strptime(event['datetime'], "%Y-%m-%d %H:%M")
        time_difference = (event_time - current_time).total_seconds() / 60  # Time difference in minutes

        # Debug information
        print(f"Event '{event['title']}' scheduled for {event['datetime']}")
        print(f"Time difference is: {time_difference} minutes")

        if -1 <= time_difference <= 1:
            print(f"Alarm triggered for event: {event['title']}")  # Debug: Event trigger
            notification.notify(
                title="Event Reminder",
                message=f"Event '{event['title']}' is happening now!",
                timeout=10
            )
            
            # Adjust date for recurring events
            if event['recurring'] == 'daily':
                event_time += datetime.timedelta(days=1)
                event['datetime'] = event_time.strftime("%Y-%m-%d %H:%M")
                updated_events.append(event)
            elif event['recurring'] == 'weekly':
                event_time += datetime.timedelta(weeks=1)
                event['datetime'] = event_time.strftime("%Y-%m-%d %H:%M")
                updated_events.append(event)
            elif event['recurring'] == 'monthly':
                month = (event_time.month % 12) + 1
                year = event_time.year + (event_time.month // 12)
                event_time = event_time.replace(year=year, month=month)
                event['datetime'] = event_time.strftime("%Y-%m-%d %H:%M")
                updated_events.append(event)
        else:
            updated_events.append(event)

    save_events(updated_events)
    root.after(10000, check_alarms)  # Recheck every 10 seconds (10,000 milliseconds)

# GUI Setup
root = tk.Tk()
root.title("Calendar Scheduling & Alarm System")

frame = tk.Frame(root)
frame.pack(pady=10)

add_button = tk.Button(frame, text="Add Event", command=add_event_gui)
add_button.grid(row=0, column=0, padx=5)

delete_button = tk.Button(frame, text="Delete Event", command=delete_event_gui)
delete_button.grid(row=0, column=1, padx=5)

search_button = tk.Button(frame, text="Search Event", command=search_event_gui)
search_button.grid(row=0, column=2, padx=5)

refresh_button = tk.Button(frame, text="Refresh Events", command=refresh_events)
refresh_button.grid(row=0, column=3, padx=5)

event_listbox = tk.Listbox(root, width=70, height=15)
event_listbox.pack(pady=10)

refresh_events()  # Load events initially
root.after(10000, check_alarms)  # Start alarm checking loop
root.mainloop()
