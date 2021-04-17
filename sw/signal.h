#ifndef SIGNAL_H
#define SIGNAL_H

#include "kernel.h"

namespace des {

class signalInterface {
public:
    virtual void update() = 0;
    virtual std::string getName() = 0;
    virtual std::string toString() = 0;
};

template<typename TYPE>
class signal : public signalInterface
{
public:
    signal(std::string name, TYPE value)
    {
        this->value = value;
        this->name = name;
    }

    TYPE read()
    {
        return value;
    }

    void write(TYPE value)
    {
        newValue = value;
        if(newValue != this->value) {
            kernel::getInstance().updateRequest(this);
        }
    }

    void update() {
        value = newValue;
    }

    std::string getName() {
        return name;
    }

    std::string toString() {
        return std::to_string(value);
    }

private:
    TYPE value;
    TYPE newValue;
    std::string name;
};

}

#endif // SIGNAL_H
