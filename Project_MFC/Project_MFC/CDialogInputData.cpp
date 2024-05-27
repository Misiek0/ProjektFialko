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
{
	pDocum = pDoc;
	memset((void*)&lvi, 0, sizeof(LVITEMA));
	pDat = pDocum->pDat;	
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
END_MESSAGE_MAP()

BOOL CDialogInputData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	VERIFY(m_ColorBox.SubclassDlgItem(IDC_STATIC_COLOR, this));

	CString strx, stry, color;

	lvi.mask = LVIF_TEXT;
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iImage = 0;

	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ProjectMFCTYPE));

	m_smallImageList.Create(16, 16, FALSE, 1, 0);
	m_smallImageList.Add(hIcon);
	m_ListCtrl.SetImageList(&m_smallImageList, LVSIL_SMALL);

	CRect rectListCtrl;
	memset(rectListCtrl, 0, sizeof(rectListCtrl));
	m_ListCtrl.GetClientRect(&rectListCtrl);

	int list_ctrl_width = rectListCtrl.right - rectListCtrl.left;
	int column_width = list_ctrl_width / 3;

	int ret;
	int nFormat = LVCFMT_LEFT;
	ret = m_ListCtrl.InsertColumn(0, "X", nFormat, column_width, 0);
	ret = m_ListCtrl.InsertColumn(1, "Y", nFormat, column_width, 1);
	ret = m_ListCtrl.InsertColumn(2, "color", nFormat, column_width, 2);

	ASSERT(pDat);
	int no_item = pDat->size();

	lvi.iSubItem = 0;
	m_ListCtrl.SetItemCount(no_item);

	for (int i = 0; i < no_item; i++)
	{
		MY_POINT pt = (*pDat)[i];
		lvi.iItem = i;
		strx.Format("%le", pt.x);
		size_t Len = strlen(strx);
		stry.Format("%le", pt.y);
		Len += strlen(stry);
		//color.Format("%d", pt.color);
		//Len += strlen(color);

		lvi.pszText = " ";
		lvi.cchTextMax = (int)Len;
		ret = m_ListCtrl.InsertItem(&lvi);
		m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
		m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
		//m_ListCtrl.SetItemText(lvi.iItem, 2, color);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogInputData::ModifyData()
{
	char st[512];
	BOOL ret(0);
	MY_POINT tmp;

	UpdateData(TRUE);
	const int no_it(m_ListCtrl.GetItemCount());

	if (pDat)
		delete pDat;
	pDocum->pDat = pDat = new MY_DATA(no_it);

	for (int nItem = 0; nItem < no_it; ++nItem)
	{
		BOOL ret = m_ListCtrl.GetItemText(nItem, 0, st, sizeof(st));
		tmp.x = atof(st);
		ret = m_ListCtrl.GetItemText(nItem, 1, st, sizeof(st));
		tmp.y = atof(st);
		//ret = m_ListCtrl.GetItemText(nItem, 2, st, sizeof(st));
		//tmp.color = atol(st);

		pDat->Push(tmp);
	}

	UpdateData(FALSE);

	ASSERT_VALID(pDocum);
	pDocum->SetModifiedFlag();
	pDocum->UpdateAllViews(NULL);
}

// CDialogInputData message handlers
void CDialogInputData::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	ModifyData();
}

void CDialogInputData::OnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	int nItem = m_ListCtrl.GetItemCount();
	int ret = -1;
	MY_POINT tmp;
	CString strx, stry, color;

	UpdateData(TRUE);

	tmp.x = m_X;
	tmp.y = m_Y;
	strx.Format("%le", m_X);
	size_t Len = strlen(strx);
	stry.Format("%le", m_Y);
	//Len += strlen(stry);
	//color.Format("%d", m_color);
	//Len += strlen(color);

	lvi.iItem = nItem;
	lvi.pszText = " ";
	lvi.cchTextMax = (int)Len;
	ret = m_ListCtrl.InsertItem(&lvi);
	m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
	m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
	//m_ListCtrl.SetItemText(lvi.iItem, 2, color);

	UpdateData(FALSE);

	ASSERT_VALID(pDocum);
	pDocum->SetModifiedFlag();
	pDocum->UpdateAllViews(NULL);
}


void CDialogInputData::OnClickedButtonMod()
{
	// TODO: Add your control notification handler code here
	int ret = -1;
	int no_item = m_ListCtrl.GetItemCount();

	if (m_SelItem < 0 || m_SelItem >= no_item)
		return;

	MY_POINT tmp;
	CString strx, stry, color;
	int nItem = m_SelItem;

	UpdateData(TRUE);

	tmp.x = m_X;
	tmp.y = m_Y;
	strx.Format("%le", m_X);
	size_t Len = strlen(strx);
	stry.Format("%le", m_Y);
	Len += strlen(stry);
	//color.Format("%d", m_color);
	//Len += strlen(color);

	lvi.iItem = nItem;
	//lvi.pszText = " ";
	//lvi.cchTextMax = (int)Len;
	//ret = ListCtrl.InsertItem(&lvi);
	m_ListCtrl.SetItemText(lvi.iItem, 0, strx);
	m_ListCtrl.SetItemText(lvi.iItem, 1, stry);
	//m_ListCtrl.SetItemText(lvi.iItem, 2, color);

	UpdateData(FALSE);

	ModifyData();
}


void CDialogInputData::OnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	int ret = -1;
	UpdateData(TRUE);

	int no_item = m_ListCtrl.GetItemCount();

	if (m_SelItem < 0 || m_SelItem >= no_item)
		return;

	m_ListCtrl.DeleteItem(m_SelItem);
	UpdateData(FALSE);

	ASSERT_VALID(pDocum);
	pDocum->SetModifiedFlag();
	pDocum->UpdateAllViews(NULL);
}

void CDialogInputData::OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData(TRUE);

	int nItem = m_SelItem = pNMLV->iItem;
	int no_item = m_ListCtrl.GetItemCount();

	if (m_SelItem < 0 || m_SelItem >= no_item)
		return;

	int ncol = 0;
	char st[512];
	BOOL ret = m_ListCtrl.GetItemText(nItem, ncol, st, sizeof(st));
	m_X = atof(st);
	ncol = 1;
	ret = m_ListCtrl.GetItemText(nItem, ncol, st, sizeof(st));
	m_Y = atof(st);
	ncol = 2;
	//ret = m_ListCtrl.GetItemText(nItem, ncol, st, sizeof(st));
	//m_color = atol(st);

	m_ColorBox.SetItem(nItem);
	m_ColorBox.Invalidate();

	UpdateData(FALSE);
}

void CDialogInputData::OnClickedButtonColor()
{
	// TODO: Add your control notification handler code here

	CColorDialog dlg;

	if (dlg.DoModal())
	{
		m_ColorBox.SetColor(dlg.GetColor());
		m_ColorBox.Invalidate();
	}
}

///////////////////////////////////////////////////////////////////////////////
//   CColorBox

BEGIN_MESSAGE_MAP(CColorBox, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CColorBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	CRect rect;
	this->GetClientRect(&rect);

	CBrush newbrush;
	CBrush* oldbrush;
	newbrush.CreateSolidBrush(color);
	oldbrush = dc.SelectObject(&newbrush);

	dc.Rectangle(0, 0, (rect.right - rect.left), (rect.bottom - rect.top));

	dc.SelectObject(oldbrush);
	newbrush.DeleteObject();
}

void CColorBox::SetItem(int i)
{
	COLORREF tab[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
	int ii = i % (sizeof(tab) / sizeof(tab[0]));
	color = tab[ii];
}










