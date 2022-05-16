#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "HexFile.hpp"

struct termios orig_termios;

void enableRawMode();
void disableRawMode();

void getWinDims(int& width, int& height);

void update();

HexFile* f;

const char* fName;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Syntax: hexer <filename>\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "\033[2J\033[H"; // clear screen

    int width = 0;
    int height = 0;
    getWinDims(width, height);

    f = new HexFile(argv[1]);
    f->maxRows = height - 6;
    f->cols = (width - 19) / 4;
    f->select(0);

    fName = argv[1];

    char c;
    enableRawMode();
    update();

    while (true)
    {
        if (!read(STDIN_FILENO, &c, 1)) { continue; }
        if (c == 'q') { break; }

        switch (c)
        {

        // EDITOR COMMANDS
        case 'r': // Replace value
        {
            disableRawMode();
            int temp;
            std::cout << " >> (REPLACE) - Enter new value: ";
            std::cin >> std::hex >> temp >> std::dec;
            f->setCurrent((unsigned char) temp);
            enableRawMode();
            update();
            break;
        }
        case 'R': // Replace multiple values
        {
            disableRawMode();

            std::string temp;
            std::cout << " >> (REPLACE-C) - Enter new bytes: ";
            std::getline(std::cin, temp);
            std::istringstream iss(temp);
            
            while (std::getline(iss, temp, ' '))
            {
                f->setCurrent((unsigned char) strtol(temp.c_str(), NULL, 16));
                f->selectNext();
            }

            std::cout << "\033[2J"; // fast clear
            enableRawMode();
            update();
            break;
        }
        case 'i': // Insert value
        {
            disableRawMode();
            int temp;
            std::cout << " >> (INSERT) - Enter new value: ";
            std::cin >> std::hex >> temp >> std::dec;
            f->insert(temp);
            enableRawMode();
            update();
            break;
        }
        case 'I': // Insert multiple values
        {
            disableRawMode();

            std::string temp;
            std::cout << " >> (INSERT-C) - Enter new values: ";
            std::getline(std::cin, temp);
            std::istringstream iss(temp);

            while (std::getline(iss, temp, ' '))
            {
                f->insert((unsigned char) strtol(temp.c_str(), NULL, 16));
            }

            std::cout << "\033[2J"; // fast clear
            enableRawMode();
            update();
            break;
        }
        case 'a': // Append value
        {
            disableRawMode();
            int temp;
            std::cout << " >> (APPEND) - Enter new value: ";
            std::cin >> std::hex >> temp >> std::dec;

            int sel = f->SelectionAsInt();

            if (sel == -1) { break; }

            if (sel < f->Bytes() - 1)
            {
                f->insert(sel + 1, temp);
                f->selectNext();
                enableRawMode();
                update();
                break;
            }

            f->push_back(temp);
            f->selectNext();
            enableRawMode();
            update();
            break;
        }
        case 'A': // Append multiple values
        {
            disableRawMode();

            std::string temp;
            std::cout << " >> (APPEND-C) - Enter new values: ";
            std::getline(std::cin, temp);
            std::istringstream iss(temp);

            while (std::getline(iss, temp, ' '))
            {
                int sel = f->SelectionAsInt();
                if (sel == -1) { break; }

                if (sel < f->Bytes() - 1)
                {
                    f->insert(sel + 1, (unsigned char) strtol(temp.c_str(), NULL, 16));
                }
                else
                {
                    std::cout << "PUSH\n";
                    f->push_back((unsigned char) strtol(temp.c_str(), NULL, 16));
                }

                f->selectNext();
            }

            std::cout << "\033[2J"; // fast clear
            enableRawMode();
            update();
            break;
        }
        case 'x': // Delete value
            f->del();
            update();
            break;
        case 'X': // Delete multiple values
        {
            disableRawMode();

            int c;

            std::cout << " >> (DEL-C) - Enter number of values to delete: ";
            std::cin >> c;

            for (int i = 0; i < c; ++i)
            {
                f->del();
            }

            enableRawMode();
            update();
            break;
        }
        case 'g': // Goto byte
        {
            disableRawMode();
            int pos;

            std::cout << " >> (GOTO) - Enter new pos: ";
            std::cin >> std::hex >> pos >> std::dec;

            f->select(pos);
            enableRawMode();
            update();
            break;
        }
        case 'G': // Goto end EOF
            f->select(f->Bytes() - 1);
            update();
            break;
        case '/': // Find byte
        {
            disableRawMode();
            int temp;
            std::cout << "Find: ";
            std::cin >> std::hex >> temp >> std::dec;
            f->highlightAll((unsigned char) temp);
            enableRawMode();
            update();
            break;
        }
        case 'K': // Turn off highlighting
            f->unhighlightAll();
            update();
            break;
        case 'n': // Goto next highlighted byte
            f->selectNextHighlight();
            update();
            break;
        case 'N': // Goto previous highlighted byte
            f->selectPrevHighlight();
            update();
            break;
        case 'W': // Write to file
        {
            disableRawMode();
            std::string temp;
            std::cout << "Enter file to write to: ";
            std::cin >> temp;
            f->writeTo(temp);
            enableRawMode();
            update();
            break;
        }
        case 'O': // Open new file
        {
            disableRawMode();
            std::string temp;
            std::cout << "Enter file to open: ";
            std::cin >> temp;

            int width = 0;
            int height = 0;
            getWinDims(width, height);

            delete f;
            f = new HexFile(temp);
            f->maxRows = height - 6;
            f->cols = (width - 19) / 4;
            f->select(0);

            fName = temp.c_str();

            enableRawMode();
            update();
            break;
        }

        // CURSOR NAVIGATION

        // UP
        case 'k':
            f->selectPrevRow();
            update();
            break;
        // LEFT
        case 'h':
            f->selectPrev();
            update();
            break;
        // DOWN
        case 'j':
            f->selectNextRow();
            update();
            break;
        // RIGHT
        case 'l':
            f->selectNext();
            update();
            break;

        // Detect Arrow keypresses
        case 27:
            if (!read(STDIN_FILENO, &c, 1)) { break; }
            if (c != '[') { break; }

            if (!read(STDIN_FILENO, &c, 1)) { break; }

            switch (c)
            {
            case 'A':
                f->selectPrevRow();
                update();
                break;
            case 'B':
                f->selectNextRow();
                update();
                break;
            case 'C':
                f->selectNext();
                update();
                break;
            case 'D':
                f->selectPrev();
                update();
                break;
            }
        default:
            std::cout << "FILENAME: " << f->Filename << '\n';
            update();
        }
    }

    std::cout << "\033[2J\033[H"; // clear screen

    delete f;
    return 0;
}

void update()
{
    f->autoSizeRows();

    //std::cout << "\033[2J\033[H"; // fast way to clear the screen
    std::cout << "\033[H"; // move cursor to beginning to overwrite
    std::cout << "\033[2K"; // Clear first line
    std::cout << "File: " << fName << "\n\033[2K\n" << *f;
}

void getWinDims(int& width, int& height)
{
    struct winsize ws;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) { return; }

    width = ws.ws_col;
    height = ws.ws_row;
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    // timeout to read chars
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
