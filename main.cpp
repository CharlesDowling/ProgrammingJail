#include <iostream>
#include <ncurses.h>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>

using namespace std;

//Text Editor Declaration
void text_editor( char *ch, string char_buffer , long character , string selected_file);

string file_browser();
void file_ex_help();

int main()
{
    //Initialize window and screen settings
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    //Create Screen Buffer
    string char_buffer;

    //Create variable holders
    char ch;
    char mch;
    string selected_file = "N/A";
    string status = "NORMAL";

    //This shit long so text never has issues
    long character;

    //Main Menu
    while ( mch != 27 ){
        clear();

        printw("Current program status: ");
        printw(status.c_str());
        printw("\nWelcome to the Editor!\n\n");
        printw("Copyright 2025 - Charles E. Dowling Jr.\n\n");
        printw("Current Working Directory: ");
        //TODO
        //printw();
        printw("Selected File:   ");
        printw(selected_file.c_str());
        printw("\n\n");

        printw("1. File Browser\t\t2. Text Editor\n");
        printw("3. TBC         \t\t4. TBC\n");
        refresh();

        //Get user selection
        mch = getch();

        //Clears screen after submission
        clear();

        //Check which option the user selected
        //No default in case the user is a dumb cunt - Trial by Fire!
        switch (mch){
            case 49:
                selected_file = file_browser();
                break;
            case 50:
                if (filesystem::exists(selected_file)){
                    ifstream file(selected_file);
                    string line;
                    while (getline(file, line)){
                        char_buffer = char_buffer + line;
                    }
                    text_editor(&ch, char_buffer, character, selected_file);
                } else {
                    status = "NO FILE SELECTED!";
                }
                char_buffer = "";
                break;
            case 51:
                //Compiler Options
                //To be done later
                break;
            case 52:
                //Run compiled program
                //To be done later
                break;
        }
    }

    //YEET THE PROGRAM
    endwin();

    return 0;
}

void text_editor( char *ch, string char_buffer , long character, string selected_file){

    *ch = 0;

    for(character = 0; character < char_buffer.length(); character++){
        addch(char_buffer[character]);
    }

    while ( true ){

                refresh();
                *ch = getch();
                clear();
                //addch(ch);

                if (*ch != 27){
                    //Checks if the character is backspace
                    if ( *ch != 8 && *ch != 7){
                        //Add character if not backspace
                        char_buffer = char_buffer + *ch;
                    } else {
                        //Remove character at end of string if backspace
                        if(char_buffer.length() > 0){
                            char_buffer.pop_back();
                        }
                    }
                }
                //Buffers window
                for(character = 0; character < char_buffer.length(); character++){
                    addch(char_buffer[character]);
                }

                if (char_buffer[(char_buffer.length()-1)] == 7){
                    char_buffer.pop_back();
                    char_buffer.pop_back();
                }

                //User exit
                //Probably going to add a bunch of escape character options later...
                if ( *ch == 27 ){
                    clear();
                    *ch = 0;
                    printw("Save File? (S/s)");
                    printw("Exit Text Editor? (X/x): ");
                    printw("Cancel? (esc)");
                    *ch = getch();
                    if (*ch == 88 || *ch == 120) {
                        break;
                    } else if (*ch == 83 || *ch == 115){
                        ofstream file(selected_file);
                        file << char_buffer;
                        break;
                    } else {
                        *ch = 0;
                        clear();
                        for(character = 0; character < char_buffer.length(); character++){
                            addch(char_buffer[character]);
                        }
                    }
                }
                //End of While Loop
    }
}

string file_browser(){
    //noraw();

    string command = "";
    string status = "NORMAL";
    string selected_file = "";
    char command_ch;

    while (true){
        clear();
        std::string path = ".";

        //Tell the user how much of a dumb cunt they are
        printw("File Explorer Status: ");
        printw(status.c_str());

        //Prints the current working directory
        printw("\n\nCurrent Working Directory: ");
        printw(filesystem::current_path().c_str());

        //Pretty spaces
        printw("\n\n");

        for (const auto & entry : filesystem::directory_iterator(path)){
            printw(entry.path().c_str());
            printw("\n");
            refresh();

        }

        for(long character = 0; character < command.length(); character++){
                    addch(command[character]);
        }

        command_ch = getch();

        if (command_ch != 10 && command_ch != 8){
            command = command + command_ch;
        } else if (command_ch == 8 && command.length() > 0){
            command.pop_back();
        }

        //For whatever fucking infuriating reason
        //BASH decided that backspaces make the last
        //character an ASCII bell...CUNTS!
        if (command[(command.length()-1)] == 7){
            command.pop_back();
            command.pop_back();
        }

        printw(command.c_str());

        if (command_ch == 27){
            break;
        }

        //Check if enter key has been pressed
        if (command_ch == 10){


            if (command == "quit"){
                //Quits the menu
                break;

            } else if ( command[0] =='c' && command[1] == 'd' && command[2] == ' '){
                //Changes directory

                //Gets directory name from command
                string new_directory = command.substr(3, (static_cast<int>(command.length()) - 2));

                //Test for if directory exists
                if (filesystem::exists(new_directory)){

                    //Changes File Explorer Status
                    status = "CHANGED DIRECTORY TO - " + new_directory;

                    printw(filesystem::current_path().c_str());

                    //Checks if Absolute path or Relative
                    if (new_directory[0] == '/'){
                        //Absolute Path
                        filesystem::current_path(new_directory);

                    } else {
                        //Relative Path
                        filesystem::current_path("./" + new_directory);

                    }

                } else {
                        status = "REQUESTED DIRECTORY " + new_directory + " NOT FOUND";
                }

                printw("\nNew Directory:");
                printw(new_directory.c_str());

            } else if (command[0] == '.' && command[1] == '/'){
                selected_file = command;
                break;
            } else if ( command[0] =='h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'p'){
                file_ex_help();
            }
            printw("\nMOST RECENT COMMAND:");
            printw(command.c_str());

            for (int x = 0; x < command.length(); x++){
                printw("%d", x);
                printw(":");
                printw("%x", command[x]);
                printw("\n");
            }

            getch();
            command = "";
        }
        //End of giant if statement

        command_ch = ' ';

    }
    raw();

    return selected_file;
}

void file_ex_help(){
    clear();
    printw("####File Explorer Help Menu####");
    printw("--------------------------------");
    printw("\n\n\n");
    printw("Select a file by typing in its name including the './'");
    printw("\n\n\n");
    printw("Commands:\n\n");
    printw("cd - Change directory\n");
    printw("help - displays this menu\n");
    printw("\n\nPRESS ANY KEY TO CONTINUE");
    refresh();
    getch();
}

