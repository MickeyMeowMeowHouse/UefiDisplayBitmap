//

#define RT_BITMAP	2
#define IDB_BITMAP1	102
#define LANGID_CHINESE_SIMPLIFIED	2052

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

void FillMemory(VOID* Buffer,UINT8 Value,UINTN Size);
void StringAnsiToUnicode(CHAR16 *StrDest,CHAR8 *StrSrc,UINTN Limit);
CHAR16* StringCopyNW(CHAR16* dest,CHAR16* src,UINTN limit);
UINTN StringLengthW(CHAR16* String);
void StringReverseW(CHAR16* String);
INT32 StringCompareW(CHAR16* String1,CHAR16* String2);
void IntegerToDecimalW(INT32 Value,CHAR16* String);
void IntegerToDecimalW64(INT64 Value,CHAR16* String);
void IntegerToHexW(INT32 Value,CHAR16* String);
void IntegerToHexW64(INT64 Value,CHAR16* String);
void IntegerToHEXW(INT32 Value,CHAR16* String);
void IntegerToHEXW64(INT64 Value,CHAR16* String);
void __cdecl ConsolePrintfW(CHAR16* Format,...);

#if defined(_EFI_BOOT)
EFI_BOOT_SERVICES* EfiBoot=NULL;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL* StdIn=NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdOut=NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr=NULL;
EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput=NULL;
EFI_UNICODE_COLLATION_PROTOCOL* UnicodeCollation=NULL;

EFI_GUID EfiGraphicsOutputGuid=EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
EFI_GUID EfiLoadedImageGuid=EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID EfiUnicodeCollationGuid=EFI_UNICODE_COLLATION_PROTOCOL_GUID;
#else
extern EFI_BOOT_SERVICES* EfiBoot;
extern EFI_SIMPLE_TEXT_INPUT_PROTOCOL* StdIn;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdOut;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
extern EFI_UNICODE_COLLATION_PROTOCOL* UnicodeCollation;
#endif