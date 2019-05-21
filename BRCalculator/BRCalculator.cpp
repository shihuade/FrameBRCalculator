//
//  BRCalculator.cpp
//  BRCalculator
//
//  Created by edward.shi on 5/21/19.
//  Copyright © 2019 edward.shi. All rights reserved.
//

#include <stdlib.h>
#include "string.h"
#include "BRCalculator.hpp"

BRCalculator::BRCalculator() :
    m_fFrameRate(0),
    m_iFrameNum(0),
    m_iFrmListLen(0),
    m_iDelimStartNum(0),
    m_cDelimStart(','),
    m_cDelimEnd('\n'),
    m_pFrmBitsList(NULL),
    m_pFrmBRList(NULL),
    m_pFrmAveBRList(NULL),
    m_pBRStatFile(NULL)
{
}

BRCalculator::~BRCalculator()
{
    release();
}

int32_t BRCalculator::init(double fFrmRate)
{
    m_fFrameRate = fFrmRate;
    m_iFrameRate = static_cast<int32_t>(fFrmRate + 0.5);
    m_iDelimStartNum = 2;
    m_cDelimStart = ',';
    m_cDelimEnd = '\n';
    
    m_iFrmListLen = RC_ST_DURATION * (m_fFrameRate + 10);
    
    m_pFrmBRList = (int32_t*) malloc(sizeof(int32_t) * m_iFrmListLen);
    RCCheckedNull(m_pFrmBRList);
    memset(m_pFrmBRList, 0, sizeof(int32_t) * m_iFrmListLen);
    
    m_pFrmAveBRList = (int32_t*) malloc(sizeof(int32_t) * m_iFrmListLen);
    RCCheckedNull(m_pFrmAveBRList);
    memset(m_pFrmAveBRList, 0, sizeof(int32_t) * m_iFrmListLen);
    
    m_pFrmBitsList = (int32_t*) malloc(sizeof(int32_t) * m_iFrmListLen);
    RCCheckedNull(m_pFrmBitsList);
    memset(m_pFrmBitsList, 0, sizeof(int32_t) * m_iFrmListLen);

    
    if (m_pBRStatFile) {
        fclose(m_pBRStatFile);
        m_pBRStatFile = NULL;
    }
    
    m_pBRStatFile = fopen("BRStatisticInfo.csv", "w");
    if (!m_pBRStatFile) {
        printf("failed to open output statistic file!");
        return BR_FAIL;
    }
    fprintf(m_pBRStatFile, "FrmIdx, FrmBits, FrmInsBR, AveBR\n");
    
    return BR_OK;
}

void BRCalculator::release()
{
    if (m_pFrmBRList) {
        free(m_pFrmBRList);
        m_pFrmBRList = NULL;
    }
    
    if (m_pFrmAveBRList) {
        free(m_pFrmAveBRList);
        m_pFrmAveBRList = NULL;
    }

    if (m_pFrmBitsList) {
        free(m_pFrmBitsList);
        m_pFrmBitsList = NULL;
    }
    
    if (m_pBRStatFile) {
        fclose(m_pBRStatFile);
        m_pBRStatFile = NULL;
    }
}

void BRCalculator::parseFrameBits(char* pLine, const int32_t kLen)
{
    char cFrmBits[kLen];
    bool bStart = false;
    int32_t iNum = 0, iDelimNum = 0;
    
    for(int32_t i = 0; i < kLen; i++) {
        if (pLine[i] == m_cDelimStart) {
            iDelimNum++;
            
            if (iDelimNum == m_iDelimStartNum) {
                bStart =  true;
                continue;
            }
        }

        if (pLine[i] == '\n') {
            break;
        }
        
        if (bStart) {
            cFrmBits[iNum] = pLine[i];
            iNum++;
        }
    }

    cFrmBits[iNum] = '\n';
    m_iFrameBits= atoi(cFrmBits);
}

int32_t BRCalculator::updateFrameBR()
{
    int32_t iPreBRIdx = 0;
    int32_t iPreIdx = (m_iFrameNum - 1 + m_iFrmListLen) % m_iFrmListLen;
    int32_t iCurIdx = m_iFrameNum % m_iFrmListLen;
    double freDuration = RC_MAX(RC_ST_DURATION, m_iFrameNum / m_fFrameRate);
    double fCurDuration = RC_MAX(RC_ST_DURATION, (m_iFrameNum + 1) / m_fFrameRate);

    //ave br from first frame
    m_pFrmAveBRList[iCurIdx] = m_pFrmAveBRList[iPreIdx] * freDuration + m_iFrameBits;
    m_pFrmAveBRList[iCurIdx] /= fCurDuration;
    
    //short term bit rate within pass one second
    m_pFrmBRList[iCurIdx] = m_pFrmBRList[iPreIdx] * RC_ST_DURATION + m_iFrameBits;
    if (fCurDuration > RC_ST_DURATION) {
        iPreBRIdx = (m_iFrameNum - RC_ST_DURATION * m_iFrameRate) % m_iFrmListLen;
        m_pFrmBRList[iCurIdx] -= m_pFrmBitsList[iPreBRIdx];
    }
    m_pFrmBRList[iCurIdx] /= RC_ST_DURATION;
    
    fprintf(m_pBRStatFile, "%d, %d, %d, %d, \n",
            m_iFrameNum, m_iFrameBits,
            m_pFrmBRList[iCurIdx], m_pFrmAveBRList[iCurIdx]);
    
    printf("%d, %d, %d, %d, \n", m_iFrameNum, m_iFrameBits, m_pFrmBRList[iCurIdx], m_pFrmAveBRList[iCurIdx]);

    
    m_pFrmBitsList[iCurIdx] = m_iFrameBits;
    m_iFrameNum++;

    return 0;

}
