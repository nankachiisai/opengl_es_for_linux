#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>

class Exception {
public:
	Exception() {
	}

	Exception(std::string errorMessage) {
		setErrorMessage(errorMessage);
	}

	std::string getErrorMessage() {
		return errorMessage;
	}

	void setErrorMessage(std::string errorMessage) {
		this->errorMessage = errorMessage;
	}

private:
	std::string errorMessage;
};

#endif