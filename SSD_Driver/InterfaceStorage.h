#pragma once

class InterfaceStorage {
public:
    virtual ~InterfaceStorage() = default;
    virtual bool read(int lba, unsigned int& result) = 0;
};