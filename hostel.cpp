#include "hostel.h"
using namespace std;

// =============================================
//  GLOBAL UTILITIES
// =============================================
void line()    { cout << "========================================\n"; }
void clearIn() { cin.clear(); cin.ignore(1000, '\n'); }

bool readStr(char* buf, int size, const char* prompt, bool flushFirst) {
    if (flushFirst) clearIn();
    cout << prompt;
    cin.getline(buf, size);
    if (strlen(buf) == 0) { cout << "  [!] Field cannot be empty.\n"; return false; }
    return true;
}

bool readInt(int& out, const char* prompt) {
    cout << prompt; cin >> out;
    if (cin.fail() || out <= 0) { clearIn(); cout << "  [!] Enter a positive number.\n"; return false; }
    clearIn(); return true;
}

bool readFloat(float& out, const char* prompt) {
    cout << prompt; cin >> out;
    if (cin.fail() || out < 0) { clearIn(); cout << "  [!] Enter a valid amount.\n"; return false; }
    clearIn(); return true;
}

// =============================================
//  CLASS METHODS IMPLEMENTATION
// =============================================

Person::Person() { strcpy(name, ""); strcpy(contact, ""); }

Student::Student() { strcpy(id, ""); room = 0; fee = 0; paid = false; }

void Student::display() const {
    cout << "  Name    : " << name    << "\n"
         << "  ID      : " << id      << "\n"
         << "  Contact : " << contact << "\n"
         << "  Room    : " << room    << "\n"
         << "  Fee     : Rs." << fee  << "\n"
         << "  Status  : " << (paid ? "PAID" : "UNPAID") << "\n";
}

Room::Room() { num = 0; strcpy(type, ""); available = true; }

void Room::display() const {
    cout << "  Room No : " << num  << "\n"
         << "  Type    : " << type << "\n"
         << "  Status  : " << (available ? "Available" : "Occupied") << "\n";
}

// =============================================
//  HOSTEL CLASS METHODS
// =============================================

Hostel::Hostel() : sc(0), rc(0) { loadStudents(); loadRooms(); }
Hostel::~Hostel() { saveStudents(); saveRooms(); }

void Hostel::toUpperID(char* id) {
    for (int i = 0; id[i]; i++)
        if (id[i] >= 'a' && id[i] <= 'z') id[i] = (char)(id[i] - 32);
}

int Hostel::findS(const char* id) const {
    for (int i = 0; i < sc; i++)
        if (strcmp(students[i].id, id) == 0) return i;
    return -1;
}

int Hostel::findR(int num) const {
    for (int i = 0; i < rc; i++)
        if (rooms[i].num == num) return i;
    return -1;
}

void Hostel::saveStudents() {
    ofstream f("students.txt");
    if (!f) { cout << "  [!] Warning: Could not save students to file.\n"; return; }
    for (int i = 0; i < sc; i++)
        f << students[i].name    << "|"
          << students[i].id      << "|"
          << students[i].contact << "|"
          << students[i].room    << "|"
          << students[i].fee     << "|"
          << students[i].paid    << "\n";
}

void Hostel::loadStudents() {
    ifstream f("students.txt"); if (!f) return;
    char ln[200];
    while (sc < MAX && f.getline(ln, 200)) {
        Student& s = students[sc];
        char* tok = strtok(ln,   "|"); if (!tok) continue; strncpy(s.name,    tok, 49); s.name[49]    = '\0';
              tok = strtok(NULL, "|"); if (!tok) continue; strncpy(s.id,      tok, 14); s.id[14]      = '\0';
              tok = strtok(NULL, "|"); if (!tok) continue; strncpy(s.contact, tok, 14); s.contact[14] = '\0';
              tok = strtok(NULL, "|"); if (!tok) continue;
              s.room = atoi(tok);
              if (s.room <= 0) {
                  cout << "  [!] Warning: skipping corrupt student record (invalid room).\n";
                  continue;
              }
              tok = strtok(NULL, "|"); if (!tok) continue; s.fee  = (float)atof(tok);
              tok = strtok(NULL, "|"); if (!tok) continue; s.paid = (atoi(tok) != 0);
        sc++;
    }
}

void Hostel::saveRooms() {
    ofstream f("rooms.txt");
    if (!f) { cout << "  [!] Warning: Could not save rooms to file.\n"; return; }
    for (int i = 0; i < rc; i++)
        f << rooms[i].num << "|" << rooms[i].type << "|" << rooms[i].available << "\n";
}

