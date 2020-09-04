@echo off
set edkpath=C:\UefiDKII
set mdepath=C:\UefiDKII\MdePkg
set libpath=C:\UefiDKII\Bin\MdePkg
set binpath=..\bin\compchk_uefix64
set objpath=..\bin\compchk_uefix64\Intermediate

title Compiling Hello UEFI Bitmap, Checked Build, UEFI (AMD64 Architecture)
echo Project: Hello UEFI Bitmap
echo Platform: Unified Extensible Firmware Interface
echo Preset: Debug/Checked Build
echo Powered by zero.tangptr@gmail.com
clang-cl --version
lld-link --version
pause

echo ============Start Compiling============
echo Compiling UEFI Booting Facility...
clang-cl ..\src\efimain.c /I"%mdepath%\Include" /I"%mdepath%\Include\X64" /D"_EFI_BOOT" /Zi /W3 /WX /Od /Oy- /Fa"%objpath%\efimain.cod" /Fo"%objpath%\efimain.obj" /GS- /Gr /TC /c

echo ============Start Linking============
echo Linking NoirVisor EFI Loader Application...
lld-link "%objpath%\efimain.obj" "..\src\bitmap.res" /NODEFAULTLIB /LIBPATH:"%libpath%\compchk_uefix64" "BaseLib.lib" "BaseDebugPrintErrorLevelLib.lib" "BaseMemoryLib.lib" "BasePrintLib.lib" "UefiLib.lib" "UefiDebugLibConOut.lib" "UefiMemoryAllocationLib.lib" "UefiDevicePathLib.Lib" "UefiBootServicesTableLib.Lib" "UefiRuntimeServicesTableLib.Lib" /OUT:"%binpath%\bootx64.efi" /SUBSYSTEM:EFI_APPLICATION /ENTRY:"EfiMain" /DEBUG /PDB:"%objpath%\bootx64.pdb" /Machine:X64

echo Completed!
pause.