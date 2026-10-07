//============================================================================
// Name        : Course Catalog
// Author      : Anyka Perzynski-Drilling
// Date        : 9/2026
// Version     : 2.0
// Description : CS Capstone Enhancement: Data Structures and Algorithms
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <cctype>
using namespace std;

class CourseCatalog {
    private:
        //============================================================================
        // Initialize Data Structures
        //============================================================================
        struct Course {
            string courseName;
            vector<string> prerequisites;
        };

        // Key = course ID, value = corresponding Course object
        unordered_map<string, Course> catalog;

        //============================================================================
        // Helper Methods
        //============================================================================

        // O(1)
        // Ref: https://stackoverflow.com/questions/26002132/how-to-check-string-is-only-letters
        bool isValidId (const string& id) {
            // Must be 7 characters long, 4 chars and 3 nums
            if (id.length() == 7) {
                string firstFour = id.substr(0,4);
                string lastThree = id.substr(id.length() - 3, 3);

                // If any character is not a letter, flag as invalid
                for (char const &c : firstFour) {
                    if (!isalpha(c)) return false;
                }

                // If any character is not a digit, flag as invalid
                for (char const &c : lastThree) {
                    if (!isdigit(c)) return false;
                }

                return true;
            }
            return false;
        }

        // O(N)
        bool isValidName (const string& name) {
            // Name must be all chars
            if (name.empty()) return false;

            // If any character is a digit flag as invalid
            for (char const &c : name) {
                if (isdigit(c)) return false;
            }
            return true;
        }

        // O(N)
        bool isValidCourse (const vector<string>& fields) {
            // If line has AT LEAST 2 entries that are not empty
            if (fields.size() >= 2 && !fields[0].empty() && !fields[1].empty()) {
                // If each field contains expected data, return true
                if (isValidId(fields[0]) && isValidName(fields[1])) {
                    return true;
                }
            }
            return false;
        }

    public:
        //============================================================================
        // Helper Methods
        //============================================================================

        // O(n)
        //Credit: https://medium.com/@ryan_forrester_/splitting-strings-in-c-a-complete-guide-cf162837f4ba
        vector<string> split(const string& str, const string& delimiters = ",;\t") {
            // Vector to store the resulting tokens (substrings)
            vector<string> tokens;
            // Track the beginning index of the current token
            size_t start = 0;
            // Track the position of the next delimiter character from start
            size_t end = str.find_first_of(delimiters);

            // While delimiter is not found, extract substring and add it to the tokens vector
            while (end != string::npos) {
                tokens.push_back(str.substr(start, end - start));
                start = end + 1;
                end = str.find_first_of(delimiters, start);
            }

            tokens.push_back(str.substr(start));
            // Return the vector of tokens
            return tokens;
        }

        // O(1)
        void addCourse(const string& id, const string& name, vector<string>& prerequisites){
            if (catalog.find(id) == catalog.end()) {
                catalog[id] = Course{name, prerequisites};
            }
            else {
                cout << "Warning: Duplicate course ID ignored: " << id << endl;
            }
        }

        // O(M log M * L)
        // Ex) CSCI100, CSCI101, CSCI204, CSCI400
        vector<string> alphaNumSort(const vector<string>& courses) const {
            // Extract all course IDs
            vector<string> sortedCourseIds = courses;
            // sort
            sort(sortedCourseIds.begin(), sortedCourseIds.end());
            // return sorted course vector
            return sortedCourseIds;
        }

        // O(1)
        void printTableHeader () const{
            cout << left << setw(12) << "ID" << "| " << setw(45) << "Name" << "| " << "Prerequisites" << endl;
            cout << string(80, '-') << endl;
        }

