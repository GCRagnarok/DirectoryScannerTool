#include "../include/DirectoryScanner.h"

// Constructor and Destructor -------------------------------------------------------------------------------
DirectoryScanner::DirectoryScanner()
{
	Init();
}

DirectoryScanner::~DirectoryScanner()
{
	Clean(0);
}

// Core Functions -------------------------------------------------------------------------------

/// Initialize the File Scanner with a welcome message
void DirectoryScanner::Init()
{
	std::cout << "Initializing File Scanner...\n";
	std::cout << "\nFile Scanner Tool\n";
}

/// Display the main menu and handle user input
void DirectoryScanner::MainMenu()
{
	std::cout << "\n---------- Main Menu ----------\n";
	std::cout << "\nPlease enter a number to select an option:\n";
	std::cout << "\n1: Enter the directory to scan.\n2: Scan a config file (Scan multiple directories).\n3: Exit the program.\n";

	int input = 0;

	// Loop until valid input is received
	while (true)
	{
		std::cout << "\nWaiting for input...";
		std::cin >> input;

		// Check if input is valid (1-3) and handle invalid input
		if (std::cin.fail() || (input != 1 && input != 2 && input != 3))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nInvalid input. Please enter a number between 1-3.\n";
			std::cout << "\n1: Enter the directory to scan.\n2: Scan a config file (Scan multiple directories).\n3: Exit the program.\n";
		}
		else
		{
			break;
		}
	}

	// Switch on input to call the appropriate function
	switch (input)
	{
	case 1:
		ScanInputDirectory();
		break;
	case 2:
		ScanConfigFile();
		break;
	case 3:
		Clean(0);
		break;
	}
}

/// Scan the user's input directory for files
void DirectoryScanner::ScanInputDirectory()
{
	std::cout << "\n---------- Input Directory Scanner ----------\n";

	std::string dirPath;

	// Loop until a valid directory path is entered
	while (true)
	{
		std::cout << "\nPlease enter the directory path to scan: ";
		std::getline(std::cin >> std::ws, dirPath);

		// Get the attributes of the directory path
		DWORD attribs = GetFileAttributesA(dirPath.c_str());

		// Check if the path is valid and is a directory
		if (attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY))
		{
			break;
		}
		else
		{
			std::cout << "\nInvalid directory path. Please try again.\n";
		}
	}

	// Prompt and set options if not set by command line
	if (!m_SummaryOnlyCommand)
	{
		PromptAndSetSummaryOnly();
	}

	if (!m_OutputCommand)
	{
		PromptAndSetOutputFilename();
	}

	if (!m_ExtensionsCommand)
	{
		PromptAndSetAdditionalExtensions();
	}
	else if (!m_AdditionalExtensions.empty())
	{
		ParseAndAddExtensions();
	}

	// Call function to scan the provided directory and return the output
	ReturnOutput(dirPath);

	// If an output filename is set, print the full path of the output file
	if (!m_OutputFilename.empty())
	{
		char fullPath[MAX_PATH];

		if (GetFullPathNameA(m_OutputFilename.c_str(), MAX_PATH, fullPath, nullptr) > 0)
		{
			std::cout << "\nResults have been saved to file: " << fullPath << "\n";
		}
		else
		{
			std::cout << "\nResults have been saved to file: " << m_OutputFilename << "\n";
		}

		// Clear the output filename if output command was not used to reset for next scan
		if (!m_OutputCommand)
		{
			m_OutputFilename.clear();
		}
	}

	std::cout << "\nReturning to the main menu...\n";
	MainMenu();
}

