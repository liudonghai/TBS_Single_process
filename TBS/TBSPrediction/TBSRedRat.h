#pragma once
#pragma managed
#using <mscorlib.dll>
#include <string>
#using "RedRat.dll" 
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
ref class CTBSRedRat
{
public:
	CTBSRedRat();
	~CTBSRedRat();
public:
	static INT tbs_ir_get_info(CString *cstrName);
	static VOID tbs_ir_send_key(CString  cstrRedRat, CHAR* pKeyFile, CHAR* pKeyName);
};

