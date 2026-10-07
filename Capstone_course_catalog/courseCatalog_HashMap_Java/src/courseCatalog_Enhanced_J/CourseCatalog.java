//============================================================================
// Name        : Course Catalog
// Author      : Anyka Perzynski-Drilling
// Date        : 9/2026
// Version     : 2.0
// Description : CS Capstone Enhancement: Data Structures and Algorithms
//============================================================================

package courseCatalog_Enhanced_J;

import java.util.regex.Pattern;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

public class CourseCatalog {
	
	//============================================================================
    // Private
    //============================================================================
	// Ref: https://stackoverflow.com/questions/5168144/does-java-support-structs
	private record Course(String name, ArrayList<String>prerequisites) {}
	
	// Key = course ID, value = corresponding Course object
	private HashMap<String, Course> catalog = new HashMap<>();
	
	//============================================================================
    // Private Helper Methods
    //============================================================================
	
	//O(1)
	private boolean isValidId(String id) {
		// Must be 7 characters long, 4 chars and 3 nums
		if (id.length() == 7) {
			char[] firstFour = (id.substring(0,4)).toCharArray();
			char[] lastThree = (id.substring(4,7)).toCharArray();
			
			// If any character is not a letter, flag as invalid
			for (char c : firstFour) {
				if (!Character.isLetter(c)) return false; 
			}
			
			// If any character is not a digit, flag as invalid
			for (char c : lastThree) {
				if (!Character.isDigit(c)) return false; 
			}
			return true;
		}
		return false;
	}
	
	//O(n)
	private boolean isValidName(String name) {
		// Name must be all chars
		if (name.isEmpty()) return false;
		
		// If any character is a digit flag as invalid
		char[] temp = name.toCharArray();
		
		for (char c : temp) {
			if (Character.isDigit(c)) return false; 
		}
		return true;
	}
	
	// O(1)
	private boolean isValidCourse(ArrayList<String>fields) {
		// If line has AT LEAST 2 entries that are not empty
		if((fields.size() >= 2) && (!fields.get(0).isEmpty()) && (!fields.get(1).isEmpty())) {
			// If each field contains expected data, return true
			if (isValidId(fields.get(0).trim()) && isValidName(fields.get(1).trim())) {
				return true;
			}
		}
		return false;
	}
	
	//============================================================================
    // Public
    //============================================================================
	
	//============================================================================
    // Public Helper Methods
    //============================================================================
	
	// O(n)
	public static ArrayList<String> split(String str) {
	    if (str == null || str.isEmpty()) {
	        return new ArrayList<>(List.of(""));
	    }
	    
	    String[] tokens = str.split("[,;\\t]+", -1);
	    return new ArrayList<>(Arrays.asList(tokens));
	}
	
	// O(1)
	public void addCourse(String id, String name, ArrayList<String>prerequisites) {
		if (!catalog.containsKey(id)) {
			catalog.put(id, new Course(name, prerequisites));
		}
		else {
			System.out.println("Waning: Duplicate course ID ignored: " + id);
		}
		
	}
	
	// O(K log K)
	// Ref: https://www.baeldung.com/java-hashmap-sort
	// Ex) CSCI100, CSCI101, CSCI204, CSCI400
	public static ArrayList<String> alphaNumSort(ArrayList<String> courses) {
		// Extract all course IDs
		ArrayList<String> sortedCourseIDs = new ArrayList<>(courses);
		// sort
		Collections.sort(sortedCourseIDs);
		// return sorted course array list
		return sortedCourseIDs;
	}
	
	// O(1)
	public static void printTableHeader() {
		System.out.printf("%-12s%-3s%-45s%-3s%s%n", "ID", "| ", "Name", "| ", "Prerequisites", "\n" );
		System.out.println("-".repeat(80));
	}
	
	// O(n)
	public void printCourseDetails(String id, Course course) {
		// Course info
		System.out.printf("%-12s%-3s%-45s%-3s", id, "| ", course.name(), "| ");
		
		// If prerequisites exist, print
		if(!course.prerequisites().isEmpty()) {
			for (int i = 0; i < course.prerequisites().size(); i++) {
				System.out.print(course.prerequisites().get(i));
				// Print commas between prerequisites
				if(i < course.prerequisites().size() - 1) {
					System.out.print(", ");
				}
			}
		}
		System.out.println("");
	}
	
	//============================================================================
    // Methods
    //============================================================================

	// O(1)
	public void printCourseById(String id) {
		// Get course with queried ID
		Course result = catalog.get(id);
		
		// If course exists, print
		if(result != null) {
			System.out.printf("%40s", "FOUND \n" );
			System.out.println("-".repeat(80));
			printTableHeader();
			printCourseDetails(id, result);
			// table footer
			System.out.println("-".repeat(80));
		}
		else {
			System.out.println("No course found with code: " + id + "\n");
		}
	}
	
