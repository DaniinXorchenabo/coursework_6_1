//
// Created by Daniil on 27.04.2023.
//

#include "verification.h"
#include "../repair_producer/repair_producer.h"

//
// Created by Daniil on 26.04.2023.
//


//std::mt19937* rng;
//std::uniform_int_distribution<std::mt19937::result_type>* dist6;


//void handler(int sig, siginfo_t * sig_info, void *){
//
//}
//
//void repair_producer(){
//
//
//}


void verification(pid_t repair_pid, pid_t storage_pid, uint64_t start_time){
//    std::random_device dev;
//    rng = new std::mt19937(dev());
//    dist6 = new std::uniform_int_distribution<std::mt19937::result_type>(1,6); // distribution in range [1, 6]
//    std::cout
//            << getpid() << ": "
//            <<  (*dist6)(*rng) << std::endl;


//    struct sigaction act{};
//    memset(&act, 0, sizeof(act));
//    act.sa_sigaction = handler;
//    sigaction(SIGUSR1, &act, nullptr);
    union sigval my_sigval{};
    my_sigval.sival_int = 10;
    std::chrono::milliseconds timespan2(10); // or whatever

    int signo;
    sigset_t newmask;

    sigemptyset(&newmask);

    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

//    Sigprocmask(SIG_BLOCK, &newmask, NULL); /* блокируем SIGUSR1 */
    sigprocmask(SIG_BLOCK, &newmask, nullptr); /* блокируем SIGUSR1 */


    while (true) {
        sigwait(&newmask, &signo);
        if (signo == SIGUSR1 || signo == SIGUSR2) {
            std::cout
                    << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                    << "Получено изделие из "
                    << ((signo == SIGUSR1) ? "цеха производства" : "ремонтного цеха")
                    << std::endl;
            if (((double) rand() / (RAND_MAX)) > ((signo == SIGUSR1) ? 0.5 : 0.025)) {
                std::cout
                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                        << "Изделие исправно, отправляем на склад" << std::endl;

                int is_success = sigqueue(storage_pid, SIGUSR1, my_sigval);
                while (is_success != 0) {
                    std::this_thread::sleep_for(timespan2);
                    is_success = sigqueue(storage_pid, SIGUSR1, my_sigval);
                }
            } else {
                std::cout
                        << getpid() << ":\t" <<  ((double)(get_curr_time() -  start_time)) / 1000.0 << ":\t"
                        << "Изделие не работает, отправляем в ремонтный цех" << std::endl;

                int is_success = sigqueue(repair_pid, SIGUSR1, my_sigval);
                while (is_success != 0) {
                    std::this_thread::sleep_for(timespan2);
                    is_success = sigqueue(repair_pid, SIGUSR1, my_sigval);
                }

            }
        }
    }
}