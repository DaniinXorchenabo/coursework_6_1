//
// Created by Daniil on 26.04.2023.
//

#include <csignal>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "producer.h"
#include "../repair_producer/repair_producer.h"



void producer(pid_t pid_of_tester, uint64_t start_time){
    std::chrono::milliseconds timespan(500); // or whatever
    std::chrono::milliseconds timespan2(10); // or whatever
    std::chrono::milliseconds timespan3(20000); // or whatever
    union sigval my_sigval{};
    my_sigval.sival_int = 100;
    for (int i =0; i < 100; i++){
        std::this_thread::sleep_for(timespan);
        int is_success = sigqueue(pid_of_tester, SIGUSR1, my_sigval);
        while (is_success != 0){
            std::this_thread::sleep_for(timespan2);
            is_success = sigqueue(pid_of_tester, SIGUSR1, my_sigval);
        }

        std::cout
                << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                << "Произвёл изделие № " << i + 1 << std::endl;
    }
    std::this_thread::sleep_for(timespan3);


}