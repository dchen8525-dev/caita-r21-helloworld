// HelloWorldAddin.h
// CATIA CAA Hello World Plugin - Workbench Addin

#ifndef __HelloWorldAddin_h__
#define __HelloWorldAddin_h__

#include "CATBaseUnknown.h"
#include "CATIAfrGeneralWksAddin.h"

class CATCmdContainer;

/**
 * @brief CATIA Workbench Addin implementation
 * 
 * This class implements CATIAfrGeneralWksAddin interface to register
 * our Hello World command into CATIA general workshop.
 */
class HelloWorldAddin : public CATBaseUnknown
{
    CATDeclareClass;

public:
    HelloWorldAddin();
    virtual ~HelloWorldAddin();

    // CATIAfrGeneralWksAddin interface implementation
    virtual void CreateCommands();
    virtual CATCmdContainer* CreateToolbars();
};

#endif // __HelloWorldAddin_h__
