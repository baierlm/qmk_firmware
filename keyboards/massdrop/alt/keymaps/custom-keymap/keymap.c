#include QMK_KEYBOARD_H
#include "quantum.h"
enum layers {
    LY_DEFAULT = 0,
    LY_GAME,
    LY_FN,
    LY_GERMAN,
    LY_GREEK,
    LY_TEX,
};

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    STOPPER_ALT,           //Never use
};

enum greek_keycodes {
    TEX_ALPHA=STOPPER_ALT,
    TEX_BETA,
    TEX_GAMMA,
    TEX_DELTA,
    TEX_EPSILON,
    TEX_ETA,
    TEX_THETA,
    TEX_LAMBDA,
    TEX_MU,
    TEX_XI,
    TEX_PI,
    TEX_RHO,
    TEX_SIGMA,
    TEX_TAU,
    TEX_PHI,
    TEX_PSI,
    TEX_OMEGA,
    STOPPER_GREEK,     //Never use
};

char* greek_strings[][2] = {
    {"\\alpha", "\\Alpha"},
    {"\\beta", "\\Beta"},
    {"\\gamma", "\\Gamma"},
    {"\\delta", "\\Delta"},
    {"\\epsilon", "\\Epsilon"},
    {"\\eta", "\\Eta"},
    {"\\theta", "\\Theta"},
    {"\\lambda", "\\Lambda"},
    {"\\mu", "\\Mu"},
    {"\\xi", "\\Xi"},
    {"\\pi", "\\Pi"},
    {"\\rho", "\\Rho"},
    {"\\sigma", "\\Sigma"},
    {"\\tau", "\\Tau"},
    {"\\phi", "\\Phi"},
    {"\\psi", "\\Psi"},
    {"\\omega", "\\Omega"},
};

enum latex_keycodes {
    TEX_FRAC = STOPPER_GREEK,
    TEX_BOLD,
    TEX_DOT,
    TEX_MATH,
    TEX_LBRACK,
    TEX_RBRACK,
    TEX_EXP,
    TEX_SUM,
    TEX_PROD,
    TEX_LARROW,
    TEX_RARROW,
    TEX_TILDE,
    TEX_UNDERSET,
    TEX_PARTIAL,
    TEX_HAT,
    STOPPER, // represents last, never use
};
// TAPDANCE START
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;


enum tabdance_codes {
    TD_SPACE,
};
td_state_t cur_dance(qk_tap_dance_state_t *state);
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }
    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create instances of 'td_tap_t' for each tap dance.
static td_tap_t space = {.is_press_action = true, .state = TD_NONE };
void space_td(qk_tap_dance_state_t *state, void *user_data) {
    space.state = cur_dance(state);
    switch (space.state) {
        case TD_SINGLE_HOLD: layer_on(LY_GREEK); break;
        case TD_DOUBLE_HOLD: layer_on(LY_TEX); break;
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_SPC); register_code(KC_SPC);
        default:break;
    }
}

void space_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (space.state) {
        case TD_SINGLE_HOLD: layer_off(LY_GREEK); break;
        case TD_DOUBLE_HOLD: layer_off(LY_TEX); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_SPC);
        default:break;
    }
    space.state = TD_NONE;
}
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, space_td, space_reset),
};
//TAPDANCE END


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

/*still basically free
 * Esc, Home, RightAlt
 *
 * tab free
 * ctrl, Win, Lalt, Ralt
 *
 * hold free
 * Tab
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LY_DEFAULT] = LAYOUT_65_ansi_blocker(
        KC_CAPS,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        LT(LY_GREEK, KC_TAB),    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_MUTE, \
        LT(LY_GERMAN, KC_ESC),  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_VOLU, \
        KC_LSPO,        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,          KC_UP,   KC_VOLD, \
        KC_LCTL,        KC_LGUI, KC_LALT,               LT(LY_TEX, KC_SPC),                          TD(TD_SPACE), MO(LY_FN),  KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [LY_GAME] = LAYOUT_65_ansi_blocker(
        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_MUTE, \
        KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_VOLU, \
        KC_LSPO,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,          KC_UP,   KC_VOLD, \
        KC_LCTL,  KC_LGUI, KC_LALT,               KC_SPC,                        KC_RALT, MO(LY_FN),  KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [LY_FN] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, TG(LY_GAME), \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, KC_PSCR, KC_SLCK, KC_PAUS, _______, KC_END, \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______,          _______, KC_PGUP, \
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, _______, DBG_MTRX, DBG_KBD, _______, _______,          KC_PGUP, KC_PGDN, \
        _______, _______, _______,                            _______,                            _______, _______, KC_HOME, KC_PGDN, KC_END  \
    ),
    // German layer
    [LY_GERMAN] = LAYOUT_65_ansi_blocker(
        _______, _______, _______,       _______, _______, _______, _______, _______,   _______, _______,   _______, _______, _______, _______, _______, \
        _______, _______, _______,    RALT(KC_5), _______, _______, _______, RALT(KC_Y),_______, RALT(KC_P),_______, _______, _______, _______, _______, \
        _______, RALT(KC_Q), RALT(KC_S), _______, _______, _______, KC_LEFT, KC_DOWN,   KC_UP,   KC_RGHT,   _______, KC_GRV,           _______, _______, \
        _______, _______, _______,       _______, _______, _______, _______, _______,   _______, _______,   _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                               _______,    _______,     _______, _______, _______  \
    ),
    // Tex layer
    [LY_GREEK] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, TEX_OMEGA, TEX_EPSILON, TEX_RHO, TEX_TAU, TEX_PSI, _______, _______, _______, TEX_PI, _______, _______, _______, _______, \
        _______, TEX_ALPHA, TEX_SIGMA, TEX_DELTA, TEX_PHI, TEX_GAMMA, TEX_THETA, _______, _______, TEX_LAMBDA, _______, _______,          _______, _______, \
        _______, _______, TEX_XI, _______, _______, TEX_BETA, _______, TEX_MU, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                           _______, _______, _______, _______, _______  \
    ),
    [LY_TEX] = LAYOUT(
        _______, _______, _______, _______, _______, _______, TEX_HAT, _______, TEX_PROD, _______, _______, TEX_UNDERSET, TEX_SUM, _______, _______, \
        _______, _______, _______, TEX_EXP, _______, _______, _______, _______, _______,  _______, TEX_PARTIAL,TEX_LBRACK,TEX_RBRACK,_______, _______, \
        _______, _______, _______,TEX_DOT, TEX_FRAC, _______, _______, _______, _______,  _______, _______, TEX_TILDE,          _______, _______, \
        _______, _______, _______, _______, _______,TEX_BOLD, _______,TEX_MATH, TEX_LARROW, TEX_RARROW, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    */
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

