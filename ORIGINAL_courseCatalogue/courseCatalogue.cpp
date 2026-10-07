//============================================================================
// Name        : courseCatalogue.cpp
// Author      : Anyka Perzynski-Drilling
// Version     : 1.0
// Description : CS 300 Project Two
//============================================================================


#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

class Course {
public:
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

//============================================================================
// Methods
//============================================================================

/**
 *    Parse
 *    Credit: https://medium.com/@ryan_forrester_/splitting-strings-in-c-a-complete-guide-cf162837f4ba
 */

vector<string> split(const string& str, char delimiter) {
    // Vector to store the resulting tokens (substrings)
    vector<string> tokens;
    // Track the beginning index of the current token
    size_t start = 0;
    // Track the position of the next delimiter character from start
    size_t end = str.find(delimiter);

    // While delimiter is not found, extract substring and add it to the tokens vector
    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    // Return the vector of tokens
    return tokens;
}

/**
 *    Read and Validate Data File
 */

vector<Course> processCourseFile() {
    set<string> validCourseNums;
    vector<Course> courses;
    string filepath;

    // Get data file path from user
    cout << "Enter the filename: ";
    getline(cin, filepath); // getline is necessary here as cin stops reading at the first space
    cout << endl;

    //
    // Step 1: Open file and create a list of valid existing courses numbers
    //

    ifstream inputFile(filepath);
    // If file successfully opens, validate the data
    if (inputFile.is_open()) {
        string line;
        int lineNumber = 0;

        cout << "Loading file data..." << endl;
        cout << endl;

        while (getline(inputFile, line)) {
            // Skip empty lines
            if (line.empty()) continue;
            // Read each line and parse
            vector<string> fields = split(line, ',');
            // Validate at least 2 entries exist in each line
            if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
                cout << "Error: Invalid course entry for line " << lineNumber << "." << endl;
                continue;
            }
            else {
                validCourseNums.insert(fields[0]);
            }
            // Increment lineNumber for error handeling
            lineNumber++;
        }
        inputFile.close();
    }
    // If cannot open file, error
    else {
        cout << "Error: Could not open file to validate courses. Please check if the file exists and the path is correct." << endl;
    }

    //
    // Step 2: Open file and create a list of valid existing courses and prerequisites
    //

    inputFile.open(filepath);
    // If file successfully opens, validate the data
    if (inputFile.is_open()) {
        string line;
        int lineNumber = 0;

        while (getline(inputFile, line)) {
            if (line.empty()) continue;
            // Read each line and parse
            vector<string> fields = split(line, ',');

            // Skip lines with an invalid entry (again)
            if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
                // Error was displayed in step 1 :)
                continue;
            }
            // If course has prerequisites (more than two data entries), validate prerequisites agaisnt valid existing courses numbers list
            bool inValidPrequisiteFound = false; // flag if invalid prerequisite is found
            if (fields.size() > 2 && !(fields[0].empty()) && !(fields[1].empty())) {
                // for each prerequisite
                for (size_t i = 2; i < fields.size(); ++i) {
                    // Skip empty trailing commas
                    if (fields[i].empty()) {
                        continue;
                    }
                    // If prerequisite is not in set, error
                    if (validCourseNums.count(fields[i]) == 0) {
                        cout << "Error: Invalid Prerequisite Course Entered in line " << lineNumber << endl;
                        inValidPrequisiteFound = true; // Enable flag
                        break;
                    }
                }
            }

            // If all entries are valid, add valid new course to the course catalogue
            if (inValidPrequisiteFound == false) {
                Course newCourse;
                newCourse.courseNumber = fields[0];
                newCourse.courseName = fields[1];
                // For each listed prerequisite, add prerequisite to newCourse
                for (size_t i = 2; i < fields.size(); ++i) {
                    if (!fields[i].empty()) {
                        newCourse.prerequisites.push_back(fields[i]);
                    }
                }
                // Store newcourse into the course catalogue
                courses.push_back(newCourse);
            }
            // Increment lineNumber for error handeling
            lineNumber++;
        }
        inputFile.close();

        // When finished, print success message
        cout << endl;
        cout << "Courses loaded successfully :)" << endl;
    }
    // If cannot open file, error
    else {
        cout << "Error: Could not open file to create catalogue. Please check if the file exists and the path is correct." << endl;
    }
    // Return the loaded courses
    return courses;
}

/**
 *    Search the data structure for a specific course and print out course information and prerequisites
 */