	// O(N log N)
	public void printCatalog() {
		// Get all IDs from catalog
		ArrayList<String> allIds =  new ArrayList<>();
		for (String id : catalog.keySet()) {
			allIds.add(id);
		}
		// Sort
		ArrayList<String> sortedIds = alphaNumSort(allIds);
		// Print
		System.out.printf("%40s", "ABCU Courses \n" );
		System.out.println("=".repeat(80));
		printTableHeader();
		for (String id : sortedIds) {
			printCourseDetails(id, catalog.get(id));
		}
		// table footer
		System.out.println("-".repeat(80));
	}
	
	// O(N + C log C)
	public void printCompSciCatalog() {
		// Get all compsci IDs from catalog
		ArrayList<String> compSciIds =  new ArrayList<>();
		for (String id : catalog.keySet()) {
			// if course ID contains CSCI or MATH, add to list
			if(id.startsWith("CSCI") || id.startsWith("MATH")) {
				compSciIds.add(id);
			}
		}
		// Sort
		ArrayList<String> sortedCompSciIds = alphaNumSort(compSciIds);
		
		// Print
		System.out.printf("%45s", "Computer Science Courses \n");
		System.out.println("=".repeat(80));
		printTableHeader();
		for (String id : sortedCompSciIds) {
			printCourseDetails(id, catalog.get(id));
		}
		// table footer
		System.out.println("-".repeat(80));
	}
	
	// O(N * M)
	public HashMap<String, Course> processCourseFile(String filepath){
		Set<String> validCourseIds = new HashSet<>();
		//HashMap<String, Course> courses;
        

		// If file successfully opens, validate the data
        try (RandomAccessFile fileReader = new RandomAccessFile(filepath, "r")) {
        	String line;
        	int lineNumber = 0; 
        	
        	// Report to user file is being processed
        	System.out.println("Loading file data...");
        	
        	// Step 1: Create a list of valid course IDs
            while ((line = fileReader.readLine()) != null) {
            	// Increment line counter
            	lineNumber++;
            	// skip empty lines
            	if (line.isEmpty()) continue;
            	
            	// Read each line and parse
            	ArrayList<String> fields = split(line);
            	// Validate each course entry contains a code and a name
                // If valid, add to list of existing courses
            	if(isValidCourse(fields)) {
            		validCourseIds.add(fields.get(0));
            	}
            }
            
            // Restart file read from the top
            fileReader.seek(0); // Move cursor back to beginning
            lineNumber = 0; // Reset line counter
            
            // Step 2: Create a list of valid course IDs
            while ((line = fileReader.readLine()) != null) {
            	// Increment line counter
            	lineNumber++;
            	// skip empty lines
            	if (line.isEmpty()) continue;
            	// Read each line and parse
            	ArrayList<String> fields = split(line);
            	
            	// If the course is valid, validate prerequisites (if any)
            	if(isValidCourse(fields)) {
            		String courseCode = fields.get(0);
        			String courseName = fields.get(1);
        			
            		boolean validPrerequisites = true;
            		ArrayList<String> prerequisites = new ArrayList<>();
            		
            		// If three or more field exist for course entry
            		if (fields.size() >= 3) {
            			for (int i = 2; i < fields.size(); i++) {
            				// Skip empty lines
                        	if (fields.get(i).isEmpty()) continue;
                        	// If prerequisite is not a code, flag
                        	if (!isValidId(fields.get(i))) {
                        		validPrerequisites = false;
                        		System.out.println("Error: Invalid prerequisite course entered at line " + lineNumber);
                        		break;
                        	}
                        	
                        	// If prerequisite is not in valid course IDs, flag
                        	if (!validCourseIds.contains(fields.get(i))) {
                        		validPrerequisites = false;
                        		System.out.println("Error: Invalid prerequisite course entered at line " + lineNumber);
                        		break;
                        	}
                        	// If prerequisite is valid, add to catalog
                        	prerequisites.add(fields.get(i));
            			}
            			// If course is 400 or above, it must list prerequisites
            			if ((courseCode.charAt(4) >= '4' && courseCode.charAt(4) <= '9') && prerequisites.isEmpty()) {
            				validPrerequisites = false;
            			}
            			
            			// Remove any duplicate prerequisites (recursively)
            			prerequisites = new ArrayList<>(new LinkedHashSet<>(prerequisites));
            		}
            		// Add validated course to catalog
            		if (validPrerequisites == true) {            			
            			addCourse(courseCode, courseName, prerequisites);
            		}
            	}
            	// Skip invalid entries
            	else {
            		continue;
            	}
            }
            // Close read file
            fileReader.close();
            // Print success to report end of process
            System.out.println("File vallidation complete. Courses loaded successfully.");
        } 
        // If file could not be open, error
        catch (FileNotFoundException e) {
            System.err.println("Error: Could not open file to validate courses. Please check if the file exists and the path is correct." + e.getMessage());
        } 
        catch (IOException e) {
        	System.out.println("Error: An interruption has occured.");;
			e.printStackTrace();
		}
        
		return catalog;
	}
	
