/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace ege {
	int32_t getLogId();
};
#define EGE_BASE(info,data)  ELOG_BASE(ege::getLogId(),info,data)

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


