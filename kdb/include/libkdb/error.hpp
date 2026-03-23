#ifndef KDB_ERROR_HPP
#define KDB_ERROR_HPP 

#include <stdexcept>
#include <string>
namespace kdb { 
    class error : public std::runtime_error {
    public:
        // [[noreturn]]
        static void send(const std::string& what) {
            throw error{what};
        }

        // [[noreturn]]
        static void send_errno(const std::string& perfix) {
            throw error{perfix + ": " + std::strerror(errno)};
        }
    
    private:
        error(const std::string& what) : std::runtime_error(what) {} 
    };

} // namespace kdb

#endif // KDB_ERROR_HPP
