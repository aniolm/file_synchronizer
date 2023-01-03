#include "..//inc/view.hpp"

#include <set>
#include <filesystem>

namespace fs = std::filesystem;

void View::setListener(i_ViewListener* listener)
{
	this->listener = listener;
}

void View::printMenu() {
    std::cout << "### FILE SYNCHRONIZER ### \n\n";
}

void View::printOptions() {
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Force sync-up  \n";
}

void View::waitForButton() {
    system("read -n 1 -s -p \"Press any key to continue...\"");
}

void View::printDirectory() {
    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const& dirEntry : fs::directory_iterator(mainDirectoryPath)) {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton();
}

bool View::validationForPrinting(std::string name) {
    if (name == "all") {
        return true;
    }
    if (!fs::exists(fs::current_path() / name)) {
        std::cout << "Not exist file or directory\n";
        waitForButton();
        return false;
    }
    return true;
}

void View::printFiles() {
    std::set<fs::path> sorted_by_name;
    std::cout << "Give folder name or choose 'all' to print files: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;

    if (validationForPrinting(dirName)) {
        if (dirName == "all") {
            dirName = mainDirectoryPath;
        }
        for (auto const& dirEntry : fs::recursive_directory_iterator(dirName)) {
            sorted_by_name.insert(dirEntry.path());
        }

        for (auto const& fileName : sorted_by_name) {
            if (fs::is_directory(fileName)) {
                std::cout << "-------------------\n";
                std::cout << "Directory: " << fileName.filename() << ":\n";
            }
            if (fs::is_regular_file(fileName)) {
                std::cout << fileName.filename() << "\n";
            }
        }

        if (fs::is_empty(mainDirectoryPath / dirName))
            std::cout << "Folder is empty...";

    std::cout << "\n";
    waitForButton();
    }

}

void View::run()
{   
    printMenu();
    printOptions();
    uint32_t action;
	while (true)
	{
        std::cin.clear();
        std::cin >> action;
		switch (static_cast<Action>(action)) {
            case Action::AddDir:
                std::cout << "added directory \n";
                listener->addDirectory(std::cin);
                break;
            case Action::RemoveDir:
                std::cout << "Removed directory \n";
                listener->removeDirectory();
                break;
            case Action::RemoveFile:
                std::cout << "Removed file \n";
                listener->removeFile();
                break;
            case Action::PrintDir:
                listener->printDirectory();
                break;
            case Action::PrintFiles:
                listener->printFiles();
                break;
            case Action::SetIntervalTime:
                listener->setIntervalTime();
                break;
             case Action::StartSync:
                listener->startSync();
                break;
            case Action::ForceSync:
                listener->forceSync();
                break;
            case Action::Exit:
                listener->exit();
                break;
            default:
                std::cout << "Incorrect action selected! Please try again...\n";
                waitForButton();
                break;
        }
	}
}