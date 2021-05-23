//https://beta.docs.qmk.fm/using-qmk/hardware-features/lighting/feature_rgb_matrix#custom-rgb-matrix-effects-id-custom-rgb-matrix-effects
RGB_MATRIX_EFFECT(FIRST)
RGB_MATRIX_EFFECT(SECOND)
RGB_MATRIX_EFFECT(my_cool_effect2)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

//static const uint8_t KEY_TO_LED_MAP[MATRIX_ROWS][MATRIX_COLS] = {
//  { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14},
//  {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
//  {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, __, 42, 43},
//  {44, __, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57},
//  {58, 59, 60, __, __, __, 61, __, __, __, 62, 63, 64, 65, 66},
//};

// 100,  99,  98,  97, 96, 95, 94, 93, 92, 91, 90,  89,  88,  87,  86
//     ------------------------------------------------------------
// 101 | 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14| 85
// 102 |15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29| 84
//     |30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, __, 42, 43|
// 103 |44, __, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57| 83
// 104 |58, 59, 60, __, __, __, 61, __, __, __, 62, 63, 64, 65, 66| 82
//     ------------------------------------------------------------
//  67,  68,  69,  70, 71, 72, 73, 74, 75, 76, 77,  78,  79,  80,  81  

static bool FIRST(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
      if(HAS_FLAGS(i, LED_FLAG_UNDERGLOW)){
        rgb_matrix_set_color(i, 0xff, 0xff, 0xff);
    }else{
        rgb_matrix_set_color(i, 0x77, 0x33, 0x33);
    }
  }
  return led_max < DRIVER_LED_TOTAL;
}


static bool SECOND(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  rgb_matrix_set_color(0, 0xff, 0xff, 0xff);
  rgb_matrix_set_color(67, 0xff, 0xff, 0xff);
  rgb_matrix_set_color(81, 0xff, 0xff, 0xff);
  rgb_matrix_set_color(86, 0xff, 0xff, 0xff);
  rgb_matrix_set_color(100, 0xff, 0xff, 0xff);
  rgb_matrix_set_color(104, 0xff, 0xff, 0xff);
  return led_max < DRIVER_LED_TOTAL;
}

// e.g: A more complex effect, relying on external methods and state, with
// dedicated init and run methods
static uint8_t some_global_state;
static void my_cool_effect2_complex_init(effect_params_t* params) {
  some_global_state = 1;
}
static bool my_cool_effect2_complex_run(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
  }

  return led_max < DRIVER_LED_TOTAL;
}
static bool my_cool_effect2(effect_params_t* params) {
  if (params->init) my_cool_effect2_complex_init(params);
  return my_cool_effect2_complex_run(params);
}
#endif
