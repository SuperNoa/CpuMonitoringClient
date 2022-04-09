#include "WindowsHardwareInfo.h"

#include <QDebug>

#include "WTypes.h"

/*
 *  Refer to https://docs.microsoft.com/it-it/windows/desktop/WmiSdk/example--getting-wmi-data-from-the-local-computer
 */

WindowsHardwareInfo::WindowsHardwareInfo() :
    m_hres          { 0 },
    m_pLoc          { nullptr },
    m_pSvc          { nullptr },
    m_initSucceeded { false }
{
    m_initSucceeded = init();
}

double WindowsHardwareInfo::getCpuTemperature()
{
    QString infoToReturn;

    queryOnBoardCpuInfo(m_hres, m_pSvc, infoToReturn);
}

WindowsHardwareInfo::~WindowsHardwareInfo()
{

#ifdef _DEBUG
    qDebug() << "Destructor called";
#endif

    // Do cleanup
    // ========
    if(m_pSvc)
        m_pSvc->Release();

    if(m_pLoc)
        m_pLoc->Release();

    // Uninitialize COM.
    CoUninitialize();
}

bool WindowsHardwareInfo::init()
{

#define INITIALIZING_COM {

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    /*
     *  https://docs.microsoft.com/en-us/windows/desktop/learnwin32/initializing-the-com-library
     *
     *  HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
     *  @LPVOID pvReserved  :   The first parameter is reserved and must be NULL.
     *  @ DWORD dwCoInit    :   The second parameter specifies the threading model that your program will use.
     *                           COM supports two different threading models, apartment threaded and multithreaded.
     *                           Generally, a thread that creates a window should use the COINIT_APARTMENTTHREADED flag,
     *                           and other threads should use COINIT_MULTITHREADED
    */
    m_hres = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED );

    if (FAILED(m_hres))
    {
        qDebug() << "Failed to initialize COM library. Error code = 0x" << m_hres;

        return false;
    }

#define INITIALIZING_COM_END }

#define PROCESS_SECURITY_LEVEL {

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------
    m_hres = CoInitializeSecurity(
        nullptr,                     // Security descriptor
        -1,                          // COM negotiates authentication service
        nullptr,                     // Authentication services
        nullptr,                     // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
        nullptr,                     // Authentication info
        EOAC_NONE,                   // Additional capabilities of the client or server
        nullptr);                    // Reserved

    if (FAILED(m_hres))
    {
        qDebug() << "Failed to initialize security. Error code = 0x" << m_hres;

         // Uninitialize COM.
        CoUninitialize();

        return false;
    }

#define PROCESS_SECURITY_LEVEL_END }

#define OBTAIN_INITIAL_LOCATOR_TO_WMI {

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    // Initialize the IWbemLocator interface
    m_hres = CoCreateInstance(
                CLSID_WbemLocator,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator,
                reinterpret_cast<LPVOID *>(&m_pLoc) ); // (LPVOID *) &pLoc)

    if (FAILED(m_hres))
    {
        qDebug() << "Failed to create IWbemLocator object. Err code = 0x" << m_hres;

        // Uninitialize COM.
        CoUninitialize();

        return false;
    }

#define OBTAIN_INITIAL_LOCATOR_TO_WMI_END }

#define CONNECT_TO_WMI {

    // Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    // Connect to the root\cimv2 namespace with the current user and obtain pointer pSvc
    // to make IWbemServices calls. ROOT\\DEFAULT
    m_hres = m_pLoc->ConnectServer(
            BSTR(L"ROOT\\cimv2"),   // Object path of WMI namespace
            nullptr,                // User name. NULL = current user
            nullptr,                // User password. NULL = current
            nullptr,                // Locale. NULL indicates current
            NULL,                   // Security flags.
            nullptr,                // Authority (for example, Kerberos).
            nullptr,                // Context object
            &m_pSvc);               // pointer to IWbemServices proxy

    if (FAILED(m_hres))
    {
        qDebug() << "Could not connect. Error code = 0x" << m_hres;

        // Clean IWbemLocator
        m_pLoc->Release();

        // Uninitialize COM.
        CoUninitialize();

        return false;
    }

#ifdef _DEBUG
    qDebug() << "Connected to ROOT\\CIMV2 WMI namespace" << endl;
#endif

#define CONNECT_TO_WMI_END }

#define SETTING_SECURITY_LEVELS_ON_THE_PROXY {

    // Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------

    // Set the proxy so that impersonation of the client occurs.
    m_hres = CoSetProxyBlanket(m_pSvc,
       RPC_C_AUTHN_WINNT,
       RPC_C_AUTHZ_NONE,
       nullptr,
       RPC_C_AUTHN_LEVEL_CALL,
       RPC_C_IMP_LEVEL_IMPERSONATE,
       nullptr,
       EOAC_NONE
    );

    if (FAILED(m_hres))
    {
        qDebug() << "Could not set proxy blanket. Error code = 0x" << m_hres;

        // Clean IWbemServices proxy
        m_pSvc->Release();

        // Clean IWbemLocator
        m_pLoc->Release();

        // Uninitialize COM.
        CoUninitialize();

        return false;
    }

#define SETTING_SECURITY_LEVELS_ON_THE_PROXY_END }

    return true;
}

bool WindowsHardwareInfo::queryOnBoardCpuInfo(HRESULT hres, IWbemServices *pSvc, QString& infoToReturn) const
{
    // Step 6: --------------------------------------------------
    // Use the IWbemServices pointer to make requests of WMI ----

    IEnumWbemClassObject* pEnumerator = nullptr;

    BSTR bstr_wql = SysAllocString(L"WQL");
    BSTR bstr_sql = SysAllocString(L"SELECT * FROM Win32_Processor");

    hres = pSvc->ExecQuery(
        bstr_wql,
        bstr_sql,
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &pEnumerator);


    // Step 7: -------------------------------------------------
    // Get the data from the query in step 6 -------------------

    IWbemClassObject *pclsObj = nullptr;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }

        VARIANT vtUUID;

        // Get the value of the Name property
        hr = pclsObj->Get(L"Manufacturer", 0, &vtUUID, nullptr, nullptr);

        // BSTR => wchar_t*
        infoToReturn = QString::fromWCharArray( vtUUID.bstrVal );

        VariantClear(&vtUUID);

        pclsObj->Release();
    }

    pEnumerator->Release();

    return true;
}

