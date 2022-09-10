#define _CRT_SECURE_NO_WARNINGS 1
#include "log.h"
#include <Windows.h>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <filesystem>

static HANDLE handle_stdout{nullptr};
static std::ofstream* file_output{nullptr};

bool logging::initialize(std::string &error) {
    if(!GetConsoleWindow() && !AllocConsole()) {
        error = "failed to allocate console";
        return false;
    }

    {
        auto console_window = GetConsoleWindow();
        if(!console_window) {
            error = "failed to find console window";
            return false;
        }

        SetFocus(console_window);
    }

    handle_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if(handle_stdout == INVALID_HANDLE_VALUE) {
        error = "failed to get stdout handle";
        return false;
    }

    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream time_buffer{};
        time_buffer << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
        auto file_name = "hook_log_" + time_buffer.str() + ".txt";

        file_output = new std::ofstream{file_name};
        logging::info("Initialized logging to file {}\\{}", std::filesystem::current_path().string(), file_name);
    }

    return true;
}

void logging::finalize() {
    handle_stdout = nullptr;
    FreeConsole();
}

void logging::message(const std::string_view &level, const std::string_view &message) {
    char buffer_time[100];
    time_t curr_time;
    time(&curr_time);
    auto curr_tm = localtime(&curr_time);
    strftime(buffer_time, 100, "%H:%M:%S", curr_tm);

    auto full_message = fmt::format("{} [{}] {}\n", buffer_time, level, message);

    if(file_output) {
        *file_output << full_message << std::flush;
    }

    DWORD bytes_written{};
    WriteConsoleA(handle_stdout, full_message.c_str(), (DWORD) full_message.length(), &bytes_written, nullptr);
}

std::string logging::last_error_message() {

    auto last_error = ::GetLastError();
    if(!last_error) {
        return "success";
    }

    std::string message{};
    message.resize(1024);
    size_t size = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, last_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) message.data(), message.size(),
            nullptr
    );

    message.resize(size - 1); // Remove the tailing new line
    return message;
}