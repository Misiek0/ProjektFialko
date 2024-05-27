#pragma once
#include "Project_MFCDoc.h"
#include "MyData.h"

class CColorBox : public CStatic
{
public:
	COLORREF color;
	MY_DATA* pDat;

	CColorBox() : pDat(NULL) { color = RGB(255, 255, 255); }
	void SetData(MY_DATA* pData) { pDat = pData; }
	void SetItem(int i);
	void SetColor(COLORREF col) { color = col; }

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
};


// CDialogInputData dialog

class CDialogInputData : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogInputData)

public:
	CDialogInputData(CProjectMFCDoc* pDoc, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogInputData();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void ModifyData();

	DECLARE_MESSAGE_MAP()
public:
	// my ListCtrl
	CListCtrl m_ListCtrl;
	LV_ITEM lvi;
	CImageList m_smallImageList;
	MY_DATA* pDat;
	CProjectMFCDoc* pDocum;
	int m_SelItem;

	double m_X;
	double m_Y;

	CColorBox m_ColorBox;

	afx_msg void OnClickedButtonAdd();
	afx_msg void OnClickedButtonMod();
	afx_msg void OnClickedButtonDel();
	
	afx_msg void OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedButtonColor();
};
