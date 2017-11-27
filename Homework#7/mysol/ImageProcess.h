/* 
 * File:   ImageProcess.h
 * Author: Kai Li
 *
 * Created on November 12, 2017, 5:02 PM
 * Copyright (C) 2017 lik7@miamiOH.edu
 */

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <omp.h>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "PNG.h"
#include "Color.h"

class ImageProcess {
    using imVec = std::vector<std::pair<int, int>>;
    using matchCount = int; 
    using misMatchCount = int; 
    using matchMisPair = std::pair<matchCount, misMatchCount>; 
    
public:
    /**
     * Constructor of ImageProcess, a wrapper class for matching process
     * @param               largePNGPath, file path of the original image 
     * @param               maskPath, file path of the mask image 
     * @param               outFilePath, file path of the resulting image 
     * @param               isMask, if sub-image is a mask PNG
     * @param               perPixMatch, percentage pixel match 
     * @param               pixTolerance, pixel color tolerance 
     */
    ImageProcess(const std::string& largePNGPath, const std::string& maskPath, 
                 const std::string& outFilePath, bool isMask, 
                 int perPixMatch, int pixTolerance);
    ImageProcess(const ImageProcess& orig);
    /**
     *  Helper method for getting color 
     * @param x
     * @param y
     * @param width
     * @param buffer
     * @return 
     */
    Color getColor(int x, int y, int width,
        const std::vector<unsigned char>& buffer);
    /**
     * Average Background Color with Large PNG and mask PNG
     * @param i             current row index from large PNG
     * @param j             current col index from large PNG
     * @param im1           large PNG
     * @param im2           mask PNG
     * @param maskBackCol   background color of mask    
     * @return 
     */
    Color backGroundAverages(int i, int j, const PNG& im1, const PNG& im2, 
        const Color& maskBackCol);
    /**
     * Checking if cropped region in large PNG is the same shade as the
     * background of mask PNG
     * @param i             current row index from large PNG
     * @param j             current col index from large PNG
     * @param im1           large PNG
     * @param im2           mask PNG
     * @param backAvg       background avg color
     * @param cp            pair<matchCount, misMatchCount>
     */
    void sameShadeMatching(int i, int j, const PNG& im1, const PNG& im2, 
    const Color& backAvg, matchMisPair& cp);
    /**
     * helper method for sameShadeMatching to if large PNG and mask PNG 
     * same shade values 
     * @param               largeColor
     * @param               backAvg
     * @return 
     */
    bool isSameShade(const Color& largeColor, const Color& backAvg);
    /**
     * net match calculation method 
     * @param row           current row of large PNG 
     * @param col           current col of large PNG
     * @param cp            pair<matchCount, misMatchCount>
     * @param mask          mask PNG
     * @param imList        match image coordinate lists 
     */
    void netMatch(int row, int col, const matchMisPair& cp,  
        const PNG& mask, imVec& imList);
    /**
     * MathingFinding, calls all other methods that are required to find 
     * a match image in the large PNG 
     */
    void matchFinding(); 
    /**
     * function to print the vector of matched image coordinates 
     * @param imlist        match image coordinate lists 
     * @param mask          mask image to be searched for 
     */
    void printVec(const imVec& imlist, const PNG& mask);
    /**
     * draw boxes on the given resulting large PNG 
     * @param png           result image to be drawn
     * @param row           upper left corner row of matched image 
     * @param col           upper left corner col of matched image 
     * @param width         width of mask image 
     * @param height        height of mask image 
     */
    void drawBox(PNG& png, int row, int col, int width, int height);
    /**
     * caller method to draw box 
     * @param png           result image to be drawn
     * @param imList        list of match images 
     * @param width         width of the mask image 
     * @param height        height of the mask image 
     */
    void draw(PNG& png, const imVec& imList, int width, int height);
    
    /**
 * check if sub-image overlaps with the matched imaged in the image list 
 * @param row           current row index of the sub-image 
 * @param col           current col index of the sub-image 
 * @param imageCoor         image coordinates of the matched list 
 * @param maskWidth     mask width 
 * @param maskHeight    mask height 
 * @return              true, if sub-image overlaps with existed matched image 
 *                      false, otherwise 
 */
    bool isInRegion(int row, int col, 
    const matchMisPair& imageCoor, int maskWidth, int maskHeight);
  
    virtual ~ImageProcess();
    
private:
    std::string largePNGPath; 
    std::string maskPath;
    std::string outFilePath; 
    bool isMask; 
    int perPixMatch; 
    int pixTolerance; 
};

#endif /* IMAGEPROCESS_H */

