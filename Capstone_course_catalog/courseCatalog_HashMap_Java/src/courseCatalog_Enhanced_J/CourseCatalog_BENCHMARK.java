//============================================================================
// Name        : Course Catalog BENCHMARK
// Author      : Anyka Perzynski-Drilling
// Date        : 9/2026
// Version     : 1.0
// Description : CS Capstone Enhancement: Data Structures and Algorithms
//============================================================================

package courseCatalog_Enhanced_J;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.TimeUnit;

public class CourseCatalog_BENCHMARK {
	
	//============================================================================
    // Private
    //============================================================================
	
	// Ref: https://stackoverflow.com/questions/5168144/does-java-support-structs
	public record Course(String name, ArrayList<String>prerequisites) {}
	
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
	
	// O(M)
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
			boolean temp = false;
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
	
	// O(P)
	public void printCourseDetails(String id, Course course) {
		// Course info
		System.out.printf("%-12s%-3s%-45s%-3s", id, "| ", course.name(), "| ");
		
		// If prerequisites exist, print
		if(!course.prerequisites().isEmpty()) {
			for (int i = 0; i < course.prerequisites().size(); i++) {
				System.out.print(course.prerequisites().get(i));
				// print commas between prerequisites
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
		Course result = catalog.get(id);
		
		if(result != null) {
			boolean found = true;
		}
		else {
			boolean found = false;
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
		ArrayList<String> sortedCompSciIds = alphaNumSort(compSciIds);;
	}
	
	// O(N * M)
	public HashMap<String, Course> processCourseFile(String filepath){
		Set<String> validCourseIds = new HashSet<>();
		//HashMap<String, Course> courses;
        

		// If file successfully opens, validate the data
        try (RandomAccessFile fileReader = new RandomAccessFile(filepath, "r")) {
        	String line;
        	int lineNumber = 0; 
        	
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
            		boolean validPrerequisites = true;
            		ArrayList<String> prerequisites = new ArrayList<>();
            		
            		// If three or more field exist for course entry
            		if (fields.size() >= 3) {
            			for (int i = 2; i < fields.size(); i++) {
            				// skip empty lines
                        	if (fields.get(i).isEmpty()) continue;
                        	
                        	// If prerequisite is not in valid course IDs, flag
                        	if (!validCourseIds.contains(fields.get(i))) {
                        		validPrerequisites = false;
                        	}
                        	
                        	// If prerequisite is valid, add to catalog
                        	prerequisites.add(fields.get(i));
            			}
            		}
            		// Add validated course to catalog
            		if (validPrerequisites == true) {
            			String courseCode = fields.get(0);
            			String courseName = fields.get(1);
            			
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
        } 
        // If file could not be open, error
        catch (FileNotFoundException e) {} 
        catch (IOException e) {}
        
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
	
	public void parseBenchmark(String filepath) {
		int loadedSize = 0;
		for (int i = 0; i < 5; i++) {
			//clean slate
			catalog.clear();
			// Record starting time
			long start = System.nanoTime();
			processCourseFile(filepath);
			// Record ending time
			long stop = System.nanoTime();
			
			// Prevent dead code elimination
			if (!catalog.isEmpty()) {
				 loadedSize = catalog.size();
			}
			
			long duration = (stop - start);
			long elapsedTimeInMicros = TimeUnit.NANOSECONDS.toMicros(duration);
			
			// Print time
			System.out.println("Time taken: " + elapsedTimeInMicros + " microseconds");
		}
		
		System.out.println(loadedSize);
	}
	
	public void sortCatalogBenchmark(String filepath) {
		int loadedSize = 0;
		catalog.clear();
		processCourseFile(filepath);
		
		for (int i = 0; i < 5; i++) {
			// Record starting time
			long start = System.nanoTime();
			printCatalog();
			// Record ending time
			long stop = System.nanoTime();
			
			// Prevent dead code elimination
			if (!catalog.isEmpty()) {
				 loadedSize = catalog.size();
			}
			
			long duration = (stop - start);
			long elapsedTimeInMicros = TimeUnit.NANOSECONDS.toMicros(duration);
			
			// Print time
			System.out.println("Time taken: " + elapsedTimeInMicros + " microseconds");
		}
		
		System.out.println(loadedSize);
	}
	
	public void sortCompSciCatalogBenchmark(String filepath) {
		int loadedSize = 0;
		catalog.clear();
		processCourseFile(filepath);
		
		for (int i = 0; i < 5; i++) {
			// Record starting time
			long start = System.nanoTime();
			printCompSciCatalog();
			// Record ending time
			long stop = System.nanoTime();
			
			// Prevent dead code elimination
			if (!catalog.isEmpty()) {
				 loadedSize = catalog.size();
			}
			
			long duration = (stop - start);
			long elapsedTimeInMicros = TimeUnit.NANOSECONDS.toMicros(duration);
			
			// Print time
			System.out.println("Time taken: " + elapsedTimeInMicros + " microseconds");
		}
		
		System.out.println(loadedSize);
	}
	
	public void lookupBenchmark(String filepath) {
		catalog.clear();
		processCourseFile(filepath);
		
		for (int i = 0; i < 5; i++) {
			// Get 1000 random course IDs
			List<String> randomIds = new ArrayList<>(catalog.keySet());
			if (randomIds.size() > 1000) {
				randomIds = randomIds.subList(0, 1000);
			}
			
			// Shuffles IDs for true random lookup
			Collections.shuffle(randomIds);
			
			// Record starting time
			long start = System.nanoTime();
			
			for (int j = 0; j < randomIds.size(); j++) {
				printCourseById(randomIds.get(j));
			}
			
			// Record end time
			long stop = System.nanoTime();
			
			// Prevent dead code elimination
			if (!catalog.isEmpty()) {
				System.out.println(catalog.size());
			}
			
			// Print time elapsed
			long duration = (stop - start);
			long elapsedTimeInMicros = TimeUnit.NANOSECONDS.toMicros(duration);
			double avgPerLookup = (double) elapsedTimeInMicros / randomIds.size();
			
			System.out.println("Time taken: " + elapsedTimeInMicros + " microseconds. " + avgPerLookup + " microseconds per lookup.");
		}
	}
	
	// Ref: https://webeyez.com/insights/guides/how-to-calculate-execution-time-in-java
	public void executionBenchmark(String filepath) {
		List<Long> timeRecords = new ArrayList<>();
		long elapsedTimeInMicros = 0;
		
		// For 100 runs:
		for (int i = 0; i < 100; i++) {
			catalog.clear();
			// Record starting time
			long start = System.nanoTime();
			
			// Process file, perform 100 lookups, print sorted CompSci catalog
			processCourseFile(filepath);
			//printCatalog();
			
			// Get 10 random IDs
			List<String> randomIds = new ArrayList<>(catalog.keySet());
			if (randomIds.size() > 100) {
				randomIds = randomIds.subList(0, 100);
			}
			
			// Shuffles IDs for true random lookup
			Collections.shuffle(randomIds);
			
			for (int j = 0; j < randomIds.size(); j++) {
				printCourseById(randomIds.get(j));
			}
			
			printCompSciCatalog();
			
			// Record end time
			long stop = System.nanoTime();
			
			// Record elapsed
			long duration = (stop - start);
			elapsedTimeInMicros = TimeUnit.NANOSECONDS.toMicros(duration);
			timeRecords.add(elapsedTimeInMicros);
		}
		// Print time elapsed
		for (int i = 0; i < timeRecords.size(); i++) {
			System.out.println("Trial " + i + " | Time taken: " + timeRecords.get(i) + " microseconds.");
		}
	}

	public CourseCatalog_BENCHMARK() {}

	//============================================================================
	// BENCHMARK ENTRY
	//============================================================================
	public static void main(String[] args) {
		// Declare test data set
		String filepath = "../../../Test Data/ABCU_Catalog_MOCK_DATA_10k.csv"; // Manually change for now
		// The execution benchmark is conducted separately to measure cold vs warm state
		boolean runExecutionBenchmark = false;
		
		// Initialize application code
		CourseCatalog_BENCHMARK courseCatalog = new CourseCatalog_BENCHMARK();
		
		// TESTS:
		if (runExecutionBenchmark) {
			courseCatalog.executionBenchmark(filepath);
		}
		else {
			System.out.println("File Parse Benchmark: ");
			courseCatalog.parseBenchmark(filepath);
			
			System.out.println("Sort All Catalog Benchmark: ");
			courseCatalog.sortCatalogBenchmark(filepath);
			
			System.out.println("Sort CompSci Catalog Benchmark: ");
			courseCatalog.sortCompSciCatalogBenchmark(filepath);
			
			System.out.println("Lookup Benchmark: ");
			courseCatalog.lookupBenchmark(filepath);
		}

	}
}
