#include <iostream>
#include <string>

#include <SUGIPROJ/user_interfaces/cmd_ui.h>

int sugi::cmd_ui::nextStep() {
    std::cout << "Enter 0 for Stop, 1 for Move Step Backward, 2 for Move Step Forward" << std::endl;
    std::string input;
    std::cin >> input;
    return std::stoi(input);
}