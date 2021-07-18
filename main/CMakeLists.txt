set(srcs
"app_main.cpp"

"lvgl_tick.c"

"device/encoder.cpp"
"device/oled_ssd1309.cpp"
"ui/encoder_button_input.cpp"
"ui/style.cpp"
"ui/widgets/lv_spinhex.c"
)
file(GLOB fonts "ui/fonts/*.c")

idf_component_register(
    SRCS ${srcs} ${fonts}
    INCLUDE_DIRS "."
)

# compiler flags for the `main` component can be specified here
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2a)

# compiler flags to be applied to all components can be specified here
target_compile_options(${COMPONENT_LIB} PUBLIC -DLV_LVGL_H_INCLUDE_SIMPLE)
