#include <iostream>
#include <unistd.h>

#include <string_view>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace {
    pid_t attach(int argc, char **argv){
        pid_t pid = 0;
        // Passing PID 
        if (argc == 3 && argv[1] == std::string_view("-p")) {
            pid = std::stoi(argv[2]);
            if (pid < 0)
            {
                std::cerr << "Invalid PID" << std::endl;
                return -1;
            }
            
            if(ptrace(PTRACE_ATTACH, pid, /*addr*/nullptr, /*data*/nullptr) < 0) {
                std::perror("Could not attach");
            }
        }
        // Passing process name
        else {
            const char *program_path = argv[1];
            if ((pid = fork()) < 0)
            {   
                std::perror("Fork failed");
                return -1;
            }
            if (pid == 0)
            {
                // We're in the child process
                // Execute debugee.
                // 如果我们处于子进程，我们应该用我们想要调试的程序替换当前正在执行的程序
                // 在我们exec之前，我们必须先调用ptrace(PTRACE_TRACEME)将程序设置为可调试的
                if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
                    std::perror("Could not set trace");
                    return -1;
                } 
                if(execlp(program_path, program_path, nullptr) < 0) {
                    std::perror("Exec failed");
                    return -1;
                }
            }
            
        }
        return pid;
    }
}

int main(int argc, char **argv){

    if(argc == 1) {
        std::cerr << "No arguments given\n";
        return 1;
    }

    pid_t pid = attach(argc, argv);

    // 在附加进程后，等待其暂停执行，接受用户输入
    int wait_status;
    int options = 0;
    if(waitpid(pid, &wait_status, options) < 0) 
    {
        std::perror("Waitpid failed");
    }


    return 0;
}