void Hostel::loadRooms() {
    ifstream f("rooms.txt"); if (!f) return;
    char ln[100];
    while (rc < MAX && f.getline(ln, 100)) {
        Room& r = rooms[rc];
        char* tok = strtok(ln,   "|"); if (!tok) continue; r.num = atoi(tok);
              tok = strtok(NULL, "|"); if (!tok) continue; strncpy(r.type, tok, 9); r.type[9] = '\0';
              tok = strtok(NULL, "|"); if (!tok) continue; r.available = (atoi(tok) != 0);
        rc++;
    }
}

void Hostel::addStudent() {
    if (sc >= MAX) { cout << "  [!] Hostel full.\n"; return; }
    Student s;
    if (!readStr(s.name, 50, "  Name    : ", true)) return;
    {
        bool hasLetter = false;
        for (int i = 0; s.name[i]; i++)
            if ((s.name[i] >= 'a' && s.name[i] <= 'z') || (s.name[i] >= 'A' && s.name[i] <= 'Z')) { hasLetter = true; break; }
        if (!hasLetter) { cout << "  [!] Name must contain at least one letter.\n"; return; }
    }
    if (!readStr(s.id, 15, "  ID      : ")) return;
    toUpperID(s.id);
    if (findS(s.id) != -1) { cout << "  [!] ID already exists.\n"; return; }
    if (!readStr(s.contact, 15, "  Contact : ")) return;
    {
        int clen = (int)strlen(s.contact);
        for (int i = 0; s.contact[i]; i++) {
            if (s.contact[i] < '0' || s.contact[i] > '9') { cout << "  [!] Contact must be digits only.\n"; return; }
        }
        if (clen < 10 || clen > 13) { cout << "  [!] Contact must be 10-13 digits.\n"; return; }
    }
    if (!readInt(s.room, "  Room No : ")) return;
    if (s.room > 9999) { cout << "  [!] Room number too large.\n"; return; }
    int ri = findR(s.room);
    if (ri == -1)             { cout << "  [!] Room " << s.room << " does not exist.\n";      return; }
    if (!rooms[ri].available) { cout << "  [!] Room " << s.room << " is already occupied.\n"; return; }
    if (!readFloat(s.fee, "  Fee     : ")) return;
    if (s.fee == 0.0f) {
        char yn[4];
        cout << "  [?] Fee is Rs.0 — confirm? (y/n): ";
        cin.getline(yn, 4);
        if (yn[0] != 'y' && yn[0] != 'Y') { cout << "  [!] Cancelled.\n"; return; }
    }
    s.paid = false;
    rooms[ri].available = false;
    saveRooms();
    students[sc++] = s;
    saveStudents();
    cout << "  [+] Student added!\n";
}

void Hostel::showStudents() {
    if (sc == 0) { cout << "  No students found.\n"; return; }
    for (int i = 0; i < sc; i++) {
        cout << "  --- Student #" << (i + 1) << " ---\n";
        students[i].display();
    }
}

void Hostel::searchStudent() {
    char id[15];
    if (!readStr(id, 15, "  Enter ID: ", true)) return;
    toUpperID(id);
    int i = findS(id);
    if (i == -1) cout << "  [!] Not found.\n"; else students[i].display();
}

void Hostel::editStudent() {
    char id[15];
    if (!readStr(id, 15, "  Enter ID to edit: ", true)) return;
    toUpperID(id);
    int i = findS(id);
    if (i == -1) { cout << "  [!] Not found.\n"; return; }
    cout << "  Leave a field blank to keep current value.\n";

    char nameBuf[50];
    cout << "  Name    [" << students[i].name << "]: ";
    cin.getline(nameBuf, 50);
    if (strlen(nameBuf) > 0) {
        strncpy(students[i].name, nameBuf, 49);
        students[i].name[49] = '\0';
    }

    char contactBuf[15];
    cout << "  Contact [" << students[i].contact << "]: ";
    cin.getline(contactBuf, 15);
    if (strlen(contactBuf) > 0) {
        bool ok = true;
        for (int j = 0; contactBuf[j]; j++) {
            if (contactBuf[j] < '0' || contactBuf[j] > '9') { ok = false; break; }
        }
        if (!ok) { cout << "  [!] Contact must be digits only. Unchanged.\n"; }
        else {
            int clen = (int)strlen(contactBuf);
            if (clen < 10 || clen > 13) { cout << "  [!] Contact must be 10-13 digits. Unchanged.\n"; }
            else { strncpy(students[i].contact, contactBuf, 14); students[i].contact[14] = '\0'; }
        }
    }

    cout << "  Room    [" << students[i].room << "]: ";
    int newRoom = 0;
    cin >> newRoom; clearIn();
    if (newRoom > 0 && newRoom != students[i].room) {
        int newRi = findR(newRoom);
        if (newRi == -1) { cout << "  [!] Room does not exist. Unchanged.\n"; }
        else if (!rooms[newRi].available) { cout << "  [!] Room occupied. Unchanged.\n"; }
        else {
            int oldRi = findR(students[i].room);
            if (oldRi != -1) rooms[oldRi].available = true;
            rooms[newRi].available = false;
            students[i].room = newRoom;
            saveRooms();
        }
    }
    saveStudents();
    cout << "  [*] Student updated!\n";
}

