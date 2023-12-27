#pragma once

#include "requests.h"

bool isNumber(utility::string_t& str)
{
	for (char ch : str) {
		if (!isdigit(ch)) {
			return false;
		}
	}
	return true;
}