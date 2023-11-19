#include "glassbox.hpp"

int getgpu();
int getMemory();
int getdisks();
int getWinVersion();
int getMotherBoard();

static std::string formfactor(UINT16 type);

int main() {
	getMemory();
}

int getMemory() {
	HRESULT hres;

	std::cout << "RAM info: \n\n" << std::endl;

	hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		warn("failed to connect to the conn");
		std::cout << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}
	hres = CoInitializeSecurity(
		nullptr,
		-1,
		nullptr,
		nullptr,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		nullptr,
		EOAC_NONE,
		nullptr
	);
	if (FAILED(hres))
	{
		warn("failed to set security atributes on the conn");
		std::cout << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}
	IWbemLocator* pLoc = nullptr;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID *) &pLoc
	);
	if (FAILED(hres))
	{
		warn("failed to create the webm instance");
		std::cout << GetLastError() << std::endl;
		CoUninitialize();
		return EXIT_FAILURE;
	}
	IWbemServices* pSvc = nullptr;

	hres = pLoc->ConnectServer(
		BSTR(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc);
	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		warn("failed to connect to proxy");
		std::cout << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}
	okay("connected to wmi");

	hres = CoSetProxyBlanket(pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		warn("failed to set proxy blanket");
		std::cout << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}
	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PhysicalMemory"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator
	);
	if (FAILED(hres))
	{
		warn("failed to query physical memory information");
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		std::cout << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}
	IWbemClassObject* pqueryObj = nullptr;
	ULONG enReturn = NULL;
	std::vector<UINT> memspeed;
	int i = 1;
	while (pEnumerator) {
		hres = pEnumerator->Next(WBEM_INFINITE, 1, &pqueryObj, &enReturn);
			 
		if (enReturn == 0) {
			break;
		}
		std::cout << "Ram Stick " << i << " info: \n" << std::endl;
		VARIANT vtProp;
		hres = pqueryObj->Get(L"Speed", 0, &vtProp, 0, 0);
		wprintf(L"RAM Speed: %u MHz\n", vtProp.uintVal);
		memspeed.push_back(vtProp.uintVal);
		VariantClear(&vtProp);
		hres = pqueryObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		wprintf(L"RAM Manufacterer: %s\n", vtProp.bstrVal);
		VariantClear(&vtProp);
		hres = pqueryObj->Get(L"FormFactor", 0, &vtProp, 0, 0);
		std::string formFactor = formfactor(vtProp.uintVal);
		std::cout << "Memory Form Factor: " << formFactor << "\n" << std::endl;
		pqueryObj->Release();
		i++;
	}

	std::cout << "\nShared Information: \n" << std::endl;

	//query size with winapi (its quicker)
	MEMORYSTATUSEX membuffer;
	membuffer.dwLength = sizeof(membuffer);
	BOOL mres = GlobalMemoryStatusEx(&membuffer);

	DWORDLONG size = membuffer.ullTotalPhys;
	std::cout << "the total amount of memory is:" << size / (1024 * 1024 * 1024) << " GB" << std::endl;
	//query current memory usage winapi (quicker)
	DWORD usage = membuffer.dwMemoryLoad;
	std::cout << "memory usage is: " << usage << "%" << std::endl;
	float percent = (float)usage/(float)100;
	float inUse = (float)size * percent;
	std::cout << "current memory in use is: " << std::round(inUse / (1024 * 1024)) << "MB" << std::endl;
	// Cleanup
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();
}

static std::string formfactor(UINT16 type) {
	switch ((int)type) 
	{
	case 0:
		return "Unknown";
		break;
	case 1:
		return "Other";
		break;
	case 2:
		return "SIP";
		break;
	case 3:
		return "DIP";
		break;
	case 4:
		return "ZIP";
		break;
	case 5:
		return "SOJ";
		break;
	case 6:
		return "Proprietary";
		break;
	case 7:
		return "SIMM";
		break;
	case 8:
		return "DIMM";
		break;
	case 9:
		return "TSOP";
		break;
	case 10:
		return "PGA";
		break;
	case 11:
		return "RIMM";
		break;
	case 12:
		return "SODIMM";
		break;
	case 13:
		return "SRIMM";
		break;
	case 14:
		return "SMD";
		break;
	case 15:
		return "SSMP";
		break;
	case 16:
		return "QFP";
		break;
	case 17:
		return "TQFP";
		break;
	case 18:
		return "SOIC";
		break;
	case 19:
		return "LCC";
		break;
	case 20:
		return "PLCC";
		break;
	case 21:
		return "BGA";
		break;
	case 22:
		return "FPBGA";
		break;
	case 23:
		return "LGA";
		break;
	default:
		return "ERROR";
		break;
	};


}