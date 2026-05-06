/* student_health_system.c
   Title   : Student Health & Emergency Contacts System
   Project : Final Project – Data Structures & Algorithms
 Data Structures:
   1. Array       – holds all student records (fixed-size, direct access)
   2. Linked List – emergency contacts per student (dynamic, heapallocated)
   3. Stack       – activity log, LIFO, stamped with the logged in user
Algorithms:
   1. Linear Search – find student by ID or name keyword  (O(n))
   2. Bubble Sort   – sort students A-Z before listing    (O(n^2))
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Constants */
#define MAX_STUDENTS   50
#define MAX_NAME       50
#define MAX_ID         15
#define MAX_PHONE      20
#define MAX_ADDRESS    100
#define MAX_CONDITION  100
#define MAX_LOG        160
#define LOG_CAP        20
#define ADMIN_PASS     "admin123"

/* Role IDs */
#define ROLE_STUDENT  0
#define ROLE_STAFF    1
#define ROLE_ADMIN    2

/* DATA STRUCTURE 2 – Linked List (Emergency Contacts) */
typedef struct ContactNode {
    char name[MAX_NAME];
    char relationship[MAX_NAME];
    char phone[MAX_PHONE];
    struct ContactNode *next;
} ContactNode;

/* DATA STRUCTURE 1 – Array of Student Records */
typedef struct {
    char id[MAX_ID];
    char name[MAX_NAME];
    int age;
    char bloodType[5];
    char address[MAX_ADDRESS];
    char condition[MAX_CONDITION];
    ContactNode *contacts;  
    int active;   
} Student;

Student students[MAX_STUDENTS];
int     studentCount = 0;

/* DATA STRUCTURE 3 – Stack (Activity Log) */
typedef struct {
    char entries[LOG_CAP][MAX_LOG];
    int  top;
} Stack;

Stack actLog;
char currentUser[MAX_NAME] = "Unknown";
int  currentRole = ROLE_STUDENT;

int isValidName(const char *s) {
    if (!s || strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i]) && s[i] != ' ') return 0;
    return 1;
}

int isValidPhone(const char *s) {
    int len = (int)strlen(s);
    if (len < 10 || len > 11) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit((unsigned char)s[i]) && s[i] != '-' && s[i] != '+' && s[i] != ' ')
            return 0;
    return 1;
}

int isValidBlood(const char *s) {
    const char *valid[] = { "A+","A-","B+","B-","AB+","AB-","O+","O-", NULL };
    for (int i = 0; valid[i]; i++)
        if (strcmp(s, valid[i]) == 0) return 1;
    return 0;
}

int isValidAge(const char *s, int *out) {
    if (!s || strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    *out = atoi(s);
    return (*out >= 1 && *out <= 120);
}

int isValidID(const char *s) {
    int len = (int)strlen(s);
    if (len == 0 || len >= MAX_ID) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalnum((unsigned char)s[i])) return 0;
    return 1;
}

void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin)) {
        int len = (int)strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    }
}

/*STACK OPERATIONS*/
void stackInit(void) { actLog.top = -1; }

void stackPush(const char *action) {
    if (actLog.top < LOG_CAP - 1) {
        actLog.top++;
    } else {
        for (int i = 0; i < LOG_CAP - 1; i++)
            strcpy(actLog.entries[i], actLog.entries[i + 1]);
    }
    snprintf(actLog.entries[actLog.top], MAX_LOG, "[%s] %s", currentUser, action);
}

void stackView(void) {
    if (actLog.top == -1) { printf("  Log is empty.\n"); return; }
    printf("\n  -- Activity Log (newest first) --\n");
    for (int i = actLog.top; i >= 0; i--)
        printf("  %2d. %s\n", actLog.top - i + 1, actLog.entries[i]);
}

