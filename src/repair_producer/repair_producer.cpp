//
// Created by Daniil on 26.04.2023.
//
#include <set>
#include <atomic>
#include <cstring>
#include <array>
#include <iostream>
#include <chrono>
#include <thread>
#include "repair_producer.h"

std::atomic<pid_t> verification_pid = 0;
std::atomic<int> curr_arr_index = 0;
std::atomic<uint64_t> delays[1000];

uint64_t get_curr_time() {
    return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void handler(int sig, siginfo_t *sig_info, void *) {
    if (verification_pid == 0) {
        verification_pid = sig_info->si_pid;
    }
    auto res = curr_arr_index++;
    delays[res % 1000] = get_curr_time() + (uint64_t)(((double) rand() / (RAND_MAX)) * 1500);
//    delays[res % 1000].store((uint64_t)17);

}

void repair_producer(uint64_t start_time) {
    std::chrono::milliseconds timespan(100); // or whatever
    std::chrono::milliseconds timespan2(10); // or whatever
    int last_curr_ind = 0;


    for (int i = 0; i < 1000; i++) {
        delays[i] = 0;
    }

    std::multiset<uint64_t> times{};

    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO; /* Important. */
    sigaction(SIGUSR1, &act, nullptr);
    union sigval my_sigval{};
    my_sigval.sival_int = 100;

//    Sigprocmask(SIG_BLOCK, &newmask, NULL); /* блокируем SIGUSR1 */

    while (true) {
        bool res = false;
        if (times.empty()) {
//            std::this_thread::sleep_for(timespan);
//            continue;
        } else {
            auto first_item = (times.begin());
//            std::cout
//                    << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"                    << "Плановое время " << *first_item
//                    << " || Сейчас время "<< get_curr_time() << "\n";
            res = *first_item < get_curr_time();
            if (res) {
                times.erase(*first_item);
                std::cout
                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                        << "Отремонтировали" << std::endl;
                std::cout
                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                        << "Отправляем на проверку" << std::endl;

                int is_success =  sigqueue(verification_pid, SIGUSR2, my_sigval);
                while (is_success != 0){
                    std::this_thread::sleep_for(timespan2);
                    is_success =  sigqueue(verification_pid, SIGUSR2, my_sigval);
                }
                std::cout
                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                        << "На проверку отправили" << std::endl;
            }
        }
        if (last_curr_ind != curr_arr_index) {
            int count = 0;
            for (int i = last_curr_ind % 1000; i  != curr_arr_index; i = (i + 1) % 1000) {
                auto _v = delays[i].load();
//                std::cout
//                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
//                        << "добавляем новое значение времени в очередь: " << _v << "\n";
//                std::atomic<uint64_t> new_val(_v);
                auto new_val = _v;
                times.insert(_v);
                count += 1;
            }
            std::cout
                    << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                    << "Для ремонта получено новых изделий: " << count << std::endl;
            std::cout
                    << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                    << "Всего ремонтируется на данный момент: " << times.size() << std::endl;
            last_curr_ind = curr_arr_index.load();
        } else if (!res) {
            std::this_thread::sleep_for(timespan);

        }


    }
}
