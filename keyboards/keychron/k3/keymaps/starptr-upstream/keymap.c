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
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______  ,
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
      KC_CAPS,    H_LSFT_A,   H_LALT_S,   H_DIR_D,    H_LCTL_F,   KC_G,       KC_H,       H_RCTL_J,   H_DIR_K,    H_RALT_L,   H_RSFT_SCLN,KC_QUOT,                KC_ENT,                 KC_HOME  ,
      KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,                            KC_UP,      KC_END   ,
      KC_LCTL,    KC_LALT,    KC_LGUI,                                        KC_SPC,                                         KC_RGUI,    MO(FN),     KC_RCTL,    KC_LEFT,    KC_DOWN,    KC_RGHT
  ),
  [BASE_WINDOWS] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    H_LGUI_F,   _______,    _______,    H_RGUI_J,   _______,    _______,    _______,    _______,                _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______  ,
      _______,    _______,    WIN_L,                                          _______,                                        WIN_R,      _______,    _______,    _______,    _______,    _______
  )
  [NO_TAP_HOLD] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    KC_A,       KC_S,       KC_D,       KC_F,       _______,    _______,    KC_J,       KC_K,       KC_L,       KC_SCLN,    _______,                _______,                _______  ,
      KC_LSFT,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_RSFT,                            _______,    _______  ,
      KC_LCTL,    KC_LALT,    _______,                                        _______,                                        _______,    MO(FN),     KC_RCTL,    _______,    _______,    _______
  ),

  [DIR] = LAYOUT_75_ansi(
  /*  0           1           2           3           4           5           6           7           8           9           10          11          12          13          14          15       */
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    KC_LEFT,    KC_DOWN,      KC_UP,   KC_RIGHT,    _______,    _______,                _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______  ,
      _______,    _______,    _______,                                        _______,                                        _______,    MO(FN),     _______,    _______,    _______,    _______
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
      QK_BOOT,    KC_BRID,    KC_BRIU,    KC_MSSN,    KC_FIND,    RGB_VAD,    RGB_VAI,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_MSCR,    KC_INS,     RGB_TOG  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______  ,
      _______,    _______,     WINMAC,    _______,    _______,    _______,    TG_NOHT,    _______,    _______,    _______,    _______,    _______,                _______,                _______  ,
      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            RGB_SAI,    _______  ,
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

typedef enum {
    STYLE_MACOS,
    STYLE_WINDOWS,
} OSStyle;
void osstyle_toggle(OSStyle* state) {
    if (*state == STYLE_MACOS) {
        layer_on(BASE_WINDOWS);
        *state = STYLE_WINDOWS;
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv_noeeprom(142, 255, 239);
    } else {
        layer_off(BASE_WINDOWS);
        *state = STYLE_MACOS;
        rgb_matrix_sethsv_noeeprom(0, 255, 255);
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_ALL);
    }
}

uint16_t gui_kc(bool is_left) {
    if (is_left) {
        return KC_LGUI;
    }
    return KC_RGUI;
}

uint16_t alt_kc(bool is_left) {
    if (is_left) {
        return KC_LALT;
    }
    return KC_RALT;
}

uint16_t ctl_kc(bool is_left) {
    if (is_left) {
        return KC_LCTL;
    }
    return KC_RCTL;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static OSStyle current_style = STYLE_MACOS;
  static int win_gui_held_count = 0;
  static uint16_t win_gui_held_type = WIN_L; // When one of the gui keys are unpressed, we know it must be the other one that's held
  static uint16_t win_gui_resolved_sticky = KC_LGUI; // After the gui key is pressed with another key, we know what the gui key should really be
  static uint16_t* win_gui_maybe_resolved = NULL;
  static uint16_t win_gui_held_timer = 0;

  switch (keycode) {
  case WINMAC: {
    // Reset the withheld state on enter/exit of windows style
    if (win_gui_maybe_resolved != NULL) {
        unregister_code(*win_gui_maybe_resolved);
        win_gui_maybe_resolved = NULL;
    }

    if (record->event.pressed) {
        // Set state
        osstyle_toggle(&current_style);
    }

    // Don't process this key further
    return false;
  }
  case WIN_L:
  case WIN_R: {
    // Check if key was pressed down
    if (record->event.pressed) {
        // The key that was just pressed down counts as the key that's held
        if (keycode == WIN_L) {
            win_gui_held_type = WIN_L;
        } else {
            win_gui_held_type = WIN_R;
        }

        win_gui_held_count++;

        if (win_gui_held_count >= 2) {
            // The gui key was pressed down again (probably by pressing both of them)
            return false;
        }
        // The gui key was pressed down for the first time (i.e. the other gui key was not pressed down already)
        win_gui_held_timer = timer_read();
        return false;
    }

    // Key was unpressed
    win_gui_held_count--;

    // Check if the unpressed key was the last gui key that was held
    if (win_gui_held_count == 0) {
        // If win_gui is already held in the OS, we need to release it (eg. the alt is still held after alt-tab)
        if (win_gui_maybe_resolved != NULL) {
            unregister_code(*win_gui_maybe_resolved);
            win_gui_maybe_resolved = NULL;
        }

        if (timer_elapsed(win_gui_held_timer) < TAPPING_TERM) {
            // The unpress was so soon after the press that it counts as a tap
            tap_code(gui_kc(win_gui_held_type == WIN_L));
            return false;
        }
        return false;
    }

    // There are still other gui keys that are held
    // The key that wasn't just unpressed is the type of key that's still held
    if (keycode == WIN_L) {
        win_gui_held_type = WIN_R;
    } else {
        win_gui_held_type = WIN_L;
    }

    return false;
  }
  case KC_TAB:
  case KC_SPC: {
    if (win_gui_held_count == 0) {
        // Not a special case we have to handle; just proceed as normal
        return true;
    }

    if (record->event.pressed) {
        // Check if win_gui is already held in the OS (eg. the ctrl is held after ctrl-c)
        if (win_gui_maybe_resolved != NULL) {
            // If the already-held-in-the-OS state is ALT, we don't need to do anything in process_record_user; just continue processing the key
            if (*win_gui_maybe_resolved == KC_LALT || *win_gui_maybe_resolved == KC_RALT) {
                return true;
            }
            unregister_code(*win_gui_maybe_resolved);
            win_gui_maybe_resolved = NULL;
        }

        uint16_t kc_alt = alt_kc(win_gui_held_type == WIN_L);
        win_gui_resolved_sticky = kc_alt;
        win_gui_maybe_resolved = &win_gui_resolved_sticky;
        register_code(*win_gui_maybe_resolved);
        return true; // Continue processing the key
    }

    return true; // Continue processing the key
  }
  default: {
    if (win_gui_held_count == 0) {
        // Not a special case we have to handle; just proceed as normal
        return true;
    }

    if (record->event.pressed) {
        // Check if win_gui is already held in the OS (eg. the alt is held after alt-tab)
        if (win_gui_maybe_resolved != NULL) {
            // If the already-held-in-the-OS state is CTL, we don't need to do anything in process_record_user; just continue processing the key
            if (*win_gui_maybe_resolved == KC_LCTL || *win_gui_maybe_resolved == KC_RCTL) {
                return true;
            }
            unregister_code(*win_gui_maybe_resolved);
            win_gui_maybe_resolved = NULL;
        }

        uint16_t kc_ctl = ctl_kc(win_gui_held_type == WIN_L);
        win_gui_resolved_sticky = kc_ctl;
        win_gui_maybe_resolved = &win_gui_resolved_sticky;
        register_code(*win_gui_maybe_resolved);
        return true; // Continue processing the key
    }

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
