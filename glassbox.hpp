#include<Windows.h>

#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf("[i] " msg "\n", ##__VA_ARGS__)



typedef struct _VOLUMEINFORMATION {
	LPCWSTR lpRootPathName;
	LPWSTR  lpVolumeNameBuffer;
	DWORD   nVolumeNameSize;
	LPDWORD lpVolumeSerialNumber;
	LPDWORD lpMaximumComponentLength;
	LPDWORD lpFileSystemFlags;
	LPWSTR  lpFileSystemNameBuffer;
	DWORD   nFileSystemNameSize;
} VolumeInformation;