#ifndef KDB_PROCESS_HPP
#define KDB_PROCESS_HPP 

#include <filesystem>
#include <memory>
#include <sys/types.h>

namespace kdb {
    enum class process_state {
        stopped,   // 进程已暂停
        running,   // 进程正在运行
        exited,    // 进程已退出
        terminated // 进程已终止
    };

    struct stop_reason
    {
        stop_reason(int wait_staus);

        process_state reason;
        std::uint8_t info; 
    };


    class process {
        public: 
            ~process();
            static std::unique_ptr<process> launch(std::filesystem::path path);
            static std::unique_ptr<process> attach(pid_t pid);

            void resume();
            stop_reason wait_on_signal();

            // 禁止默认构造函数 强制客户端使用静态成员，然后删除拷贝构造函数
            // 以禁用拷贝和移动行为
            process() = delete;
            process(const process&) = delete;
            process& operator=(const process&) = delete;
            pid_t pid() const { return pid_; }

            process_state state() const { return state_; }

        private:
            pid_t pid_ = 0;
            bool terminate_on_end_ =true; // 跟踪终止
            process_state state_ = process_state::stopped;

            process(pid_t pid, bool terminate_on_end) :
            pid_(pid), terminate_on_end_(terminate_on_end) {} 
    };

}

#endif //  KDB_PROCESS_HPP 
