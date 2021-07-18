/**
 * @file lv_spinhex.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_spinhex.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_spinhex_class

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_spinhex_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj);
static void lv_spinhex_event(const lv_obj_class_t* class_p, lv_event_t* e);
static void lv_spinhex_updatevalue(lv_obj_t* obj);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_spinhex_class = {
    .constructor_cb = lv_spinhex_constructor,
    .event_cb = lv_spinhex_event,
    .instance_size = sizeof(lv_spinhex_t),
    .editable = LV_OBJ_CLASS_EDITABLE_TRUE,
    .base_class = &lv_textarea_class};
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t* lv_spinhex_create(lv_obj_t* parent) {
  LV_LOG_INFO("begin")
  lv_obj_t* obj = lv_obj_class_create_obj(MY_CLASS, parent);
  lv_obj_class_init_obj(obj);
  return obj;
}

/*=====================
 * Setter functions
 *====================*/

/**
 * Set spinhex value
 * @param spinhex pointer to spinhex
 * @param i value to be set
 */
void lv_spinhex_set_value(lv_obj_t* obj, int32_t i) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  if (i > spinhex->range_max) i = spinhex->range_max;
  if (i < spinhex->range_min) i = spinhex->range_min;

  spinhex->value = i;

  lv_spinhex_updatevalue(obj);
}

/**
 * Set spinhex rollover function
 * @param spinhex pointer to spinhex
 * @param b true or false to enable or disable (default)
 */
void lv_spinhex_set_rollover(lv_obj_t* obj, bool b) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  spinhex->rollover = b;
}

/**
 * Set spinhex digit format (digit count and decimal format)
 * @param spinhex pointer to spinhex
 * @param digit_count number of digit excluding the decimal separator and the sign
 * @param separator_position number of digit before the decimal point. If 0, decimal point is not
 * shown
 */
void lv_spinhex_set_digit_format(lv_obj_t* obj, uint8_t digit_count, uint8_t separator_position) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  if (digit_count > LV_SPINHEX_MAX_DIGIT_COUNT) digit_count = LV_SPINHEX_MAX_DIGIT_COUNT;

  if (separator_position >= digit_count) separator_position = 0;
  if (separator_position > LV_SPINHEX_MAX_DIGIT_COUNT)
    separator_position = LV_SPINHEX_MAX_DIGIT_COUNT;

  if (digit_count < LV_SPINHEX_MAX_DIGIT_COUNT) {
    int64_t max_val = lv_pow(10, digit_count);
    if (spinhex->range_max > max_val - 1) spinhex->range_max = max_val - 1;
    if (spinhex->range_min < -max_val + 1) spinhex->range_min = -max_val + 1;
  }

  spinhex->digit_count = digit_count;
  spinhex->dec_point_pos = separator_position;

  lv_spinhex_updatevalue(obj);
}

/**
 * Set spinhex step
 * @param spinhex pointer to spinhex
 * @param step steps on increment/decrement
 */
void lv_spinhex_set_step(lv_obj_t* obj, uint32_t step) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  spinhex->step = step;
  lv_spinhex_updatevalue(obj);
}

/**
 * Set spinhex value range
 * @param spinhex pointer to spinhex
 * @param range_min maximum value, inclusive
 * @param range_max minimum value, inclusive
 */
void lv_spinhex_set_range(lv_obj_t* obj, int32_t range_min, int32_t range_max) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  spinhex->range_max = range_max;
  spinhex->range_min = range_min;

  if (spinhex->value > spinhex->range_max) spinhex->value = spinhex->range_max;
  if (spinhex->value < spinhex->range_min) spinhex->value = spinhex->range_min;

  lv_spinhex_updatevalue(obj);
}

/**
 * Set cursor position to a specific digit for edition
 * @param spinhex pointer to spinhex
 * @param pos selected position in spinhex
 */
void lv_spinhex_set_pos(lv_obj_t* obj, uint8_t pos) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;
  int32_t step_limit;
  step_limit = LV_MAX(
      spinhex->range_max, (spinhex->range_min < 0 ? (-spinhex->range_min) : spinhex->range_min));
  int32_t new_step = spinhex->step * lv_pow(10, pos);
  if (pos <= 0)
    spinhex->step = 1;
  else if (new_step <= step_limit)
    spinhex->step = new_step;

  lv_spinhex_updatevalue(obj);
}
/*=====================
 * Getter functions
 *====================*/

