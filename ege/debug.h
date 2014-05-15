/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_DEBUG_H__
#define __EGE_DEBUG_H__

#include <etk/log.h>

namespace ege {
	int32_t getLogId();
};
// TODO : Review this problem of multiple intanciation of "std::stringbuf sb"
#define EGE_BASE(info,data) \
	do { \
		if (info <= etk::log::getLevel(ege::getLogId())) { \
			std::stringbuf sb; \
			std::ostream tmpStream(&sb); \
			tmpStream << data; \
			etk::log::logStream(ege::getLogId(), info, __LINE__, __class__, __func__, tmpStream); \
		} \
	} while(0)

#define EGE_CRITICAL(data)      EGE_BASE(1, data)
#define EGE_ERROR(data)         EGE_BASE(2, data)
#define EGE_WARNING(data)       EGE_BASE(3, data)
#ifdef DEBUG
	#define EGE_INFO(data)          EGE_BASE(4, data)
	#define EGE_DEBUG(data)         EGE_BASE(5, data)
	#define EGE_VERBOSE(data)       EGE_BASE(6, data)
	#define EGE_TODO(data)          EGE_BASE(4, "TODO : " << data)
#else
	#define EGE_INFO(data)          do { } while(false)
	#define EGE_DEBUG(data)         do { } while(false)
	#define EGE_VERBOSE(data)       do { } while(false)
	#define EGE_TODO(data)          do { } while(false)
#endif

#define EGE_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			EGE_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

#endif