uint8_t mod_state;
bool handle_greek_letter(uint16_t keycode, keyrecord_t *record){
    if(record->event.pressed){
        if(MODS_SHIFT){
            clear_mods();SEND_STRING(greek_strings[keycode-TEX_ALPHA][1]);
            set_mods(mod_state);
        }else{
            SEND_STRING(greek_strings[keycode-TEX_ALPHA][0]);
        }
    }
    return false;
}

void go_left(int num){
    for(int i = 0; i < num; i++){
        tap_code(KC_LEFT);
    }
}

bool handle_tex(uint16_t keycode, keyrecord_t *record){
    if(record->event.pressed){
        if(MODS_SHIFT){
            clear_mods();
            switch(keycode){
                case TEX_BOLD: SEND_STRING("\\bm ");break;
                case TEX_DOT: SEND_STRING("\\ddot ");break;
                case TEX_MATH: SEND_STRING("\\mathcal ");break;
                case TEX_LBRACK: SEND_STRING("\\left\\{ ");break;
                case TEX_RBRACK: SEND_STRING("\\right\\} ");break;
                case TEX_LARROW: SEND_STRING("\\Leftarrow ");break;
                case TEX_RARROW: SEND_STRING("\\Rightarrow ");break;
                case TEX_TILDE: SEND_STRING("\\tilde ");break;
                case TEX_PARTIAL: SEND_STRING("\\frac{\\partial }{\\partial }"); go_left(12); break;
                case TEX_HAT: SEND_STRING("\\bar ");break;
            }
            set_mods(mod_state);
        }else{
            switch(keycode){
                case TEX_BOLD: SEND_STRING("\\textbf ");break;
                case TEX_DOT:  SEND_STRING("\\dot ");break;
                case TEX_MATH: SEND_STRING("\\mathbb ");break;
                case TEX_LBRACK: SEND_STRING("\\left[ ");break;
                case TEX_RBRACK: SEND_STRING("\\right] ");break;
                case TEX_LARROW: SEND_STRING("\\leftarrow ");break;
                case TEX_RARROW: SEND_STRING("\\rightarrow ");break;
                case TEX_TILDE: SEND_STRING("\\sim ");break;
                case TEX_PARTIAL: SEND_STRING("\\partial ");break;
                case TEX_HAT: SEND_STRING("\\hat ");break;

                case TEX_EXP: SEND_STRING("\\exp{}"); go_left(1); break;
                case TEX_UNDERSET: SEND_STRING("\\underset{}{}"); go_left(3); break;
                case TEX_FRAC: SEND_STRING("\\frac{}{}"); go_left(3); break;
                case TEX_SUM: SEND_STRING("\\sum_{}^{}"); go_left(4); break;
                case TEX_PROD: SEND_STRING("\\prod_{}^{}"); go_left(4); break;
            }
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    //#ifdef CONSOLE_ENABLE
    //    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    //#endif 
    static uint32_t key_timer;
    mod_state = get_mods();
    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        case TEX_ALPHA ... TEX_OMEGA: return handle_greek_letter(keycode, record);
        case TEX_FRAC ... STOPPER: return handle_tex(keycode, record);
        default:
            return true; //Process all other keycodes normally
    }
}

void rgb_matrix_indicators_user(void){
    
	//uint8_t this_led = host_keyboard_leds();

	//if (!g_suspend_state && rgb_matrix_config.enable) {

    /*if(biton32(layer_state) > 0){
       for(int row = 0; row < MATRIX_ROWS; row++){
            for(int column = 0; column < MATRIX_COLS; row++){
               if(keymaps[biton32(layer_state)][row][column] != _______){
			       rgb_matrix_set_color(row*column, 0xFF, 0x00, 0x00);
               }  
            }
       } 
    }*/

	switch (biton32(layer_state)) {
        case LY_GAME:
			rgb_matrix_set_color(17, 0xFF, 0x00, 0x00);
			rgb_matrix_set_color(31, 0xFF, 0x00, 0x00);
			rgb_matrix_set_color(32, 0xFF, 0x00, 0x00);
			rgb_matrix_set_color(33, 0xFF, 0x00, 0x00);
            break;
        case 2:
            //rgb_matrix_mode(RGB_MATRIX_DIGITAL_RAIN);
            //rgb_matrix_mode(RGB_MATRIX_CUSTOM_SECOND);
            break;
        case 3:
            //rgb_matrix_mode(RGB_MATRIX_CUSTOM_my_cool_effect2);
            break;
        default:
            break;
    }
}
