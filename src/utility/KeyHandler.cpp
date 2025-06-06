#include "KeyHandler.hpp"
#include <iostream>    // For debugging/error output
#include <unistd.h>    // For STDIN_FILENO, getchar

namespace arrow {

// Constructor
KeyHandler::KeyHandler() : running(true), last_key('\0') {
    // Save original termios settings and set raw mode
    set_raw_mode();
    // Start the input thread
    input_thread = std::thread(&KeyHandler::input_loop, this);
}

// Destructor
KeyHandler::~KeyHandler() {
    // Signal the input thread to stop
    running.store(false);

    // *IMPORTANT NOTE FOR CLEAN SHUTDOWN WITH getchar():*
    // If getchar() is currently blocking in the input_loop,
    // the thread will not terminate until another key is pressed.
    // A robust solution for this would typically involve:
    // 1. Using `ncurses` with `nodelay()` which makes getch() non-blocking.
    // 2. Using `select()` or `poll()` on STDIN_FILENO with a timeout.
    // 3. On some systems, writing a dummy character to stdin (very tricky and not portable).
    // For this termios-based example, the user must press a key to unblock `getchar()`
    // after the simulation's main loop signals quit, or you accept a delay in shutdown.
    // For demonstration, we'll assume a key will eventually be pressed or the user accepts this.

    // Join the input thread to ensure it finishes before the destructor exits
    if (input_thread.joinable()) {
        input_thread.join();
    }

    // Restore terminal settings
    restore_terminal_mode();
    std::cout << "\nKeyHandler: Input thread stopped and terminal restored.\n";
}

// Input thread's main function
void KeyHandler::input_loop() {
    while (running.load()) {
        char c = getchar(); // This will block until a character is available

        // Check if the signal to stop arrived while getchar() was blocking
        if (!running.load()) {
            break; // Exit the loop if signalled to stop
        }

        // Protect the shared variable with a mutex
        std::lock_guard<std::mutex> lock(key_mutex);
        last_key = c;
    }
}

// Sets the terminal to raw mode
void KeyHandler::set_raw_mode() {
    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &original_termios);

    struct termios new_termios = original_termios;

    // Disable canonical mode (ICANON) and echoing (ECHO)
    new_termios.c_lflag &= ~(ICANON | ECHO);

    // Set VMIN and VTIME for non-blocking read behavior:
    // VMIN = 1: read will block until at least 1 character is available.
    // VTIME = 0: no timeout; read will return as soon as VMIN characters are available.
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;

    // Apply the new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

// Restores the terminal to its original settings
void KeyHandler::restore_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

// Retrieves the last key pressed and clears it
char KeyHandler::get_last_key() {
    std::lock_guard<std::mutex> lock(key_mutex);
    char key = last_key;
    last_key = '\0'; // Clear the key after reading it
    return key;
}

// Checks if a specific key was pressed and clears it
bool KeyHandler::was_key_pressed(char key) {
    std::lock_guard<std::mutex> lock(key_mutex);
    if (last_key == key) {
        last_key = '\0'; // Clear the key
        return true;
    }
    return false;
}

bool KeyHandler::was_any_key_pressed() {
    std::lock_guard<std::mutex> lock(key_mutex);
    return last_key != '\0';
}

} // namespace arrow