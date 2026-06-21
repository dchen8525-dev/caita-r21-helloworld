// HelloWorldCmd.cpp
// CATIA CAA Hello World Command Implementation

#include "HelloWorldCmd.h"
#include "HelloWorldDlg.h"
#include "HelloWorldLog.h"
#include "CATApplicationFrame.h"
#include "CATCreateExternalObject.h"

// Implement the class
CATImplementClass(HelloWorldCmd,
                  Implementation,
                  CATCommand,
                  CATNull);

// Command headers instantiate commands by class name through this factory.
CATCreateClass(HelloWorldCmd);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
HelloWorldCmd::HelloWorldCmd()
    : CATCommand(NULL, "HelloWorldCmd")
{
    m_pDialog = NULL;
    HelloWorldLog("COMMAND", "constructor this=%p", this);

    // Let the command selector activate this command without replacing the
    // current CATIA workbench command. Undefined commands are ignored by it.
    RequestStatusChange(CATCommandMsgRequestSharedMode);
    HelloWorldLog("COMMAND", "shared mode requested this=%p", this);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
HelloWorldCmd::~HelloWorldCmd()
{
    HelloWorldLog("COMMAND", "destructor this=%p dialog=%p", this, m_pDialog);
    if (m_pDialog)
    {
        m_pDialog->RequestDelayedDestruction();
        m_pDialog = NULL;
    }
}

//-----------------------------------------------------------------------------
// Activate - Called when the command is activated (toolbar button clicked)
//-----------------------------------------------------------------------------
CATStatusChangeRC HelloWorldCmd::Activate(CATCommand* iCmd,
                                          CATNotification* iNotif)
{
    HelloWorldLog("COMMAND", "Activate entered this=%p", this);
    CATApplicationFrame* pFrame = CATApplicationFrame::GetFrame();
    if (!pFrame)
    {
        HelloWorldLog("COMMAND", "Activate stopped: application frame is NULL");
        return CATStatusChangeRCCompleted;
    }

    CATDialog* pWindow = pFrame->GetMainWindow();
    if (!pWindow)
    {
        HelloWorldLog("COMMAND", "Activate stopped: main window is NULL");
        return CATStatusChangeRCCompleted;
    }

    // Destroy previous dialog if exists
    if (m_pDialog)
    {
        m_pDialog->RequestDelayedDestruction();
        m_pDialog = NULL;
    }

    // Create and display the dialog
    m_pDialog = new HelloWorldDlg(pWindow, "HelloWorldDialog");
    if (m_pDialog)
    {
        // Shared commands must receive and handle the dialog notifications.
        m_pDialog->SetFather(this);
        AddAnalyseNotificationCB(m_pDialog,
            m_pDialog->GetDiaCLOSENotification(),
            (CATCommandMethod)&HelloWorldCmd::CloseDialog,
            NULL);
        AddAnalyseNotificationCB(m_pDialog,
            m_pDialog->GetWindCloseNotification(),
            (CATCommandMethod)&HelloWorldCmd::CloseDialog,
            NULL);
        m_pDialog->SetVisibility(CATDlgShow);
        HelloWorldLog("COMMAND", "dialog displayed dialog=%p", m_pDialog);
    }
    else
    {
        HelloWorldLog("COMMAND", "dialog allocation returned NULL");
    }

    return CATStatusChangeRCCompleted;
}

void HelloWorldCmd::CloseDialog(CATCommand* iCmd,
                                CATNotification* iNotif,
                                CATCommandClientData iClientData)
{
    HelloWorldLog("COMMAND", "CloseDialog this=%p dialog=%p", this, m_pDialog);
    if (m_pDialog)
        m_pDialog->SetVisibility(CATDlgHide);

    // The command destructor owns the final dialog destruction.
    RequestDelayedDestruction();
}

//-----------------------------------------------------------------------------
// Desactivate - Called when the command is deactivated
//-----------------------------------------------------------------------------
CATStatusChangeRC HelloWorldCmd::Desactivate(CATCommand* iCmd,
                                             CATNotification* iNotif)
{
    HelloWorldLog("COMMAND", "Desactivate this=%p", this);
    return CATStatusChangeRCCompleted;
}

//-----------------------------------------------------------------------------
// Cancel - Called when the command is canceled
//-----------------------------------------------------------------------------
CATStatusChangeRC HelloWorldCmd::Cancel(CATCommand* iCmd,
                                        CATNotification* iNotif)
{
    HelloWorldLog("COMMAND", "Cancel this=%p dialog=%p", this, m_pDialog);
    if (m_pDialog)
    {
        m_pDialog->RequestDelayedDestruction();
        m_pDialog = NULL;
    }

    return CATStatusChangeRCCompleted;
}
