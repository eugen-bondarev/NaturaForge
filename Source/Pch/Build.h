#pragma once

#include <string>

#define NATURAFORGE_DEVELOPMENT  0
#define NATURAFORGE_PRODUCTION   1

#define NATURAFORGE_BUILD_TYPE   NATURAFORGE_PRODUCTION

inline std::string NATURAFORGE_ROOT;

#ifdef NDEBUG
# define NATURAFORGE_RELEASE
#else
# define NATURAFORGE_DEBUG
#endif