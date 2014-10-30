
// PhotoManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoManager.h"
#include "PhotoManagerDlg.h"
#include "afxdialogex.h"
#include "Util/File.h"
#include "Util/ModuleFree.h"
#include "Strsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhotoManagerDlg dialog



CPhotoManagerDlg::CPhotoManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhotoManagerDlg::IDD, pParent),
    m_srcPath(""),
    m_dstPath("")
    , m_output2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT4, m_output2);
}

BEGIN_MESSAGE_MAP(CPhotoManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CPhotoManagerDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CPhotoManagerDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDOK, &CPhotoManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPhotoManagerDlg message handlers

BOOL CPhotoManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhotoManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhotoManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhotoManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPhotoManagerDlg::OnBnClickedButton1()
{
    CFolderPickerDialog dlgFile(L"C:/");
    if (IDOK == dlgFile.DoModal()){
        m_srcPath = dlgFile.GetPathName();
    }
    this->GetDlgItem(IDC_EDIT1)->SetWindowTextW(m_srcPath);
}


void CPhotoManagerDlg::OnBnClickedButton2()
{
    CFolderPickerDialog dlgFile(L"C:/");
    if (IDOK == dlgFile.DoModal()){
        m_dstPath = dlgFile.GetPathName();
    }
    this->GetDlgItem(IDC_EDIT2)->SetWindowTextW(m_dstPath);
}

void CPhotoManagerDlg::print(wchar_t* msg)
{
    m_output2 += msg;
    UpdateData(false);
}

void CPhotoManagerDlg::print(char* msg)
{
    m_output2 += msg;
    UpdateData(false);
}

void CPhotoManagerDlg::OnBnClickedOk()
{
    // Check source directory
    const std::wstring srcFolder(m_srcPath.GetBuffer());
    if (ZH::UTIL::File::exist(srcFolder)){
        print(L"Source folder exist. - OK\r\n");
    }
    else{
        print(L"Source folder does not exist! - error\r\n");
        return;
    }

    // Check dest directory
    const std::wstring dstFolder(m_dstPath.GetBuffer());
    if (ZH::UTIL::File::exist(dstFolder)){
        print(L"Dst folder exist. - OK\r\n");
    }
    else{
        print(L"Dst folder does not exist! - error\r\n");
        return;
    }

    // Disable button
    GetDlgItem(IDOK)->EnableWindow(false);

    // Collect files
    std::vector<std::wstring>* files = NULL;
    ZH::UTIL::File::collect_files(srcFolder, L"\.(jpg|jpeg|png|bmp)$", files, true);
    unsigned int image_count = (unsigned int)files->size();
    wchar_t message[512];
    StringCbPrintf(message, 512, L"Total detected image count: %d\r\n", image_count);
    print(message);

    // Foreach image check its created date
    unsigned int index = 0;
    const unsigned int MAX_TIME_STR_LEN = 100;
    char time_str[MAX_TIME_STR_LEN];
    std::vector<std::wstring>::const_iterator cIt = files->begin();
    for (; cIt != files->end(); ++cIt){
        StringCbPrintf(message, 512, L"\r\n[%d/%d]\r\nProcessing file %s\r\n", index, image_count, cIt->c_str());
        print(message);

        if (!ZH::UTIL::File::getPhotoTakenTime(*cIt, time_str, MAX_TIME_STR_LEN)){
            print(L"Get photo taken time failed!\r\n");
            continue;
        }

        print((char*)time_str);



        index++;
    }

    ZH::UTIL::FreeVector(files);

    // Enable button
    GetDlgItem(IDOK)->EnableWindow(true);
}
