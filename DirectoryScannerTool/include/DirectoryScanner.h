#pragma once

// Standard library headers
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

// Platform-specific headers
#define NOMINMAX
#include <windows.h>

class DirectoryScanner
{
public:
	// Constructor and Destructor
	DirectoryScanner();
	~DirectoryScanner();

	// Core Functions
	void Init();
	void MainMenu();
	void Clean(int exitCode);

	// Helper Functions
	void SetSummaryOnlyCommand(bool value) { m_SummaryOnlyCommand = value; }
	void SetSummaryOnly(bool value) { m_SummaryOnly = value; }

	void SetOutputCommand(bool value) { m_OutputCommand = value; }
	void SetOutputFilename(const std::string& filename) { m_OutputFilename = filename; }

	void SetExtensionsCommand(bool value) { m_ExtensionsCommand = value; }
	void SetAdditionalExtensions(const std::string& arg) { m_AdditionalExtensions = arg; }

private:
	// Core Functions
	void ScanInputDirectory();
	void ScanConfigFile();
	void ReturnOutput(const std::string& dirPath);

	// Helper Functions
	bool PromptYesNo(const std::string& prompt);
	void PromptAndSetSummaryOnly();
	void PromptAndSetOutputFilename();
	void PromptAndSetAdditionalExtensions();

	void ParseAndAddExtensions();
	bool HasValidExtension(const std::string& fileName);
	void ScanDirectoryRecursive(const std::string& dirPath, std::vector<std::pair<std::string, uintmax_t>>& files);

	std::string formatSize(uintmax_t bytes);

	// Member Variables 
	bool m_SummaryOnly = false;
	bool m_SummaryOnlyCommand = false;

	std::string m_OutputFilename;
	bool m_OutputCommand = false;

	bool m_ExtensionsCommand = false;
	std::string m_AdditionalExtensions;
	std::vector<std::string> m_ValidExtensions = { "png", "bmp", "dds", "tga", "mp4", "avi", "mov", "mkv" };
};
