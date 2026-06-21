// CATIA CAA Hello World Plugin - Module Entry Point
// This file contains the module initialization code

#include "HelloWorldLog.h"

class HelloWorldModuleTrace
{
public:
    HelloWorldModuleTrace()
    {
        HelloWorldLog("MODULE", "HelloWorldModule.dll loaded");
    }

    ~HelloWorldModuleTrace()
    {
        HelloWorldLog("MODULE", "HelloWorldModule.dll unloaded");
    }
};

static HelloWorldModuleTrace g_HelloWorldModuleTrace;
