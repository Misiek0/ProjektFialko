// CDialogInputData.cpp : implementation file
//

#include "pch.h"
#include "Project_MFC.h"
#include "CDialogInputData.h"
#include "afxdialogex.h"

// CDialogInputData dialog

IMPLEMENT_DYNAMIC(CDialogInputData, CDialogEx)

CDialogInputData::CDialogInputData(CProjectMFCDoc* pDoc, CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_INPUT_DATA, pParent)
    , m_X(0)
    , m_Y(0)
    , m_Color(RGB(0, 0, 0))
    , m_Name(nullptr)
    , pDocum(nullptr) // Initialize pDocum to nullptr
    , pDat(nullptr)   // Initialize pDat to nullptr
{
    if (pDoc == nullptr)
    {
        AfxMessageBox(_T("Document pointer is null!"));
        return;
    }

    pDocum = pDoc;
    pDat = pDocum->pDat;

    if (pDat == nullptr)
    {
        AfxMessageBox(_T("Data pointer in document is null!"));
        return;
    }

    memset(&lvi, 0, sizeof(LVITEMA));
}



CDialogInputData::~CDialogInputData()
{
    m_smallImageList.DeleteImageList();
}

void CDialogInputData::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
}

BEGIN_MESSAGE_MAP(CDialogInputData, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CDialogInputData::OnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_MOD, &CDialogInputData::OnClickedButtonMod)
    ON_BN_CLICKED(IDC_BUTTON_DEL, &CDialogInputData::OnClickedButtonDel)
    ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_CTRL, &CDialogInputData::OnItemchangingListCtrl)
    ON_BN_CLICKED(IDOK, &CDialogInputData::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_COLOR, &CDialogInputData::OnClickedButtonColor)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_BINARY, &CDialogInputData::OnClickedButtonSaveBinary)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_BINARY, &CDialogInputData::OnClickedButtonLoadBinary)
END_MESSAGE_MAP()

BOOL CDialogInputData::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    VERIFY(m_ColorBox.SubclassDlgItem(IDC_STATIC_COLOR, this));

    CString strx, stry, strColor, strName;
    lvi.mask = LVIF_TEXT;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iImage = 0;

    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ProjectMFCTYPE));
    m_smallImageList.Create(16, 16, FALSE, 1, 0);
    m_smallImageList.Add(hIcon);
    m_ListCtrl.SetImageList(&m_smallImageList, LVSIL_SMALL);

    CRect rectListCtrl;
    m_ListCtrl.GetClientRect(&rectListCtrl);

    int list_ctrl_width = rectListCtrl.Width();
    int column_width = list_ctrl_width / 4;

    m_ListCtrl.InsertColumn(0, "X", LVCFMT_LEFT, column_width, 0);
    m_ListCtrl.InsertColumn(1, "Y", LVCFMT_LEFT, column_width, 1);
    m_ListCtrl.InsertColumn(2, "color", LVCFMT_LEFT, column_width, 2);
    m_ListCtrl.InsertColumn(3, "name", LVCFMT_LEFT, column_width, 3);

    ASSERT(pDat);
    int no_item = pDat->size();
    lvi.iSubItem = 0;
    m_ListCtrl.SetItemCount(no_item);

    for (int i = 0; i < no_item; i++)
    {
        MY_POINT pt = (*pDat)[i];
        lvi.iItem = i;
        strx.Format("%le", pt.x);
        stry.Format("%le", pt.y);
        strColor.Format("%02X%02X%02X", GetRValue(pt.color), GetGValue(pt.color), GetBValue(pt.color)); // Correct RGB order
        strName = pt.name;
        lvi.pszText = " ";
        lvi.cchTextMax = (int)(strx.GetLength() + stry.GetLength() + strColor.GetLength());
        m_ListCtrl.InsertItem(&lvi);
        m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
        m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
        m_ListCtrl.SetItemText(lvi.iItem, 2, strColor);
        m_ListCtrl.SetItemText(lvi.iItem, 3, strName);
    }

    return TRUE;
}

