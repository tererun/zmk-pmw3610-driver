#pragma once

#include <stdint.h>

int pmw3610_scroll_direction_set(uint32_t axis, uint32_t direction);
int pmw3610_scroll_report_value(uint32_t axis, int32_t delta);
