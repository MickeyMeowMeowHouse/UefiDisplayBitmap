@echo off
set edkpath=C:\UefiDKII
set mdepath=C:\UefiDKII\MdePkg
set binpath=.\compfre_uefix64
set objpath=.\compfre_uefix64\Intermediate

title Compiling Hello UEFI Bitmap, Checked Build, UEFI (AMD64 Architecture)
echo Project: Hello UEFI Bitmap
echo Platform: Unified Extensible Firmware Interface
echo Preset: Release/Free Build
echo Powered by zero.tangptr@gmail.com
clang-cl --version
lld-link --version
pause

echo ============Start Compiling============
echo Compiling UEFI Booting Facility...
clang-cl .\efimain.c /I"%mdepath%\Include" /I"%mdepath%\Include\X64" /D"_EFI_BOOT" /Zi /W3 /WX /O2 /Oy- /Fa"%objpath%\efimain.cod" /Fo"%objpath%\efimain.obj" /GS- /Gy /Gr /TC /c

clang-cl .\format.c /I"%mdepath%\Include" /I"%mdepath%\Include\X64" /Zi /W3 /WX /O2 /Oy- /Fa"%objpath%\format.cod" /Fo"%objpath%\format.obj" /GS- /Gy /Gr /TC /c -Wno-varargs

clang-cl .\string.c /I"%mdepath%\Include" /I"%mdepath%\Include\X64" /Zi /W3 /WX /O2 /Oy- /Fa"%objpath%\string.cod" /Fo"%objpath%\string.obj" /GS- /Gy /Gr /TC /c

echo ============Start Linking============
echo Linking NoirVisor EFI Loader Application...
lld-link "%objpath%\efimain.obj" "%objpath%\format.obj" "%objpath%\string.obj" ".\bitmap.res" /NODEFAULTLIB /OPT:REF /OUT:"%binpath%\bootx64.efi" /SUBSYSTEM:EFI_APPLICATION /ENTRY:"EfiMain" /DEBUG /PDB:"%objpath%\bootx64.pdb" /Machine:X64

echo Completed!
pause.