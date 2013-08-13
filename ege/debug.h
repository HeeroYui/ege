/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_DEBUG_H__
#define __EGE_DEBUG_H__

#include <etk/types.h>
#include <etk/Debug.h>

extern const char * egeLibName;

#define EGE_CRITICAL(data)    ETK_CRITICAL(egeLibName, data)
#define EGE_WARNING(data)     ETK_WARNING(egeLibName, data)
#define EGE_ERROR(data)       ETK_ERROR(egeLibName, data)
#define EGE_INFO(data)        ETK_INFO(egeLibName, data)
#define EGE_DEBUG(data)       ETK_DEBUG(egeLibName, data)
#define EGE_VERBOSE(data)     ETK_VERBOSE(egeLibName, data)
#define EGE_ASSERT(cond,data) ETK_ASSERT(egeLibName, cond, data)
#define EGE_CHECK_INOUT(cond) ETK_CHECK_INOUT(egeLibName, cond)
#define EGE_TODO(cond)        ETK_TODO(egeLibName, cond)

#endif

