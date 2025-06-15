/*
Copyright 2020 Dimitris Mantzouranis
Copyright 2022 Pablo Ramirez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include "_wait.h"
#include "action.h"
#include "action_layer.h"
#include "color.h"
#include "deferred_exec.h"
#include "keycode.h"
#include "mousekey.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "rgb_matrix.h"
#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"

#include "keycode_config.h"
extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    BASE = 0,
    BASE_WINDOWS,
    NO_TAP_HOLD,
    DIR,
    FN,
};
#define TG_NOHT TG(NO_TAP_HOLD)     // alias

#define KC_TASK LGUI(KC_TAB)        // Task viewer
#define KC_FLXP LGUI(KC_E)          // Windows file explorer

#define KC_MSSN LGUI(KC_F3)         // Mission Control
#define KC_FIND LALT(LGUI(KC_SPC))  // Finder
#define KC_MSCR LSFT(LGUI(KC_3))    // Mac screenshot
#define KC_MSNP LSFT(LGUI(KC_4))    // Mac snip tool

// Home-row mods
#define H_LSFT_A LSFT_T(KC_A)
#define H_LCTL_S LCTL_T(KC_S)
#define H_LCTL_F LCTL_T(KC_F)
#define H_DIR_D LT(DIR, KC_D)
#define H_LALT_Z LALT_T(KC_Z)
#define H_LALT_S LALT_T(KC_S)
#define H_LSFT_F LSFT_T(KC_F)
#define H_LCTL_A LCTL_T(KC_A)
#define H_LGUI_F LGUI_T(KC_F)

#define H_RSFT_SCLN RSFT_T(KC_SCLN)
#define H_RCTL_L RCTL_T(KC_L)
#define H_RCTL_J RCTL_T(KC_J)
#define H_DIR_K LT(DIR, KC_K)
#define H_RALT_SLSH RALT_T(KC_SLSH)
#define H_RALT_L RALT_T(KC_L)
#define H_RSFT_J RSFT_T(KC_J)
#define H_RCTL_SCLN RCTL_T(KC_SCLN)
#define H_RGUI_J RGUI_T(KC_J)

#define ALTRIGHT LALT(KC_RIGHT)
#define ALTLEFT  LALT(KC_LEFT)

enum custom_keys {
  WINMAC = SAFE_RANGE, // swap win & mac layers
  // See upstream for description
  WIN_L,
  WIN_R,
};

/*
  Blank template:
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,                _______  ,
      _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,    _______  ,
      _______,    _______,    _______,                                        _______,                                        _______,    _______,    _______,    _______,    _______,    _______

*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
    *****************************************************************************************************************
    * ESC  * F1   * F2   * F3   * F4   * F5   * F6   * F7   * F8   * F9   * F10  * F11  * F12  * PSCR * DEL  * RMOD *
    *****************************************************************************************************************
    * ~    * 1    * 2    * 3    * 4    * 5    * 6    * 7    * 8    * 9    * 0    * *    * =    * BSPC        * PGUP *
    *****************************************************************************************************************
    * TAB     * Q    * W    * E    * R    * T    * Y    * U    * I    * O    * P    * [    * ]    * \        * PGDN *
    *****************************************************************************************************************
    * CAPS      * A    * S    * D    * F    * G    * H    * J    * K    * L    * ;    * '    * ENT           * HOME *
    *****************************************************************************************************************
    * LSFT         * Z    * X    * C    * V    * B    * N    * M    * ,    * .    * /    * RSFT       * UP   * END  *
    *****************************************************************************************************************
    * LCTL  * LALT  * LGUI  * SPC                                         * LGUI * FN   * RCTL * LEFT * DOWN * RGHT *
    *****************************************************************************************************************
  */
  [BASE] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_MSNP,    KC_DEL,     RGB_MOD  ,
      KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,                KC_PGUP  ,
      KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,                KC_PGDN  ,
      KC_CAPS,    H_SFTA,     H_ALTS,     H_DIRD,     H_CTLF,     KC_G,       KC_H,       H_CTLJ,     H_DIRK,     H_ALTL,     H_SFTSCLN,  KC_QUOT,                KC_ENT,                 KC_HOME  ,
      _______,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RPRN,                KC_UP,      KC_END   ,
      KC_LBRC,    KC_LALT,    KC_LGUI,                                        KC_SPC,                                         KC_RGUI,    MO(FN),     KC_RBRC,    KC_LEFT,    KC_DOWN,    KC_RGHT
  ),
  [NO_TAP_HOLD] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_MSNP,    KC_DEL,     RGB_MOD  ,
      KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,                KC_PGUP  ,
      KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,                KC_PGDN  ,
      KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,                 KC_HOME  ,
      KC_LSPO,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSPC,    KC_UP,      KC_END   ,
      KC_LCPO,    KC_LALT,    KC_LGUI,                                        KC_SPC,                                         KC_RGUI,     MO(FN),    KC_RCPC,    KC_LEFT,    KC_DOWN,    KC_RGHT
  ),
  /*
    Standard Numpad
  */
  [KEYPAD] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      _______,    _______,    _______,    _______,    _______,    _______,    _______,     KC_NUM,    KC_PSLS,    KC_PAST,    KC_PMNS,    _______,    _______,    _______,    _______,    _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,      KC_P7,      KC_P8,      KC_P9,    KC_PPLS,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,      KC_P4,      KC_P5,      KC_P6,    KC_PPLS,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,      KC_P1,      KC_P2,      KC_P3,    KC_PENT,    _______,                _______,                _______  ,
      _______,                _______,    _______,    _______,    _______,    _______,    _______,      KC_P0,      KC_P0,    KC_PDOT,    KC_PENT,                _______,    _______,    _______  ,
      _______,    _______,    _______,                                        _______,                                        _______,    _______,    _______,    _______,    _______,    _______
  ),

  /*
    Helper keys for developing keyboard firmware
  */
  [DIR] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
    _______,    _______,   ALTRIGHT,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
    _______,    _______,    _______,    _______,    _______,    _______,    KC_LEFT,    KC_DOWN,      KC_UP,   KC_RIGHT,    _______,    _______,                _______,                _______  ,
    _______,                _______,    _______,    _______,    _______,    ALTLEFT,    _______,    _______,    _______,    _______,    _______,                _______,    _______,    _______  ,
    _______,    _______,    _______,                                        _______,                                        _______,    _______,    _______,    _______,    _______,    _______
  ),

  /*
    Helper keys for developing keyboard firmware
  */
  [DEV] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
    _______,    _______,    _______,    _______,    _______,      DT_UP,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
    _______,    _______,    _______,    _______,    _______,    DT_PRNT,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
    _______,    _______,    _______,    _______,    _______,    DT_DOWN,    _______,    _______,    _______,    _______,    _______,    _______,                _______,                _______  ,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,    _______  ,
    _______,    _______,    _______,                                        _______,                                        _______,    _______,    _______,    _______,    _______,    _______
  ),

  /*
    *****************************************************************************************************************
    *RESET * BRID * BRIU * TASK * FLXP * RVAD * RVAI * MPRV * MPLY * MNXT * MUTE * VOLD * VOLU *      *  INS * RTOG *
    *****************************************************************************************************************
    *      *      *      *      *      *      *      *      *      *      *      *      *      *             * RM_P *
    *****************************************************************************************************************
    *         *      *      *      *      *      *      *      *      *      *      *      *      *          *      *
    *****************************************************************************************************************
    *           *      *      *      *      *      *      *      *      *      *      *      *               *      *
    *****************************************************************************************************************
    *              *      *      *      *      *      *      *      *      *      *      *            * RSAI *      *
    *****************************************************************************************************************
    *       *       *       *                                             *      *      *      * RHUD * RSAD * RHUI *
    *****************************************************************************************************************
  */
  /*
   Function layer should always be at the top
  */
  [FN] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      RESET,      KC_BRID,    KC_BRIU,    KC_MSSN,    KC_FIND,    RGB_VAD,    RGB_VAI,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_MSCR,    KC_INS,     RGB_TOG  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,     WINMAC,    TG(DEV),     VALAFK,    _______,    TG_NOHT,    _______,    _______,    _______,    _______,    _______,                _______,                _______  ,
      _______,                _______,    _______,    CMB_TOG,    _______,    _______,    _______, TG(KEYPAD),    _______,    _______,    _______,                _______,    RGB_SAI,    _______  ,
      _______,    _______,    _______,                                        _______,                                        _______,    _______,    _______,    RGB_HUD,    RGB_SAD,    RGB_HUI
  )
};

