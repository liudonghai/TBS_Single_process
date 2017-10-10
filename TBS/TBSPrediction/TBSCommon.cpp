#include "stdafx.h"
#include "TBSCommon.h"
#include <assert.h>

INT CTBSCommon::iTBSNum = 0;
INT CTBSCommon::iTBSPresent = 0;
TBS_PRESENT_THREAD_t *CTBSCommon::m_PresentThread=new TBS_PRESENT_THREAD_t[9];

CTBSCommon::CTBSCommon()
{
}

CTBSCommon::~CTBSCommon()
{
}

//�ַ����ָ����
//������
//	in��
//		srcStr����Ҫ�ָ���ַ�����
//		Div���ָ��ַ���
//	out:
//		desStr:�ָ��Ժ���ַ������飻
void CTBSCommon::tbs_com_string_split(string srcStr, CHAR cSplit, string* pDesStr)
{
	UINT iNum	= 0;
	UINT i		= 0;
	UINT uIndex = 0;
	UINT uLong	= 0;
	UINT uLength = srcStr.size();
	while (i <= uLength)
	{
		if (srcStr[i] != cSplit)
		{
			uLong++;
			if (i == uLength)
			{
				*pDesStr = srcStr.substr(uIndex, uLong);
				return;
			}
			i++;
		}
		else
		{
			if (0 != uLong)
			{
				*pDesStr = srcStr.substr(uIndex, uLong);
				uIndex += uLong + 1;
				uLong = 0;
				pDesStr++;
				iNum++;
				if (iNum == 3)
					return;
				i++;
			}
		}
	}
}

// �ж��ļ����ļ����Ƿ����
bool CTBSCommon::tbs_is_file_exist(const CString csFilePath)
{
	DWORD dwAttrib = GetFileAttributes(csFilePath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

/*
���ܣ�ɾ���ļ������ļ���
	������
		strPath��·����
		bConfir���Ƿ���Ҫȷ�ϣ�TRUE��Ҫ��
	����ֵ��
		0��ִ�гɹ���
*/

bool CTBSCommon::tbs_com_delete_directory(CString strPath, bool bConfir)
{
	strPath += '\0';
	SHFILEOPSTRUCT  shDelFile;
	memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCT));
	shDelFile.fFlags |= FOF_SILENT;             // ����ʾ����  
	shDelFile.fFlags |= FOF_NOERRORUI;          // �����������Ϣ  
	//shDelFile.fFlags |= FOF_NOCONFIRMATION;   // ֱ��ɾ����������ȷ��  
	//shDelFile.fFlags &= ~FOF_ALLOWUNDO;
	shDelFile.wFunc = FO_DELETE;				// ִ�еĲ���  
	shDelFile.pFrom = strPath;					// �����Ķ���Ҳ����Ŀ¼��ע�⣺�ԡ�\0\0����β��  
	shDelFile.pTo	= NULL;						// ��������ΪNULL 
	if (!bConfir)
	{
		shDelFile.fFlags |= FOF_NOCONFIRMATION; // ֱ��ɾ����������ȷ��
	}
	BOOL bres = SHFileOperation(&shDelFile);    //ɾ��  
	return !bres;
}

/*
�������ܣ��жϸ���·�����ļ������ļ����Ƿ����

������
	���룺
		cstrPath:�ļ�·��
	����ֵ��
		�����ڣ�0;
		���������ļ���1��
		��������Ŀ¼��2.
*/
INT CTBSCommon::tbs_file_check(const CString cstrPath)
{
	assert(!cstrPath.IsEmpty());
	INT iResult = 0;
	BOOL bResult;
	CFileFind m_FileFind;

	bResult = m_FileFind.FindFile(cstrPath);
	if (bResult)
	{
		m_FileFind.FindNextFileW();
		if (m_FileFind.IsDirectory())
		{
			iResult = 2;
		}
		else
		{
			iResult = 1;
		}
	}
	else
	{
		iResult = 0;
	}
	m_FileFind.Close();
	return iResult;
}

CString CTBSCommon::tbs_file_code_check(CString cstrFileName)
{
	ifstream m_File(cstrFileName, ios::in | ios::binary);
	unsigned char uiData = 0;
	m_File.read((char*)&uiData, sizeof(uiData));
	INT iCode = uiData << 8;
	m_File.read((char*)&uiData, sizeof(uiData));
	iCode += uiData;

	CString cstrPageCode;

	switch (iCode)
	{
	case 0xfffe:  //65534  
		cstrPageCode = L"Unicode";
		break;
	case 0xfeff://65279  
		cstrPageCode = L"Unicode big endian";
		break;
	case 0xefbb://61371  
		cstrPageCode = L"UTF-8";
		break;
	default:
		cstrPageCode = L"ANSI";
	}
	m_File.close();
	return cstrPageCode;
	
}
CString CTBSCommon::tbs_file_code_check(string strFileName)
{
	ifstream m_File(strFileName, ios::in | ios::binary);
	unsigned char uiData = 0;
	m_File.read((char*)&uiData, sizeof(uiData));
	INT iCode = uiData << 8;
	m_File.read((char*)&uiData, sizeof(uiData));
	iCode += uiData;

	CString cstrPageCode;

	switch (iCode)
	{
	case 0xfffe:  //65534  
		cstrPageCode = L"Unicode";
		break;
	case 0xfeff://65279  
		cstrPageCode = L"Unicode big endian";
		break;
	case 0xefbb://61371  
		cstrPageCode = L"UTF-8";
		break;
	default:
		cstrPageCode = L"ANSI";
	}
	m_File.close();
	return cstrPageCode;
	
}

void CTBSCommon::ConvertUTF8ToANSI(CHAR* pUTF8, CHAR **pANSI)
{
	int iLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pUTF8, -1, NULL, 0);
	
	//������Ҫ��unicode����     
	WCHAR * wszANSI = new WCHAR[iLen];
	//memset(wszANSI, 0, iLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, wszANSI, iLen); //��utf8ת��unicode    

	iLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);//�õ�Ҫ��ansi����     
	CHAR *szANSI = new CHAR[iLen + 1];
	memset(szANSI, 0, iLen + 1);
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, iLen, NULL, NULL);//��unicodeת��ansi     
	*pANSI = szANSI;
	delete wszANSI;
	//delete szANSI;
}