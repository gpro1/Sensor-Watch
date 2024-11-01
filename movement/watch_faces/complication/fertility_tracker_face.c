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
#include <string.h>

void fertility_tracker_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr)
{
    //(void) settings;
    if(*context_ptr == NULL)
    {
        *context_ptr = malloc(sizeof(fertility_tracker_mem_t));
        memset(context_ptr, 0, sizeof(fertility_tracker_mem_t));
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
    watch_date_time temp_time;
    char buf[6];
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
                    movement_illuminate_led();
                    break;

                case TEMP_ENTRY_1:
                    movement_illuminate_led();
                    break;

                case TEMP_ENTRY_2:
                    movement_illuminate_led();
                    break;

                case TEMP_ENTRY_3:
                    movement_illuminate_led();
                    break;

                case TEMP_ENTRY_4: 
                    movement_illuminate_led();
                    break;

                case CONFIRM_ENTRY:
                    movement_illuminate_led();
                    break;

                case ERROR:
                    movement_illuminate_led();
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
                    watch_display_string(" T",0);
                    snprintf(buf, sizeof(buf), "%2hu%hu%hu%hu", (uint8_t)(face_buf->temp_input[0]/10), face_buf->temp_input[1], face_buf->temp_input[2], face_buf->temp_input[3]);
                    watch_display_string(buf, 4);
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
                    watch_display_string("SA",0);
                    face_buf->confirm_input = true;
                    watch_display_string("SAVE", 5);
                    face_buf->state = CONFIRM_ENTRY;
                break;

                case CONFIRM_ENTRY:
                    face_buf->state = CALENDAR;
                    watch_display_string(" CAL ",4);
                    watch_display_string("  ",0);
                    if(face_buf->confirm_input == true)
                    {
                        face_buf->fluid_buf[face_buf->data_index] = face_buf->fluid_input;
                        face_buf->temp_buf[face_buf->data_index] = face_buf->temp_input[0] + face_buf->temp_input[1] + (0.1 * face_buf->temp_input[2]) + (0.01 * face_buf->temp_input[3]);
                        face_buf->time_buf[face_buf->data_index] = face_buf->time_input;
                    }

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
                    face_buf->fluid_input += 1;
                    if(face_buf->fluid_input > 3)
                    {
                        face_buf->fluid_input = 0;
                    }
                    display_fluid_type(face_buf->fluid_input);
                    break;

                case TEMP_ENTRY_1:
                    //Increment upper two temperature digits, display current selection
                    face_buf->temp_input[0] += 10;
                    if(face_buf->temp_input[0] > 100)
                    {
                        face_buf->temp_input[0] = 90;
                    }
                    snprintf(buf, sizeof(buf), "%2hu%hu%hu%hu", (uint8_t)(face_buf->temp_input[0]/10), face_buf->temp_input[1], face_buf->temp_input[2], face_buf->temp_input[3]);
                    watch_display_string(buf, 4);
                    break;

                case TEMP_ENTRY_2:
                    //Increment temperature digit 2, display current selection
                    face_buf->temp_input[1] += 1;
                    if(face_buf->temp_input[1] > 9)
                    {
                        face_buf->temp_input[1] = 0;
                    }
                    snprintf(buf, sizeof(buf), "%2hu%hu%hu%hu", (uint8_t)(face_buf->temp_input[0]/10), face_buf->temp_input[1], face_buf->temp_input[2], face_buf->temp_input[3]);
                    watch_display_string(buf, 4);
                    break;

                case TEMP_ENTRY_3:
                    //Increment temperature 10th decimal, display current selection
                    face_buf->temp_input[2] += 1;
                    if(face_buf->temp_input[2] > 9)
                    {
                        face_buf->temp_input[2] = 0;
                    }
                    snprintf(buf, sizeof(buf), "%2hu%hu%hu%hu", (uint8_t)(face_buf->temp_input[0]/10), face_buf->temp_input[1], face_buf->temp_input[2], face_buf->temp_input[3]);
                    watch_display_string(buf, 4);
                    break;

                case TEMP_ENTRY_4: 
                    //Increment temperature 100th decimal, display current selection
                    face_buf->temp_input[3] += 1;
                    if(face_buf->temp_input[3] > 9)
                    {
                        face_buf->temp_input[3] = 0;
                    }
                    snprintf(buf, sizeof(buf), "%2hu%hu%hu%hu", (uint8_t)(face_buf->temp_input[0]/10), face_buf->temp_input[1], face_buf->temp_input[2], face_buf->temp_input[3]);
                    watch_display_string(buf, 4);
                    break;

                case CONFIRM_ENTRY:
                //Cycle between Y and N for confirmation
                    if(face_buf->confirm_input == true)
                    {
                        face_buf->confirm_input = false;
                        watch_display_string(" DEL", 5);
                    }
                    else
                    {
                        face_buf->confirm_input = true;
                        watch_display_string("SAVE", 5);
                    }

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
                    
                    temp_time = watch_rtc_get_date_time();
    
                    //increment data index if it is a new day, reset input buffers
                    if(compare_dates(temp_time, face_buf->time_buf[face_buf->data_index]) != true)
                    {
                        face_buf->data_index++;
                        if(face_buf->data_index >= MEMORY_NUM_DAYS)
                        {
                            face_buf->data_index = 0;
                        }
                        face_buf->fluid_input = 0;
                        face_buf->temp_input[0] = 90;
                        face_buf->temp_input[1] = 7;
                        face_buf->temp_input[2] = 0;
                        face_buf->temp_input[3] = 0;
                        face_buf->time_input = temp_time;
                    }
                    else
                    {
                        //Set input buffers to previous entry (with updated time)
                        face_buf->fluid_input = face_buf->fluid_buf[face_buf->data_index];
                        if(face_buf->temp_buf[face_buf->data_index] >= 100.0)
                        {
                            face_buf->temp_input[0] = 100;
                        }
                        else
                        {
                            face_buf->temp_input[0] = 90;
                        }
                        face_buf->temp_input[1] = ((uint8_t)face_buf->temp_buf[face_buf->data_index] % 10);     
                        face_buf->temp_input[2] = ((uint8_t)(face_buf->temp_buf[face_buf->data_index] * 10) % 10);
                        face_buf->temp_input[3] = ((uint8_t)(face_buf->temp_buf[face_buf->data_index] * 100) % 10);
                        face_buf->time_input = temp_time;
                    }
                   
                    watch_display_string("FL",0);
                    display_fluid_type(face_buf->fluid_input);

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
                    watch_display_string(" CAL ",4);
                    watch_display_string("  ",0);
                    break;
                
                /*case DATA_ENTRY:
                    watch_display_string("Data",4);
                    break;*/

                default:
                    //watch_clear_display();
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

static void display_fluid_type(uint8_t value)
{
    switch(value)
    {
        case 0:
            watch_display_string("  nn  ", 4);
        break;

        case 1:
            watch_display_string("  G   ", 4);
        break;

        case 2:
            watch_display_string("  EL  ", 4);
        break;

        case 3:
            watch_display_string("  EE  ", 4);
        break;

        default:
            watch_display_string("  ERR ", 4);
        break;
    }
}

//Compares the date of both arguments. Returns true if they have the same date, otherwise false.
static bool compare_dates(watch_date_time time1, watch_date_time time2)
{
    bool result;
    result = (time1.unit.month == time2.unit.month);
    result &= (time1.unit.day == time2.unit.day);
    result &= (time1.unit.year == time2.unit.year);
    return result;
}