bool dip_switch_update_user(uint8_t index, bool active) {
  switch(index) {
    case 0: // OS switch
      if (active) { // Mac/iOS mode
        //layer_move(MAC_BASE);
      }
      else { // Windows/Android mode
        //layer_move(WIN_BASE);
      }
      break;
    case 1: // Connection switch
      // Probably it's not possible to do anything sensible here as switching from Cable to BT requires turning off the board. (BT / OFF / Cable)
      if (active) { // BT mode
        // do stuff
      }
      else { //Cable mode
        // do stuff
      }
      break;
  }
  return true;
}

typedef enum rgb_state {
  MAC = 0,
  WIN,
  KPAD,
  KDEV,
} rgb_state_t;
void set_rgb_matrix_with_state(rgb_state_t state) {
  switch (state) {
    case MAC: {
      rgb_matrix_sethsv_noeeprom(0, 255, 255);
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_LEFT_RIGHT);
      return;
    }
    case WIN: {
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(142, 255, 239);
      return;
    }
    case KPAD: {
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(84, 255, 255);
      return;
    }
    case KDEV: {
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(0, 255, 255);
      return;
    }
  }
}

typedef struct {
  bool initialized;
  HSV hsv;
  uint8_t mode;
} visual_state;
static visual_state get_visual_state(void) {
  return (visual_state){
    .initialized = true,
    .hsv = rgb_matrix_get_hsv(),
    .mode = rgb_matrix_get_mode(),
  };
}

