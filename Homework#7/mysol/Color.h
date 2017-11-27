/* 
 * File:   Color.h
 * Author: Kai Li
 *
 * Created on November 13, 2017, 11:44 PM
 * Copyright (C) 2017 lik7@miamiOH.edu
 */

#ifndef COLOR_H
#define COLOR_H
#include <ostream>
class Color {
    friend std::ostream& operator<<(std::ostream& os, const Color& col);
    
public:
    Color();
    Color(int red, int green, int blue, int alpha);
    Color(const Color& orig);
    int getRed() const      { return red; }; 
    int getGreen() const    { return green; }; 
    int getBlue() const     { return blue; }; 
    int getAlpha() const    { return alpha; }; 
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);
    void setTolerance(int tolerance) { this->tolerance = tolerance; };
    void setAlpha(int alpha);
    bool operator==(const Color &other) const;
    bool operator!=(const Color &other) const;
    int getRedTHigh() const { return red + tolerance; }; 
    int getRedTLow() const { return red - tolerance; }; 
    int getGreenTHigh() const { return green + tolerance; }; 
    int getGreenTLow() const { return green - tolerance; }; 
    int getBlueTHigh() const { return blue + tolerance; }; 
    int getBlueTLow() const { return blue - tolerance; };  
    virtual ~Color();
    
private:
    int red;
    int green; 
    int blue;  
    int alpha; 
    int tolerance; 
};

#endif /* COLOR_H */

