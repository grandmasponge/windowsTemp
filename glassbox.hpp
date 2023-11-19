#include<string>
#include<Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "nvapi.h"
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "nvapi64.lib")


#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf("[i] " msg "\n", ##__VA_ARGS__)



