/*
 * MIT License
 *
 * Copyright (c) 2024 Gregory Evans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FERTILITY_TRACKER_FACE_H_
#define FERTILITY_TRACKER_FACE_H_

#include "movement.h"

#define MEMORY_NUM_DAYS 60 //Number of days to store data

typedef struct {
    uint8_t fluid_buf[MEMORY_NUM_DAYS];
    float temp_buf[MEMORY_NUM_DAYS];
    uint16_t data_index;
} fertility_tracker_mem_t

void fertility_tracker_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void fertility_tracker_face_activate(movement_settings_t *settings, void *context);
bool fertility_tracker_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void fertility_tracker_face_resign(movement_settings_t *settings, void *context);

#define fertility_tracker_face ((const watch_face_t){ \
    fertility_tracker_face_setup, \
    fertility_tracker_face_activate, \
    fertility_tracker_face_loop, \
    fertility_tracker_face_resign, \
    NULL, \
})

#endif //FERTILITY_TRACKER_FACE_H_