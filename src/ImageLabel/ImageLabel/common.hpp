#pragma once
CString UTF8_To_UTF16(const char* source)
{
	unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, source, -1, NULL, NULL);
	if (len == 0)
		return CString(_T(""));
	wchar_t *buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, source, -1, buffer, len);

	CString dest(buffer);
	delete[] buffer;
	return dest;
}