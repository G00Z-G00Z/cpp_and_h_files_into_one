#pragma once
#include <Servo.h>
#include <Led.h>
#include <Keypad.h>

class LockContext;

/**
 * @brief State that the lock can be in. It handles the lock according to the
 * state, using the context itself
 *
 */
class LockState
{

protected:
    LockContext *context;

public:
    /**
     * @brief Destroy the Lock State object and it does the transition actions
     *
     */
    virtual ~LockState();
    /**
     * @brief Set the Context object. In other words, it connects the Lock
     * context to the state.
     *
     * @param context
     */
    void setContext(LockContext *context);

    /**
     * @brief Handles the lock, including reading the inputs and outputs.
     *
     */
    virtual void handleLock() = 0;
};

/**
 * @brief This context has a link to all the actuators of the lock, and it is
 * controlled by its own state.
 *
 */
class LockContext
{

protected:
    LockState *state;

    // Password
    char password[3];
    // User input
    char userInput[3];

    // Actuators
    Led *openLed;
    Led *closeLed;
    Servo *door;
    Keypad *keypad;

    // Error counter
    int errorCounter = 0;

public:
    LockContext(Led *openLed, Led *closeLed, Servo *door, Keypad *keypad, LockState *initialState);

    ~LockContext();

    /**
     * @brief Checks if the user input matches the password.
     *
     * @return true
     * @return false
     */
    bool isValidUserInput();

    /**
     * @brief Reads a digit in the key pad, blocking execution.
     *
     * @return char
     */
    char readDigitKeyPad();

    /**
     * @brief Adds 1 to the error counter
     *
     * @return int
     */
    int addError();

    /**
     * @brief Locks the lock
     *
     */
    void lock();

    /**
     * @brief Prints the current password
     *
     */
    void printPassword();

    /**
     * @brief Reads 3 digits fromthe keypad, blocking execution. Also, it fills
     * the user input.
     *
     */
    void read3digits();

    /**
     * @brief Sets the counter of errors to 0
     *
     */
    void resetError();

    /**
     * @brief Reads 3 digits from the keypad and sets them to be the password.
     *
     */
    void resetPassword();

    /**
     * @brief Transitions from one state to another.
     *
     * @param state
     */
    void transitionTo(LockState *state);

    /**
     * @brief Unlocks the lock
     *
     */
    void unlock();

    /**
     * @brief Updates the entire lock, by using the current state
     *
     */
    void update();
};
