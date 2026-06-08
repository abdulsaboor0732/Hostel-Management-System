#ifndef HOSTEL_H
#define HOSTEL_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

const int MAX = 100;

// Global Helper Functions
void line();
void clearIn();
bool readStr(char* buf, int size, const char* prompt, bool flushFirst = false);
bool readInt(int& out, const char* prompt);
bool readFloat(float& out, const char* prompt);

// BASE CLASS : Person
class Person {
public:
    char name[50], contact[15];
    Person();
};

// DERIVED CLASS : Student
class Student : public Person {
public:
    char  id[15];
    int   room;
    float fee;
    bool  paid;
    Student();
    void display() const;
};

// CLASS : Room
class Room {
public:
    int  num;
    char type[10];
    bool available;
    Room();
    void display() const;
};

// MAIN CLASS : Hostel
class Hostel {
private:
    Student students[MAX];
    Room    rooms[MAX];
    int     sc, rc;

    static void toUpperID(char* id);
    int findS(const char* id) const;
    int findR(int num) const;
    void saveStudents();
    void loadStudents();
    void saveRooms();
    void loadRooms();

public:
    Hostel();
    ~Hostel();

    // Student Functions
    void addStudent();
    void showStudents();
    void searchStudent();
    void editStudent();
    void deleteStudent();

    // Room Functions
    void addRoom();
    void showRooms();
    void checkRoom();
    void deleteRoom();
    void whoIsInRoom();

    // Fee Functions
    void assignFee();
    void updateFeeStatus();
    void showFees();

    // Menus
    void studentMenu();
    void roomMenu();
    void feeMenu();
    void mainMenu();
};

#endif // HOSTEL_H