/*LINKED LIST – Emergency Contact Operations*/
void contactAdd(Student *s, const char *name, const char *rel, const char *phone) {
    ContactNode *node = (ContactNode *)malloc(sizeof(ContactNode));
    if (!node) { printf("  Memory error.\n"); return; }

    strncpy(node->name,         name,  MAX_NAME  - 1); node->name[MAX_NAME - 1]         = '\0';
    strncpy(node->relationship, rel,   MAX_NAME  - 1); node->relationship[MAX_NAME - 1] = '\0';
    strncpy(node->phone,        phone, MAX_PHONE - 1); node->phone[MAX_PHONE - 1]        = '\0';
    node->next = NULL;

    if (!s->contacts) { s->contacts = node; return; }
    ContactNode *cur = s->contacts;
    while (cur->next) cur = cur->next;
    cur->next = node;
}

void contactViewAll(const Student *s) {
    if (!s->contacts) { printf("  No contacts on file.\n"); return; }
    ContactNode *cur = s->contacts;
    int i = 1;
    while (cur) {
        printf("    %d. %-22s | %-14s | %s\n",
               i++, cur->name, cur->relationship, cur->phone);
        cur = cur->next;
    }
}

int contactDelete(Student *s, int idx) {
    if (!s->contacts || idx < 1) return 0;
    ContactNode *cur = s->contacts;
    if (idx == 1) { s->contacts = cur->next; free(cur); return 1; }
    int i = 1;
    while (cur->next && i < idx - 1) { cur = cur->next; i++; }
    if (!cur->next) return 0;
    ContactNode *del = cur->next;
    cur->next = del->next;
    free(del);
    return 1;
}

void contactFreeAll(Student *s) {
    ContactNode *cur = s->contacts;
    while (cur) { ContactNode *nxt = cur->next; free(cur); cur = nxt; }
    s->contacts = NULL;
}

void contactInputLoop(Student *s) {
    char yn;
    printf("\n  Add an emergency contact? (y/n): ");
    scanf(" %c", &yn); getchar();

    while (yn == 'y' || yn == 'Y') {
        char cname[MAX_NAME], rel[MAX_NAME], phone[MAX_PHONE];

        do {
            printf("  Contact Name       : "); readLine(cname, sizeof(cname));
            if (!isValidName(cname)) printf("  ! Letters and spaces only.\n");
        } while (!isValidName(cname));

        printf("  Relationship       : "); readLine(rel, sizeof(rel));
        if (strlen(rel) == 0) strcpy(rel, "Unknown");

        do {
            printf("  Phone (7-15 digits): "); readLine(phone, sizeof(phone));
            if (!isValidPhone(phone)) printf("  ! Invalid phone (7-15 digits/dashes).\n");
        } while (!isValidPhone(phone));

        contactAdd(s, cname, rel, phone);
        printf("  Contact saved.\n");

        printf("  Add another? (y/n): ");
        scanf(" %c", &yn); getchar();
    }
}

/*ALGORITHM 1 – LINEAR SEARCH*/
int searchByID(const char *id) {
    for (int i = 0; i < MAX_STUDENTS; i++)
        if (students[i].active && strcmp(students[i].id, id) == 0) return i;
    return -1;
}

int searchByName(const char *kw) {
    char lkw[MAX_NAME], lname[MAX_NAME];
    int j;
    for (j = 0; kw[j] && j < MAX_NAME - 1; j++) lkw[j] = (char)tolower((unsigned char)kw[j]);
    lkw[j] = '\0';

    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (!students[i].active) continue;
        for (j = 0; students[i].name[j] && j < MAX_NAME - 1; j++)
            lname[j] = (char)tolower((unsigned char)students[i].name[j]);
        lname[j] = '\0';
        if (strstr(lname, lkw)) return i;
    }
    return -1;
}

/*ALGORITHM 2 – BUBBLE SORT (alphabetical A to Z)*/
void listSorted(void) {
    int idx[MAX_STUDENTS], n = 0;
    for (int i = 0; i < MAX_STUDENTS; i++)
        if (students[i].active) idx[n++] = i;

    if (n == 0) { printf("  No students registered yet.\n"); return; }

    for (int p = 0; p < n - 1; p++)
        for (int j = 0; j < n - p - 1; j++)
            if (strcmp(students[idx[j]].name, students[idx[j + 1]].name) > 0) {
                int tmp = idx[j]; idx[j] = idx[j + 1]; idx[j + 1] = tmp;
            }

    printf("\n  %-4s %-12s %-22s %-4s %-5s %-22s\n",
           "#", "ID", "Name", "Age", "Blood", "Condition");
    printf("  %-4s %-12s %-22s %-4s %-5s %-22s\n",
           "---","----------","--------------------","---","-----","--------------------");
    for (int i = 0; i < n; i++) {
        Student *s = &students[idx[i]];
        printf("  %-4d %-12s %-22s %-4d %-5s %-22s\n",
               i + 1, s->id, s->name, s->age, s->bloodType, s->condition);
    }
    printf("  Total: %d student(s)\n", n);
}

