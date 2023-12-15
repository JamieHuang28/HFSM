#pragma once

class WlcWorldModelInterface
{
public:
    virtual ~WlcWorldModelInterface() = default;

    virtual bool isInWlcStatus() const = 0;

    virtual RequestType request() const = 0;
};