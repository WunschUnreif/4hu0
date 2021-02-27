#include <iostream>
#include <fstream>

void record(std::string & command, bool from_engine = false) {
    static std::ofstream command_output_file("xboard.txt");
    if(from_engine) {
        command_output_file << "> " << command << std::endl;
    } else {
        command_output_file << command << std::endl;
    }
}


void deal_command(std::string & command) {
    auto first_word = command.substr(0, command.find_first_of(' '));

    if(first_word == "protover") {
        std::cout   << "feature ping=1 setboard=1 playother=1 reuse=0 myname=\"4hu0\" variants=\"xiangqi\" colors=0" 
                    << std::endl;
    }
}

int main() {
    std::string command;
    while(true) {
        std::getline(std::cin, command);
        
        record(command, true);
        deal_command(command);
    }

    return 0;
}
