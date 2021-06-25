#pragma once
#include <string>
#include <exception>

class ContainerError: public std::exception {
private:
	std::string error;
public:
	ContainerError(const std::string& error) noexcept : error("Container Error: " + error) {}
	const char* what() const noexcept override {
		return error.c_str();
	}
};

class IteratorError : public std::exception {
private:
	std::string error;
public:
	IteratorError(const std::string& error) noexcept : error("Iterator Error: " + error) {}
	const char* what() const noexcept override {
		return error.c_str();
	}
};