# Student Health and Emergency Contact System
**CC104: Data Structures and Algorithms — Final Project**

A console-based application written in C that manages student health records and emergency contacts for a school setting. Built to demonstrate a comprehensive understanding of data structures and algorithms taught throughout the course.

---

## Overview

The system allows school administrators or clinic staff to:
- Add, view, update, and delete student health records
- Store up to 3 emergency contacts per student
- Flag students for medical attention and manage a clinic queue
- Search and sort student records efficiently
- Undo the last added record

---

## Data Structures Used

| # | Data Structure | Where It's Used |
|---|---|---|
| DS1 | **Singly Linked List** | Stores all student health records in insertion order. Supports dynamic add and remove. |
| DS2 | **Stack (Linked, LIFO)** | Undo the last added student record. The most recently added student is always undone first. |
| DS3 | **Array of Structs** | Stores up to 3 emergency contacts directly inside each Student node. Fixed-size, no dynamic allocation needed. |
| DS4 | **Queue (Linked, FIFO)** | Medical attention clinic queue. Students are attended to in the order they were flagged. |
| DS5 | **Hash Table** | Fast O(1) average student lookup by ID using separate chaining. Powers every operation that needs to find a student. |

---

## Algorithms Used

| # | Algorithm | Where It's Used |
|---|---|---|
| A1 | **Merge Sort — O(n log n)** | Sorts students alphabetically by name (option 6) and by ID as a prerequisite for binary search (option 7). |
| A2 | **Binary Search — O(log n)** | Searches a pre-sorted pointer array by student ID (option 7). |
| A3 | **djb2 Hash Function — O(k)** | Maps student ID strings to hash table bucket indices. Powers the O(1) average lookup in DS5. Formula: `h = h * 33 + c`, starting seed 5381. |

---

## Features

| Option | Feature | DS / Algorithm |
|---|---|---|
| 1 | Add Student Record | DS1 (Linked List), DS3 (Contacts Array), DS5 (Hash Table), DS2 (Stack) |
| 2 | Delete Student Record | DS1, DS5 |
| 3 | View All Students | DS1 |
| 4 | View Student Detail | DS5 |
| 5 | Update Emergency Contacts | DS5, DS3 |
| 6 | Sort Students by Name | A1 (Merge Sort) |
| 7 | Search Student by ID | A1 (Merge Sort pre-sort) + A2 (Binary Search) |
| 8 | Flag for Medical Attention | DS4 (Enqueue), DS5 |
| 9 | Process Medical Queue | DS4 (Dequeue), DS5 |
| 10 | Undo Last Add | DS2 (Stack Pop), DS1, DS5 |
| 11 | Hash Table Diagnostics | DS5, A3 |

---

## How to Compile and Run

**Requirements:** GCC compiler

**Compile:**
```bash
gcc -o health_system studentHealthandEmergencyContactSystem.c
```

**Run:**
```bash
./health_system
```

**On Windows (PowerShell):**
```powershell
gcc studentHealthandEmergencyContactSystem.c -o health_system
./health_system
```

---

## Project Structure

```
├── studentHealthandEmergencyContactSystem.c   # Full source code
└── README.md                                  # This file
```
