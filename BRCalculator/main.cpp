//
//  main.cpp
//  BRCalculator
//
//  Created by edward.shi on 5/21/19.
//  Copyright © 2019 edward.shi. All rights reserved.
//

#include <iostream>
#include "BRCalculator.hpp"


int main(int argc, const char * argv[]) {
    const int32_t kLen = 200;
    char aLine[kLen];
    FILE* pInputFile = NULL;

    double fFrameRate = atof(argv[1]);
    if (fFrameRate < 0) {
        printf(" invalid frame rate \n");
        printf(" usage: app framerate frmSizeFile \n");
        return BR_FAIL;
    }
    
    pInputFile = fopen(argv[2], "r");
    if (!pInputFile) {
        printf(" failed to open input statistic file \n");
        printf(" usage: app framerate frmSizeFile \n");
        return BR_FAIL;
    }

    BRCalculator cBRCalculator;
    cBRCalculator.init(fFrameRate);

    while (!feof(pInputFile)) {
        fgets(aLine, kLen, pInputFile);
        
        cBRCalculator.parseFrameBits(aLine, kLen);
        cBRCalculator.updateFrameBR();
    }
    
    
    return 0;
}
