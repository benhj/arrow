#pragma once

#include <thread>      // For std::thread
#include <mutex>       // For std::mutex, std::lock_guard
#include <atomic>      // For std::atomic<bool>
#include <termios.h>   // For POSIX terminal control (termios)

namespace arrow {

class KeyHandler {
public:
    // Constructor: Starts the input thread
    KeyHandler();

    // Destructor: Stops the input thread and restores terminal settings
    ~KeyHandler();

    // Retrieves the last key pressed and clears it.
    // Returns '\0' if no new key has been pressed since the last call.
    char get_last_key();

    // Checks if a specific key is currently the last key pressed, then clears it.
    // Useful for checking specific commands.
    bool was_key_pressed(char key);

    // Checks if any key was pressed
    bool was_any_key_pressed();

private:
    // The input thread's main function
    void input_loop();

    // Sets the terminal to raw mode (non-canonical, no echo)
    void set_raw_mode();

    // Restores the terminal to its original settings
    void restore_terminal_mode();

    // --- Thread-safe member variables ---
    std::thread input_thread;               // The thread handling key input
    std::atomic<bool> running;              // Flag to signal the thread to stop
    std::mutex key_mutex;                   // Mutex to protect 'last_key'
    char last_key;                          // Stores the last key character received

    // Original terminal settings, to be restored when the KeyHandler is destroyed
    struct termios original_termios;
};

} // namespace arrow
