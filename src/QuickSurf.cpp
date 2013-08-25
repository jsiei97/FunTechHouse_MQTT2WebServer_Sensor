/**
 * @file QuickSurf.cpp
 * @author Johan Simonsson
 * @brief A wrapper for the libcurl easy interface.
 */

#include <QDebug>
#include <QString>
#include <curl/curl.h>

#include "QuickSurf.h"

/// @todo Read from pipe, and surf when we have a new line.
/// @todo When ok, send a signal to pidMosq

bool QuickSurf::doSurf(QString url)
{
    qDebug() << "doSurf:" << url;

    /*
    CURL *curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.toAscii().constData());
        res = curl_easy_perform(curl);

        // always cleanup
        curl_easy_cleanup(curl);
    }

    if (res != CURLE_OK)
    {
        qDebug() << "Error: curlfailed:" << (curl_easy_strerror(res));
        return false;
    }
    */

    return true;
}
