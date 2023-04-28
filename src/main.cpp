#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <atomic>
#include "producer/producer.h"
#include "storage/storage.h"
#include "repair_producer/repair_producer.h"
#include "verification/verification.h"

void redirect_stdout(const std::string &filename) {
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

int main() {

//    std::atomic<int> _d(6);
//    std::cout << _d.load() << "\n";
//    _d.fetch_add(2);
//    std::cout << _d.load()<< "\n";
//    _d.store(2);
//    std::cout << _d.load()<< "\n";
    auto now = get_curr_time();
    pid_t pid, ppid, producer_pid, storage_pid, repair_producer_pid, verification_pid;

    pid = fork();
    if (pid == 0) {
        ppid = getppid();
        std::cout
                << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                << "I am child STORAGE process (PID " << getpid()
                << ") of parent process (PID " << ppid
                << ").\n";
        storage(now);
    } else if (pid > 0) {
        storage_pid = pid;
        pid = fork();
        if (pid == 0) {
            ppid = getppid();
            std::cout
                    << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                    << "I am child REPAIR_PRODUCER process (PID " << getpid()
                    << ") of parent process (PID " << ppid
                    << ").\n";
            repair_producer(now);
        } else if (pid > 0) {
            repair_producer_pid = pid;
            pid = fork();
            if (pid == 0) {
                ppid = getppid();
                std::cout
                        << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                        << "I am child VERIFICATION process (PID " << getpid()
                        << ") of parent process (PID " << ppid
                        << ").\n";
                verification(repair_producer_pid, storage_pid, now);
            } else if (pid > 0) {
                verification_pid = pid;
                pid = fork();

                if (pid == 0) {
                    ppid = getppid();
                    std::cout
                            << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                            << "I am child PRODUCER process (PID " << getpid()
                            << ") of parent process (PID " << ppid
                            << ").\n";
                    producer(verification_pid, now);
                } else if (pid > 0) {
                    producer_pid = pid;
                    std::cout
                            << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                            << "I am parent process (PID " << getpid()
                            << ") of child process (PID " << pid
                            << ").\n";
                    std::cout
                            << getpid() << ":\t" <<  ((double)(get_curr_time() -  now)) / 1000.0 << ":\t"
                    << "Waiting for child process to terminate...\n";


                    auto res = wait(nullptr);
                    std::cout
                            << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                            << "Child process " << res << " terminated.\n";
                } else {
                    std::cout
                            << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                            << "Failed forking child process.\n";
                }
            } else {
                std::cout
                        << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                        << "Failed forking child process.\n";
            }
        } else {
            std::cout
                    << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                    << "Failed forking child process.\n";
        }

    } else {
        std::cout
                << getpid() << ":\t" << ((double) (get_curr_time() - now)) / 1000.0 << ":\t"
                << "Failed forking child process.\n";
    }


    return 0;
}