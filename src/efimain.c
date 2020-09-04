#include <Uefi.h>
#include <IndustryStandard/PeImage.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/UnicodeCollation.h>
#include "efimain.h"

EFI_STATUS ConsoleOutput(IN CHAR16 *String)
{
	return StdOut->OutputString(StdOut,String);
}

EFI_STATUS FindBitmapFromImage(IN EFI_HANDLE ImageHandle,IN UINT32 LangID,IN UINT32 BitmapID,OUT PBITMAPINFOHEADER *BitmapData)
{
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage=NULL;
	EFI_STATUS st=gBS->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(VOID**)&LoadedImage);
	if(st==EFI_SUCCESS)
	{
		EFI_IMAGE_DOS_HEADER *DosHead=(EFI_IMAGE_DOS_HEADER*)LoadedImage->ImageBase;
		st=EFI_INVALID_PARAMETER;
		if(DosHead->e_magic==EFI_IMAGE_DOS_SIGNATURE)
		{
#if defined(_EFI32)
			EFI_IMAGE_NT_HEADERS32 *NtHead=(EFI_IMAGE_NT_HEADERS32*)((UINTN)DosHead+DosHead->e_lfanew);
#else
			EFI_IMAGE_NT_HEADERS64 *NtHead=(EFI_IMAGE_NT_HEADERS64*)((UINTN)DosHead+DosHead->e_lfanew);
#endif
			if(NtHead->Signature==EFI_IMAGE_NT_SIGNATURE)
			{
				UINT32 ResDirOffset=NtHead->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
				EFI_IMAGE_RESOURCE_DIRECTORY *ResDir=(EFI_IMAGE_RESOURCE_DIRECTORY*)((UINTN)DosHead+ResDirOffset);
				EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY *ResEntry=(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINTN)ResDir+sizeof(EFI_IMAGE_RESOURCE_DIRECTORY));
				st=EFI_NOT_FOUND;
				for(UINT16 i=0;i<ResDir->NumberOfNamedEntries+ResDir->NumberOfIdEntries;i++)
				{
					if(ResEntry[i].u1.Id==RT_BITMAP && ResEntry[i].u2.s.DataIsDirectory)
					{
						EFI_IMAGE_RESOURCE_DIRECTORY *ImgDir=(EFI_IMAGE_RESOURCE_DIRECTORY*)((UINTN)ResDir+ResEntry[i].u2.s.OffsetToDirectory);
						EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY *ImgList=(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINTN)ImgDir+sizeof(EFI_IMAGE_RESOURCE_DIRECTORY));
						for(UINT16 j=0;j<ImgDir->NumberOfNamedEntries+ImgDir->NumberOfIdEntries;j++)
						{
							if(ImgList[j].u1.Id==BitmapID && ImgList[i].u2.s.DataIsDirectory)
							{
								EFI_IMAGE_RESOURCE_DIRECTORY *LangDir=(EFI_IMAGE_RESOURCE_DIRECTORY*)((UINTN)ResDir+ImgList[j].u2.s.OffsetToDirectory);
								EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY *LangList=(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINTN)LangDir+sizeof(EFI_IMAGE_RESOURCE_DIRECTORY));
								for(UINT16 k=0;k<LangDir->NumberOfNamedEntries+LangDir->NumberOfIdEntries;k++)
								{
									if(LangList[k].u1.Id==LangID)
									{
										EFI_IMAGE_RESOURCE_DATA_ENTRY *ImageEntry=(EFI_IMAGE_RESOURCE_DATA_ENTRY*)((UINTN)ResDir+LangList[k].u2.OffsetToData);
										*BitmapData=(PBITMAPINFOHEADER)((UINTN)DosHead+ImageEntry->OffsetToData);
										return EFI_SUCCESS;
									}
								}
								if(LangID)break;
							}
						}
					}
				}
			}
		}
	}
	return st;
}

UINT32 ChooseGraphicMode(IN UINT32 Horizontal,IN UINT32 Vertical)
{
	UINT32 Index=0xFFFFFFFF;
	for(UINT32 i=0;i<GraphicsOutput->Mode->MaxMode;i++)
	{
		UINTN InfoSize=0;
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo=NULL;
		EFI_STATUS st=GraphicsOutput->QueryMode(GraphicsOutput,i,&InfoSize,&ModeInfo);
		if(st==EFI_SUCCESS)
		{
			Print(L"Mode=%d Resolution: %dx%d\r\n",i,ModeInfo->HorizontalResolution,ModeInfo->VerticalResolution);
			if(Horizontal==ModeInfo->HorizontalResolution && Vertical==ModeInfo->VerticalResolution)Index=i;
		}
	}
	return Index;
}

void BlockUntilKeyStroke(IN CHAR16 Unicode)
{
	EFI_INPUT_KEY InKey;
	do
	{
		UINTN fi=0;
		gBS->WaitForEvent(1,&StdIn->WaitForKey,&fi);
		StdIn->ReadKeyStroke(StdIn,&InKey);
	}while(InKey.UnicodeChar!=Unicode);
}

