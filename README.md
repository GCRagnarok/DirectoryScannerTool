# Directory Scanner Tool User Guide

## Overview

This tool scans the provided directory, or a list of directories (from a config file), for files with specific extensions and provides a summary or detailed report of the files found, including their name and size.
Currently Supported Extensions - "png", "bmp", "dds", "tga", "mp4", "avi", "mov", "mkv".

## How to Run the Application

### 1. Build the Application
Ensure you have a C++14-compatible compiler (e.g., Visual Studio 2022).
Build the solution/project in Visual Studio.

### 2. Run the Application
Launch the executable from the build output directory, or run from the command line:
C:\PathToBuildOutputDirectory\DirectoryScannerTool.exe

### 2.1 Command-Line Arguments
Argument	Description	Example
--summary-only	Only display the summary (total files and size).	DirectoryScannerTool.exe --summary-only
--output <filename.ext>	Save results to the specified file.              	DirectoryScannerTool.exe --output results.txt
--extensions	Add additional extensions to the list of currently supported extensions.	DirectoryScannerTool.exe --extensions jpeg, tiff, wmv

You can also combine arguments:
DirectoryScannerTool.exe --summary-only --output results.txt --extensions jpeg, tiff, wmv

### 3. Main Menu Options
Upon running, you will be presented with the following options:
1: Enter the directory to scan.
2: Scan a config file (scan multiple directories).
3: Exit the program.
Input a number between 1 – 3 to proceed.

### 4. Scan a directory

### 4.1 Input Directory Scanner
Selecting option 1 in the main menu will prompt you to provide the path to the directory you wish scan, i.e C:\Users\name\FolderToScan. 
The provided path can be either relative or absolute. If the provided path is relative, please ensure the program is located in the same working directory you wish to scan.
The tool will scan recursively, ensuring that any nested files within the directory will also be included in the scan.
If no command line arguments were provided you will be prompted with additional options. See section 4 for more information.

### 4.2 Config File Scanner
Selecting option 2 in the main menu will prompt you to provide the path to the config file you wish to scan. This config file can contain a list of directories for the application to scan.

Config.txt (Example config file)
```
C:\Users\Artist\Pictures
C:\Users\Artist\Videos
D:\Projects \Textures
D:\Projects \Materials

# The following line is a comment and will be ignored by the program

# Invalid directory (will be reported as invalid)
C:\Invalid\Path

C:\Users\Artist\Folder With Spaces

# Blank lines and lines with only whitespace are ignored

```
If no command line arguments were provided you will be prompted with additional options. See section 5 for more information.

### 5. Options
If no command line arguments were provided prior to launching the application, you will be prompted with the following options when scanning either a directory or a config file:
•	Summary Only
•	Save Results
•	Add additional extensions

#### 5.1 View Summary Only
Allows the user to display ONLY the summary of the scanned directories (total number of files and total size of all files).
Input ‘n’ when prompted to see the full breakdown (individual file names and sizes) in addition to the summary. 
Input ‘y’ when prompted to view only the summary.

### 5.2 Save Results
Allows the user to save the results and provide a filename with an extension. 
This file will be saved in the output build directory next to the program executable. 
You will also be notified of the save location within the console application. 
Input ‘y’ when prompted to save the results, followed by the filename and extension you wish to save the file as. 
Input ‘n’ when prompted to skip this step and only display the information within the console application.

#### 5.3 Add additional extensions
Allows the user to add additional extensions to the list of currently supported extensions when scanning a directory.
Input ‘y’ when prompted to add additional extensions, the user will then be asked to input the extensions they wish to add.
Input ‘n’ to only scan the directory for the currently supported list of extensions.

