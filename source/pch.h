// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "mfpch.h"

// add headers that you want to pre-compile here
#include <string>
#include <iostream>

#include "resource.h"		// main symbols
#include "framework.h"
#include "Constants.hpp"

#include "mufise/mfUtils/Utils.h"
#include "config.h"

using namespace std;
using namespace mufise;

#endif //PCH_H
