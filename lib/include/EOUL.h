#pragma once

#include <EOUL\Net.h>
#include <EOUL\Save.h>
#include <EOUL\File.h>
#include <EOUL\Time.h>
#include <EOUL\Utils.h>
#include <EOUL\Maths.h>
#include <EOUL\Image.h>
#include <EOUL\Color.h>
#include <EOUL\Window.h>
#include <EOUL\String.h>
#include <EOUL\Parser.h>
#include <EOUL\Constants.h>

#if !defined(_HAS_CXX17) || !_HAS_CXX17
	#error "C++17 support is needed!"
#endif

#define EOUL_MAKE_VERSION(major, minor, patch) (unsigned int((major << 16) | (minor << 8) | (patch << 0)))
#define EOUL_VERSION_MAJOR 1
#define EOUL_VERSION_MINOR 12
#define EOUL_VERSION_PATCH 1
#define EOUL_GET_VERSION_MAJOR(version) (((version) >> 16) & 0xff)
#define EOUL_GET_VERSION_MINOR(version) (((version) >> 8) & 0xff)
#define EOUL_GET_VERSION_PATCH(version) (((version) >> 0) & 0xff)
#define EOUL_VERSION EOUL_MAKE_VERSION(EOUL_VERSION_MAJOR, EOUL_VERSION_MINOR, EOUL_VERSION_PATCH)

#ifdef EOUL_USE_ALL

	using namespace EOUL;
	using namespace Math;
	using namespace IO;
	using namespace Util;
	using namespace Net;
	using namespace Parser;

#endif