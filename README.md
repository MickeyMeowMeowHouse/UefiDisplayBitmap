# UefiDisplayBitmap
Simple demo of displaying a bitmap image in UEFI environment.

## Introduction
This project is a simple demo of displaying a bitmap image in UEFI environment.

## Requirement
To run this project, a working computer with UEFI firmware is required.

## Build
You should install LLVM and EDK II in order to build this project. <br>
You may download LLVM from GitHub: https://github.com/llvm/llvm-project/releases <br>
You may download EDK II from GitHub: https://github.com/tianocore/edk2/releases <br>
For LLVM, you need to install Win64 option. Otherwise, you might have to revise the compilation script. <br>
For EDK II, download source code of latest release and extract to path `C:\UefiDKII`. <br>
If it is your first time of compilation, run `build_prep.bat` prior to building it.

In addition, you have to compile [EDK II Libraries](https://github.com/MickeyMeowMeowHouse/EDK-II-Library) prior to your first time compilation, since this project uses some functions from the EDK II Library.

## Test
Setup a USB flash stick with GUID Partition Table (GPT). Construct a partition and format it into FAT32 file system. <br>
Copy the compiled `bootx64.efi` to `\EFI\BOOT\bootx64.efi` in the flash stick. <br>
Enter your firmware settings. Set the device option prior to the operating system. Disable Secure Boot unless you can sign the executable.

## License
This repository is under MIT license.