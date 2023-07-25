#pragma once

#include <iostream>
#include <assert.h>

#ifdef _DEBUG
#pragma comment(lib,"./Library/Debug/DataStructure.lib")
#pragma comment(lib,"./HeukKellLibrary/Debug/HeukKellLibrary.lib")
#else
#pragma comment(lib,"./Library/Release/DataStructure.lib")
#pragma comment(lib,"./HeukKellLibrary/Release/HeukKellLibrary.lib")
#endif

