#include <Uefi.h>
#include <Protocol/UnicodeCollation.h>
#include <stdarg.h>
#include "efimain.h"

int StringVPrintfW(CHAR16* Output,UINTN Limit,CHAR16* Format,va_list ArgList)
{
	CHAR16* buf=Output;
	UINTN i=0,j=0;
	FillMemory(buf,0,Limit<<1);
	for(;Format[i];i++)
	{
		if(Format[i]==L'%')
		{
			UINTN inc=1;
			switch(Format[i+1])
			{
				case L'c':
				{
					CHAR8 c=va_arg(ArgList,CHAR8);
					buf[j]=(CHAR16)c;
					inc=1;
					break;
				}
				case L'd':
				{
					int n=va_arg(ArgList,int);
					CHAR16 s[16];
					IntegerToDecimalW(n,s);
					inc=StringLengthW(s);
					StringCopyNW(&buf[j],s,Limit-j);
					i++;
					break;
				}
				case L'l':
				{
					break;
				}
				case L's':
				{
					CHAR8* s=va_arg(ArgList,CHAR8*);
					StringAnsiToUnicode(&buf[j],s,Limit-j);
					inc=StringLengthW(&buf[j]);
					i++;
					break;
				}
				case L'w':
				{
					if(Format[i+2]==L'c')
					{
						CHAR16 c=va_arg(ArgList,CHAR16);
						inc=1;
						buf[j]=c;
						i+=2;
					}
					else if(Format[i+2]==L's')
					{
						CHAR16* s=va_arg(ArgList,CHAR16*);
						inc=StringLengthW(s);
						StringCopyNW(&buf[j],s,Limit-j);
						i+=2;
					}
					else
					{
						inc=2;
						buf[j]=L'%';
						buf[j+1]=L'w';
						i+=2;
					}
					break;
				}
				case L'x':
				{
					int n=va_arg(ArgList,int);
					CHAR16 s[16];
					IntegerToHexW(n,s);
					inc=StringLengthW(s);
					StringCopyNW(&buf[j],s,Limit-j);
					i++;
					break;
				}
				case L'X':
				{
					int n=va_arg(ArgList,int);
					CHAR16 s[16];
					IntegerToHEXW(n,s);
					inc=StringLengthW(s);
					StringCopyNW(&buf[j],s,Limit-j);
					i++;
					break;
				}
				default:
				{
					buf[j]=L'%';
					break;
				}
			}
			j+=inc;
		}
		else
			buf[j++]=Format[i];
		if(j>=Limit)
		{
			j=Limit;
			break;
		}
	}
	return (int)j;
}

void __cdecl ConsolePrintfW(CHAR16* Format,...)
{
	CHAR16 buf[512];
	int len=0;
	va_list ArgList;
	va_start(ArgList,Format);
	len=StringVPrintfW(buf,512,Format,ArgList);
	va_end(ArgList);
	StdOut->OutputString(StdOut,buf);
}