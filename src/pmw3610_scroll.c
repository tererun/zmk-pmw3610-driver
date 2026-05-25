/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "pmw3610_scroll.h"

#include <errno.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <dt-bindings/zmk/pmw3610.h>

LOG_MODULE_REGISTER(pmw3610_scroll, CONFIG_ZMK_LOG_LEVEL);

static bool scroll_invert_x = IS_ENABLED(CONFIG_PMW3610_INVERT_SCROLL_X);
static bool scroll_invert_y = IS_ENABLED(CONFIG_PMW3610_INVERT_SCROLL_Y);

static int set_axis_direction(bool *axis_inverted, uint32_t direction) {
    switch (direction) {
    case PMW3610_SCROLL_DIR_NORMAL:
        *axis_inverted = false;
        return 0;
    case PMW3610_SCROLL_DIR_INVERT:
        *axis_inverted = true;
        return 0;
    case PMW3610_SCROLL_DIR_TOGGLE:
        *axis_inverted = !*axis_inverted;
        return 0;
    default:
        LOG_ERR("Unknown PMW3610 scroll direction command: %u", direction);
        return -ENOTSUP;
    }
}

int pmw3610_scroll_direction_set(uint32_t axis, uint32_t direction) {
    int err;

    switch (axis) {
    case PMW3610_SCROLL_AXIS_X:
        return set_axis_direction(&scroll_invert_x, direction);
    case PMW3610_SCROLL_AXIS_Y:
        return set_axis_direction(&scroll_invert_y, direction);
    case PMW3610_SCROLL_AXIS_BOTH:
        err = set_axis_direction(&scroll_invert_x, direction);
        if (err) {
            return err;
        }
        return set_axis_direction(&scroll_invert_y, direction);
    default:
        LOG_ERR("Unknown PMW3610 scroll axis: %u", axis);
        return -ENOTSUP;
    }
}

int pmw3610_scroll_report_value(uint32_t axis, int32_t delta) {
    bool inverted;

    switch (axis) {
    case PMW3610_SCROLL_AXIS_X:
        inverted = scroll_invert_x;
        break;
    case PMW3610_SCROLL_AXIS_Y:
        inverted = scroll_invert_y;
        break;
    default:
        return 0;
    }

    return (delta > 0) == inverted ? 1 : -1;
}
