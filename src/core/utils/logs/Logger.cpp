#include "core/utils/logs/Logger.h"

void Logger::init(const std::string& text)
{
    std::cout << MAGENTA << "------------[_" << BOLDMAGENTA << text << MAGENTA << "_]------------" << RESET << "\n";
}

void Logger::exit(const std::string& text)
{
    std::cout << MAGENTA << "------------[/" << BOLDMAGENTA << text << MAGENTA << "/]------------" << RESET << "\n";
}

void Logger::printText(const std::string& prefix, const std::string& text)
{
    std::cout << BLUE << "[_MESSAGE_] : " << BOLDBLUE << prefix << " -> " << text << RESET << "\n";
}

void Logger::printInfo(const std::string& prefix, const std::string& text)
{
    std::cout << CYAN << "[___INFO__] : " << BOLDCYAN << prefix << " -> " << text << RESET << "\n";
}

void Logger::printSuccess(const std::string& prefix, const std::string& text)
{
    std::cout << GREEN << "[_SUCCESS_] : " << BOLDGREEN << prefix << " -> " << text << RESET << "\n";
}

void Logger::printWarning(const std::string& prefix, const std::string& text)
{
    std::cout << YELLOW << "[_WARNING_] : " << BOLDYELLOW << prefix << " -> " << text << RESET << "\n";
}

void Logger::printError(const std::string& prefix, const std::string& text)
{
    std::cout << RED << "[__ERROR__] : " << BOLDRED << prefix << " -> " << text << RESET << "\n";
}