/**
 * Get the spinhex numeral value (user has to convert to float according to its digit format)
 * @param spinhex pointer to spinhex
 * @return value integer value of the spinhex
 */
int32_t lv_spinhex_get_value(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  return spinhex->value;
}
/**
 * Get the spinhex step value (user has to convert to float according to its digit format)
 * @param spinhex pointer to spinhex
 * @return value integer step value of the spinhex
 */
int32_t lv_spinhex_get_step(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  return spinhex->step;
}

/*=====================
 * Other functions
 *====================*/

/**
 * Select next lower digit for edition
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_step_next(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  int32_t new_step = spinhex->step / 10;
  if ((new_step) > 0)
    spinhex->step = new_step;
  else
    spinhex->step = 1;

  lv_spinhex_updatevalue(obj);
}

/**
 * Select next higher digit for edition
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_step_prev(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;
  int32_t step_limit;
  step_limit = LV_MAX(
      spinhex->range_max, (spinhex->range_min < 0 ? (-spinhex->range_min) : spinhex->range_min));
  int32_t new_step = spinhex->step * 10;
  if (new_step <= step_limit) spinhex->step = new_step;

  lv_spinhex_updatevalue(obj);
}

/**
 * Get spinhex rollover function status
 * @param spinhex pointer to spinhex
 */
bool lv_spinhex_get_rollover(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  return spinhex->rollover;
}

/**
 * Increment spinhex value by one step
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_increment(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  if (spinhex->value + spinhex->step <= spinhex->range_max) {
    /*Special mode when zero crossing*/
    if ((spinhex->value + spinhex->step) > 0 && spinhex->value < 0)
      spinhex->value = -spinhex->value;
    spinhex->value += spinhex->step;

  } else {
    // Rollover?
    if ((spinhex->rollover) && (spinhex->value == spinhex->range_max))
      spinhex->value = spinhex->range_min;
    else
      spinhex->value = spinhex->range_max;
  }

  lv_spinhex_updatevalue(obj);
}

/**
 * Decrement spinhex value by one step
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_decrement(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  if (spinhex->value - spinhex->step >= spinhex->range_min) {
    /*Special mode when zero crossing*/
    if ((spinhex->value - spinhex->step) < 0 && spinhex->value > 0)
      spinhex->value = -spinhex->value;
    spinhex->value -= spinhex->step;
  } else {
    /*Rollover?*/
    if ((spinhex->rollover) && (spinhex->value == spinhex->range_min))
      spinhex->value = spinhex->range_max;
    else
      spinhex->value = spinhex->range_min;
  }

  lv_spinhex_updatevalue(obj);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_spinhex_constructor(const lv_obj_class_t* class_p, lv_obj_t* obj) {
  LV_UNUSED(class_p);
  LV_LOG_TRACE("begin");

  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  /*Initialize the allocated 'ext'*/
  spinhex->value = 0;
  spinhex->dec_point_pos = 0;
  spinhex->digit_count = 5;
  spinhex->step = 1;
  spinhex->range_max = 99999;
  spinhex->range_min = -99999;
  spinhex->rollover = false;

  lv_textarea_set_one_line(obj, true);
  lv_textarea_set_cursor_click_pos(obj, true);
  lv_obj_set_width(obj, LV_DPI_DEF);

  lv_spinhex_updatevalue(obj);

  LV_LOG_TRACE("Spinhex constructor finished");
}

