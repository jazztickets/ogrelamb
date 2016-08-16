/*************************************************************************************
*	ogrelamb - http://ogrelamb.googlecode.com
*	Copyright (C) 2012  Alan Witkowski
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************************/
#include <all.h>
#ifndef LOG_H
#define LOG_H

// Log file class
class _Log {

public:

	_Log() { }
	~_Log() { File.close();	}

	// Open log file
	void Open(const char *Filename) {
		File.open(Filename);
	}

	// Handles most types
	template <typename Type>
	_Log &operator<<(const Type &Value) {
		std::clog << Value;
		File << Value;

		return *this;
	}

	// Handles endl
	_Log &operator<<(std::ostream &(*Value)(std::ostream &)) {
		std::clog << Value;
		File << Value;

		return *this;
	}

private:
                
	std::ofstream File;
};

extern _Log Log;

#endif
