// MIT License
//
// Copyright (c) 2022 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
// fpth
// ----
// fpaper to html transpiler
//

#include "include/fpaper.hpp"
#include <iostream>
#include <filesystem>
#include "include/fpaper_extract.hpp"

int main(int argc, char** argv) noexcept {
    if(argc < 2) {
        std::cout << "fpth - fpaper to html transpiler\n"
                     "--------------------------------\n" <<
                     argv[0] << " file -> generate html output\n";
        return 1;
    } const std::string file(argv[1]);

    if(std::filesystem::exists(file)) {
        FPaper init; init.Init(file);
        FPaper_Extract extract(init);
        extract.Compile();
        if(std::filesystem::exists(file + "_fpth.html")) {
            std::cout << "Overwrite to '" << file + "_fpth.html'? (y/N): ";
            char ch = std::getchar();

            if(ch == 'n' || ch == 'N' || ch == '\n') {
                std::cout << "Aborted.\n";
                return 0;
            }
        }
        std::fstream file_str(file + "_fpth.html", std::ios::out);
        file_str << extract.extracted_text << '\n';
    }
}