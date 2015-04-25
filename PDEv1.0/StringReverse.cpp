/*
*	CString Óë string »¥×ª
*/

#include "stdafx.h"
#include "StringReverse.h"

void CStringToString(CString& cstr,string& str)
{
#ifdef _UNICODE	
	char *ch = new char[cstr.GetLength()+1];
	WideCharToMultiByte(CP_ACP,0,cstr.GetBuffer(),-1,ch,cstr.GetLength()+1,NULL,NULL);
	str = ch;
#elif

#endif
}