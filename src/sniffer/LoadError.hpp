#pragma once
#include <exception>
// TODO do something better here
class LoadError : public std::exception {
public:
	const char* what() const noexcept override {
		return "Something went wrong while loading resources.";
	}
};

