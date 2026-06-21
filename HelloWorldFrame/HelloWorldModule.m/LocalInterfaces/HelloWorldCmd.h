// HelloWorldCmd.h
// CATIA CAA Hello World Command

#ifndef __HelloWorldCmd_h__
#define __HelloWorldCmd_h__

#include "CATCommand.h"

class HelloWorldDlg;

/**
 * @brief Hello World Command
 * 
 * This command is activated when the user clicks the toolbar button.
 * It creates and displays a Hello World dialog.
 */
class HelloWorldCmd : public CATCommand
{
    CATDeclareClass;

public:
    HelloWorldCmd();
    virtual ~HelloWorldCmd();

    // CATCommand overrides
    virtual CATStatusChangeRC Activate(CATCommand* iCmd,
                                       CATNotification* iNotif);
    virtual CATStatusChangeRC Desactivate(CATCommand* iCmd,
                                          CATNotification* iNotif);
    virtual CATStatusChangeRC Cancel(CATCommand* iCmd,
                                     CATNotification* iNotif);

private:
    void CloseDialog(CATCommand* iCmd,
                     CATNotification* iNotif,
                     CATCommandClientData iClientData);

    // Dialog instance
    HelloWorldDlg* m_pDialog;
};

#endif // __HelloWorldCmd_h__