static bool is_valafk = false; // Init w default mode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static bool is_mac = true; // Initialize with default mode
  static bool is_kpad = false; // Initialize with default state (disabled)
  static bool is_kdev = false; // Initialize with default state (disabled)
  static bool is_tap_hold_enabled = true; // Initialize with default state (enabled)
  static visual_state recent_visual_state = { .initialized = false };
  if (!recent_visual_state.initialized) {
    recent_visual_state = get_visual_state();
  }

  switch (keycode) {
    case WINMAC: {
      if (record->event.pressed) {
        is_mac = !is_mac; // toggle state
        if (is_mac) {
          set_rgb_matrix_with_state(MAC);
        } else {
          set_rgb_matrix_with_state(WIN);
        }

        keymap_config.swap_lalt_lgui = !keymap_config.swap_lalt_lgui;
        keymap_config.swap_ralt_rgui = !keymap_config.swap_ralt_rgui;

        layer_off(KEYPAD);
        is_kpad = false;
        layer_off(DEV);
        is_kdev = false;
      }
      return false;
    }
    case TG_NOHT: {
      if (record->event.pressed) {
        is_tap_hold_enabled = !is_tap_hold_enabled;
        recent_visual_state = get_visual_state();
        if (is_tap_hold_enabled) {
          rgb_matrix_sethsv_noeeprom(0, 255, 255);
          rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_LEFT_RIGHT);
        } else {
          rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
          rgb_matrix_sethsv_noeeprom(142, 255, 239);
        }
      } else {
        rgb_matrix_sethsv_noeeprom(recent_visual_state.hsv.h, recent_visual_state.hsv.s, recent_visual_state.hsv.v);
        rgb_matrix_mode_noeeprom(recent_visual_state.mode);
        rgb_matrix_sethsv_noeeprom(recent_visual_state.hsv.h, recent_visual_state.hsv.s, recent_visual_state.hsv.v);
      }
      return true;
    }
    case TG(KEYPAD): {
      if (record->event.pressed) {
        is_kpad = !is_kpad;
        if (is_kpad) {
          set_rgb_matrix_with_state(KPAD);
        } else {
          set_rgb_matrix_with_state(is_mac ? MAC : WIN);
        }

        layer_off(DEV);
        is_kdev = false;
      }
      return true;
    }
    case TG(DEV): {
      if (record->event.pressed) {
        is_kdev = !is_kdev;
        if (is_kdev) {
          set_rgb_matrix_with_state(KDEV);
        } else {
          set_rgb_matrix_with_state(is_mac ? MAC : WIN);
        }

        layer_off(KEYPAD);
        is_kpad = false;
      }
      return true;
    }
    case VALAFK: {
      if (record->event.pressed) {
        static deferred_token valafk_cb_tkn = INVALID_DEFERRED_TOKEN;
        is_valafk = !is_valafk; // toggle state
        if (is_valafk) {
          valafk_cb_tkn = defer_exec(1000, valafk_cb, NULL);
        } else {
          if (valafk_cb_tkn != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec(valafk_cb_tkn);
            valafk_cb_tkn = INVALID_DEFERRED_TOKEN;
          }
        }
      }
      return false;
    }
    default: {
      return true;
    }
  }
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  // debug_enable = true;
  // debug_matrix = true;
  // debug_keyboard = true;
  // debug_mouse = true;
}
