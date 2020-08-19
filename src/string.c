#include <Uefi.h>
#include <Protocol/UnicodeCollation.h>
#include "efimain.h"

void FillMemory(VOID* Buffer,UINT8 Value,UINTN Size)
{
	return EfiBoot->SetMem(Buffer,Size,Value);
}

void StringAnsiToUnicode(CHAR16 *StrDest,CHAR8 *StrSrc,UINTN Limit)
{
	return UnicodeCollation->FatToStr(UnicodeCollation,Limit,StrSrc,StrDest);
}

// Unicode-String Essential Functions with Limiter
CHAR16* StringCopyNW(CHAR16* dest,CHAR16* src,UINTN limit)
{
	for(UINTN i=0;i<limit;i++)
	{
		dest[i]=src[i];
		if(src[i]==0)break;
	}
	return dest;
}

// Unicode-String Essential Functions
UINTN StringLengthW(CHAR16* String)
{
	UINTN i=0;
	while(String[i++]);
	return i-1;
}

void StringReverseW(CHAR16* String)
{
	CHAR16* left=String;
	CHAR16 c;
	while(*String++);
	String-=2;
	while(left<String)
	{
		c=*left;
		*left++=*String;
		*String--=c;
	}
}

INT32 StringCompareW(CHAR16* String1,CHAR16* String2)
{
	UINTN i=0;
	while(String1[i] || String2[i])
	{
		if(String1[i]>String2[i])
			return 1;
		else if(String1[i]<String2[i])
			return -1;
		else
			i++;
	}
	return 0;
}

// Integer to String Functions
void IntegerToDecimalW(INT32 Value,CHAR16* String)
{
	INT32 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=L'0';
	for(;q;q/=10)
	{
		INT32 r=q%10;
		String[i++]=r+L'0';
	}
	String[i]=0;
	StringReverseW(String);
}

void IntegerToDecimalW64(INT64 Value,CHAR16* String)
{
	INT32 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=L'0';
	for(;q;q/=10)
	{
		INT32 r=q%10;
		String[i++]=r+L'0';
	}
	String[i]=0;
	StringReverseW(String);
}

void IntegerToHexW(INT32 Value,CHAR16* String)
{
	CHAR16 hp[16]={L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'a',L'b',L'c',L'd',L'e',L'f'};
	INT32 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=hp[0];
	for(;q;q>>=4)
	{
		INT32 r=q&15;
		String[i++]=hp[r];
	}
	String[i]=0;
	StringReverseW(String);
}

void IntegerToHexW64(INT64 Value,CHAR16* String)
{
	CHAR16 hp[16]={L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'a',L'b',L'c',L'd',L'e',L'f'};
	INT32 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=hp[0];
	for(;q;q>>=4)
	{
		INT32 r=q&15;
		String[i++]=hp[r];
	}
	String[i]=0;
	StringReverseW(String);
}

void IntegerToHEXW(INT32 Value,CHAR16* String)
{
	CHAR16 hp[16]={L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'A',L'B',L'C',L'D',L'E',L'F'};
	INT32 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=hp[0];
	for(;q;q>>=4)
	{
		INT32 r=q&15;
		String[i++]=hp[r];
	}
	String[i]=0;
	StringReverseW(String);
}

void IntegerToHEXW64(INT64 Value,CHAR16* String)
{
	CHAR16 hp[16]={L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'A',L'B',L'C',L'D',L'E',L'F'};
	INT64 q=Value;
	INT32 i=0;
	if(q==0)String[i++]=hp[0];
	for(;q;q>>=4)
	{
		INT32 r=q&15;
		String[i++]=hp[r];
	}
	String[i]=0;
	StringReverseW(String);
}