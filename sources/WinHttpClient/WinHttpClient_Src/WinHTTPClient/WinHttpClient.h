/**
 *  Copy right 2008 Cheng Shi.  All rights reserved.
 *  This is a class to get http response synchronously.  It should not be used in the UI thread.
 */

#ifndef WINHTTPCLIENT_H
#define WINHTTPCLIENT_H

#include <windows.h>
#include <Winhttp.h>
#include <string>
using namespace std;

static const int INT_RETRYTIMES = 3;
static wchar_t *SZ_AGENT = L"WinHttpClient";

class WinHttpClient
{
public:
    WinHttpClient(const wstring &URL);

    // It is a synchronized method and may take a long time to finish.
    bool SendHttpRequest(const wstring &httpVerb = L"GET");
    wstring GetHttpResponseHeader(void);
    wstring GetHttpResponse(void);
    wstring GetCharset(void);
    wstring GetHost(void);

private:
    WinHttpClient(const WinHttpClient &other);
    WinHttpClient &operator =(const WinHttpClient &other);

    wstring m_URL;
    wstring m_HttpResponseHeader;
    wstring m_HttpResponse;
    wstring m_charset;
    wstring m_host;
};

#endif // WINHTTPCLIENT_H