static void lv_spinhex_event(const lv_obj_class_t* class_p, lv_event_t* e) {
  LV_UNUSED(class_p);

  /*Call the ancestor's event handler*/
  lv_res_t res = LV_RES_OK;
  res = lv_obj_event_base(MY_CLASS, e);
  if (res != LV_RES_OK) return;

  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* obj = lv_event_get_target(e);
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;
  if (code == LV_EVENT_RELEASED) {
    /*If released with an ENCODER then move to the next digit*/
    lv_indev_t* indev = lv_indev_get_act();
    if (lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER) {
      if (lv_group_get_editing(lv_obj_get_group(obj))) {
        if (spinhex->step > 1) {
          lv_spinhex_step_next(obj);
        } else {
          /*Restart from the MSB*/
          spinhex->step = 1;
          uint32_t i;
          for (i = 0; i < spinhex->digit_count; i++) {
            int32_t new_step = spinhex->step * 10;
            if (new_step >= spinhex->range_max) break;
            spinhex->step = new_step;
          }
          lv_spinhex_step_prev(obj);
        }
      }
    }
    /*The cursor has been positioned to a digit.
     * Set `step` accordingly*/
    else {
      const char* txt = lv_textarea_get_text(obj);
      size_t txt_len = strlen(txt);

      if (txt[spinhex->ta.cursor.pos] == '.') {
        lv_textarea_cursor_left(obj);
      } else if (spinhex->ta.cursor.pos == (uint32_t)txt_len) {
        lv_textarea_set_cursor_pos(obj, txt_len - 1);
      } else if (spinhex->ta.cursor.pos == 0 && spinhex->range_min < 0) {
        lv_textarea_set_cursor_pos(obj, 1);
      }

      size_t len = spinhex->digit_count - 1;
      uint16_t cp = spinhex->ta.cursor.pos;

      if (spinhex->ta.cursor.pos > spinhex->dec_point_pos && spinhex->dec_point_pos != 0) cp--;
      uint32_t pos = len - cp;

      if (spinhex->range_min < 0) pos++;

      spinhex->step = 1;
      uint16_t i;
      for (i = 0; i < pos; i++) spinhex->step *= 10;
    }
  } else if (code == LV_EVENT_KEY) {
    lv_indev_type_t indev_type = lv_indev_get_type(lv_indev_get_act());

    uint32_t c = *((uint32_t*)lv_event_get_param(e)); /*uint32_t because can be UTF-8*/
    if (c == LV_KEY_RIGHT) {
      if (indev_type == LV_INDEV_TYPE_ENCODER)
        lv_spinhex_increment(obj);
      else
        lv_spinhex_step_next(obj);
    } else if (c == LV_KEY_LEFT) {
      if (indev_type == LV_INDEV_TYPE_ENCODER)
        lv_spinhex_decrement(obj);
      else
        lv_spinhex_step_prev(obj);
    } else if (c == LV_KEY_UP) {
      lv_spinhex_increment(obj);
    } else if (c == LV_KEY_DOWN) {
      lv_spinhex_decrement(obj);
    } else {
      lv_textarea_add_char(obj, c);
    }
  }
}

static void lv_spinhex_updatevalue(lv_obj_t* obj) {
  lv_spinhex_t* spinhex = (lv_spinhex_t*)obj;

  char buf[LV_SPINHEX_MAX_DIGIT_COUNT + 8];
  lv_memset_00(buf, sizeof(buf));
  char* buf_p = buf;
  uint8_t cur_shift_left = 0;

  if (spinhex->range_min < 0) {  // hide sign if there are only positive values
    /*Add the sign*/
    (*buf_p) = spinhex->value >= 0 ? '+' : '-';
    buf_p++;
  } else {
    /*Cursor need shift to left*/
    cur_shift_left++;
  }

  int32_t i;
  char digits[LV_SPINHEX_MAX_DIGIT_COUNT + 4];
  /*Convert the numbers to string (the sign is already handled so always covert positive number)*/
  lv_snprintf(digits, sizeof(digits), "%d", LV_ABS(spinhex->value));

  /*Add leading zeros*/
  int lz_cnt = spinhex->digit_count - (int)strlen(digits);
  if (lz_cnt > 0) {
    for (i = (uint16_t)strlen(digits); i >= 0; i--) {
      digits[i + lz_cnt] = digits[i];
    }
    for (i = 0; i < lz_cnt; i++) {
      digits[i] = '0';
    }
  }

  int32_t intDigits;
  intDigits = (spinhex->dec_point_pos == 0) ? spinhex->digit_count : spinhex->dec_point_pos;

  /*Add the decimal part*/
  for (i = 0; i < intDigits && digits[i] != '\0'; i++) {
    (*buf_p) = digits[i];
    buf_p++;
  }

  if (spinhex->dec_point_pos != 0) {
    /*Insert the decimal point*/
    (*buf_p) = '.';
    buf_p++;

    for (/*Leave i*/; i < spinhex->digit_count && digits[i] != '\0'; i++) {
      (*buf_p) = digits[i];
      buf_p++;
    }
  }

  /*Refresh the text*/
  lv_textarea_set_text(obj, (char*)buf);

  /*Set the cursor position*/
  int32_t step = spinhex->step;
  uint8_t cur_pos = (uint8_t)spinhex->digit_count;
  while (step >= 10) {
    step /= 10;
    cur_pos--;
  }

  if (cur_pos > intDigits) cur_pos++; /*Skip the decimal point*/

  cur_pos -= cur_shift_left;

  lv_textarea_set_cursor_pos(obj, cur_pos);
}