/*STUDENT DISPLAY – formatted record with contacts*/

void studentDisplay(int i) {
    Student *s = &students[i];
    printf("\n  ┌───────────────────────────────────────────┐\n");
    printf("  │          STUDENT HEALTH RECORD            │\n");
    printf("  ├───────────────────────────────────────────┤\n");
    printf("  │ ID         : %-28s│\n", s->id);
    printf("  │ Name       : %-28s│\n", s->name);
    printf("  │ Age        : %-28d│\n", s->age);
    printf("  │ Blood Type : %-28s│\n", s->bloodType);
    printf("  │ Address    : %-28s│\n", s->address);
    printf("  │ Condition  : %-28s│\n", s->condition);
    printf("  └───────────────────────────────────────────┘\n");
    printf("  Emergency Contacts:\n");
    contactViewAll(s);
}

void studentAdd(void) {
    if (studentCount >= MAX_STUDENTS) { printf("  Database is full.\n"); return; }

    int slot = -1;
    for (int i = 0; i < MAX_STUDENTS; i++)
        if (!students[i].active) { slot = i; break; }
    if (slot == -1) return;

    Student *s = &students[slot];
    printf("\n  -- Add New Student --\n");

    do {
        printf("  Student ID        : "); readLine(s->id, sizeof(s->id));
        if (!isValidID(s->id))
            printf("  ! Alphanumeric only, max 14 characters.\n");
        else if (searchByID(s->id) != -1) {
            printf("  ! That ID is already taken.\n");
            s->id[0] = '\0';   
        }
    } while (!isValidID(s->id));

    do {
        printf("  Full Name         : "); readLine(s->name, sizeof(s->name));
        if (!isValidName(s->name)) printf("  ! Letters and spaces only.\n");
    } while (!isValidName(s->name));

    char ageBuf[10];
    do {
        printf("  Age (1-120)       : "); readLine(ageBuf, sizeof(ageBuf));
        if (!isValidAge(ageBuf, &s->age)) printf("  ! Must be a number between 1 and 120.\n");
    } while (!isValidAge(ageBuf, &s->age));

    do {
        printf("  Blood Type        : "); readLine(s->bloodType, sizeof(s->bloodType));
        if (!isValidBlood(s->bloodType))
            printf("  ! Valid values: A+ A- B+ B- AB+ AB- O+ O-\n");
    } while (!isValidBlood(s->bloodType));

    do {
        printf("  Address           : "); readLine(s->address, sizeof(s->address));
        if (strlen(s->address) == 0) printf("  ! Address cannot be empty.\n");
    } while (strlen(s->address) == 0);

    do {
        printf("  Condition (or None): "); readLine(s->condition, sizeof(s->condition));
        if (strlen(s->condition) == 0) printf("  ! Enter a value or type 'None'.\n");
    } while (strlen(s->condition) == 0);

    s->contacts = NULL;
    s->active   = 1;
    studentCount++;

    contactInputLoop(s);

    char msg[MAX_LOG];
    snprintf(msg, sizeof(msg), "Added student: %s (ID: %s)", s->name, s->id);
    stackPush(msg);
    printf("\n  Student '%s' registered successfully.\n", s->name);
}

