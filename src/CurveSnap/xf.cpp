#include "stdafx.h"
#include "xf.h"

#include <ctime>
#include <io.h>
#include <windows.h>
#include <ShlObj.h>

namespace xf {

std::string GetModuleDir()
{
    char path[MAX_PATH+1];
    GetModuleFileName(NULL,path,MAX_PATH); 
	std::string strPath(path);
	return strPath.substr(0,strPath.rfind('\\') +1); 
}

void CopyToClipboard(const char* lpStr)
{
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();//��ռ��а�
		HANDLE hclip;    //GlobalAlloc�����ڴ�ľ��
		char *buf;     //���ص��ڴ��ַ

    size_t length = strlen(lpStr) + 1;
		hclip=GlobalAlloc(GMEM_MOVEABLE,length);//�ڶ��Ϸ����ƶ���Ŀ���ֽ�
		buf=(char*)GlobalLock(hclip);//���ڴ�����,�����������ڴ��ַ
		strcpy_s(buf, length, lpStr);        //���ַ��������ڴ���
		GlobalUnlock(hclip);     //����
		SetClipboardData(CF_TEXT,hclip);//����а��Ϸ����ڴ��е�����
		CloseClipboard();        //�رռ��а�
	}
}

std::string GetCurTimeStr()
{
    time_t now = time(0);
    //����"YYYY-MM-DD hh:mm:ss"��ʽ���ַ�����
    char s[32];
	tm tmTemp;
	localtime_s(&tmTemp, &now);
	strftime(s, sizeof(s), "%Y.%m.%d_%H%M%S", &tmTemp);
	
	return s;
}

bool PathExist(std::string path)
{
	return _access(path.c_str(), 0) == 0;
}

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}

std::string BrowseForFolder(std::string initDir)
{
    BROWSEINFO bi	= {0};
    bi.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    bi.lpszTitle	= "Select Folder of Images";
	bi.lParam		= (LPARAM)initDir.c_str();
	bi.lpfn			= BrowseCallbackProc;

    LPITEMIDLIST pidl= SHBrowseForFolder( &bi ); 
    if ( pidl != NULL ) {  // else, user canceled
        char szPath[MAX_PATH];
        BOOL fRet= SHGetPathFromIDList( pidl, szPath );
		return szPath;
    }
	else
		return "";

}

template<class T>
std::string GetXYDataString(std::vector<T> x,
                            std::vector<T> y, 
                            std::string separator, 
                            int precision_x, 
                            int precision_y,
                            BOOL fixed_x,
                            BOOL fixed_y) {
	int n = min((int)x.size(), (int)y.size());

	if (n < 1)
		return "";

	std::stringstream ss;

	for (int i=0; i < n; i++)
  {
    if (fixed_x) {
      ss << std::fixed << std::setprecision(precision_x) << x[i];
      ss.unsetf(std::ios_base::floatfield);
    }
    else {
      ss << std::setprecision(precision_x) << x[i];
    }
    ss << separator;

    if (fixed_y) {
      ss << std::fixed << std::setprecision(precision_y) << y[i];
      ss.unsetf(std::ios_base::floatfield);
    }
    else {
      ss << std::setprecision(precision_y) << y[i];
    }

    ss << "\r\n";
	}

	return ss.str();
}

}

