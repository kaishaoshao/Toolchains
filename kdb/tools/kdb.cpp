#include <iostream>
#include <unistd.h>

#include <string_view>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <vector>
#include <algorithm>
#include <sstream>

namespace {
    // 附加到一个进程
    // 如果传入的参数是 -p <pid>，则附加到指定的 PID 进程
    // 如果传入的参数是 <program_path>，则创建一个新进程并执行该程序
    // 返回附加的 PID 或新创建的 PID
    // 如果附加失败，返回 -1
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

    // 添加用户界面 && 处理用户输入

    // 将输入的字符串按指定分隔符分割成多个子字符串
    std::vector<std::string> split(std::string_view str, char delimiter) {
        std::vector<std::string>  out{};
        std::stringstream ss{std::string(str)};
        std::string item;

        while(std::getline(ss, item, delimiter)) 
            out.push_back(item);
        return out;
    }

    // 检查一个字符串是否是另一个字符串的前缀
    bool is_prefix(std::string_view str, std::string_view of) {
        if(str.size() > of.size())
            return false;
        return std::equal(str.begin(), str.end(), of.begin());
    }

    // 恢复进程的执行
    // 通过 PTRACE_CONT 命令让进程继续执行
    // 如果恢复失败，打印错误信息并退出程序
    void resume(pid_t pid) {
        if(ptrace(PTRACE_CONT, pid, /*addr*/nullptr, /*data*/nullptr) < 0)
        {
            std::perror("Could not continue\n");
            std::exit(-1);
        }
    }

    // 等待进程发送信号
    void wait_on_signal(pid_t pid) 
    {
        int wait_status;
        int options =0;
        if(waitpid(pid, &wait_status, options) < 0)
        {
            std::perror("Waitpid failed");
            std::exit(-1);
        }
    }

    // 处理用户输入的命令
    void handle_command(pid_t pid, std::string_view line) 
    {
        auto args = split(line, ' ');
        auto command = args[0];
        if(is_prefix(command, "continue")) 
        {
            resume(pid);
            wait_on_signal(pid);
        }
        else 
            std::cerr << "Unknown command\n";

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

    char *line = nullptr;
    // 使用 readline 库来处理用户输入
    // readline 会自动处理输入的历史记录
    // while((line = realine("kdb> ") != nullptr) {
    //     handle_command(pid, line);
    //     add_history(line);
    //     free(line);
    // } 
    while((line = readline("kdb> ")) != nullptr)
    {
        std::string line_str;
        if(line == std::string_view("")) 
        {
            free(line);
            if(history_length > 0) 
                line_str = history_list()[history_length - 1]->line;
        }  
        else
        {
            line_str = line;
            add_history(line);
            free(line);
        }
        if(!line_str.empty())
            handle_command(pid, line_str);
    }
    
    return 0;
}
