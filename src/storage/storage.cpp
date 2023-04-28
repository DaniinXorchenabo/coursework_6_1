//
// Created by Daniil on 26.04.2023.
//

#include "storage.h"
#include "../repair_producer/repair_producer.h"


void storage(uint64_t start_time){
    int count = 0;
    int signo;
    sigset_t newmask;

    sigemptyset(&newmask);

    sigaddset(&newmask, SIGUSR1);
//    sigaddset(&newmask, SIGUSR2);

//    Sigprocmask(SIG_BLOCK, &newmask, NULL); /* блокируем SIGUSR1 */
    sigprocmask(SIG_BLOCK, &newmask, nullptr); /* блокируем SIGUSR1 */


    while (true){
        sigwait(&newmask, &signo);
        if (signo == SIGUSR1){
            count++;
            std::cout
                    << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                    << "Посылка получена на хранение. Теперь на складе всего полылок: " << count
                    << std::endl;

        }
    }
}
