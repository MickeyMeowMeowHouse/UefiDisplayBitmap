//

#include <Uefi.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePathUtilities.h>
#include <Protocol/HiiFont.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/UgaDraw.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/GlobalVariable.h>
#include <Library/UefiLib.h>

#define RT_BITMAP	2
#define IDB_BITMAP1	102
#define LANGID_CHINESE_SIMPLIFIED	2052

EFI_STATUS EFIAPI UefiBootServicesTableLibConstructor(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS EFIAPI UefiRuntimeServicesTableLibConstructor(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS EFIAPI UefiLibConstructor(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS EFIAPI DevicePathLibConstructor(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable);

extern EFI_BOOT_SERVICES *gBS;

EFI_SIMPLE_TEXT_INPUT_PROTOCOL *StdIn=NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdOut=NULL;
EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput=NULL;

EFI_GUID gEfiSimpleFileSystemProtocolGuid=EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID gEfiGraphicsOutputProtocolGuid=EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
EFI_GUID gEfiUgaDrawProtocolGuid=EFI_UGA_DRAW_PROTOCOL_GUID;
EFI_GUID gEfiHiiFontProtocolGuid=EFI_HII_FONT_PROTOCOL_GUID;
EFI_GUID gEfiGlobalVariableGuid=EFI_GLOBAL_VARIABLE;
EFI_GUID gEfiSimpleTextOutProtocolGuid=EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID;
EFI_GUID gEfiDevicePathProtocolGuid=EFI_DEVICE_PATH_PROTOCOL_GUID;
EFI_GUID gEfiDevicePathUtilitiesProtocolGuid=EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
EFI_GUID gEfiDevicePathToTextProtocolGuid=EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
EFI_GUID gEfiDevicePathFromTextProtocolGuid=EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
EFI_GUID gEfiLoadedImageProtocolGuid=EFI_LOADED_IMAGE_PROTOCOL_GUID;

typedef struct tagBITMAPINFOHEADER
{
	UINT32 HeaderSize;
	UINT32 PixelWidth;
	UINT32 PixelHeight;
	UINT16 Planes;          ///< Must be 1
	UINT16 BitPerPixel;     ///< 1, 4, 8, or 24
	UINT32 CompressionType;
	UINT32 ImageSize;       ///< Compressed image size in bytes
	UINT32 XPixelsPerMeter;
	UINT32 YPixelsPerMeter;
	UINT32 NumberOfColors;
	UINT32 ImportantColors;
}BITMAPINFOHEADER,*PBITMAPINFOHEADER;