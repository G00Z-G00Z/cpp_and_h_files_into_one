#include <LockStateMachine.h>
#include <Arduino.h>

LockState::~LockState() {}

void LockState::setContext(LockContext *context) { this->context = context; }

LockContext::LockContext(Led *openLed,
                         Led *closeLed,
                         Servo *door,
                         Keypad *keypad,
                         LockState *initialState) : state(nullptr),
                                                    openLed(nullptr),
                                                    closeLed(nullptr),
                                                    door(nullptr),
                                                    keypad(nullptr)
{
    this->state = initialState;
    this->openLed = openLed;
    this->closeLed = closeLed;
    this->door = door;
    this->keypad = keypad;
}

LockContext::~LockContext()
{
    delete this->state;
    this->closeLed = nullptr;
    this->door = nullptr;
    this->keypad = nullptr;
    this->openLed = nullptr;
    this->state = nullptr;
}

void LockContext::transitionTo(LockState *state)
{
    if (this->state != nullptr)
        delete this->state;
    this->state = state;
    this->state->setContext(this);
}

void LockContext::update()
{
    this->state->handleLock();
}

void LockContext::resetPassword()
{
    for (size_t i = 0; i < 3; i++)
    {
        this->password[i] = this->readDigitKeyPad();
    }
}

char LockContext::readDigitKeyPad()
{
    char key;

    do
    {
        key = this->keypad->getKey();
    } while (!key);

    Serial.println(key);

    return key;
}

void LockContext::read3digits()
{
    for (size_t i = 0; i < 3; i++)
    {
        this->userInput[i] = this->readDigitKeyPad();
    }
}

bool LockContext::isValidUserInput()
{
    for (size_t i = 0; i < 3; i++)
        if (this->password[i] != this->userInput[i])
            return false;

    return true;
}

void LockContext::lock()
{
    this->openLed->off();
    this->closeLed->on();
    this->door->write(90);
}

void LockContext::unlock()
{
    this->openLed->on();
    this->closeLed->off();
    this->door->write(0);
}

int LockContext::addError()
{
    this->errorCounter++;
    return this->errorCounter;
}

void LockContext::resetError()
{
    this->errorCounter = 0;
}

void LockContext::printPassword()
{
    for (size_t i = 0; i < 3; i++)
    {
        Serial.print(this->password[i]);
    }
    Serial.println();
}