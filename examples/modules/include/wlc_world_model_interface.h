#pragma once

class WlcWorldModelInterface
{
public:
    virtual ~WlcWorldModelInterface() = default;

    virtual bool isInWlcStatus() const = 0;

    virtual int request() const = 0;
};