/// Scan the user's config file for multiple directories
void DirectoryScanner::ScanConfigFile()
{
	std::cout << "\n---------- Config File Scanner ----------\n";

	std::string configPath;
	std::ifstream configFile;

	// Loop until a valid config file path is entered
	while (true)
	{
		std::cout << "\nPlease enter the config file path to scan: ";
		std::getline(std::cin >> std::ws, configPath);

		configFile.open(configPath);

		if (configFile.is_open())
		{
			break;
		}
		else
		{
			std::cout << "Failed to open config file: " << configPath << "\n";
		}
	}

	// Prompt and set options if not set by command line
	if (!m_SummaryOnlyCommand)
	{
		PromptAndSetSummaryOnly();
	}

	if (!m_OutputCommand)
	{
		PromptAndSetOutputFilename();
	}

	if (!m_ExtensionsCommand)
	{
		PromptAndSetAdditionalExtensions();
	}
	else if (!m_AdditionalExtensions.empty())
	{
		ParseAndAddExtensions();
	}

	std::string dirPath;
	bool foundAny = false;

	// While loop to read each line from the config file
	while (std::getline(configFile, dirPath))
	{
		// Trim whitespace
		dirPath.erase(0, dirPath.find_first_not_of(" \t\r\n"));
		dirPath.erase(dirPath.find_last_not_of(" \t\r\n") + 1);

		// Skip empty lines
		if (dirPath.empty()) continue;

		// Get the attributes of the directory path
		DWORD attribs = GetFileAttributesA(dirPath.c_str());

		// Check if the path is valid and is a directory
		if (attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::cout << "\nScanning directory from config: " << dirPath << "\n";

			// Call function to scan the provided directory and return the output
			ReturnOutput(dirPath);

			foundAny = true;
		}
		else
		{
			std::cout << "\nInvalid directory in config: " << dirPath << "\n";
		}
	}

	// If no valid directories were found in the config file, print a message
	if (!foundAny)
	{
		std::cout << "\nNo valid directories found in config file.\n";
	}

	// If an output filename is set, print the full path of the output file (print filename if GetFullPathName fails)
	if (!m_OutputFilename.empty() && foundAny)
	{
		char fullPath[MAX_PATH];

		if (GetFullPathNameA(m_OutputFilename.c_str(), MAX_PATH, fullPath, nullptr) > 0)
		{
			std::cout << "\nResults have been saved to file: " << fullPath << "\n";
		}
		else
		{
			std::cout << "\nResults have been saved to file: " << m_OutputFilename << "\n";
		}

		// Clear the output filename to reset for next scan if output command was not used
		if (!m_OutputCommand)
		{
			m_OutputFilename.clear();
		}
	}

	std::cout << "\nReturning to the main menu...\n";
	MainMenu();
}


/// Scan the specified directory recursively and return the output of files found
void DirectoryScanner::ReturnOutput(const std::string& dirPath)
{
	std::vector<std::pair<std::string, uintmax_t>> files;
	std::ofstream fileOut;
	bool writeToFile = !m_OutputFilename.empty();

	// Calls function to fill files vector with files found in the directory recursively
	ScanDirectoryRecursive(dirPath, files);

	// If an output filename is set, open the file in append mode and write the header
	if (writeToFile)
	{
		fileOut.open(m_OutputFilename, std::ios::app);

		if (fileOut.is_open())
		{
			fileOut << "\n========== Scan Results for: " << dirPath << " ==========\n";
		}
	}

	uintmax_t totalSize = 0;

	// Check if detailed output required and print to console
	if (!m_SummaryOnly)
	{
		std::cout << "\n---------- Files and Sizes ----------\n";
		std::cout << "\nFiles found:\n";

		// Loop through the files found and print their names and sizes
		for (const auto& file : files) {
			std::string line = "  " + file.first + " (" + formatSize(file.second) + ")\n";
			std::cout << line;
			totalSize += file.second;
		}
	}
	else
	{
		// loop through the files found and calculate the total size
		for (const auto& file : files)
		{
			totalSize += file.second;
		}
	}

	// Print the summary to console
	std::cout << "\n---------- Summary ----------\n";
	std::cout << "\nTotal files: " << files.size() << "\n";
	std::cout << "Total size: " << formatSize(totalSize) << "\n";
	std::cout << "\n-----------------------------\n";

	// Check if results should be written to a file and if the file is open
	if (writeToFile && fileOut.is_open())
	{
		if (!m_SummaryOnly)
		{
			fileOut << "\n---------- Files and Sizes ----------\n";
			fileOut << "\nFiles found:\n";

			// Loop through the files found and write their names and sizes to the file
			for (const auto& file : files)
			{
				std::string line = "  " + file.first + " (" + formatSize(file.second) + ")\n";
				fileOut << line;
			}

			// Write the summary to the file
			fileOut << "\n---------- Summary ----------\n";
			fileOut << "\nTotal files: " << files.size() << "\n";
			fileOut << "Total size: " << formatSize(totalSize) << "\n";
		}
		else
		{
			fileOut << "\n---------- Summary ----------\n";
			fileOut << "\nTotal files: " << files.size() << "\n";
			fileOut << "Total size: " << formatSize(totalSize) << "\n";
		}

		fileOut.close();
	}
}

