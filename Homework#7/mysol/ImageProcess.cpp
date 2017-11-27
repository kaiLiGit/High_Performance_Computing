/* 
 * File:   ImageProcess.cpp
 * Author: Kai Li
 * 
 * Created on November 12, 2017, 5:02 PM
 * Copyright (C) 2017 lik7@miamiOH.edu
 */

#include <utility>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ImageProcess.h"

ImageProcess::ImageProcess(const std::string& largePNGPath, 
                 const std::string& maskPath, 
                 const std::string& outFilePath, bool isMask, 
                 int perPixMatch, int pixTolerance): 
                largePNGPath(largePNGPath), maskPath(maskPath), 
                outFilePath(outFilePath), isMask(isMask), 
                perPixMatch(perPixMatch), pixTolerance(pixTolerance) {
}

ImageProcess::ImageProcess(const ImageProcess& orig) {
    largePNGPath = orig.largePNGPath; 
    maskPath = orig.maskPath; 
    outFilePath = orig.outFilePath; 
    isMask = orig.isMask;
    perPixMatch = orig.perPixMatch; 
    pixTolerance = orig.pixTolerance; 
}

Color 
ImageProcess::getColor(int x, int y, int width,
        const std::vector<unsigned char>& buffer) {
    // row + col * width * 4 (RGBA values)
    unsigned int index = y + x * width;
    // multiply index by 4 to offset the set of colors (RGBA) stored in buffer
    index = index * 4; 
    Color col; 
    if (index + 3 < buffer.size()) {
    col.setRed(static_cast<int>(buffer[index])); 
    col.setGreen(static_cast<int>(buffer[index + 1]));
    col.setBlue(static_cast<int>(buffer[index + 2]));
    col.setAlpha(static_cast<int>(buffer[index + 3])); 
    }
    return col;
}

Color 
ImageProcess::backGroundAverages(int i, int j, const PNG& im1, const PNG& im2, 
        const Color& maskBackCol) {
    int w1, w2, h2; 
    int redBack = 0, greenBack = 0, blueBack = 0, blackPix = 0; 
    w1 = im1.getWidth(); w2 = im2.getWidth(); h2 = im2.getHeight(); 
    // iterating block by block in row by row and col by col 
    // x, y coordinates for large PNG, x takes [i, i + w2] y takes [j, j + h2]
    // k, z coordinates for mask PNG, k takes [0, w2] z takes [0, h2]
    for (int x = i, k = 0; (x < (i + h2)) && (k < h2); x++, k++) {
        for (int y = j, z = 0; (y < (j + w2)) && (z < w2); y++, z++) {
            // large PNG color 
            Color largeCol = getColor(x, y, w1, im1.getBuffer());  
            // Mask PNG color 
            Color maskCol = getColor(k, z, w2, im2.getBuffer());
            if (maskCol == maskBackCol) {   // mask color is black 
                redBack   += largeCol.getRed(); 
                greenBack += largeCol.getGreen(); 
                blueBack  += largeCol.getBlue(); 
                // alphaBack += largeCol.getAlpha();
                blackPix++; 
            }  
        } 
    }
    // determine background color average of mask and LargePNG, alpha is 1 
    Color backAvg; 
    if (blackPix > 0) {
        backAvg.setRed((redBack / blackPix));
        backAvg.setGreen((greenBack / blackPix));
        backAvg.setBlue((blueBack / blackPix));
        backAvg.setAlpha(1); 
    } 
    // std::cout << "Number of black mask pixels:"<< blackPix << std::endl;
    // std::cout << "Total of red, blue, green values:" << redBack << ", " 
    //        << greenBack << " , " << blueBack << std::endl;
    // std::cout << "Average Background color:"<< backAvg << std::endl;
    return backAvg; 
}

bool
ImageProcess::isSameShade(const Color& largeColor, const Color& backAvg) {
    return ((largeColor.getRed() <= backAvg.getRedTHigh()
         && largeColor.getRed() >= backAvg.getRedTLow())  
         && (largeColor.getGreen() <= backAvg.getGreenTHigh()
         && largeColor.getGreen() >= backAvg.getGreenTLow())
         && (largeColor.getBlue() <= backAvg.getBlueTHigh()
         && largeColor.getBlue() >= backAvg.getBlueTLow())); 
}

void 
ImageProcess::sameShadeMatching(int i, int j, const PNG& im1, const PNG& im2, 
    const Color& backAvg, matchMisPair& cp) {
    int w1, w2, h2;   
    w1 = im1.getWidth();
    w2 = im2.getWidth(); h2 = im2.getHeight(); 
    // fore-color is white, maskBackColor is black
    Color maskBackColor(0, 0, 0, 1); 
    // iterating block by block in row by row and col by col 
    // x, y coordinates for large PNG, x takes [i, i + w2] y takes [j, j + h2]
    // k, z coordinates for mask PNG, k takes [0, w2] z takes [0, h2]
    for (int x = i, k = 0; (x < (i + h2)) && (k < h2); x++, k++) {
        for (int y = j, z = 0; (y < (j + w2)) && (z < w2); y++, z++) {  
            Color largeColor = getColor(x, y, w1, im1.getBuffer());
            Color maskColor = getColor(k, z, w2, im2.getBuffer());
            // if maskColor is black
            if ((maskColor == maskBackColor) && 
                    isSameShade(largeColor, backAvg)) { 
                cp.first++;  // cp.first, match count 
            } else if (maskColor != maskBackColor
                    && (!isSameShade(largeColor, backAvg))) { 
                cp.first++;  // cp.first, match count
            } else { 
                cp.second++;  // cp.second, mismatch count 
            }
        } 
    }  
}

