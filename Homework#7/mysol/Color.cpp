/** 
 * File:   Color.cpp
 * Author: Kai Li
 * 
 * Created on November 13, 2017, 11:44 PM
 * Copyright (C) 2017 lik7@miamiOH.edu
 */

#include "Color.h"

Color::Color() {
    red = 0; 
    green = 0; 
    blue = 0; 
    alpha = 1;
}

Color::Color(int red, int green, int blue, int alpha) :
red(red), green(green), blue(blue), alpha(alpha) {
}


Color::Color(const Color& orig) {
    red = orig.red;
    green = orig.green; 
    blue = orig.blue; 
    alpha = orig.alpha; 
}

Color::~Color() {
}

std::ostream& operator<<(std::ostream& os, const Color& col) {
    return (os << "red: " << std::to_string(col.red)
              << " green: " << std::to_string(col.green) << 
                 " blue: " << std::to_string(col.blue) << 
                 " alpha: " << std::to_string(col.alpha) 
                            << std::endl);
}

void Color::setRed(int red) {
    this->red = red; 
}

void Color::setGreen(int green) {
    this->green = green; 
}

void Color::setBlue(int blue) {
    this->blue = blue; 
}

void Color::setAlpha(int alpha) {
    this->alpha = alpha; 
}

bool Color::operator==(const Color &other) const {
    return (red == other.red && green == other.green && blue == 
            other.blue);
}

bool Color::operator !=(const Color& other) const {
    return (red != other.red && green != other.green && blue != other.blue);
}