	// O(1)
	// Credit: https://www.asciiart.eu/books/books
	public static void printWelcome() {
		String bookArt = """
			        .--.                   .---.
				   .---|__|           .-.     |~~~|
				.--|===|--|_          |_|     |~~~|--.
				|  |===|  |'\\     .---!~|  .--|   |--|
				|%%|   |  |.'\\    |===| |--|%%|   |  |
				|%%|   |  |\\.'\\   |   | |__|  |   |  |
				|  |   |  | \\  \\  |===| |==|  |   |  |
				|  |   |__|  \\.'\\ |   |_|__|  |~~~|__|
				|  |===|--|   \\.'\\|===|~|--|%%|~~~|--|
				^--^---'--^    `-'`---^-^--^--^---'--'""";
		System.out.println("=".repeat(38));
		System.out.println(bookArt);
		System.out.println();
		System.out.println("ABCU Computer Science Course Catalog");
		System.out.println("=".repeat(38));
	}
	

	// O(1)
	public static void printMenu() {
		System.out.println("");
		System.out.printf("%20s", "Menu\n" );
		System.out.println("=".repeat(38));
		System.out.println("  1. Load Data Structure");
		System.out.println("  2. Print Course Catalog");
		System.out.println("  3. Print CompSci Course List");
		System.out.println("  4. Find Course");
		System.out.println("  9. Exit");
		System.out.println("=".repeat(38));
	}
	
	// O(1)
	public int getMenuChoice(Scanner input, int maxChoice) {
		int choice;
		while(true) {
			System.out.println("Enter a choice: ");
			try {
				choice = Integer.parseInt(input.nextLine().trim());
				// If the input integer is in range of 1 - maxchoice, use input and exit loop.
				if ((choice >= 1 && choice <= maxChoice) || choice == 9) {
					return choice;
				}
				// If choice is not in range, display error
				else {
					System.out.println("Invaild input. Please, enter a number between 1 and " + maxChoice + " or 9.");
				}
			}
			// If input is not a number, display error
			catch (NumberFormatException e) {
				System.out.println("Invaild input. Please, enter a number.");
			}
		}
	}
	
	// O(User time)
	public void menu() {
		int choice; 
		String filepath;
		Scanner input = new Scanner(System.in);
		
		// Display welcome banner
		printWelcome();
		
		do {
			// Print menu with options
			printMenu();
			
			// Get user input (1-4 and 9)
			choice = getMenuChoice(input, 4);
			
			switch(choice) {
			case 1:
				System.out.println("Enter the filepath and name: ");
				filepath = input.nextLine().trim();
				processCourseFile(filepath);
				break;
				
			case 2:
				// If file is loaded, display results, else error
				if (!catalog.isEmpty()) {
					// Print all the courses in the catalog
					printCatalog();
				}
				else {
					System.out.println("Error: uh oh... there's no data to show!");
					System.out.println("There's still hope! Load the file and try again.");
				}
				break;
				
			case 3:
				// If file is loaded, display results, else error
				if (!catalog.isEmpty()) {
					// Print all the courses in the Computer Science department
					printCompSciCatalog();					
				}
				else {
					System.out.println("Error: uh oh... there's no data to show!");
					System.out.println("There's still hope! Load the file and try again.");
				}
				break;
				
			case 4:
				// If file is loaded, display results, else error
				if (!catalog.isEmpty()) {
					// Get desired course number from user
					System.out.println("Enter the course code: ");
					String queryId = input.nextLine().trim();
					System.out.println("\n");
					
					// Search for queried course number
                    printCourseById(queryId);
				}
				else {
					System.out.println("Error: uh oh... there's no data to show!");
					System.out.println("There's still hope! Load the file and try again.");
				}
				break;
				
			case 9:
				// Exit the program
				System.out.println("Exiting the program. Goodbye!");
				break;
			
			default: 
				// Handles unexpected input (though getMenuChoice should prevent this)
				System.out.println("An unexpected error occurred with your choice.");
			}

		} while (choice != 9);
	}

	public CourseCatalog() {}

	//============================================================================
	// APPLICATION ENTRY
	//============================================================================
	
	public static void main(String[] args) {
		CourseCatalog courseCatalog = new CourseCatalog();
		// Begin program
		courseCatalog.menu();
	}
}
