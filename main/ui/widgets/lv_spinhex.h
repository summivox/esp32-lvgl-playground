/**
 * @file lv_spinhex.h
 *
 */

#ifndef LV_SPINHEX_H
#define LV_SPINHEX_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*Testing of dependencies*/
#if LV_USE_TEXTAREA == 0
#error "lv_spinhex: lv_ta is required. Enable it in lv_conf.h (LV_USE_TEXTAREA  1) "
#endif

/*********************
 *      DEFINES
 *********************/
#define LV_SPINHEX_MAX_DIGIT_COUNT 10

/**********************
 *      TYPEDEFS
 **********************/

/*Data of spinhex*/
typedef struct {
  lv_textarea_t ta; /*Ext. of ancestor*/
  /*New data for this type*/
  int32_t value;
  int32_t range_max;
  int32_t range_min;
  int32_t step;
  uint16_t digit_count : 4;
  uint16_t dec_point_pos : 4; /*if 0, there is no separator and the number is an integer*/
  uint16_t rollover : 1;      // Set to true for rollover functionality
} lv_spinhex_t;

extern const lv_obj_class_t lv_spinhex_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a spinhex objects
 * @param par pointer to an object, it will be the parent of the new spinhex
 * @return pointer to the created spinhex
 */
lv_obj_t* lv_spinhex_create(lv_obj_t* parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set spinhex value
 * @param spinhex pointer to spinhex
 * @param i value to be set
 */
void lv_spinhex_set_value(lv_obj_t* obj, int32_t i);

/**
 * Set spinhex rollover function
 * @param spinhex pointer to spinhex
 * @param b true or false to enable or disable (default)
 */
void lv_spinhex_set_rollover(lv_obj_t* obj, bool b);

/**
 * Set spinhex digit format (digit count and decimal format)
 * @param spinhex pointer to spinhex
 * @param digit_count number of digit excluding the decimal separator and the sign
 * @param separator_position number of digit before the decimal point. If 0, decimal point is not
 * shown
 */
void lv_spinhex_set_digit_format(lv_obj_t* obj, uint8_t digit_count, uint8_t separator_position);

/**
 * Set spinhex step
 * @param spinhex pointer to spinhex
 * @param step steps on increment/decrement. Can be 1, 10, 100, 1000, etc the digit that will
 * change.
 */
void lv_spinhex_set_step(lv_obj_t* obj, uint32_t step);

/**
 * Set spinhex value range
 * @param spinhex pointer to spinhex
 * @param range_min maximum value, inclusive
 * @param range_max minimum value, inclusive
 */
void lv_spinhex_set_range(lv_obj_t* obj, int32_t range_min, int32_t range_max);

/**
 * Set cursor position to a specific digit for edition
 * @param spinhex pointer to spinhex
 * @param pos selected position in spinhex
 */
void lv_spinhex_set_pos(lv_obj_t* obj, uint8_t pos);
/*=====================
 * Getter functions
 *====================*/

/**
 * Get spinhex rollover function status
 * @param spinhex pointer to spinhex
 */
bool lv_spinhex_get_rollover(lv_obj_t* obj);

/**
 * Get the spinhex numeral value (user has to convert to float according to its digit format)
 * @param spinhex pointer to spinhex
 * @return value integer value of the spinhex
 */
int32_t lv_spinhex_get_value(lv_obj_t* obj);

/**
 * Get the spinhex step value (user has to convert to float according to its digit format)
 * @param spinhex pointer to spinhex
 * @return value integer step value of the spinhex
 */
int32_t lv_spinhex_get_step(lv_obj_t* obj);

/*=====================
 * Other functions
 *====================*/

/**
 * Select next lower digit for edition by dividing the step by 10
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_step_next(lv_obj_t* obj);

/**
 * Select next higher digit for edition by multiplying the step by 10
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_step_prev(lv_obj_t* obj);

/**
 * Increment spinhex value by one step
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_increment(lv_obj_t* obj);

/**
 * Decrement spinhex value by one step
 * @param spinhex pointer to spinhex
 */
void lv_spinhex_decrement(lv_obj_t* obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif /*LV_USE_SPINHEX*/
