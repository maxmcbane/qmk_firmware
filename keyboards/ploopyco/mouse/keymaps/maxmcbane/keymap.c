/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// safe range starts at `PLOOPY_SAFE_RANGE` instead.
bool is_mo1_used = false;
bool is_mo2_used = false;
bool is_volumn_scoll = false;

enum custom_keycodes {
    LT_BTN4 = PLOOPY_SAFE_RANGE,
    LT_BTN5,
};

void process_wheel_user(report_mouse_t* mouse_report, int16_t h, int16_t v) {
    int8_t v1 = v;
    if (!is_volumn_scoll) {
        mouse_report->h = h;
        mouse_report->v = v;
    } else {
        is_mo2_used = true;
        if (v1 > 0) {
            tap_code_delay(KC_VOLU, 15);
        } else if (v1 < 0) {
            tap_code_delay(KC_VOLD, 15);
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_BTN4:
            if (record->event.pressed) {
                //on keydown
                is_mo1_used = false;
                is_mo2_used = true;
                layer_on(1);
            } else { 
                //on keyup
                if (!is_mo1_used) { 
                    tap_code(KC_BTN4);
                }
                layer_off(1);
            }
            return false;
        case LT_BTN5:
            if (record->event.pressed) {
                //on keydown
                is_mo1_used = true;
                is_mo2_used = false;
                is_volumn_scoll = true;
                layer_on(2);
            } else { 
                //on keyup
                if (!is_mo2_used) { 
                    tap_code(KC_BTN5);
                }
                layer_off(2);
                is_volumn_scoll = false;
            }
            return false;


        
        default:
            is_mo1_used = true;
            is_mo2_used = true;
            break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                 C(KC_S), KC_BTN1, KC_BTN3, KC_BTN2, RCS(KC_Q), LT_BTN4, LT_BTN5, DPI_CONFIG),
    [1] = LAYOUT(/* layer1 */
                 KC_ESC , KC_BSPC, _______, KC_DEL , KC_ENT   , LT_BTN4, _______, _______   ),
    [2] = LAYOUT(/* layer2 */
                 KC_MPRV, KC_MPLY, KC_MUTE, KC_MNXT, _______  , _______, LT_BTN5, _______   ),
    [3] = LAYOUT(/* layer3 */
                 _______, _______, _______, _______, _______  , _______, _______, _______   ),
    [4] = LAYOUT(/* layer4 */
                 _______, _______, _______, _______, _______  , _______, _______, _______   )
};
