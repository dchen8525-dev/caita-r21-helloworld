// HelloWorldDlg.cpp
// CATIA CAA Hello World Dialog Implementation

#include "HelloWorldDlg.h"
#include "HelloWorldLog.h"
#include "CATDlgGridConstraints.h"
#include "CATUnicodeString.h"
#include <stdio.h>
#include <time.h>

// Implement the class
CATImplementClass(HelloWorldDlg,
                  Implementation,
                  CATDlgDialog,
                  CATNull);

static CATUnicodeString U(const wchar_t* iText)
{
    CATUnicodeString text;
    text.BuildFromWChar(iText);
    return text;
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
HelloWorldDlg::HelloWorldDlg(CATDialog* iParent,
                              const CATString& iObjectName)
    : CATDlgDialog(iParent,
                   iObjectName,
                   CATDlgWndModal | CATDlgWndBtnClose | CATDlgGridLayout)
{
    HelloWorldLog("DIALOG", "constructor this=%p parent=%p", this, iParent);
    m_pPartName = NULL;
    m_pCategory = NULL;
    m_pMaterial = NULL;
    m_pNotes = NULL;
    m_pNumberButton = NULL;
    m_pStatusLabel = NULL;

    BuildDialog();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
HelloWorldDlg::~HelloWorldDlg()
{
    HelloWorldLog("DIALOG", "destructor this=%p", this);
}

//-----------------------------------------------------------------------------
// BuildDialog - Create and layout dialog controls
//-----------------------------------------------------------------------------
void HelloWorldDlg::BuildDialog()
{
    HelloWorldLog("DIALOG", "BuildDialog entered this=%p", this);
    // Set dialog title
    SetTitle(U(L"\x96F6\x4EF6\x53D6\x53F7"));

    CATDlgLabel* pPartNameLabel = new CATDlgLabel(this, "PartNameLabel");
    pPartNameLabel->SetTitle(U(L"\x96F6\x4EF6\x540D\x79F0"));
    pPartNameLabel->SetGridConstraints(0, 0, 1, 1, CATGRID_LEFT);
    m_pPartName = new CATDlgEditor(this, "PartNameEditor");
    m_pPartName->SetText(U(L"\x6F14\x793A\x96F6\x4EF6"));
    m_pPartName->SetVisibleTextWidth(28);
    m_pPartName->SetGridConstraints(0, 1, 1, 1, CATGRID_LEFT);

    CATDlgLabel* pCategoryLabel = new CATDlgLabel(this, "CategoryLabel");
    pCategoryLabel->SetTitle(U(L"\x96F6\x4EF6\x7C7B\x522B"));
    pCategoryLabel->SetGridConstraints(1, 0, 1, 1, CATGRID_LEFT);
    m_pCategory = new CATDlgCombo(this, "CategoryCombo", CATDlgCmbDropDown);
    CATUnicodeString categories[5];
    categories[0] = U(L"\x673A\x68B0\x4EF6");
    categories[1] = U(L"\x7535\x6C14\x4EF6");
    categories[2] = U(L"\x6807\x51C6\x4EF6");
    categories[3] = U(L"\x5916\x8D2D\x4EF6");
    categories[4] = U(L"\x5176\x4ED6");
    m_pCategory->SetLine(categories, 5);
    m_pCategory->SetSelect(0, 0);
    m_pCategory->SetGridConstraints(1, 1, 1, 1, CATGRID_LEFT);

    CATDlgLabel* pMaterialLabel = new CATDlgLabel(this, "MaterialLabel");
    pMaterialLabel->SetTitle(U(L"\x6750\x6599"));
    pMaterialLabel->SetGridConstraints(2, 0, 1, 1, CATGRID_LEFT);
    m_pMaterial = new CATDlgEditor(this, "MaterialEditor");
    m_pMaterial->SetText(U(L"\x94A2"));
    m_pMaterial->SetVisibleTextWidth(28);
    m_pMaterial->SetGridConstraints(2, 1, 1, 1, CATGRID_LEFT);

    CATDlgLabel* pNotesLabel = new CATDlgLabel(this, "NotesLabel");
    pNotesLabel->SetTitle(U(L"\x5907\x6CE8"));
    pNotesLabel->SetGridConstraints(3, 0, 1, 1, CATGRID_LEFT);
    m_pNotes = new CATDlgEditor(this, "NotesEditor");
    m_pNotes->SetText(U(L"\x6F14\x793A\x5C5E\x6027\x6570\x636E"));
    m_pNotes->SetVisibleTextWidth(28);
    m_pNotes->SetGridConstraints(3, 1, 1, 1, CATGRID_LEFT);

    m_pNumberButton = new CATDlgPushButton(this, "GetNumberButton");
    m_pNumberButton->SetTitle(U(L"\x70B9\x51FB\x53D6\x53F7"));
    m_pNumberButton->SetGridConstraints(4, 1, 1, 1, CATGRID_CENTER);
    AddAnalyseNotificationCB(m_pNumberButton,
        m_pNumberButton->GetPushBActivateNotification(),
        (CATCommandMethod)&HelloWorldDlg::OnGetNumber,
        NULL);

    m_pStatusLabel = new CATDlgLabel(this, "StatusLabel");
    m_pStatusLabel->SetTitle(U(L"\x8BF7\x586B\x5199\x6F14\x793A\x5C5E\x6027"));
    m_pStatusLabel->SetGridConstraints(5, 0, 1, 2, CATGRID_CENTER);

    SetGridColumnResizable(0, 0);
    SetGridColumnResizable(1, 0);

    HelloWorldLog("DIALOG", "BuildDialog completed name=%p category=%p number=%p",
                  m_pPartName, m_pCategory, m_pNumberButton);
}

void HelloWorldDlg::OnGetNumber(CATCommand* iCmd,
                                CATNotification* iNotif,
                                CATCommandClientData iClientData)
{
    const int categoryIndex = m_pCategory ? m_pCategory->GetSelect() : -1;
    time_t now = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &now);
    char partNumber[32];
    sprintf_s(partNumber, sizeof(partNumber),
              "DEMO-%04d%02d%02d-%02d%02d%02d",
              localTime.tm_year + 1900, localTime.tm_mon + 1,
              localTime.tm_mday, localTime.tm_hour,
              localTime.tm_min, localTime.tm_sec);

    HelloWorldLog("FORM", "number generated=%s categoryIndex=%d",
                  partNumber, categoryIndex);
    if (m_pStatusLabel)
    {
        CATUnicodeString result = U(L"\x96F6\x4EF6\x7F16\x53F7\xFF1A");
        result.Append(CATUnicodeString(partNumber));
        m_pStatusLabel->SetTitle(result);
    }
}
