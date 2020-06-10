/**
 *  Copy right 2008 Cheng Shi.  All rights reserved.
 */
#include <iostream>
using namespace std;
#include "WinHttpClient.h"

int main(wchar_t **argv, int argc)
{
    wstring url = L"http://www.codeproject.com/";
    WinHttpClient client(url);

    wprintf(L"Sending HTTP request...\n");
    // This method is synchronous and will take a long time.
    if (client.SendHttpRequest())
    {
        wstring httpResponseHeader = client.GetHttpResponseHeader();
        wstring httpResponse = client.GetHttpResponse();
        wprintf(L"HTTP Response Header:\n%s\n\n", httpResponseHeader.c_str());
        wprintf(L"HTTP Response:\n%s\n\n", httpResponse.c_str());
    }

    cin.get();
    return 0;
}
