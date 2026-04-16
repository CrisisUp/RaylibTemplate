#ifndef PROFILING_H
#define PROFILING_H

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#define PROFILE_ZONE_SCOPED ZoneScoped
#define PROFILE_FRAME_MARK FrameMark
#else
#define PROFILE_ZONE_SCOPED
#define PROFILE_FRAME_MARK
#endif

#endif // PROFILING_H
