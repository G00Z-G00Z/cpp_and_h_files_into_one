#include <LockStates.h>
#include <Arduino.h>
#define ERROR_PENALTY_MS 5000

// Change pass state
ChangePasswordState::ChangePasswordState(bool isFirstTime = false) : isFirstTimeResettingPassword(isFirstTime) {}

void ChangePasswordState::resetPassword()
{
    Serial.println("Setting new password");
    this->context->resetPassword();
    Serial.println("Successfully set password! Now password is: ");
    this->context->printPassword();
    this->context->resetError();
    this->context->transitionTo(new LockedState());
}

void ChangePasswordState::handleLock()
{
    this->context->unlock();

    if (isFirstTimeResettingPassword)
    {
        this->resetPassword();
        return;
    }

    Serial.println("Changing the password");
    Serial.println("Enter the current password");
    this->context->read3digits();

    if (this->context->isValidUserInput())
    {
        Serial.println("Correct! Please enter new password");
        this->resetPassword();
        return;
    }

    Serial.println("Incorrect password. Locking!");
    this->context->transitionTo(new LockedState());
}

// Locked state
void LockedState::handleLock()
{
    Serial.println("Locking!");
    this->context->lock();
    this->context->read3digits();
    this->context->transitionTo(new ValidatingState());
}

// Validation
void ValidatingState::handleLock()
{
    Serial.println("Validating!");

    // Valid user input
    if (this->context->isValidUserInput())
    {
        Serial.println("Correct!!");
        this->context->transitionTo(new UnlockedState());
        return;
    }

    // Invalid user input
    int errors = this->context->addError();

    Serial.print("Incorrect! You have ");
    Serial.print(3 - errors);
    Serial.println(" more tries");

    // Critical errors
    if (errors >= 3)
    {
        this->context->transitionTo(new ErrorState());
    }
    else
    {
        this->context->transitionTo(new LockedState());
    }
}

void ErrorState::handleLock()
{
    Serial.println("Incorrect 3 times! Blocked for 15 secs");
    this->context->lock();
    delay(ERROR_PENALTY_MS);
    this->context->resetError();
    Serial.println("Blocked state finished");
    this->context->transitionTo(new LockedState());
}

void UnlockedState::handleLock()
{
    Serial.println("Unlocked!");
    Serial.println("Click '#' to lock again, or click '*' to reset password.");

    this->context->unlock();
    this->context->resetError();
    char key = this->context->readDigitKeyPad();

    switch (key)
    {
    case '*':
        this->context->transitionTo(new ChangePasswordState());
        break;

    case '#':
        this->context->transitionTo(new LockedState());
        break;

    default:
        break;
    }
}
