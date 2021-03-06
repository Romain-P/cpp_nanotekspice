//
// EPITECH PROJECT, 2018
// 
// File description:
// 
//

#include "Exceptions.hpp"

namespace nts {
    NtsError::NtsError(std::string const &message) : _message(message) {
    }

    const char *NtsError::what() const noexcept {
        return (_message.data());
    }

    ComputeError::ComputeError(std::string const &message) : NtsError(message) {
    }

    LinkError::LinkError(std::string const &message) : NtsError(message) {
    }

    SetError::SetError(std::string const &message) : NtsError(message) {
    }

    ParsingError::ParsingError(std::string const &message) : NtsError(message) {}
}