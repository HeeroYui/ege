/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>

int32_t ege::getLogId() {
	static int32_t g_val = elog::registerInstance("ege");
	return g_val;
}