void studentSearch(void) {
    printf("\n  Search by: 1. ID   2. Name   > ");
    char ch[4]; readLine(ch, sizeof(ch));

    int idx = -1;
    if (ch[0] == '1') {
        char id[MAX_ID];
        do {
            printf("  Student ID: "); readLine(id, sizeof(id));
            if (!isValidID(id)) printf("  ! Invalid ID format.\n");
        } while (!isValidID(id));
        idx = searchByID(id);

    } else if (ch[0] == '2') {
        char kw[MAX_NAME];
        do {
            printf("  Name keyword: "); readLine(kw, sizeof(kw));
            if (strlen(kw) == 0) printf("  ! Keyword cannot be empty.\n");
        } while (strlen(kw) == 0);
        idx = searchByName(kw);

    } else { printf("  Invalid choice.\n"); return; }

    if (idx == -1) { printf("  Student not found.\n"); return; }

    if (currentRole == ROLE_STUDENT && strcmp(students[idx].id, currentUser) != 0) {
        printf("  Access denied. You may only view your own record.\n"); return;
    }

    studentDisplay(idx);
    char msg[MAX_LOG];
    snprintf(msg, sizeof(msg), "Viewed student: %s", students[idx].name);
    stackPush(msg);
}

void studentUpdate(void) {
    char id[MAX_ID];
    do {
        printf("\n  Student ID to update: "); readLine(id, sizeof(id));
        if (!isValidID(id)) printf("  ! Invalid ID format.\n");
    } while (!isValidID(id));

    int idx = searchByID(id);
    if (idx == -1) { printf("  Student not found.\n"); return; }

    Student *s = &students[idx];
    printf("  Updating record for: %s\n", s->name);
    printf("  1.Name  2.Age  3.Blood Type  4.Address  5.Condition   > ");
    char ch[4]; readLine(ch, sizeof(ch));

    switch (ch[0]) {
        case '1': {
            char tmp[MAX_NAME];
            do {
                printf("  New Name: "); readLine(tmp, sizeof(tmp));
                if (!isValidName(tmp)) printf("  ! Letters and spaces only.\n");
            } while (!isValidName(tmp));
            strcpy(s->name, tmp);
            break;
        }
        case '2': {
            char ageBuf[10];
            do {
                printf("  New Age: "); readLine(ageBuf, sizeof(ageBuf));
                if (!isValidAge(ageBuf, &s->age)) printf("  ! Must be 1-120.\n");
            } while (!isValidAge(ageBuf, &s->age));
            break;
        }
        case '3': {
            do {
                printf("  New Blood Type: "); readLine(s->bloodType, sizeof(s->bloodType));
                if (!isValidBlood(s->bloodType))
                    printf("  ! Valid: A+ A- B+ B- AB+ AB- O+ O-\n");
            } while (!isValidBlood(s->bloodType));
            break;
        }
        case '4': {
            do {
                printf("  New Address: "); readLine(s->address, sizeof(s->address));
                if (strlen(s->address) == 0) printf("  ! Cannot be empty.\n");
            } while (strlen(s->address) == 0);
            break;
        }
        case '5': {
            do {
                printf("  New Condition: "); readLine(s->condition, sizeof(s->condition));
                if (strlen(s->condition) == 0) printf("  ! Cannot be empty.\n");
            } while (strlen(s->condition) == 0);
            break;
        }
        default: printf("  Invalid choice.\n"); return;
    }

    char msg[MAX_LOG];
    snprintf(msg, sizeof(msg), "Updated student: %s (ID: %s)", s->name, s->id);
    stackPush(msg);
    printf("  Record updated successfully.\n");
}

void studentDelete(void) {
    char id[MAX_ID];
    do {
        printf("\n  Student ID to delete: "); readLine(id, sizeof(id));
        if (!isValidID(id)) printf("  ! Invalid ID format.\n");
    } while (!isValidID(id));

    int idx = searchByID(id);              
    if (idx == -1) { printf("  Student not found.\n"); return; }

    printf("  About to delete: %s\n", students[idx].name);
    char yn;
    printf("  Confirm delete? (y/n): ");
    scanf(" %c", &yn); getchar();
    if (yn != 'y' && yn != 'Y') { printf("  Deletion cancelled.\n"); return; }

    char msg[MAX_LOG];
    snprintf(msg, sizeof(msg), "Deleted student: %s (ID: %s)",
             students[idx].name, students[idx].id);
    stackPush(msg);

    contactFreeAll(&students[idx]);
    memset(&students[idx], 0, sizeof(Student));
    studentCount--;
    printf("  Student deleted.\n");
}

