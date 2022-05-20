#include "include.cpp"
#include <string>

int main(int argv, char** args) {
    // Constructing application
    application myApp;
    if (argv > 1) {
        std::string aiModeField = std::string(args[1]);
        myApp.setAiMode(!(aiModeField == "-m" || aiModeField == "m" || aiModeField == "manual"));
    }
    else {
        myApp.setAiMode(true);
    }
    // Start the application
    myApp.start();
    return 0;
}
