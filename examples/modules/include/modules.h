#pragma once
#include <iostream>
#include <string>

class Car {
    std::string name_;
public:
    Car(std::string name = "car") : name_(name) {}
    ~Car() {}
    void drive() {
        std::cout << name_ << ".driving" << std::endl;
    }
    void stop() {
        std::cout << name_ << ".stopped" << std::endl;
    }
};