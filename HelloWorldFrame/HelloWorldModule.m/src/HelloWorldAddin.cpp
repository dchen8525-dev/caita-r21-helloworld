// HelloWorldAddin.cpp
// CATIA CAA Hello World Plugin - Workbench Addin Implementation

#include "HelloWorldAddin.h"
#include "HelloWorldLog.h"
#include "TIE_CATIAfrGeneralWksAddin.h"
#include "CATCommandHeader.h"
#include "CATCmdContainer.h"
#include "CATCreateWorkshop.h"

// Implement the class
// General workshop add-ins are data extensions of a late type registered in
// the interface dictionary. They are not standalone implementation classes.
CATImplementClass(HelloWorldAddin,
                  DataExtension,
                  CATBaseUnknown,
                  HelloWorldGeneralWksAddin);

// Expose CATIAfrGeneralWksAddin through the CAA TIE mechanism.  The previous
// custom factory was not queried by the CATIA component runtime.
TIE_CATIAfrGeneralWksAddin(HelloWorldAddin);

MacDeclareHeader(HelloWorldHeader);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
HelloWorldAddin::HelloWorldAddin()
{
    HelloWorldLog("ADDIN", "constructor this=%p", this);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
HelloWorldAddin::~HelloWorldAddin()
{
    HelloWorldLog("ADDIN", "destructor this=%p", this);
}

//-----------------------------------------------------------------------------
// CreateCommands - Instantiate command headers
//-----------------------------------------------------------------------------
void HelloWorldAddin::CreateCommands()
{
    HelloWorldLog("ADDIN", "CreateCommands entered");
    // Create the command header
    // Parameters: HeaderID, LoadName (DLL), ClassName, Argument
    CATCommandHeader* header = new HelloWorldHeader("HelloWorldCmd",
                                                    "HelloWorldModule",
                                                    "HelloWorldCmd",
                                                    (void*) NULL);
    HelloWorldLog("ADDIN", "CreateCommands completed header=%p", header);
}

//-----------------------------------------------------------------------------
// CreateToolbars - Create toolbar and add commands
//-----------------------------------------------------------------------------
CATCmdContainer* HelloWorldAddin::CreateToolbars()
{
    HelloWorldLog("ADDIN", "CreateToolbars entered");
    // Create a new toolbar
    NewAccess(CATCmdContainer, pToolbar, HelloWorldToolbar);

    // Create a starter for the command
    NewAccess(CATCmdStarter, pStarter, HelloWorldStarter);
    
    // Attach the command header to the starter
    SetAccessCommand(pStarter, "HelloWorldCmd");
    
    // Add the starter to the toolbar
    SetAccessChild(pToolbar, pStarter);

    // Register a visible toolbar view in the General workshop.
    AddToolbarView(pToolbar, 1, Right);

    HelloWorldLog("ADDIN", "CreateToolbars completed toolbar=%p starter=%p",
                  pToolbar, pStarter);
    return pToolbar;
}
