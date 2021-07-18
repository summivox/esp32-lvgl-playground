// Copyright 2021 summivox. All rights reserved.
// Authors: summivox@gmail.com

#pragma once

#include <cinttypes>

#include "fmt/core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

#include "common/logging.hpp"
#include "math/polyfill.hpp"
#include "ui/style.hpp"

namespace ui {
namespace view {

constexpr char TAG[] = "ui/view";

const lv_color_t kOn = lv_color_black();
const lv_color_t kOff = lv_color_white();

}  // namespace view
}  // namespace ui
