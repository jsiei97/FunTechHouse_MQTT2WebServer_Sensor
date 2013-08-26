/**
 * @file QuickSurf.cpp
 * @author Johan Simonsson
 * @brief A wrapper for the libcurl easy interface.
 */

/*
 * Copyright (C) 2013 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

    return true;
}
