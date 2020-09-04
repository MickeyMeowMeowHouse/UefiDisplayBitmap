@echo off

title Project: Hello UEFI Bitmap, Cleanup, UEFI (AMD64 Architecture)
echo Project: Hello UEFI Bitmap
echo Platform: Unified Extensible Firmware Interface
echo Preset: Cleanup
echo Powered by zero.tangptr@gmail.com

del ..\bin\compchk_uefix64 /q /s
del ..\bin\compfre_uefix64 /q /s

echo Completed!
pause.