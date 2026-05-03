# STUDENT HEALTH & EMERGENCY CONTACT SYSTEM 
# DSA Final Project

---

# OVERVIEW
STUDENT HEALTH & EMERGENCY CONTACT SYSTEM is a student health record manager built in C for school use. It runs entirely in the terminal with no graphical user interface. The system uses a role-based login, three types of users exist, each with different access levels. Students can only view their own health record. Staff can add, search, update, and manage records. Admins have full access including deleting records and viewing the activity log. When logging in, every action taken during that session is recorded in an activity log stamped with the user's name so there is always a trace of who did what

---

# DATA STRUCTURES AND ALGORTIHMS USED
1. Array - stores all student records in a fixed 50-slot array. Each slot holds one complete student profile including their ID, name, age, blood type, address, and medical condition. A slot is marked active or free using a flag, so deleted records release their slot for reuse
Features it supports: adding students, searching, updating, deleting, and listing all records

2. Linked List - each student has their own chain of emergency contact nodes. Every node stores a contact's name, relationship, and phone number, and points to the next contact in the chain. Nodes are created in heap memory when a contact is added and freed when removed
Features it supports: adding emergency contacts during registration, viewing contacts, and removing individual contacts per student

3. Stack - every action in the system, from adding, updating, deleting, searching, to logging out is pushed onto a stack of up to 20 entries. Each entry is stamped with the logged-in user's name. The most recent action always sits on top. When the stack is full, the oldest entry is dropped to make room for the new one
Features it supports: the activity log that admins can view to audit all system actions

1. Linear Search - scans the student array from the first slot to the last until a match is found. Returns the position of the match or signals not found. The name search converts both the keyword and stored names to lowercase before comparing, so searching cruz finds Juan dela Cruz
Features it supports: Search/View student, Update student, Delete student, Manage contacts, and the student login check
Complexity: O(n)

2. Bubble Sort - before displaying the full student list, slot numbers of all active students are collected into a temporary index array and sorted by comparing student names. The actual student records never move, only the order of the index numbers changes. After each pass through the list, the alphabetically last unsorted name settles into its final position
Features it supports: List All Students, displays every student sorted A–Z by name
Complexity: O(n²)

---

# HOW TO RUN 
REQUIREMENTS: GCC or any C99-compatible compiler 

# Step 1 — Compile
gcc -o student_health student_health_system.c

# Step 2 — Run
./student_health        # for Linux / macOS
student_health.exe      # for Windows

---

# ACCOUNT LOGIN GUIDE
+-------------+------------+------------+ \n
|    ROLE     | LOGIN WITH | PASSWORD   | \n
+-------------+------------+------------+ \n
| 1. STUDENT  | student id |   none     | \n
| 2. STAFF    | name       |   none     | \n
| 3. ADMIN    | name       | admin123   | \n
+-------------+------------+------------+ \n

---
