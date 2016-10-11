// PictureExDemoDlg.h : header file
//

#if !defined(AFX_PICTUREEXDEMODLG_H__CEFF87E7_8346_11D5_991B_B9143490767B__INCLUDED_)
#define AFX_PICTUREEXDEMODLG_H__CEFF87E7_8346_11D5_991B_B9143490767B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PictureEx.h"	// Added by ClassView
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPictureExDemoDlg dialog

class CPictureExDemoDlg : public CDialog
{
// Construction
public:
	CPictureEx m_Picture;
	CPictureExDemoDlg(CWnd* pParent = NULL);	// standard constructor
	char* name_of_file;
	CString file;
// Dialog Data
	//{{AFX_DATA(CPictureExDemoDlg)
	enum { IDD = IDD_PICTUREEXDEMO_DIALOG };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureExDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	HCURSOR m_hCursor;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPictureExDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit text_box1;
	CString text_value_1;
	afx_msg void OnBnClickedButton2();
	double interval_time;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREEXDEMODLG_H__CEFF87E7_8346_11D5_991B_B9143490767B__INCLUDED_)