void SetBiggestConsole()
{
	UINTN MaxRow=0,MaxCol=0;
	UINT32 MaxIndex=0;
	for(UINT32 i=0;i<StdOut->Mode->MaxMode;i++)
	{
		UINTN Row=0,Col=0;
		StdOut->QueryMode(StdOut,i,&Col,&Row);
		if(Row>MaxRow)
		{
			MaxCol=Col;
			MaxRow=Row;
			MaxIndex=i;
		}
		else if(Row==MaxRow && Col>MaxCol)
		{
			MaxCol=Col;
			MaxIndex=i;
		}
	}
	StdOut->SetAttribute(StdOut,EFI_WHITE);
	StdOut->SetMode(StdOut,MaxIndex);
	Print(L"Console Resolution: %dx%d\r\n",MaxCol,MaxRow);
}

EFI_STATUS EfiInit(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS st=EFI_SUCCESS;
	UefiBootServicesTableLibConstructor(ImageHandle,SystemTable);
	UefiRuntimeServicesTableLibConstructor(ImageHandle,SystemTable);
	UefiLibConstructor(ImageHandle,SystemTable);
	DevicePathLibConstructor(ImageHandle,SystemTable);
	st=gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID**)&GraphicsOutput);
	StdIn=SystemTable->ConIn;
	StdOut=SystemTable->ConOut;
	SetBiggestConsole();
	return st;
}

EFI_STATUS EFIAPI EfiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	// Initialize Protocols and Variables we need.
	EFI_STATUS st=EfiInit(ImageHandle,SystemTable);
	if(st==EFI_SUCCESS)
	{
		PBITMAPINFOHEADER BmpData=NULL;
		// Locate the Bitmap from PE Resource Table.
		st=FindBitmapFromImage(ImageHandle,LANGID_CHINESE_SIMPLIFIED,IDB_BITMAP1,&BmpData);
		if(st==EFI_SUCCESS)
		{
			// Choose the Graphics Resolution Mode we need.
			UINT32 Mode=ChooseGraphicMode(BmpData->PixelWidth,BmpData->PixelHeight);
			Print(L"Located Bitmap! Image Size: %dx%d\r\n",BmpData->PixelWidth,BmpData->PixelHeight);
			// We don't support BMP Compression.
			if(BmpData->CompressionType)
			{
				Print(L"Bitmap Compression Algorithm is unsupported!\r\n");
				goto Error;
			}
			// We support 24-bit True-Color Bitmap only.
			if(BmpData->BitPerPixel!=24)
			{
				Print(L"Only 24-bit True-Color Format Bitmap is supported!\r\n");
				goto Error;
			}
			// Check if our mode is supported.
			if(Mode!=0xFFFFFFFF)
				Print(L"We will use Graphics Output Mode %d!\r\n",Mode);
			else
			{
				Print(L"Your Graphics Adapter does not support the required resolution!\r\n");
Error:
				Print(L"Press Enter key to continue...\r\n");
				BlockUntilKeyStroke(L'\r');
				return EFI_UNSUPPORTED;
			}
			// Block the console here because we, as users, might want to check what have right now.
			Print(L"Press Enter key to continue...\r\n");
			BlockUntilKeyStroke(L'\r');
			// Set the resolution mode. Screen will be cleared simultaneously.
			st=GraphicsOutput->SetMode(GraphicsOutput,Mode);
			if(st==EFI_SUCCESS)
			{
				// Allocate the BLT Buffer we need.
				EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer=NULL;
				st=gBS->AllocatePool(EfiLoaderData,BmpData->PixelWidth*BmpData->PixelHeight*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),(VOID**)&BltBuffer);
				if(st==EFI_SUCCESS)
				{
					// Get pointer of the Pixel Array.
					UINT8 *BmpPixels=(UINT8*)((UINTN)BmpData+BmpData->HeaderSize);
					// Traverse the Pixel Rows.
					for(UINT32 i=BmpData->PixelHeight-1;i>0;i--)
					{
						UINT32 j=BmpData->PixelHeight-i-1;
						// Calculate the pointer of Row Pixel Array.
						// Note that Row Pixel Array is aligned at 4-byte boundary.
						// The summation guarantees the alignment. Don't know why? Learn Mathematics!
						UINT8 *HoriPixels=(UINT8*)((UINTN)BmpPixels+j*3*BmpData->PixelWidth+(BmpData->PixelWidth&3));
						// Traverse the Pixels in a Row.
						for(UINT32 m=0;m<BmpData->PixelWidth;m++)
						{
							// Locate the Pixel.
							UINT32 n=i*BmpData->PixelWidth+m;
							UINT32 k=m*3;
							// Copy the Pixel onto BLT Buffer.
							BltBuffer[n].Blue=HoriPixels[k];
							BltBuffer[n].Green=HoriPixels[k+1];
							BltBuffer[n].Red=HoriPixels[k+2];
							BltBuffer[n].Reserved=0;
						}
					}
					// Preparation of BLT Buffer is completed. Perform BLT.
					st=GraphicsOutput->Blt(GraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,0,0,BmpData->PixelWidth,BmpData->PixelHeight,0);
					// Free BLT Buffer.
					gBS->FreePool(BltBuffer);
				}
			}
		}
	}
	// Block the system so you can see what's going on.
	BlockUntilKeyStroke(L'\r');
	return st;
}