/* 
 * File:   main.cpp
 * Author: Kai Li
 *
 * Created on November 19, 2017, 12:16 PM
 * Copyright (C) 2017 lik7@miamiOH.edu
 */
#include <cstdlib>
#include <iostream>
#include <vector>  
#include <string> 
#include "PNG.h"
#include "ImageProcess.h"

void cmdSwich(int argc, char** argv, std::string& largePNGPath, 
              std::string& maskPath, std::string& outFilePath, 
              int& perPixMatch, int& colorTolerance) { 
    switch (argc) {
            case 4:
                largePNGPath = argv[1]; maskPath = argv[2]; 
                outFilePath = argv[3];
                break;
            case 5:  
                largePNGPath = argv[1]; maskPath = argv[2]; 
                outFilePath = argv[3];
                // isMask is always assumed to be true, so argv[4] is skipped
                break;
            case 6: 
                largePNGPath = argv[1]; maskPath = argv[2]; 
                outFilePath = argv[3];
                // isMask is always assumed to be true, so argv[4] is skipped
                perPixMatch = std::stoi(argv[5]); 
                break;
            case 7:
                largePNGPath = argv[1]; maskPath = argv[2]; 
                outFilePath = argv[3];
                // isMask is always assumed to be true, so argv[4] is skipped
                perPixMatch = std::stoi(argv[5]); 
                colorTolerance = std::stoi(argv[6]); 
                break;
        } 
}

std::string 
charArrToStr(int argc, char** argv) {
    std::string result; 
    for (int i = 0; i < argc; i++) {
        result += argv[i];
    }
    return result;
}

int main(int argc, char** argv) {
    std::string argvStr(charArrToStr(argc, argv)), 
            largePNGPath, maskPath, outFilePath;
    bool isMask = true;
    int perPixMatch = 75, colorTolerance = 32; 
    // argv[] must have at least three command line arguments + .exe file name
    if (argc < 4) {
        std::cout << "missing arguments, your argument: " 
                << argvStr << std::endl;
    } else if (argc >= 4) {  // get command values
        cmdSwich(argc, argv, largePNGPath, maskPath, outFilePath, perPixMatch, 
                colorTolerance); 
        ImageProcess impro(largePNGPath, maskPath, outFilePath, 
                       isMask, perPixMatch, colorTolerance);
        impro.matchFinding(); 
    } 
    return 0;
}