void Hostel::deleteStudent() {
    char id[15];
    if (!readStr(id, 15, "  Enter ID: ", true)) return;
    toUpperID(id);
    int i = findS(id);
    if (i == -1) { cout << "  [!] Not found.\n"; return; }

    cout << "  Confirm delete student '" << students[i].name << "' (y/n): ";
    char yn[4]; cin.getline(yn, 4);
    if (yn[0] != 'y' && yn[0] != 'Y') { cout << "  [!] Cancelled.\n"; return; }

    int ri = findR(students[i].room);
    if (ri != -1) rooms[ri].available = true;

    for (int j = i; j < sc - 1; j++) students[j] = students[j + 1];
    sc--;
    saveStudents(); saveRooms();
    cout << "  [-] Student deleted!\n";
}

void Hostel::addRoom() {
    if (rc >= MAX) { cout << "  [!] Room limit reached.\n"; return; }
    int num;
    if (!readInt(num, "  Room No : ")) return;
    if (findR(num) != -1) { cout << "  [!] Room already exists.\n"; return; }

    int ch;
    cout << "  Type: 1=Single  2=Double  3=Triple\n  Choice: "; cin >> ch;
    if (cin.fail() || ch < 1 || ch > 3) { clearIn(); cout << "  [!] Invalid choice.\n"; return; }
    clearIn();

    const char* types[] = { "Single", "Double", "Triple" };
    rooms[rc].num = num;
    strncpy(rooms[rc].type, types[ch - 1], 9);
    rooms[rc].type[9] = '\0';
    rooms[rc].available = true;
    rc++;
    saveRooms();
    cout << "  [+] Room added!\n";
}

void Hostel::showRooms() {
    if (rc == 0) { cout << "  No rooms found.\n"; return; }
    for (int i = 0; i < rc; i++) {
        cout << "  --- Room #" << (i + 1) << " ---\n";
        rooms[i].display();
    }
}

void Hostel::checkRoom() {
    int num;
    if (!readInt(num, "  Room No: ")) return;
    int i = findR(num);
    if (i == -1) cout << "  [!] Room not found.\n";
    else cout << "  Room " << num << " is: " << (rooms[i].available ? "Available" : "Occupied") << "\n";
}

void Hostel::deleteRoom() {
    int num;
    if (!readInt(num, "  Room No to delete: ")) return;
    int i = findR(num);
    if (i == -1)             { cout << "  [!] Room not found.\n"; return; }
    if (!rooms[i].available) { cout << "  [!] Cannot delete — room is occupied.\n"; return; }

    cout << "  Confirm delete room " << num << " (y/n): ";
    char yn[4]; cin.getline(yn, 4);
    if (yn[0] != 'y' && yn[0] != 'Y') { cout << "  [!] Cancelled.\n"; return; }

    for (int j = i; j < rc - 1; j++) rooms[j] = rooms[j + 1];
    rc--;
    saveRooms();
    cout << "  [-] Room deleted!\n";
}

void Hostel::whoIsInRoom() {
    int num;
    if (!readInt(num, "  Room No: ")) return;
    if (findR(num) == -1) { cout << "  [!] Room not found.\n"; return; }
    bool found = false;
    for (int i = 0; i < sc; i++) {
        if (students[i].room == num) { students[i].display(); found = true; }
    }
    int ri = findR(num);
    if (!found && rooms[ri].available)
        cout << "  Room " << num << " is empty.\n";
    else if (!found && !rooms[ri].available)
        cout << "  [!] Room is marked Occupied but no student is assigned — data may be out of sync.\n";
}

