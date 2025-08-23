#include <iostream>
using namespace std;

class Course {
private:
    string instructorName;
    string courseName;
    string courseCode;
    int credits;

public:
    // Constructor create the initial value 
    Course(string instructor, string name, string code, int credit)
        : instructorName(instructor), courseName(name), courseCode(code), credits(credit) {}

    // Display course information
    void displayInfo()  {
        cout << "Instructor Name: " << instructorName << endl;
        cout << "Course Name: " << courseName << endl;
        cout << "Course Code: " << courseCode << endl;
        cout << "Credits: " << credits << endl;
    }

    // Check if course is high credit
    bool isHighCredit() const {
        return credits > 3;
    }

    // Print lab requirement based on credits
    void isLabRequired() const {
        cout << (credits > 4 ? "REQUIRE LAB" : "DON'T HAVE LAB") << endl;
    }
    // Optional extention: Setters if you want to update values later
    void setInstructorName( string name) { instructorName = name; }
    void setCourseName( string name) { courseName = name; }
    void setCourseCode( string code) { courseCode = code; }
    void setCredits(int credit) { credits = credit; }
};

int main() {
    // Create a course object using constructor
    Course course1("Dr. Nguyen", "Data Structures", "CS202", 4);

    // Display course info
    course1.displayInfo();

    // Check and display if it's a high credit course
    if (course1.isHighCredit()) {
        cout << "This is a high credit course." << endl;
    } else {
        cout << "This is not a high credit course." << endl;
    }

    // Show lab requirement
    course1.isLabRequired();

    return 0;
}