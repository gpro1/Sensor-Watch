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
    (fertility_tracker_mem_t*)context_ptr->state = CALENDAR;
}

void fertility_tracker_face_activate(movement_settings_t *settings, void *context)
{
    (void) settings;
    (void) context;
}

bool fertility_tracker_face_loop(movement_event_t event, movement_settings_t *settings, void *context)
{
    (void) settings;
    fertility_tracker_mem_t *face_buf= (fertility_tracker_mem_t *) context;
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

        case EVENT_ALARM_BUTTON_UP:
            //new case depending on face state
            break;
        
        case EVENT_ALARM_LONG_UP:
            //Change between calendar view and data entry
            switch(face_buf->state)
            {
                case CALENDAR:
                    face_buf->state = DATA_ENTRY;
                    break;
                
                case DATA_ENTRY:
                    face_buf->state = CALENDAR;
                    break;
                
                default:
                    face_buf->state = CALENDAR;
                    break;
            }
            break;

        case EVENT_TICK:
            switch(face_buf->state)
            {
                case CALENDAR:
                    watch_display_string(" Cal",4);
                    break;
                
                case DATA_ENTRY:
                    watch_display_string("Data",4);
                    break;

                default:
                    watch_clear_display();
                    break;
            }

            break;

        default:
            break;

    }

void fertility_tracker_face_resign(movement_settings_t *settings, void *context)
{
    (void) settings;
    (void) context;
}