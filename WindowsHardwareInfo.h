#ifndef WINDOWSHARDWAREINFO_H
#define WINDOWSHARDWAREINFO_H

#include <QString>

//#include <comdef.h>
#include <WbemIdl.h>

#include <wincred.h>
#include <strsafe.h>

class WindowsHardwareInfo
{

public:
    WindowsHardwareInfo();

    ~WindowsHardwareInfo();

    // Copy Constructor (Do deep copy)
    WindowsHardwareInfo(const WindowsHardwareInfo &windowsHardwareInfo);

    // Copy assignment (Do deep copy)
    WindowsHardwareInfo& operator=(const WindowsHardwareInfo& windowsHardwareInfo) = delete;

    int getCpuTemperature();

private:

    bool init();

    bool queryOnBoardCpuInfo(HRESULT hres, IWbemServices *pSvc, int& infoToReturn) const;

private:
    HRESULT         m_hres;
    IWbemLocator    *m_pLoc;
    IWbemServices   *m_pSvc;

    bool            m_initSucceeded;

};

#endif // WINDOWSHARDWAREINFO_H
