/**
 * @file QuickSurf.h
 * @author Johan Simonsson  
 * @brief A wrapper for the libcurl easy interface.
 */

#ifndef  __QUICKSURF_H
#define  __QUICKSURF_H


#include <QString>

class QuickSurf 
{
     private: 
     public: 
         static bool doSurf(QString url);

};

#endif  // __QUICKSURF_H 