void searchCourse(const vector<Course>& courses) {
    // Get desired course number from user
    string queryCourseNumber;
    cout << "Enter the course number: ";
    cin >> queryCourseNumber;
    cout << endl;

    // Iterate over the vector and print details for each course directly
    bool courseFound = false; // Flag to stop searching if course is found
    for (const auto& course : courses) {
        // If the queried course number matches a course number in the catalogue, display course info
        if (course.courseNumber == queryCourseNumber) {
            courseFound = true;
            // Print course number and name
            cout << course.courseNumber << ": " << course.courseName << endl;
            // Print all prerequisites if they exist
            if (!course.prerequisites.empty()) {
                cout << "    - Prerequisite(s): ";
                for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                    cout << course.prerequisites[i];
                    // print commas between prerequisites
                    if (i < course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
        }
    }
    // If no course was found, display message
    if (!courseFound) {
        cout << "No course found by course number: " << queryCourseNumber << endl;
    }
}

/**
 *
 *   Sort in alphanumeric order
 *   ex) CSCI100, CSCI101, CSCI204, CSCI400
 *
 *   Note: This function uses the algorithmic sort function with a lambda function as a custom comparison
 *   References:
 *       - geeksforgeeks.org/cpp/lambda-expression-in-c/
 *       - https://stackoverflow.com/questions/34757448/sorting-a-vector-of-objects-alphabetically-in-c
 *       - https://www.reddit.com/r/cpp_questions/comments/yoiwz0/how_to_use_a_lambda_function_with_stdfind_first/
 *
 */

vector<Course> alphaNumSort(const vector<Course>& courses) {
    // Create copy of courses to manipulate
    vector<Course> sortedCourses = courses;

    // Use sort with a custom comparison (lambda function)
    sort(sortedCourses.begin(), sortedCourses.end(),
        [](const Course& a, const Course& b) {
            // Compare the courseNumber strings
            return a.courseNumber < b.courseNumber;
        });
    // return sorted course vector
    return sortedCourses;
}

/**
 *    Print all compsci courses in alphanumeric order.
 *    ex) CSCI100, CSCI101, CSCI204, CSCI400
 */

void displayCompSciCourse(const vector<Course>& courses) {
    vector<Course> compSciCourses;

    // If course contains the code CSCI or MATH, add to comp sci course list
    for (const auto& course : courses) {
        if ((course.courseNumber).find("CSCI") != std::string::npos || (course.courseNumber).find("MATH") != std::string::npos) {
            compSciCourses.push_back(course);
        }
    }

    // Sort the compSciCourses vector and store in separate vector
    vector<Course> sortedCompSciCourses = alphaNumSort(compSciCourses);

    // Header for display
    cout << string(6, '=') << " ABCU Computer Science Courses " << string(6, '=') << endl;
    cout << endl;

    // For all courses in sortedCompSciCourses, print all courses and their prerequisites
    for (const auto& course : sortedCompSciCourses) {
        cout << "+ " << course.courseNumber << ": " << course.courseName << endl;
        // Print prerequisites if they exist
        if (!course.prerequisites.empty()) {
            cout << "    - Prerequisite(s): ";
            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                cout << course.prerequisites[i];
                // print commas between prerequisites
                if (i < course.prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    cout << string(44, '=') << endl;
}

/**
 *    Welcome banner
 */

void welcome() {
    //string literal(R"()") avoids having to escape backslashes and makes the art look exactly as it will be printed
    // credit: https://www.asciiart.eu/books/books
    const std::string bookArt = R"(
       .--.                   .---.
   .---|__|           .-.     |~~~|
.--|===|--|_          |_|     |~~~|--.
|  |===|  |'\     .---!~|  .--|   |--|
|%%|   |  |.'\    |===| |--|%%|   |  |
|%%|   |  |\.'\   |   | |__|  |   |  |
|  |   |  | \  \  |===| |==|  |   |  |
|  |   |__|  \.'\ |   |_|__|  |~~~|__|
|  |===|--|   \.'\|===|~|--|%%|~~~|--|
^--^---'--^    `-'`---^-^--^--^---'--'
)";
    cout << string(38, '=') << endl;
    cout << bookArt << std::endl;
    cout << endl;
    cout << "ABCU Computer Science Course Catalogue" << endl;
    cout << endl;
    cout << string(38, '=') << endl;
}

/**
 *    Get and validate the user input for the actions menu
 */

unsigned int getMenuChoice(unsigned int maxChoice) {
    unsigned int choice;
    while (true) {
        cout << "Enter a choice: ";
        cin >> choice;
        cout << endl;
        if (cin.fail()) { //if the input is not a integer
            cout << "Invaild input. Please, enter a number." << endl;
            // clear user choice
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (choice >= 1 && choice <= maxChoice || choice == 9) { //if the input integer is in range of 1 - maxchoice, use input and exit loop.
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        else { //if the integer is out of range, input a new integer.
            cout << "Invaild input :( Please, enter a number between 1 and " << maxChoice << " or 9." << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

/**
 *    User menu
 */

void menu() {
    unsigned int choice = 0;
    string filepath;
    vector<Course> catalogue; // Stores loaded data

    // Display welcome banner
    welcome();

    do {
        // Print menu with options
        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;

        // Get user input (1-3 and 9)
        choice = getMenuChoice(3);

        switch (choice) {
        case 1:
            // Complete the method call to load the bids
            catalogue = processCourseFile();
            break;

        case 2:
            // If file is loaded, display results, else error
            if (!catalogue.empty()) {
                // Print all the courses in the Computer Science department
                displayCompSciCourse(catalogue);
            }
            else {
                cout << "Error: uh oh... there's no data to show!" << endl;
                cout << "There's still hope! Load the file and try again." << endl;
                cout << endl;
            }
            break;

        case 3:
            // If file is loaded, display results, else error
            if (!catalogue.empty()) {
                // Search for queried course number
                searchCourse(catalogue);
            }
            else {
                cout << "Error: uh oh... there's no data to show!" << endl;
                cout << "There's still hope! Load the file and try again." << endl;
                cout << endl;
            }
            break;

        case 9:
            // Exit the program
            cout << "Exiting the program. Goodbye!" << endl;
            break;

        default:
            // Handles unexpected input (though getMenuChoice should prevent this)
            cout << "An unexpected error occurred with your choice." << endl;
            break;
        }
    } while (choice != 9);
}

int main() {

    menu();
    return 0;
}