// PictureExDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PictureExDemo.h"
#include "PictureExDemoDlg.h"

#include "recognition\\neuron network.h"
#include <atltime.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CPictureEx	m_Picture2;
	CPictureEx	m_Picture;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_GIF3, m_Picture2);
	DDX_Control(pDX, IDC_GIF2, m_Picture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureExDemoDlg dialog

CPictureExDemoDlg::CPictureExDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPictureExDemoDlg::IDD, pParent)
	, text_value_1(_T(""))
	, interval_time(0)
{
	//{{AFX_DATA_INIT(CPictureExDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCursor = AfxGetApp()->LoadCursor(IDC_HAND);
}

void CPictureExDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureExDemoDlg)
	DDX_Control(pDX, IDC_GIFFIRST, m_Picture);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT1, text_box1);
	DDX_Text(pDX, IDC_EDIT1, text_value_1);
	DDX_Text(pDX, IDC_EDIT2, interval_time);
}

BEGIN_MESSAGE_MAP(CPictureExDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CPictureExDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPictureExDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPictureExDemoDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureExDemoDlg message handlers

BOOL CPictureExDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if (m_Picture.Load(MAKEINTRESOURCE(IDR_FELIX),_T("GIF")))
		m_Picture.Draw();
	Read_weight();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPictureExDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPictureExDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPictureExDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPictureExDemoDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (m_Picture.Load(MAKEINTRESOURCE(IDR_EYES),_T("GIF")))
		m_Picture.Draw();

	if (m_Picture2.Load(MAKEINTRESOURCE(IDR_TYPE),_T("GIF")))
		m_Picture2.Draw();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPictureExDemoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	m_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if (rect.PtInRect(point) && m_hCursor)
	{
		SetCursor(m_hCursor);
		return TRUE;
	};

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CPictureExDemoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	m_Picture.GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (rect.PtInRect(point))
		ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"),
			_T("http://www.rsdn.ru"),_T(""),NULL,0);

	CDialog::OnLButtonDown(nFlags, point);
}
void CPictureExDemoDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog open(TRUE);
	const size_t size = 100;
	char file_name[size];
	if (open.DoModal() == IDOK)
	{
		name_of_file = new char[size];
		file = open.GetPathName();
		m_Picture.Load(file);
		m_Picture.Draw();
		strcpy_s(file_name,file);
		strcpy(name_of_file,file_name);
	}
}

void CPictureExDemoDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (name_of_file != NULL)
	{
		m_Picture.Load(file);
		m_Picture.Draw();
		clock_t t1 = clock();
		int k;
		for (int i=0;i<100;i++)
		{
			k = recognise(name_of_file);
		}
		//for (int i=0;i<10000000;i++);
		clock_t t2 = clock();
		double interval = double(t2-t1)/CLOCKS_PER_SEC;
		interval *= 1000;
		interval_time = interval/100;
		switch (k)
		{
			case 1:
				text_value_1 = "This is letter a";
				break;
			case 2:
				text_value_1 = "This is letter b";
				break;
			case 3:
				text_value_1 = "This is letter c";
				break;
			case 4:
				text_value_1 = "This is letter d";
				break;
			case 5:
				text_value_1 = "This is letter e";
				break;
			case 6:
				text_value_1 = "This is letter f";
				break;
			case 7:
				text_value_1 = "This is letter g";
				break;
			case 8:
				text_value_1 = "This is letter h";
				break;
			case 9:
				text_value_1 = "This is letter i";
				break;
			case 10:
				text_value_1 = "This is letter j";
				break;
			case 11:
				text_value_1 = "This is letter k";
				break;
			case 12:
				text_value_1 = "This is letter l";
				break;
			case 13:
				text_value_1 = "This is letter m";
				break;
			case 14:
				text_value_1 = "This is letter n";
				break;
			case 15:
				text_value_1 = "This is letter o";
				break;
			case 16:
				text_value_1 = "This is letter p";
				break;
			case 17:
				text_value_1 = "This is letter q";
				break;
			case 18:
				text_value_1 = "This is letter r";
				break;
			case 19:
				text_value_1 = "This is letter s";
				break;
			case 20:
				text_value_1 = "This is letter t";
				break;
			case 21:
				text_value_1 = "This is letter u";
				break;
			case 22:
				text_value_1 = "This is letter v";
				break;
			case 23:
				text_value_1 = "This is letter w";
				break;
			case 24:
				text_value_1 = "This is letter x";
				break;
			case 25:
				text_value_1 = "This is letter y";
				break;
			case 26:
				text_value_1 = "This is letter z";
				break;
		}
		UpdateData(0);
	}
}