bool 
ImageProcess::isInRegion(int row, int col, 
    const matchMisPair& imageCoor, int maskWidth, int maskHeight) {
       return (((row <= (imageCoor.first + (maskHeight))) 
                    && (row >= imageCoor.first) 
                    && (col <= imageCoor.second + (maskWidth))
                    && (col >= imageCoor.second)) 
       || ((row) <= (imageCoor.first + (maskHeight)) 
                    && (row >= imageCoor.first) 
                    && (col + (maskWidth - 1) <= imageCoor.second + (maskWidth))
                    && (col + (maskWidth - 1) >= imageCoor.second)) 
       || ((row + (maskHeight - 1)) <= (imageCoor.first + (maskHeight)) 
                    && (row + (maskHeight - 1) >= imageCoor.first) 
                    && (col + (maskWidth - 1) <= imageCoor.second + (maskWidth))
                    && (col + (maskWidth - 1) >= imageCoor.second)) 
       || ((row + (maskHeight - 1) <= (imageCoor.first + (maskHeight))) 
                    && (row + (maskHeight - 1) >= imageCoor.first) 
                    && (col <= imageCoor.second + (maskWidth))
                    && (col >= imageCoor.second)));
}

void 
ImageProcess::netMatch(int row, int col, const matchMisPair& cp,  
        const PNG& mask, imVec& imList) {
    int netMatchCount = cp.first - cp.second;
    // std::cout << "matched:" << cp.first << 
    // " mismatched: " << cp.second << "\n"; 
    // std::cout << "Matching pixel count at row: " << row << ", col: " << col  
    //        << " = " << netMatchCount <<  std::endl;
    // std::cout << "Matching pixel needed :" << 
    //        (mask.getWidth() * mask.getHeight() * perPixMatch / 100) 
    //         << std::endl; 
    // if netMatch > mask.getWidth * mask.getHeight * perPixMatch / 100 
    // a match is found 
    if (netMatchCount > 
            (mask.getWidth() * mask.getHeight() * perPixMatch / 100)) {
        // cropped image upper-left corner coordinate
        std::pair<int, int> im(row, col);
        // If element not in the match list, push it to the list
        if (imList.empty()) {
#pragma omp critical(imList_modified)           
            imList.push_back(im); 
        } else {
            bool isExisted = false; 
#pragma omp critical(imList_access)
    {
        for (const std::pair<int, int>& image : imList) {
            // if image is within the match image range 
            if (isInRegion(row, col, image, mask.getWidth(), 
                    mask.getHeight())) {
                isExisted = true; 
                }   // else don't add the new image
            }             
    } 
        if (!isExisted) {
#pragma omp critical(imList_modified)
           imList.push_back(im);
        } } 
    }
}

void 
ImageProcess::printVec(const imVec& imlist, const PNG& mask) {
    int numMatch = 0;
    for (const std::pair<int, int>& imp : imlist) {
        std::cout << "sub-image matched at: "
                  << imp.first << ", " << imp.second << ", "
                  << imp.first + mask.getHeight() << ", "
                  << imp.second + mask.getWidth() << std::endl; 
        numMatch++;
    }
    std::cout << "Number of matches: " << numMatch << std::endl;
}

void 
ImageProcess::drawBox(PNG& png, int row, int col, int width, int height) {
    // Draw horizontal lines
    for (int i = 0; (i < width); i++) {
        png.setRed(row, col + i);
        png.setRed(row + height, col + i);
    }
    // Draw vertical lines
    for (int i = 0; (i < height); i++) {
        png.setRed(row + i, col);
        png.setRed(row + i, col + width);
    }
}

void 
ImageProcess::draw(PNG& png, const imVec& imList, int width, int height) {
    for (const std::pair<int, int>& im : imList) {
        drawBox(png, im.first, im.second, width, height);
    }
}

void
ImageProcess::matchFinding() {
    imVec imList;  
#pragma omp parallel shared(imList)  
  {
    PNG im1, mask, resultIm; 
    im1.load(largePNGPath); mask.load(maskPath); resultIm.load(largePNGPath);
    int w1, h1, w2, h2;  
    w1 = im1.getWidth();  h1 = im1.getHeight();   
    w2 = mask.getWidth(); h2 = mask.getHeight(); 
    //  reference mask background color, always black
    Color maskBackColor(0, 0, 0, 1);   
#pragma omp for schedule(guided) 
    for (int i = 0; (i <= h1 - h2); i++) {
        for (int j = 0; (j <= w1 - w2); j++) { 
            Color backAvg = backGroundAverages(i, j, im1, mask, maskBackColor);
            // setting background average tolerance before comparing shades
            backAvg.setTolerance(pixTolerance);
            // cp, count pair<matchCount, misMatchCount>
            matchMisPair cp;  
            sameShadeMatching(i, j, im1, mask, backAvg, cp);
            netMatch(i, j, cp, mask, imList);
        }
    } 
  }
    PNG mask, resultIm; mask.load(maskPath); resultIm.load(largePNGPath);
    // all regions have been searched 
    // sort algorithm internally sorts pair<row, col> by row first then 
    // column next 
    std::sort(imList.begin(), imList.end()); printVec(imList, mask);
    int w2 = mask.getWidth(), h2 = mask.getHeight(); 
    // [w2, h2] => [width, height] of large image
    // (w2 - 1) and (h2 - 1) to avoid segment fault problem
    draw(resultIm, imList, w2 - 1, h2 - 1); resultIm.write(outFilePath);
}

ImageProcess::~ImageProcess() {
    // ImageProcess instance of object is destroyed or its allotted memory is 
    // cleared
}

