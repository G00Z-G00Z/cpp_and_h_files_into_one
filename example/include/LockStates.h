#pragma once
#include <LockStateMachine.h>

/**
 * @brief Changes the password. If it is not the first time, then it will prompt
 * you to enter the current password
 *
 */
class ChangePasswordState : public LockState
{
    bool isFirstTimeResettingPassword = false;

    /**
     * @brief Internal funciton for handling resetting password.
     *
     */
    void resetPassword();

public:
    /**
     * @brief Construct a new Change Password State object.
     *
     * @param isFirstTime Only use true, if it is the initial state of the
     * context.
     */
    ChangePasswordState(bool isFirstTime = false);
    void handleLock() override;
};

/**
 * @brief Locks the lock, and listens for inputs
 *
 */
class LockedState : public LockState
{
public:
    void handleLock() override;
};

/**
 * @brief Validates the password, and decides whether to go to the error state,
 * locked state or the unlocked state.
 *
 */
class ValidatingState : public LockState
{
public:
    void handleLock() override;
};

/**
 * @brief Blocks the lock for 15 secs. After, it sends it to the lock state.
 *
 */
class ErrorState : public LockState
{
public:
    void handleLock() override;
};

/**
 * @brief Unlocks the lock, and listens whether or not to go to the locked or
 * change password state
 *
 */
class UnlockedState : public LockState
{
public:
    void handleLock() override;
};