void CDialogInputData::ModifyData()
{
    char st[512];
    UpdateData(TRUE);
    const int no_it = m_ListCtrl.GetItemCount();

    if (pDat)
        delete pDat;
    pDat = new MY_DATA(no_it);
    pDocum->pDat = pDat;

    for (int nItem = 0; nItem < no_it; ++nItem)
    {
        m_ListCtrl.GetItemText(nItem, 0, st, sizeof(st));
        double x = atof(st);
        m_ListCtrl.GetItemText(nItem, 1, st, sizeof(st));
        double y = atof(st);
        m_ListCtrl.GetItemText(nItem, 2, st, sizeof(st));

        // Parse the color from hexadecimal string
        int r, g, b;
        sscanf_s(st, "%02X%02X%02X", &r, &g, &b);
        COLORREF color = RGB(r, g, b);
        
        m_ListCtrl.GetItemText(nItem, 3, st, sizeof(st));
        char* name = new char[strlen(st) + 1];  // Allocate memory for name
        strcpy_s(name, strlen(st) + 1, st);

        pDat->Push({ x, y, color, name });
    }

    UpdateData(FALSE);
    pDocum->SetModifiedFlag();
    pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnBnClickedOk()
{
    CDialogEx::OnOK();
    ModifyData();
}

void CDialogInputData::OnClickedButtonAdd()
{
    UpdateData(TRUE);
    CString strx, stry, strColor, strName;
    strx.Format("%le", m_X);
    stry.Format("%le", m_Y);
    strColor.Format("%02X%02X%02X", GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color)); // If storing color as text
    strName = m_Name;

    lvi.iItem = m_ListCtrl.GetItemCount();
    lvi.pszText = " ";
    lvi.cchTextMax = (int)(strx.GetLength() + stry.GetLength() + strColor.GetLength());
    m_ListCtrl.InsertItem(&lvi);
    m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
    m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
    m_ListCtrl.SetItemText(lvi.iItem, 2, strColor); // If storing color as text
    m_ListCtrl.SetItemText(lvi.iItem, 3, strName);

    UpdateData(FALSE);
    pDocum->SetModifiedFlag();
    pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnClickedButtonMod()
{
    if (m_SelItem < 0 || m_SelItem >= m_ListCtrl.GetItemCount())
        return;

    UpdateData(TRUE);
    CString strx, stry, strColor, strName;
    strx.Format("%le", m_X);
    stry.Format("%le", m_Y);
    strColor.Format("%02X%02X%02X", GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color)); // Correct RGB order
    strName = m_Name;

    m_ListCtrl.SetItemText(m_SelItem, 0, strx);
    m_ListCtrl.SetItemText(m_SelItem, 1, stry);
    m_ListCtrl.SetItemText(m_SelItem, 2, strColor); // If storing color as text
    m_ListCtrl.SetItemText(m_SelItem, 3, strName); // If storing color as text

    UpdateData(FALSE);
    ModifyData();
}

void CDialogInputData::OnClickedButtonDel()
{
    if (m_SelItem < 0 || m_SelItem >= m_ListCtrl.GetItemCount())
        return;

    m_ListCtrl.DeleteItem(m_SelItem);
    UpdateData(FALSE);
    pDocum->SetModifiedFlag();
    pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    *pResult = 0;

    if (pNMLV->uChanged & LVIF_STATE && !(pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_SELECTED))
    {
        m_SelItem = pNMLV->iItem;

        char st[512];
        m_ListCtrl.GetItemText(m_SelItem, 0, st, sizeof(st));
        m_X = atof(st);
        m_ListCtrl.GetItemText(m_SelItem, 1, st, sizeof(st));
        m_Y = atof(st);

        m_ColorBox.SetItem(m_SelItem);
        m_ColorBox.Invalidate();

        m_ListCtrl.GetItemText(m_SelItem, 3, st, sizeof(st));
        m_Name = "XD";

        UpdateData(FALSE);
    }
}

void CDialogInputData::OnClickedButtonColor()
{
    CColorDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        m_Color = dlg.GetColor();
        m_ColorBox.SetColor(m_Color);
        m_ColorBox.Invalidate();
    }
}

void CDialogInputData::OnClickedButtonSaveBinary()
{
    //pDat->saveToFile("testFile.bin");
}

void CDialogInputData::OnClickedButtonLoadBinary()
{
    //pDat->loadFromFile("testFile.bin", 2);
}

///////////////////////////////////////////////////////////////////////////////
//   CColorBox

BEGIN_MESSAGE_MAP(CColorBox, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CColorBox::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    CBrush newBrush(color);
    CBrush* oldBrush = dc.SelectObject(&newBrush);
    dc.Rectangle(&rect);
    dc.SelectObject(oldBrush);
}

void CColorBox::SetItem(int i)
{
    COLORREF colors[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
    color = colors[i % _countof(colors)];
}






