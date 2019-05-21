//
//  BRCalculator.hpp
//  BRCalculator
//
//  Created by edward.shi on 5/21/19.
//  Copyright © 2019 edward.shi. All rights reserved.
//

#ifndef BRCalculator_hpp
#define BRCalculator_hpp

#include <stdio.h>

using namespace std;


enum {
    BR_OK   = 0,
    BR_FAIL = 1
};

#define RC_ST_DURATION    1
#define RC_MAX(a,b) (((a) > (b)) ? (a) : (b))

#define RCCheckedNull(p)\
    if (!p){\
        printf("error::null pointer \n");\
        return BR_OK; \
    }

class BRCalculator
{
public:
    BRCalculator();
    ~BRCalculator();
    
    int32_t init(double fFrmRate);
    void release();
    void parseFrameBits(char* pLine, const int32_t kLen);
    int32_t updateFrameBR();

protected:
    
protected:
    double m_fFrameRate;
    int32_t m_iFrameRate;
    int32_t m_iFrameNum;
    int32_t m_iFrameBits;
    
    int32_t m_iDelimStartNum;
    char m_cDelimStart;
    char m_cDelimEnd;

    int32_t m_iRefBitrate;
    int32_t m_iFrmListLen;
    int32_t* m_pFrmBitsList;
    int32_t* m_pFrmBRList;
    double* m_pFrmAveBRList;

    FILE* m_pBRStatFile;
};

#endif /* BRCalculator_hpp */