void contactsManage(void) {
    char id[MAX_ID];
    do {
        printf("\n  Student ID: "); readLine(id, sizeof(id));
        if (!isValidID(id)) printf("  ! Invalid ID format.\n");
    } while (!isValidID(id));

    int idx = searchByID(id);
    if (idx == -1) { printf("  Student not found.\n"); return; }

    Student *s = &students[idx];
    printf("\n  Contacts for: %s\n", s->name);
    printf("  1. View   2. Add   3. Delete   > ");
    char ch[4]; readLine(ch, sizeof(ch));

    if (ch[0] == '1') {
        contactViewAll(s);

    } else if (ch[0] == '2') {
        contactInputLoop(s);
        char msg[MAX_LOG];
        snprintf(msg, sizeof(msg), "Added contact(s) to student: %s", s->name);
        stackPush(msg);

    } else if (ch[0] == '3') {
        contactViewAll(s);
        if (!s->contacts) return;

        printf("  Contact number to remove: ");
        char numBuf[6]; readLine(numBuf, sizeof(numBuf));

        int valid = 1, num = 0;
        for (int i = 0; numBuf[i]; i++)
            if (!isdigit((unsigned char)numBuf[i])) { valid = 0; break; }
        if (!valid || (num = atoi(numBuf)) < 1) {
            printf("  ! Invalid number.\n"); return;
        }

        if (contactDelete(s, num)) {
            char msg[MAX_LOG];
            snprintf(msg, sizeof(msg), "Removed contact #%d from student: %s", num, s->name);
            stackPush(msg);
            printf("  Contact removed.\n");
        } else {
            printf("  ! Contact number out of range.\n");
        }
    } else {
        printf("  Invalid choice.\n");
    }
}

/*LOGIN*/
void doLogin(void) {
    printf("\n  ╔════════════════════════════════════════════╗\n");
    printf("  ║  STUDENT HEALTH & EMERGENCY CONTACTS SYS  ║\n");
    printf("  ╚════════════════════════════════════════════╝\n");
    printf("\n  Login as:\n");
    printf("  1. Student  (view own record only)\n");
    printf("  2. Staff    (add, search, update, contacts)\n");
    printf("  3. Admin    (full access + activity log)\n");
    printf("  Choice: ");

    char ch[4]; readLine(ch, sizeof(ch));

    if (ch[0] == '1') {
        currentRole = ROLE_STUDENT;
        do {
            printf("  Your Student ID: "); readLine(currentUser, sizeof(currentUser));
            if (!isValidID(currentUser)) printf("  ! Invalid ID format.\n");
        } while (!isValidID(currentUser));
        printf("  Logged in as Student: %s\n", currentUser);

    } else if (ch[0] == '2') {
        currentRole = ROLE_STAFF;
        do {
            printf("  Your name: "); readLine(currentUser, sizeof(currentUser));
            if (!isValidName(currentUser)) printf("  ! Letters and spaces only.\n");
        } while (!isValidName(currentUser));
        printf("  Logged in as Staff: %s\n", currentUser);

    } else if (ch[0] == '3') {
        currentRole = ROLE_ADMIN;
        do {
            printf("  Your name: "); readLine(currentUser, sizeof(currentUser));
            if (!isValidName(currentUser)) printf("  ! Letters and spaces only.\n");
        } while (!isValidName(currentUser));

        int tries = 3, ok = 0;
        while (tries-- > 0) {
            char pass[32];
            printf("  Password: "); readLine(pass, sizeof(pass));
            if (strcmp(pass, ADMIN_PASS) == 0) { ok = 1; break; }
            printf("  ! Wrong password. %d attempt(s) left.\n", tries);
        }
        if (!ok) {
            printf("  Too many failed attempts. Logging in as Staff instead.\n");
            currentRole = ROLE_STAFF;
        } else {
            printf("  Logged in as Admin: %s\n", currentUser);
        }
    } else {
        printf("  Invalid choice. Defaulting to Student login.\n");
        currentRole = ROLE_STUDENT;
        strcpy(currentUser, "Unknown");
    }
}

