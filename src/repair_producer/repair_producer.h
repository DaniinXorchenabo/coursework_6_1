//
// Created by Daniil on 26.04.2023.
//

#ifndef COURSE_WORK_3_2_REPAIR_PRODUCER_H
#define COURSE_WORK_3_2_REPAIR_PRODUCER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

uint64_t get_curr_time();

void repair_producer(uint64_t start_time);



#endif //COURSE_WORK_3_2_REPAIR_PRODUCER_H
