//
// EPITECH PROJECT, 2018
//
// File description:
//
//

#include <Exceptions.hpp>
#include <iostream>
#include "ConsoleAnalyser.hpp"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

namespace nts {
    //reference
    char constexpr ConsoleAnalyser::STATEMENT_DELIMITER;
    char constexpr ConsoleAnalyser::COMMAND_EXIT[];
    char constexpr ConsoleAnalyser::COMMAND_DISPLAY[];
    char constexpr ConsoleAnalyser::COMMAND_SIMULATE[];
    char constexpr ConsoleAnalyser::COMMAND_LOOP[];
    char constexpr ConsoleAnalyser::COMMAND_DUMP[];

    bool ConsoleAnalyser::loop_mode = false;

    bool ConsoleAnalyser::parse_options() {
        if (_parsed)
            return false;

        for (size_t i=2; i < _argc; ++i) {
            std::string opt(_args[i]);
            if (!parse_statement(opt))
                throw ParsingError("Invalid option");
        }

        return (_parsed = true);
    }

    bool ConsoleAnalyser::init_analyser() {
        if (_init)
            return false;
        _init = true;

        handle_sigint();

        std::cout << "> ";
        for(std::string line; std::getline(std::cin, line);) {
            if (line == COMMAND_EXIT)
                break;
            else if (line == COMMAND_DISPLAY)
                _manager->Display();
            else if (line == COMMAND_SIMULATE)
                _manager->Simulate();
            else if (line == COMMAND_LOOP)
                start_loop();
            else if (line == COMMAND_DUMP)
                _manager->Dump();
            else if (!parse_statement(line))
                throw ParsingError("Unknown command");

            std::cout << "> ";
        }

        return true;
    }

    bool ConsoleAnalyser::parse_statement(std::string &line) const {
        size_t delimiter_pos = line.find(STATEMENT_DELIMITER);

        if (delimiter_pos == std::string::npos)
            return false;

        std::string component = line.substr(0, delimiter_pos);
        line.erase(0, delimiter_pos + 1);
        char state = (char) std::stoi(line);

        Tristate parsed;

        switch(state) {
            case true:
                parsed = TRUE;
                break;
            case false:
                parsed = FALSE;
                break;
            case -true:
                parsed = UNDEFINED;
                break;
            default:
                throw ParsingError("Invalid command statement found");
        }

        _manager->ChangePinValue(component, parsed);
        return true;
    }

    void ConsoleAnalyser::handle_sigint() {
        struct sigaction sigIntHandler;

        sigIntHandler.sa_handler = [](int) -> void {
            std::cout<<std::endl;
            loop_mode = false;
        };
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;

        sigaction(SIGINT, &sigIntHandler, nullptr);
    }

    void ConsoleAnalyser::start_loop() {
        loop_mode = true;

        while (loop_mode)
            _manager->Simulate();
    }
}