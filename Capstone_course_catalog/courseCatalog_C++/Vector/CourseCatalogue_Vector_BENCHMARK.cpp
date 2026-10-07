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
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

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

vector<Course> processCourseFile(string filepath) {
    set<string> validCourseNums;
    vector<Course> courses;

    //
    // Step 1: Open file and create a list of valid existing courses numbers
    //

    ifstream inputFile(filepath);
    // If file successfully opens, validate the data
    if (inputFile.is_open()) {
        string line;
        int lineNumber = 0;

        while (getline(inputFile, line)) {
            // Skip empty lines
            if (line.empty()) continue;
            // Read each line and parse
            vector<string> fields = split(line, ',');
            // Validate at least 2 entries exist in each line
            if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
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
        //cout << "Error: Could not open file to validate courses. Please check if the file exists and the path is correct." << endl;
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
    }
    // If cannot open file, error
    else {}
    // Return the loaded courses
    return courses;
}

/**
 *    Search the data structure for a specific course and print out course information and prerequisites
 */

void searchCourse(const vector<Course>& courses, string queryCourseNumber) {
    // Iterate over the vector and print details for each course directly
    bool courseFound = false; // Flag to stop searching if course is found
    for (const auto& course : courses) {
        // If the queried course number matches a course number in the catalogue, display course info
        if (course.courseNumber == queryCourseNumber) {
            courseFound = true;
        }
    }
    // If no course was found, display message
    if (!courseFound) {}
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
    string id; // Temp varaible

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
            catalogue = processCourseFile(filepath);
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
                searchCourse(catalogue, id);
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

void parseBenchmark (string filepath) {
    vector<Course> catalogue;

    for (int i = 0; i < 5; i++) {
        // Clean slate
        catalogue.clear();
        // Record starting time
        auto start = high_resolution_clock::now();
        catalogue = processCourseFile(filepath);
        // Record ending time
        auto stop = high_resolution_clock::now();

        // Prevent dead code elimination
        // Ref: https://medium.com/@azad217/understanding-the-volatile-keyword-and-compiler-optimizations-22d974de9ef3
        if (!catalogue.empty()) {
            volatile size_t loadedSize = catalogue.size();
            (void)loadedSize;
        }

        // Print time
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken: " << duration.count() << " microseconds" << endl;
    }
}

void sortCompSciCatalogBenchmark (string filepath) {
    vector<Course> catalogue;
    catalogue.clear();
    catalogue = processCourseFile(filepath);

    if (!catalogue.empty()) {
        for (int i = 0; i < 5; i++) {
            // Record starting time
            auto start = high_resolution_clock::now();
            displayCompSciCourse(catalogue);
            // Record ending time
            auto stop = high_resolution_clock::now();

            // Prevent dead code elimination
            // Ref: https://medium.com/@azad217/understanding-the-volatile-keyword-and-compiler-optimizations-22d974de9ef3
            if (!catalogue.empty()) {
                volatile size_t loadedSize = catalogue.size();
                (void)loadedSize;
            }

            // Print time
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time taken: " << duration.count() << " microseconds" << endl;
        }
    }
}

void lookupBenchmark (string filepath) {
    vector<Course> catalogue;
    catalogue.clear();
    catalogue = processCourseFile(filepath);

    if (!catalogue.empty()) {
        for (int i = 0; i < 5; i++) {
            vector<string> randomIds;
            // Get 1,000 random IDs
            int count = 0;
            for (const auto& course : catalogue) {
                if (count >= 100) break;
                randomIds.push_back(course.courseNumber);
                count ++;
            }
            // Shuffle vector
            // Ref: https://www.geeksforgeeks.org/cpp/how-to-shuffle-a-vector-in-cpp/
            // Initialize random number generator
            random_device rd;
            mt19937 g(rd());
            shuffle(randomIds.begin(), randomIds.end(), g);

            // Lookup each ID and record the time
            // Record starting time
            auto start = high_resolution_clock::now();

            // Test for 1k ids
            for (int j = 0; j < randomIds.size(); j++) {
                searchCourse(catalogue, randomIds[j]);
            }

            // Record ending time
            auto stop = high_resolution_clock::now();

            // Prevent dead code elimination
            // Ref: https://medium.com/@azad217/understanding-the-volatile-keyword-and-compiler-optimizations-22d974de9ef3
            if (!catalogue.empty()) {
                volatile size_t loadedSize = catalogue.size();
                (void)loadedSize;
            }

            // Record time
            auto duration = duration_cast<microseconds>(stop - start);
            double avgPerLookup = static_cast<double>(duration.count()) / randomIds.size();
            cout << "Time taken: " << duration.count() << " microseconds. " << avgPerLookup << " per lookup." << endl;
        }
    }
}

void executionBenchmark(string filepath) {
    vector<Course> catalogue;
    vector<long long> timeRecords;
    catalogue.clear();
    catalogue = processCourseFile(filepath);

    // For 100 runs
    for (int i = 0; i < 100; i++) {
        catalogue.clear();
        // Record starting time
        auto start = high_resolution_clock::now();

        processCourseFile(filepath);

        // Get 100 random IDs
        vector<string> randomIds;
        int count = 0;
        for (const auto& course : catalogue) {
            if (count >= 100) break;
            randomIds.push_back(course.courseNumber);
            count ++;
        }
        // Shuffle vector
        // Ref: https://www.geeksforgeeks.org/cpp/how-to-shuffle-a-vector-in-cpp/
        // Initialize random number generator
        random_device rd;
        mt19937 g(rd());
        shuffle(randomIds.begin(), randomIds.end(), g);

        for (int j = 0; j < randomIds.size(); j++) {
            searchCourse(catalogue, randomIds[j]);
        }

        displayCompSciCourse(catalogue);

        // Record ending time
        auto stop = high_resolution_clock::now();

         // Save time
        auto duration = duration_cast<microseconds>(stop - start);
        timeRecords.push_back(duration.count());
    }
    // Print
    for (int i = 0; i < timeRecords.size(); i++) {
        cout << "Trial " << i << " | Time taken: " << timeRecords.at(i) << " microseconds." << endl;
    }
}

//============================================================================
// BENCHMARK ENTRY
//============================================================================

int main() {
    // Declare test data set
    string filepath = "../../Test Data/ABCU_Catalog_MOCK_DATA_10k.csv"; // Manually change for now
    // The execution benchmark is conducted separately to measure cold vs warm state
    bool runExecutionBenchmark = false;

    // TESTS:
    if (runExecutionBenchmark) {
        cout << "Execution Benchmark: " << endl;
        executionBenchmark(filepath);
    }
    else {
        cout << "File Parse Benchmark: " << endl;
        parseBenchmark(filepath);

        //cout << "Sort All Catalog Benchmark: " << endl;
        //sortCatalogBenchmark(filepath);

        cout << "Sort CompSci Catalog Benchmark: " << endl;
        sortCompSciCatalogBenchmark(filepath);

        cout << "Lookup Benchmark: " << endl;
        lookupBenchmark(filepath);
    }
    // EOF
    return 0;
}