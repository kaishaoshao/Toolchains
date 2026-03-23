#include "libkdb/process.hpp"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


std::unique_ptr<kdb::process>
kdb::process::launch(std::filesystem::path path){
    pid_t pid;
    if(pid = fork() < 0) {
        // Error: fork failed
    }

    if(pid == 0) {
        if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
            // Error: ptrace failed
        }
        if(execlp(path.c_str(), path.c_str(), nullptr) < 0) {
            // Error: exec failed
        }
    }

    std::unique_ptr<kdb::process> proc(new kdb::process(pid, /*terminate_on_end=*/true));
    proc->wait_on_signal();
    return proc;
}

std::unique_ptr<kdb::process>
kdb::process::attach(pid_t pid) {
    if(pid == 0) {
        // Error: Invalid PID
    }
    if(ptrace(PTRACE_ATTACH, pid, /*addr*/nullptr, /*data*/nullptr) < 0) {
        // Error: Could not attach
    }
    std::unique_ptr<kdb::process> proc(new kdb::process(pid, /*terminate_on_end=*/false));
    proc->wait_on_signal();
    return proc;
}




