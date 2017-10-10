#include "stdafx.h"
#include "TBSRedRat.h"
#include <string>
#include <list>
#include <tchar.h>
#using <mscorlib.dll>
#include <string>
#using "RedRat.dll" 
#pragma managed
using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Xml::Serialization;
using namespace RedRat;
using namespace RedRat::IR;
using namespace RedRat::USB;
using namespace RedRat::Util;
using namespace RedRat::RedRat3;
using namespace RedRat::RedRat3::USB;
using namespace RedRat::AVDeviceMngmt;
using namespace std;



CTBSRedRat::CTBSRedRat()
{ 
}

CTBSRedRat::~CTBSRedRat()
{
}

// 通过RedRat发送红外键值
VOID CTBSRedRat::tbs_ir_send_key(CString  cstrRedRat, CHAR* pKeyFile, CHAR* pKeyName)
{
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, pKeyFile);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, pKeyName);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrRedRat);

	if(strlen(pKeyFile)==0)
	{
		AfxMessageBox(L"IR数据文件为空，请导入IR数据文件");
		return;
	}
	if (strlen(pKeyName) == 0)
	{
		AfxMessageBox(L"键值为空");
		return;
	}
	if (cstrRedRat == L"")
	{
		AfxMessageBox(L"请指定IR设备");
		return;
	}
	try {
		INT i;
		INT iNum;
		INT iIndex;
		cli::array<RedRat::LocationInfo^> ^rr3Li = RedRat3USBImpl::FindRedRat3s();
		iNum = rr3Li->System::Collections::ICollection::Count;

		for (i = 0; i < iNum; i++)
		{
			IRedRat3	^redRat3;
			redRat3 = (IRedRat3^)rr3Li[i]->GetRedRat();
			USBDeviceInfo^ devInfo = static_cast<USBDeviceInfo^>(redRat3->DeviceInformation);
			if (cstrRedRat == rr3Li[i]->ProductName + "_" + devInfo->SerialNumberAsUint)
			{
				iIndex = i;
				break;
			}
			if (i == iNum - 1)
			{
				AfxMessageBox(L"所选RedRat设备不存在！");
			}
		}

		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iIndex);

		IRedRat3^ redRat3 = (IRedRat3^)rr3Li[iIndex]->GetRedRat();
		redRat3->Blink();
		FileStream^ fs;
		try {
			
			XmlSerializer^ m_XMLser = gcnew XmlSerializer(AVDeviceDB::typeid);
			//fs = gcnew FileStream((gcnew FileInfo(gcnew String(sKeyFile.c_str())))->FullName, FileMode::Open);
			fs = gcnew FileStream((gcnew FileInfo(System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)pKeyFile)))->FullName, FileMode::Open);

			AVDeviceDB^ avDeviceDB = static_cast<AVDeviceDB^>(m_XMLser->Deserialize(fs));
			fs->Close();

			
			IRPacket^ sig = avDeviceDB->GetIRPacket(gcnew String("TBS"), System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)pKeyName));
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
			if (dynamic_cast<ModulatedSignal^>(sig) != nullptr) 
			{
				ModulatedSignal^ modSig = static_cast<ModulatedSignal^>(sig);
				redRat3->OutputModulatedSignal(modSig);
			}
			if (dynamic_cast<DoubleSignal^>(sig) != nullptr) 
			{
				DoubleSignal^ douSig = static_cast<DoubleSignal^>(sig);
				redRat3->OutputModulatedSignal(douSig);
			}
		}
		catch (Exception^ ex) {
			AfxMessageBox(CString(ex->ToString()));
			if (fs)
			{
				fs->Close();
			}
		}
	}
	catch (System::Exception^ ex) {
		AfxMessageBox(CString(ex->ToString()));
	}
}


INT CTBSRedRat::tbs_ir_get_info(CString *pName)
{
	INT			iNum = 0;
	try {
		cli::array<RedRat::LocationInfo^> ^rr3Li = RedRat3USBImpl::FindRedRat3s();
		iNum = rr3Li->System::Collections::ICollection::Count;

		for (INT i = 0; i < iNum; i++)
		{
			IRedRat3	^redRat3;
			redRat3 = (IRedRat3^)rr3Li[i]->GetRedRat();
			USBDeviceInfo^ devInfo = static_cast<USBDeviceInfo^>(redRat3->DeviceInformation);
			pName[i] = rr3Li[i]->ProductName + "_" + devInfo->SerialNumberAsUint;
		}

	}
	catch (System::Exception^ ex) {
		AfxMessageBox(CString(ex->ToString()));
	}
	return iNum;
}