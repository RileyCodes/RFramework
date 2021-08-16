#pragma once
#include <Windows.h>
#include <string>
#include <stdexcept>
//Credit: https://stackoverflow.com/questions/14762456/getclipboarddatacf-text
using std::string;
class RaiiTextGlobalLock
{
public:
    explicit RaiiTextGlobalLock(HANDLE hData)
        : m_hData(hData)
    {
        m_psz = static_cast<const char*>(GlobalLock(m_hData));
        if (!m_psz)
            throw std::runtime_error("Can't acquire lock on clipboard text.");
    }

    ~RaiiTextGlobalLock()
    {
        GlobalUnlock(m_hData);
    }

    const char* Get() const
    {
        return m_psz;
    }

private:
    HANDLE m_hData;
    const char* m_psz;

    // Ban copy
    RaiiTextGlobalLock(const RaiiTextGlobalLock&);
    RaiiTextGlobalLock& operator=(const RaiiTextGlobalLock&);
};



class RClipboard
{
public:
    RClipboard()
    {
        if (!OpenClipboard(nullptr))
            throw std::runtime_error("Can't open clipboard.");
        // ... or define some custom exception class for clipboard errors.
    }

    ~RClipboard()
    {
        CloseClipboard();
    }
    static string GetClipboardTextSync()
    {
        string text;
	    while(!GetClipboardText(text))
	    {
            Sleep(1);
	    }
        return text;
    }

    static bool GetClipboardText(string& out)
    {
    	try
    	{
            RClipboard clipboard;
            HANDLE hData = GetClipboardData(CF_TEXT);
            if (hData == nullptr)
                return false;
            RaiiTextGlobalLock textGlobalLock(hData);
            string text(textGlobalLock.Get());

            out = text;
            return true;
    	}
        catch (std::runtime_error& runtime_error)
        {
            return false;
        }
        return false;
    }

    static bool SetClipboardText(const std::string& s) {
        HWND hwnd = nullptr;
        if (!OpenClipboard(hwnd))
            return false;
        if (!EmptyClipboard())
            return false;
    	
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
        if (!hg) {
            CloseClipboard();
            return false;
        }
        memcpy_s(GlobalLock(hg), s.size() + 1, s.c_str(), s.size() + 1);
        GlobalUnlock(hg);
        if (!SetClipboardData(CF_TEXT, hg))
            return false;
        CloseClipboard();
        GlobalFree(hg);
        return true;
    }

    static void SetClipboardTextSync(const std::string& s)
    {
        while (!SetClipboardText(s))
        {
            Sleep(1);
        }
    }
private:
    RClipboard(const RClipboard&);
    RClipboard& operator=(const RClipboard&);
};

