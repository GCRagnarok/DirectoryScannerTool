#include "include/DirectoryScanner.h"

int main(int argc, char* argv[])
{
    // Declare variables for command arguements
    bool summaryOnly = false;
    std::string outputFilename;

    bool summaryOnlyCommand = false;
    bool outputCommand = false;

    bool extensionsCommand = false;
    std::string extensions;

    // Loop through command line arguements to set command options
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--summary-only")
        {
            summaryOnly = true;
            summaryOnlyCommand = true;
        }
        else if (arg == "--output" && i + 1 < argc)
        {
            outputFilename = argv[++i];
            outputCommand = true;
        }
        else if (arg == "--extensions" && i + 1 < argc)
        {
            extensions = argv[++i];
            extensionsCommand = true;
        }
    }
    // Create an instance of FileScanner
    DirectoryScanner scanner;

    // Set options based on command line arguments
    scanner.SetSummaryOnly(summaryOnly);
    scanner.SetSummaryOnlyCommand(summaryOnlyCommand);

    scanner.SetOutputFilename(outputFilename);
    scanner.SetOutputCommand(outputCommand);

    scanner.SetAdditionalExtensions(extensions);
    scanner.SetExtensionsCommand(extensionsCommand);

    // Call MainMenu to start the program
    scanner.MainMenu();
    return 0;
}
