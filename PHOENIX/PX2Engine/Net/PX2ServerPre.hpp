// PX2ServerPre.hpp

#ifndef PX2SERVERPRE_HPP
#define PX2SERVERPRE_HPP

#include "PX2CorePre.hpp"
#include "PX2NetPre.hpp"

#if defined(__LINUX__) || defined (__ANDROID__)
#define PX2_HAVE_FD_POLL
#include <poll.h>
#endif

#endif