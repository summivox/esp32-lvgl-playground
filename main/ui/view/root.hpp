// Copyright 2021 summivox. All rights reserved.
// Authors: summivox@gmail.com

#pragma once

#include "ui/view/common.hpp"

namespace ui {
namespace view {

struct Root {
  lv_group_t* default_group;

  lv_obj_t* container;

  lv_obj_t* label_hello;
  lv_obj_t* btn_1;
  lv_obj_t* btn_1_label;
  lv_obj_t* btn_2;
  lv_obj_t* btn_2_label;

  lv_obj_t* spinbox_1;
  lv_obj_t* label_cjk;

  ~Root() {
    lv_obj_del(container);
    // TODO
  }

  Root(lv_obj_t* parent) {
    default_group = lv_group_create();
    lv_group_set_default(default_group);

#if 0
    container = lv_obj_create(parent);
    lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
#else
    container = parent;
#endif
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(
        container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    label_hello = lv_label_create(container);
    lv_obj_add_style(label_hello, &g_style->text_medium, 0);
    lv_label_set_text_static(label_hello, "H");

    btn_1 = lv_btn_create(container);
    lv_obj_set_size(btn_1, 25, 15);
    btn_1_label = lv_label_create(btn_1);
    lv_obj_add_style(btn_1_label, &g_style->text_small, 0);
    lv_label_set_text_static(btn_1_label, "b1");
    lv_obj_center(btn_1_label);
    lv_obj_add_event_cb(
        btn_1,
        [](lv_event_t* e) { ESP_LOGI(TAG, "pressed: 0x%08x", (uint32_t)e->user_data); },
        LV_EVENT_PRESSED,
        (void*)1);

    btn_2 = lv_btn_create(container);
    lv_obj_set_size(btn_2, 30, 15);
    btn_2_label = lv_label_create(btn_2);
    lv_obj_add_style(btn_2_label, &g_style->text_small, 0);
    lv_label_set_text_static(btn_2_label, "b2xx");
    lv_obj_center(btn_2_label);
    lv_obj_add_event_cb(
        btn_2,
        [](lv_event_t* e) { ESP_LOGI(TAG, "pressed: 0x%08x", (uint32_t)e->user_data); },
        LV_EVENT_PRESSED,
        (void*)2);

    spinbox_1 = lv_spinbox_create(container);
    lv_obj_set_size(spinbox_1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(spinbox_1, &g_style->text_small, 0);
    lv_obj_set_style_pad_all(spinbox_1, 0, LV_PART_MAIN);
    lv_spinbox_set_digit_format(spinbox_1, 4, 0);
    lv_spinbox_set_range(spinbox_1, -2000, +2000);
    lv_obj_add_event_cb(
        spinbox_1,
        [](lv_event_t* e) {
          ESP_LOGI(
              TAG,
              "changed: 0x%08x ; value=%d",
              (uint32_t)e->user_data,
              lv_spinbox_get_value(e->target));
        },
        LV_EVENT_VALUE_CHANGED,
        (void*)3);

    label_cjk = lv_label_create(container);
    lv_obj_add_flag(label_cjk, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_style_text_font(label_cjk, &lv_font_simsun_16_cjk, 0);
    lv_obj_set_style_text_letter_space(label_cjk, 1, 0);
    lv_label_set_text_static(label_cjk, u8"あア相談\n\uf15b\uf304\uF2ED\uF11C" LV_SYMBOL_REFRESH);
  }

  void Render() {
    // TODO
  }
};

}  // namespace view
}  // namespace ui