/*SESSION LOOPS – each role sees a different menu*/
void runStudentSession(void) {
    char ch[4];
    do {
        printf("\n  ┌─── STUDENT MENU ─────────────────────────┐\n");
        printf("  │  Logged in as: %-26s│\n", currentUser);
        printf("  ├──────────────────────────────────────────┤\n");
        printf("  │  1. View My Record                       │\n");
        printf("  │  0. Logout                               │\n");
        printf("  └──────────────────────────────────────────┘\n");
        printf("  Choice: ");
        readLine(ch, sizeof(ch));

        if (ch[0] == '1') {
            int idx = searchByID(currentUser);
            if (idx == -1)
                printf("  No record found for ID '%s'.\n", currentUser);
            else {
                studentDisplay(idx);
                stackPush("Viewed own record");
            }
        } else if (ch[0] != '0') {
            printf("  Invalid choice.\n");
        }
    } while (ch[0] != '0');
}

void runStaffSession(void) {
    char ch[4];
    do {
        printf("\n  ┌─── STAFF MENU ───────────────────────────┐\n");
        printf("  │  Logged in as: %-26s│\n", currentUser);
        printf("  ├──────────────────────────────────────────┤\n");
        printf("  │  1. Add Student                          │\n");
        printf("  │  2. Search / View Student                │\n");
        printf("  │  3. Update Student Info                  │\n");
        printf("  │  4. Manage Emergency Contacts            │\n");
        printf("  │  5. List All Students (Sorted A-Z)       │\n");
        printf("  │  0. Logout                               │\n");
        printf("  └──────────────────────────────────────────┘\n");
        printf("  Choice: ");
        readLine(ch, sizeof(ch));

        switch (ch[0]) {
            case '1': studentAdd();     break;
            case '2': studentSearch();  break;
            case '3': studentUpdate();  break;
            case '4': contactsManage(); break;
            case '5': listSorted();     break;
            case '0': break;
            default:  printf("  Invalid choice.\n");
        }
    } while (ch[0] != '0');
}

void runAdminSession(void) {
    char ch[4];
    do {
        printf("\n  ┌─── ADMIN MENU ───────────────────────────┐\n");
        printf("  │  Logged in as: %-26s│\n", currentUser);
        printf("  ├──────────────────────────────────────────┤\n");
        printf("  │  1. Add Student                          │\n");
        printf("  │  2. Search / View Student                │\n");
        printf("  │  3. Update Student Info                  │\n");
        printf("  │  4. Delete Student                       │\n");
        printf("  │  5. Manage Emergency Contacts            │\n");
        printf("  │  6. List All Students (Sorted A-Z)       │\n");
        printf("  │  7. View Activity Log                    │\n");
        printf("  │  0. Logout                               │\n");
        printf("  └──────────────────────────────────────────┘\n");
        printf("  Choice: ");
        readLine(ch, sizeof(ch));

        switch (ch[0]) {
            case '1': studentAdd();     break;
            case '2': studentSearch();  break;
            case '3': studentUpdate();  break;
            case '4': studentDelete();  break;
            case '5': contactsManage(); break;
            case '6': listSorted();     break;
            case '7': stackView();      break;
            case '0': break;
            default:  printf("  Invalid choice.\n");
        }
    } while (ch[0] != '0');
}

/*MAIN*/
int main(void) {
    memset(students, 0, sizeof(students));
    stackInit();

    char again;
    do {
        doLogin();
        if      (currentRole == ROLE_STUDENT) runStudentSession();
        else if (currentRole == ROLE_STAFF)   runStaffSession();
        else                                  runAdminSession();

        stackPush("User logged out");

        printf("\n  Switch user / login again? (y/n): ");
        scanf(" %c", &again); getchar();

    } while (again == 'y' || again == 'Y');

    printf("\n  Goodbye!\n\n");

    for (int i = 0; i < MAX_STUDENTS; i++)
        if (students[i].active) contactFreeAll(&students[i]);

    return 0;
}