        // O(N)
        // ID, name, prerequisite(s)
        void printCourseDetails(const string id, const Course& course) const {
            // Course info
            cout << left << setw(12) << id << "| " << setw(45) << course.courseName << "| ";

            // If prerequisites exist, print
            if (!course.prerequisites.empty()){
                for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                    cout << course.prerequisites[i];
                    // print commas between prerequisites
                    if (i < course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
            }
            cout << endl;
        }

        //============================================================================
        // Methods
        //============================================================================

        // O(1 + N)
        void printCourseById (string queryId) const {
            // Get course
            auto result = catalog.find(queryId);
            // If course exists, print details
            if (result != catalog.end()) {
                const Course& course = catalog.at(queryId);
                cout << right << setw(40) << "FOUND" << endl;
                cout << string(80, '=') << endl;
                printTableHeader();
                printCourseDetails(queryId, course);
                // table footer
                cout << string(80, '-') << endl << endl;
            }
            else {
                cout << "No course found with code: " << queryId << endl;
            }
        }

        // O(N log N * L + N * L)
        void printCatalog () const {
            // Get all IDs from catalog
            vector<string> allIds;
            for (const auto& [id, course] : catalog) {
                allIds.push_back(id);
            }
            // Sort
            vector<string> sortedIds = alphaNumSort(allIds);
            // Print
            cout << right << setw(40) << "ABCU Courses" << endl;
            cout << string(80, '=') << endl;
            printTableHeader();
            for (const string& id : sortedIds) {
                printCourseDetails(id, catalog.at(id));
            }
            // table footer
            cout << string(80, '-') << endl << endl;
        }

        // O(N * L + MlogM * L + M*P)
        void printCompSciCatalog () const {
            // Get all compsci IDs from catalog
            vector<string> compSciIds;
            for (const auto& [id, course] : catalog) {
                // if course ID contains CSCI or MATH, add to list
                if (id.find("CSCI") != string::npos || id.find("MATH") != string::npos) {
                    compSciIds.push_back(id);
                }
            }
            // Sort
            vector<string> sortedCompSciIds = alphaNumSort(compSciIds);
            // Print
            cout << right << setw(45) << "Computer Science Courses" << endl;
            cout << string(80, '=') << endl;
            printTableHeader();
            for (const string& id : sortedCompSciIds) {
                printCourseDetails(id, catalog.at(id));
            }
            // table footer
            cout << string(80, '-') << endl << endl;
        }

        unordered_map<string, Course> processCourseFile(const string& filepath) {
            set<string> validCourseIds;

            ifstream inputFile(filepath);
            // If file successfully opens, validate the data
            if (inputFile.is_open()) {
                string line;
                int lineNumber = 0;

                // Report to user file is being processed
                cout << "Loading file data..." << endl;
                cout << endl;

                // Step 1: Create a list of valid course IDs
                // O(logN)
                while (getline(inputFile, line)) {
                    // increment line counter
                    lineNumber++;
                    // Skip empty lines
                    if (line.empty()) continue;

                    // Read each line and parse
                    vector<string> fields = split(line);

                    // Validate each course entry contains a code and a name
                    // If valid, add to list of existing courses
                    if (isValidCourse(fields)){
                        validCourseIds.insert(fields[0]);
                    }
                }

                // Restart file read from the top
                // Ref: https://stackoverflow.com/questions/5343173/returning-to-beginning-of-file-after-getline
                inputFile.clear();  // Clear EOF and fail bits
                inputFile.seekg(0); // Move cursor back to beginning
                lineNumber = 0;     // Reset line counter

                // Step 2: Create a list of valid course IDs
                // O(logN)
                while (getline(inputFile, line)) {
                    // increment line counter
                    lineNumber++;
                    // Skip empty lines
                    if (line.empty()) continue;

                    // Read each line and parse
                    vector<string> fields = split(line);

                    // If the course is valid, validate prerequisites (if any)
                    if (isValidCourse(fields)){
                        string courseCode = fields[0];
                        string courseName = fields[1];

                        bool validPrerequisite = true; // flag to add prerequisites
                        vector<string> prerequisites;

                        // If three or more field exist for course entry
                        if (fields.size() >= 3) {
                            // For each prerequisites entered, validate if course exists
                            for (size_t i = 2; i < fields.size(); i++) {
                                // Skip empty fields
                                if (fields[i].empty()) continue;

                                // If prerequisite is not a code, flag
                                if (!isValidId(fields[i])) {
                                    validPrerequisite = false;
                                    cout << "Error: Invalid prerequisite course code entered at line " << lineNumber << endl;
                                    break;
                                }
                                // If prerequisite does not exist as a standalone course, flag
                                if (validCourseIds.find(fields[i]) == validCourseIds.end()) {
                                    validPrerequisite = false;
                                    cout << "Error: Invalid prerequisite course entered at line " << lineNumber << endl;
                                    break;
                                }
                                // If prerequisite is valid, add to catalog
                                prerequisites.push_back(fields[i]);
                            }
                            // If course is 400 or above, it must list prerequisites
                            if ((courseCode[4] >= '4' && courseCode[4] <= '9') && prerequisites.empty()) {
                                validPrerequisite = false;
                            }

                            // Remove any duplicate prerequisites
                            sort(prerequisites.begin(), prerequisites.end());
                            // Move any duplicates to the end
                            auto it = unique(prerequisites.begin(), prerequisites.end());
                            // Remove duplicates
                            prerequisites.erase(it, prerequisites.end());
                        }
                        // Add validated course to catalog
                        if (validPrerequisite == true) {
                            addCourse(courseCode, courseName, prerequisites);
                        }
                    }
                    // Skip invalid entries
                    else {
                        continue;
                    }
                }
                // Close input file
                inputFile.close();
                // Print success to report end of process
                cout << "File validation complete. Courses loaded successfully." << endl;
            }
            // If cannot open file, error
            else {
                cout << "Error: Could not open file to validate courses. Please check if the file exists and the path is correct." << endl;
                //catalog.clear();
            }
            return catalog;
        }

        //credit: https://www.asciiart.eu/books/books
        void welcome() {
            //string literal(R"()") avoids having to escape backslashes and makes the art look exactly as it will be printed
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
            cout << "ABCU Computer Science Course Catalog" << endl;
            cout << endl;
            cout << string(38, '=') << endl;
        }

        void printMenu() {
            // Print menu with options
            cout << endl;
            cout << right << setw(20) << "Menu" << endl;
            cout << left << string(38, '=') << endl;
            cout << "  1. Load Data Structure" << endl;
            cout << "  2. Print Course Catalog" << endl;
            cout << "  3. Print CompSci Course List" << endl;
            cout << "  4. Find Course" << endl;
            cout << "  9. Exit" << endl;
            cout << string(38, '=') << endl;
            cout << endl;
        }

        unsigned int getMenuChoice(unsigned int maxChoice) {
            unsigned int choice;
            while (true) {
                cout << "Enter a choice: ";
                cin >> choice;
                cout << endl;
                //if the input integer is in range of 1 - maxchoice, use input and exit loop
                if ((choice >= 1 && choice <= maxChoice) || choice == 9) {
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return choice;
                }
                //if the input is not a integer
                else if (cin.fail()) {
                    cout << "Invalid input. Please, enter a number." << endl;
                    // clear user choice
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                else { //if the integer is out of range, input a new integer.
                    cout << "Invalid input. Please, enter a number between 1 and " << maxChoice << " or 9." << endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
            }
        }

        void menu() {
            unsigned int choice;
            string filepath;

            // Display welcome banner
            welcome();

            do {
                // Print menu with options
                printMenu();

                // Get user input (1-4 and 9)
                choice = getMenuChoice(4);

                switch (choice) {
                case 1:
                    cout << "Enter the filepath and name: ";
                    getline(cin, filepath);
                    cout << endl;
                    processCourseFile(filepath);
                    break;
                case 2:
                    // If file is loaded, display results, else error
                    if (!catalog.empty()) {
                        // Print all the courses in the catalog
                        printCatalog();
                    }
                    else {
                        cout << "Error: uh oh... there's no data to show!" << endl;
                        cout << "There's still hope! Load the file and try again." << endl;
                        cout << endl;
                    }
                    break;
                case 3:
                    // If file is loaded, display results, else error
                    if (!catalog.empty()) {
                        // Print all the courses in the Computer Science department
                        printCompSciCatalog();
                    }
                    else {
                        cout << "Error: uh oh... there's no data to show!" << endl;
                        cout << "There's still hope! Load the file and try again." << endl;
                        cout << endl;
                    }
                    break;
                case 4:
                    // If file is loaded, display results, else error
                    if (!catalog.empty()) {
                        // Get desired course number from user
                        string queryId;
                        cout << "Enter the course code: ";
                        cin >> queryId;
                        cout << endl;
                        // Search for queried course number
                        printCourseById(queryId);
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
};

//============================================================================
// APPLICATION ENTRY
//============================================================================
int main() {
    CourseCatalog catalogApp;
    catalogApp.menu();
    return 0;
}
