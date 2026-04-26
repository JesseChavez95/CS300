//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jesse L Chavez
// Version     : 1.0
// Description : Advising Assistance Program for ABCU advisors.
//               This program loads course data from a CSV file into a data
//               structure, prints an alphanumeric course list, and displays
//               individual course information with prerequisites.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Structure to store course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Global vector to store all courses
vector<Course> courseCatalog;

// Convert a string to uppercase
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) { return static_cast<char>(toupper(c)); });
    return text;
}

// Remove leading and trailing whitespace
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    size_t last = text.find_last_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    return text.substr(first, last - first + 1);
}

// Split a CSV line into tokens
vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream stream(line);

    while (getline(stream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Find the index of a course in the catalog
int findCourseIndex(const string& courseNumber) {
    string target = toUpperCase(courseNumber);

    for (int i = 0; i < static_cast<int>(courseCatalog.size()); ++i) {
        if (toUpperCase(courseCatalog[i].courseNumber) == target) {
            return i;
        }
    }

    return -1;
}

// Load data from the file into the course catalog
bool loadDataStructure(const string& fileName) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    courseCatalog.clear(); // Prevent duplicate loads
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitLine(line);

        // Each valid course must have at least a course number and title
        if (fields.size() < 2) {
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpperCase(fields[i]));
            }
        }

        courseCatalog.push_back(course);
    }

    inputFile.close();

    if (courseCatalog.empty()) {
        cout << "Error: No valid course data was loaded." << endl;
        return false;
    }

    cout << "Data loaded successfully." << endl;
    return true;
}

// Print all courses in alphanumeric order
void printCourseList() {
    if (courseCatalog.empty()) {
        cout << "Error: Please load the data file first." << endl;
        return;
    }

    vector<Course> sortedCourses = courseCatalog;

    sort(sortedCourses.begin(), sortedCourses.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    cout << "\nHere is a sample schedule:\n" << endl;

    for (const Course& course : sortedCourses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }

    cout << endl;
}

// Print information for one course
void printCourseInformation() {
    if (courseCatalog.empty()) {
        cout << "Error: Please load the data file first." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(courseNumber);
    int courseIndex = findCourseIndex(courseNumber);

    if (courseIndex == -1) {
        cout << "Error: Course " << courseNumber << " not found." << endl;
        return;
    }

    const Course& course = courseCatalog[courseIndex];

    cout << "\n" << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";

        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            int prereqIndex = findCourseIndex(course.prerequisites[i]);

            if (prereqIndex != -1) {
                cout << courseCatalog[prereqIndex].courseNumber
                    << ", " << courseCatalog[prereqIndex].courseTitle;
            }
            else {
                cout << course.prerequisites[i];
            }

            if (i < course.prerequisites.size() - 1) {
                cout << "; ";
            }
        }

        cout << endl;
    }
}

// Display menu options
void displayMenu() {
    cout << "\nMenu:" << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
}

// Main function
int main() {
    string choice;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        displayMenu();
        cout << "\nWhat would you like to do? ";
        cin >> choice;

        if (choice == "1") {
            string fileName;
            cout << "Enter file name: ";
            cin >> fileName;
            loadDataStructure(fileName);
        }
        else if (choice == "2") {
            printCourseList();
        }
        else if (choice == "3") {
            printCourseInformation();
        }
        else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}