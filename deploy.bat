SET CMAKE_C_COMPILER="C:\Qt\Tools\mingw1120_64\bin\gcc.exe"
SET QTDIR="C:\Qt\6.2.4\mingw_64"
SET PROJECT_SRC_DIR="D:\Workspace\CpuMonitoringServer"
SET PROJECT_BIN_DIR="D:\Workspace\build-CpuMonitoringServer-Desktop_Qt_6_2_4_MinGW_64_bit-Release"

"%QTDIR%\bin\windeployqt.exe" --qmldir %PROJECT_SRC_DIR% --no-compiler-runtime %PROJECT_BIN_DIR% 