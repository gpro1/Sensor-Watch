/*
 * MIT License
 *
 * Copyright (c) 2023 Edward Shin
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

#include "shell_cmd_list.h"
#include "fertility_tracker_face.h"
#include "watch_utility.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "filesystem.h"
#include "watch.h"

static int help_cmd(int argc, char *argv[]);
static int flash_cmd(int argc, char *argv[]);
static int stress_cmd(int argc, char *argv[]);
static int fert_test_cmd(int argc, char *argv[]);

shell_command_t g_shell_commands[] = {
    {
        .name = "?",
        .help = "print command list",
        .min_args = 0,
        .max_args = 0,
        .cb = help_cmd,
    },
    {
        .name = "help",
        .help = "print command list",
        .min_args = 0,
        .max_args = 0,
        .cb = help_cmd,
    },
    {
        .name = "flash",
        .help = "reboot to UF2 bootloader",
        .min_args = 0,
        .max_args = 0,
        .cb = flash_cmd,
    },
    {
        .name = "ls",
        .help = "usage: ls [PATH]",
        .min_args = 0,
        .max_args = 1,
        .cb = filesystem_cmd_ls,
    },
    {
        .name = "cat",
        .help = "usage: cat <PATH>",
        .min_args = 1,
        .max_args = 1,
        .cb = filesystem_cmd_cat,
    },
    {
        .name = "df",
        .help = "print filesystem free space",
        .min_args = 0,
        .max_args = 0,
        .cb = filesystem_cmd_df,
    },
    {
        .name = "rm",
        .help = "usage: rm [PATH]",
        .min_args = 1,
        .max_args = 1,
        .cb = filesystem_cmd_rm,
    },
    {
        .name = "format",
        .help = "usage: format YES",
        .min_args = 1,
        .max_args = 1,
        .cb = filesystem_cmd_format,
    },
    {
        .name = "echo",
        .help = "usage: echo TEXT {>,>>} FILE",
        .min_args = 3,
        .max_args = 3,
        .cb = filesystem_cmd_echo,
    },
    {
        .name = "stress",
        .help = "test CDC write; usage: stress [LEN] [DELAY_MS]",
        .min_args = 0,
        .max_args = 2,
        .cb = stress_cmd,
    },
    {
        .name = "fert_test",
        .help = "enable a test of the fertility watch face; usage: fert_test",
        .min_args = 0,
        .max_args = 0,
        .cb = fert_test_cmd,
    },
};

const size_t g_num_shell_commands = sizeof(g_shell_commands) / sizeof(shell_command_t);

static int help_cmd(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    printf("Command List:\r\n");
    for (size_t i = 0; i < g_num_shell_commands; i++) {
        printf(" %s\t%s\r\n",
                g_shell_commands[i].name,
                (g_shell_commands[i].help) ? g_shell_commands[i].help : ""
        );
    }

    return 0;
}

static int flash_cmd(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    watch_reset_to_bootloader();
    return 0;
}

#define STRESS_CMD_MAX_LEN  (512)
static int stress_cmd(int argc, char *argv[]) {
    char test_str[STRESS_CMD_MAX_LEN+1] = {0};

    int max_len = 512;
    int delay = 0;

    if (argc >= 2) {
        if ((max_len = atoi(argv[1])) == 0) {
            return -1;
        }
        if (max_len > 512) {
            return -1;
        }
    }

    if (argc >= 3) {
        delay = atoi(argv[2]);
    }

    for (int i = 0; i < max_len; i++) {
        snprintf(&test_str[i], 2, "%u", (i+1)%10);
        printf("%u:\t%s\r\n", (i+1), test_str);
        if (delay > 0) {
            delay_ms(delay);
        }
    }

    return 0;
}

const float temp_data[] = {
    97.85, 97.59, 96.54, 97.10, 97.45,
    90.00,
    96.75, 96.77, 96.97, 98.01, 97.19,
    90.00,
    97.11, 97.20, 97.18, 97.32, 97.73, 97.66,
    90.00,
    98.01, 98.20, 98.48, 98.44, 98.31, 97.94, 98.58, 98.42, 97.94, 97.80
};

const uint8_t fl_data[] = {
    1, 1, 1, 1,
    2, 2, 2, 2, 2,
    4, 4, 4, 4, 4, 4, 4, 4,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

static int fert_test_cmd(int argc, char *argv[])
{
    fertility_tracker_mem_t * face_buf = get_fert_data();
    int i;
    char fertile;
    watch_date_time temp_time;

    face_buf->cycle_day_num = 1;
    face_buf->current_date = watch_rtc_get_date_time();
    temp_time = watch_rtc_get_date_time();

    
    for(i = 0; i < sizeof(fl_data); i++)
    {
        //input new data, cycle iterate FSM
        face_buf->fluid_buf[face_buf->data_index] = fl_data[i];
        face_buf->temp_buf[face_buf->data_index] = temp_data[i];
        face_buf->time_buf[face_buf->data_index] = watch_rtc_get_date_time();
        face_buf->cycle_next_state = iterate_cycle_fsm(face_buf);

        //Calculate fertility and print
        if(is_fertile(face_buf))
        {
            fertile = 'F';
        }
        else
        {
            fertile = 'N';
        }

        printf("%hu : %hu \r\n", face_buf->cycle_day_num, fertile);

        //Calculate new date (years not supported)
        temp_time.unit.day++;
        if(temp_time.unit.day > days_in_month((uint8_t)temp_time.unit.month, (uint16_t)temp_time.unit.year))
        {
            temp_time.unit.day = 1;
            temp_time.unit.month++;
        }

        //Set new date
        watch_rtc_set_date_time(temp_time);

        //Update state if a new day has arrived
        if(!dates_are_equal(temp_time, face_buf->current_date))
        {
            //Check for missed days, log invalid data
            //Also, pre-populate today with invalid data in case it is missed
            num_days_missed = num_days_passed(temp_time, face_buf->current_date);
            for(i = 0; i < num_days_missed; i++)
            {
                face_buf->data_index++;                                    
                if(face_buf->data_index >= MEMORY_NUM_DAYS)
                {
                    face_buf->data_index = 0;
                }
                face_buf->fluid_buf[face_buf->data_index] = 0; 
                face_buf->temp_buf[face_buf->data_index] = INVALID_TEMP;
                face_buf->cycle_day_num++;
            }

            face_buf->current_date = temp_time;
            
            //Update the state based on next state
            if(face_buf->cycle_state != face_buf->cycle_next_state) 
            {
                if(face_buf->cycle_next_state == FLUID_CHANGE)
                {
                    //Second day of cycle because M was logged yesterday.
                    face_buf->cycle_day_num = 2;
                }
                face_buf->cycle_prev_state = face_buf->cycle_state;
                face_buf->cycle_state = face_buf->cycle_next_state;
                face_buf->cycle_state_start = temp_time;
            }
        }

    }

    return 0;
}