/// Exit the program and print a message based on the exit code
void DirectoryScanner::Clean(int exitCode)
{
	if (exitCode == 0)
	{
		std::cout << "\nExiting the program...\n";
	}
	else
	{
		std::cout << "\nExiting due to an error (code " << exitCode << ")...\n";
		exit(exitCode);
	}
}

//End of Core Functions ----------------------------------------------------------------------------------------------------------------------------------------------------

//Helper functions ----------------------------------------------------------------------------------------------------------------------------------------------------

/// Prompt user for a yes/no input and return true for 'y' and false for 'n'
bool DirectoryScanner::PromptYesNo(const std::string& prompt)
{
	char choice = '\0';

	// Loop to prompt the user until a valid input is received
	do
	{
		std::cout << prompt;
		std::cin >> choice;

		// Clear the input buffer to handle any extra characters
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Convert the input to lowercase for case-insensitive comparison
		choice = std::tolower(static_cast<unsigned char>(choice));

		if (choice != 'y' && choice != 'n')
		{
			std::cout << "\nInvalid input. Please enter 'y' or 'n'.\n";
		}
	} while (choice != 'y' && choice != 'n');

	return choice == 'y';
}

/// Prompt the user if they want to view only the summary and set summary only accordingly
void DirectoryScanner::PromptAndSetSummaryOnly()
{
	bool summaryOnlyChoice = PromptYesNo("\nWould you like to view ONLY the summary (total number of files and total size)? (y/n): ");
	SetSummaryOnly(summaryOnlyChoice);
}

/// Prompt the user if they want to save the results to a file and set the output filename accordingly
void DirectoryScanner::PromptAndSetOutputFilename()
{
	bool saveResults = PromptYesNo("\nWould you like to save the results to a file? (y/n): ");
	if (saveResults)
	{
		std::string filename;

		std::cout << "\nEnter filename (including format, i.e '.txt') to save results : ";
		std::getline(std::cin, filename);
		SetOutputFilename(filename);
	}
}

void DirectoryScanner::PromptAndSetAdditionalExtensions()
{
	// Build a comma-separated string of current extensions
	std::string extensionsList;

	for (size_t i = 0; i < m_ValidExtensions.size(); ++i)
	{
		extensionsList += m_ValidExtensions[i];
		if (i != m_ValidExtensions.size() - 1)
			extensionsList += ", ";
	}

	std::cout << "\nThe current list of valid extensions include: " << extensionsList << "\n";
	bool addExtensions = PromptYesNo("\nWould you like to add additional file extensions to scan? (y/n): ");

	if (addExtensions)
	{
		std::cout << "\nEnter additional file extensions separated by commas (e.g. 'jpg,png,gif'): ";
		std::getline(std::cin >> std::ws, m_AdditionalExtensions);

		ParseAndAddExtensions();
	}
}

