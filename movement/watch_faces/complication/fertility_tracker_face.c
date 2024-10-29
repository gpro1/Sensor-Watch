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
#include <stdlib.h>

void fertility_tracker_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr)
{
    //(void) settings;
    if(*context_ptr == NULL)
    {
        *context_ptr = malloc(sizeof(fertility_tracker_mem_t));
    } 
    ((fertility_tracker_mem_t*)context_ptr)->state = CALENDAR;
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

        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;

        case EVENT_LIGHT_BUTTON_DOWN:
            
            switch(face_buf->state)
            {
                case CALENDAR:
                    movement_illuminate_led();
                    break;

                case FLUID_ENTRY:
                    break;

                case TEMP_ENTRY_1:
                    break;

                case TEMP_ENTRY_2:
                    break;

                case TEMP_ENTRY_3:
                    break;

                case TEMP_ENTRY_4: 
                    break;

                case CONFIRM_ENTRY:
                    break;

                case ERROR:
                    break;

                default:
                    break;
            }
            break;

        case EVENT_LIGHT_BUTTON_UP:

            switch(face_buf->state)
            {
                case CALENDAR:
                break;

                case FLUID_ENTRY:
                    face_buf->state = TEMP_ENTRY_1;
                    break;

                case TEMP_ENTRY_1:
                    face_buf->state = TEMP_ENTRY_2;
                    break;

                case TEMP_ENTRY_2:
                    face_buf->state = TEMP_ENTRY_3;
                    break;

                case TEMP_ENTRY_3:
                    face_buf->state = TEMP_ENTRY_4;
                    break;

                case TEMP_ENTRY_4: 
                    face_buf->state = CONFIRM_ENTRY;
                break;

                case CONFIRM_ENTRY:
                    face_buf->state = CALENDAR;
                break;

                case ERROR:
                break;

                default:
                break;
            }
            break;

        case EVENT_ALARM_BUTTON_UP:
            //new case depending on face state
            switch(face_buf->state)
            {
                case CALENDAR:
                //Increment current date, display fertility data
                break;

                case FLUID_ENTRY:
                    //Increment fluid entry buffer, display current selection
                    face_buf->fluid_buf[face_buf->data_index] += 1;
                    if(face_buf->fluid_buf[face_buf->data_index] > 3)
                    {
                        face_buf->fluid_buf[face_buf->data_index] = 0;
                    }
                    break;

                case TEMP_ENTRY_1:
                    //Increment temperature digit 1, display current selection
                    face_buf->temp_buf[face_buf->data_index] += 10.0f;
                    //TODO: Add limit for this
                break;

                case TEMP_ENTRY_2:
                    //Increment temperature digit 2, display current selection
                    face_buf->temp_buf[face_buf->data_index] += 1.0f;
                    //TODO: Add limit for this
                    break;

                case TEMP_ENTRY_3:
                    //Increment temperature digit 3, display current selection
                    face_buf->temp_buf[face_buf->data_index] += 0.1f;
                    //TODO: Add limit for this
                    break;

                case TEMP_ENTRY_4: 
                    //Increment temperature digit 4, display current selection
                    face_buf->temp_buf[face_buf->data_index] += 0.01f;
                    //TODO: Add limit for this
                    break;

                case CONFIRM_ENTRY:
                //Cycle between Y and N for confirmation
                break;

                case ERROR:
                break;

                default:
                break;
            }
            break;
        
        case EVENT_ALARM_LONG_UP:

            switch(face_buf->state)
            {
                case CALENDAR:
                    face_buf->state = FLUID_ENTRY;
                break;

                case FLUID_ENTRY:
                break;

                case TEMP_ENTRY_1:
                break;

                case TEMP_ENTRY_2:
                break;

                case TEMP_ENTRY_3:
                break;

                case TEMP_ENTRY_4: 
                break;

                case CONFIRM_ENTRY:
                break;

                case ERROR:
                break;

                default:
                break;
            }
            
            break;

        case EVENT_ALARM_LONG_PRESS:
            switch(face_buf->state)
            {
                case CALENDAR:
                break;

                case FLUID_ENTRY:
                break;

                case TEMP_ENTRY_1:
                break;

                case TEMP_ENTRY_2:
                break;

                case TEMP_ENTRY_3:
                break;

                case TEMP_ENTRY_4: 
                break;

                case CONFIRM_ENTRY:
                break;

                case ERROR:
                break;

                default:
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

    return(true);
}

void fertility_tracker_face_resign(movement_settings_t *settings, void *context)
{
    (void) settings;
    (void) context;
}