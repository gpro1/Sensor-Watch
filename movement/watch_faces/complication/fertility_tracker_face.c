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

#include "fertility_tracker_face.h"

void fertility_tracker_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr)
{
    //(void) settings;
    if(*context_ptr = NULL)
    {
        *context_ptr = malloc(sizeof(fertility_tracker_mem_t));
    } 
}

void fertility_tracker_face_activate(movement_settings_t *settings, void *context)
{
    (void) settings;
    (void) context;
}

bool fertility_tracker_face_loop(movement_event_t event, movement_settings_t *settings, void *context)
{
    (void) settings;
    fertility_tracker_mem_t *state = (fertility_tracker_mem_t *) context;
    switch(event.event_type)
    {
        case EVENT_MODE_BUTTON_UP:
            movement_move_to_next_face();
            break;

        case EVENT_LIGHT_BUTTON_DOWN:
            movement_illuminate_led();
            break;

        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;


    }

void fertility_tracker_face_resign(movement_settings_t *settings, void *context)
{
    (void) settings;
    (void) context;
}