void DirectoryScanner::ParseAndAddExtensions()
{
	std::stringstream ss(m_AdditionalExtensions);
	std::string ext;
	while (std::getline(ss, ext, ','))
	{
		// Trim whitespace
		ext.erase(0, ext.find_first_not_of(" \t\r\n"));
		ext.erase(ext.find_last_not_of(" \t\r\n") + 1);

		// Convert to lowercase
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		// Add if not already present
		if (!ext.empty() && std::find(m_ValidExtensions.begin(), m_ValidExtensions.end(), ext) == m_ValidExtensions.end())
		{
			m_ValidExtensions.push_back(ext);
		}
	}
	m_AdditionalExtensions.clear(); // Clear the additional extensions once added to list
}

/// Check if the file has a valid extension ("png", "bmp", "dds", "tga", "mp4", "avi", "mov", "mkv")
bool DirectoryScanner::HasValidExtension(const std::string& filename)
{
	// Find the last dot in the filename to extract the extension
	size_t dot = filename.find_last_of('.');

	// If no dot found, invalid extension, return false
	if (dot == std::string::npos) return false;

	// Extract the extension after the dot
	std::string ext = filename.substr(dot + 1);

	// Convert the extension to lowercase for case-insensitive comparison
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	// Check if the extension is in the list of valid extensions and return the result
	return std::find(m_ValidExtensions.begin(), m_ValidExtensions.end(), ext) != m_ValidExtensions.end();
}

/// Recursively scan a directory and its subdirectories for files with valid extensions
void DirectoryScanner::ScanDirectoryRecursive(const std::string& dirPath, std::vector<std::pair<std::string, uintmax_t>>& files)
{
	// Check if the directory path is empty
	std::string searchPath = dirPath + "\\*";

	// Declare a structure to hold file/folder information
	WIN32_FIND_DATAA findData;

	// Use FindFirstFileA to find the first file/folder in the directory matching the search pattern
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

	// If no files are found, exit the function
	if (hFind == INVALID_HANDLE_VALUE) return;

	// Loop through all files/folders found in the directory
	do
	{
		// Skip the current directory and parent directory entries
		std::string name = findData.cFileName;
		if (name == "." || name == "..") continue;

		// Construct the full path of the file/folder
		std::string fullPath = dirPath + "\\" + name;

		// Check if the found item is a directory
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Recurse into subdirectory
			ScanDirectoryRecursive(fullPath, files);
		}
		else if (HasValidExtension(name)) // If the item is a file and has a valid extension
		{
			// Get the file size using LARGE_INTEGER to handle large files correctly
			LARGE_INTEGER size;
			size.HighPart = findData.nFileSizeHigh;
			size.LowPart = findData.nFileSizeLow;

			// Add the file's full path and size to the files vector
			files.emplace_back(fullPath, static_cast<uintmax_t>(size.QuadPart));
		}
	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);
}

/// Format the size of a file into relevant units (bytes, KB, MB, GB)
std::string DirectoryScanner::formatSize(uintmax_t bytes)
{
	// Define constants for size conversions
	const double KB = 1024.0;
	const double MB = KB * 1024.0;
	const double GB = MB * 1024.0;

	// Buffer to hold the formatted size string
	char buf[64];

	// Check the size and format it accordingly
	if (bytes >= static_cast<uintmax_t>(GB))
	{
		std::snprintf(buf, sizeof(buf), "%.2f GB", bytes / GB);
	}
	else if (bytes >= static_cast<uintmax_t>(MB))
	{
		std::snprintf(buf, sizeof(buf), "%.2f MB", bytes / MB);
	}
	else if (bytes >= static_cast<uintmax_t>(KB))
	{
		std::snprintf(buf, sizeof(buf), "%.2f KB", bytes / KB);
	}
	else // If the size is less than 1 KB, format it as bytes
	{
		std::snprintf(buf, sizeof(buf), "%llu bytes", static_cast<unsigned long long>(bytes));
	}

	return std::string(buf);
}
// End of Helper Functions ----------------------------------------------------------------------------------------------------------------------------------------------------