void Hostel::assignFee() {
    char id[15];
    if (!readStr(id, 15, "  Student ID: ", true)) return;
    toUpperID(id);
    int i = findS(id);
    if (i == -1) { cout << "  [!] Not found.\n"; return; }
    float fee;
    if (!readFloat(fee, "  Fee Amount: ")) return;

    if (fee != students[i].fee && students[i].paid) {
        cout << "  [!] Student is marked PAID. Fee changed — resetting to UNPAID.\n";
        students[i].paid = false;
    }
    students[i].fee = fee;
    saveStudents();
    cout << "  [+] Fee assigned!\n";
}

void Hostel::updateFeeStatus() {
    char id[15];
    if (!readStr(id, 15, "  Student ID: ", true)) return;
    toUpperID(id);
    int i = findS(id);
    if (i == -1) { cout << "  [!] Not found.\n"; return; }
    int ch; cout << "  1=Paid  2=Unpaid\n  Choice: "; cin >> ch;
    if (cin.fail() || ch < 1 || ch > 2) { clearIn(); cout << "  [!] Invalid.\n"; return; }
    clearIn();
    students[i].paid = (ch == 1);
    saveStudents();
    cout << "  [*] Fee status updated!\n";
}

void Hostel::showFees() {
    if (sc == 0) { cout << "  No students found.\n"; return; }
    for (int i = 0; i < sc; i++)
        cout << "  " << students[i].name << " | ID: " << students[i].id
             << " | Rs." << students[i].fee
             << " | " << (students[i].paid ? "PAID" : "UNPAID") << "\n";
}

void Hostel::studentMenu() {
    int ch;
    do {
        line(); cout << "    STUDENT MANAGEMENT\n"; line();
        cout << "  1. Add Student\n  2. Show All\n  3. Search\n  4. Edit\n  5. Delete\n  0. Back\n";
        line(); cout << "  Choice: "; cin >> ch;
        if (cin.fail()) { clearIn(); ch = -1; } else clearIn();
        cout << "\n";
        if      (ch == 1) addStudent();
        else if (ch == 2) showStudents();
        else if (ch == 3) searchStudent();
        else if (ch == 4) editStudent();
        else if (ch == 5) deleteStudent();
        else if (ch != 0) cout << "  [!] Invalid option.\n";
    } while (ch != 0);
}

void Hostel::roomMenu() {
    int ch;
    do {
        line(); cout << "    ROOM MANAGEMENT\n"; line();
        cout << "  1. Add Room\n  2. Show All\n  3. Check Availability\n  4. Who Is In Room\n  5. Delete Room\n  0. Back\n";
        line(); cout << "  Choice: "; cin >> ch;
        if (cin.fail()) { clearIn(); ch = -1; } else clearIn();
        cout << "\n";
        if      (ch == 1) addRoom();
        else if (ch == 2) showRooms();
        else if (ch == 3) checkRoom();
        else if (ch == 4) whoIsInRoom();
        else if (ch == 5) deleteRoom();
        else if (ch != 0) cout << "  [!] Invalid option.\n";
    } while (ch != 0);
}

void Hostel::feeMenu() {
    int ch;
    do {
        line(); cout << "    FEE MANAGEMENT\n"; line();
        cout << "  1. Assign Fee\n  2. Update Fee Status\n  3. Show All Fees\n  0. Back\n";
        line(); cout << "  Choice: "; cin >> ch;
        if (cin.fail()) { clearIn(); ch = -1; } else clearIn();
        cout << "\n";
        if      (ch == 1) assignFee();
        else if (ch == 2) updateFeeStatus();
        else if (ch == 3) showFees();
        else if (ch != 0) cout << "  [!] Invalid option.\n";
    } while (ch != 0);
}

void Hostel::mainMenu() {
    int ch;
    do {
        line(); cout << "   HOSTEL MANAGEMENT SYSTEM\n"; line();
        cout << "  1. Student Management\n  2. Room Management\n  3. Fee Management\n  0. Exit\n";
        line(); cout << "  Choice: "; cin >> ch;
        if (cin.fail()) { clearIn(); ch = -1; } else clearIn();
        cout << "\n";
        if      (ch == 1) studentMenu();
        else if (ch == 2) roomMenu();
        else if (ch == 3) feeMenu();
        else if (ch != 0) cout << "  [!] Invalid option.\n";
    } while (ch != 0);
    line(); cout << "  Goodbye! Have a great day.\n"; line();
}