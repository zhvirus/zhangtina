
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
    m_srcPath = theApp.GetProfileString(L"PhotoManager", L"source_dir");
    m_dstPath = theApp.GetProfileString(L"PhotoManager", L"dst_dir");
    this->GetDlgItem(IDC_EDIT1)->SetWindowTextW(m_srcPath);
    this->GetDlgItem(IDC_EDIT2)->SetWindowTextW(m_dstPath);

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
        theApp.WriteProfileString(L"PhotoManager", L"source_dir", m_srcPath);
    }
    this->GetDlgItem(IDC_EDIT1)->SetWindowTextW(m_srcPath);
}


void CPhotoManagerDlg::OnBnClickedButton2()
{
    CFolderPickerDialog dlgFile(L"C:/");
    if (IDOK == dlgFile.DoModal()){
        m_dstPath = dlgFile.GetPathName();
        theApp.WriteProfileString(L"PhotoManager", L"dst_dir", m_dstPath);
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
    // clear messages
    m_output2 = L"";

    // Check source directory
    const std::wstring srcFolder(m_srcPath.GetBuffer());
    if (!ZH::UTIL::File::exist(srcFolder)){
        print(L"照片源目录不存在，请别乱搞! - error\r\n");
        return;
    }

    // Check dest directory
    const std::wstring dstFolder(m_dstPath.GetBuffer());
    if (!ZH::UTIL::File::exist(dstFolder)){
        print(L"照片目标目录不存在，请别乱搞! - error\r\n");
        return;
    }

    // Disable button
    GetDlgItem(IDOK)->EnableWindow(false);

    // Collect image files
    print(L"正在帮亲查找照片...");
    std::vector<std::wstring>* files_image = NULL;
    ZH::UTIL::File::collect_files(srcFolder, L"\\.(jpg|jpeg|JPG|JPEG)$", files_image, true);
    unsigned int image_count = (unsigned int)files_image->size();
    wchar_t message[512];
    StringCbPrintf(message, 512, L"一共帮亲找到 %d 张照片\r\n", image_count);
    print(message);

    // Collect video files
    print(L"正在帮亲查找录像...");
    std::vector<std::wstring>* files_video = NULL;
    ZH::UTIL::File::collect_files(srcFolder, L"\\.(MOV|AVI|avi|mp4|MP4)$", files_video, true);
    unsigned int video_count = (unsigned int)files_video->size();
    StringCbPrintf(message, 512, L"一共帮亲找到 %d 个视频\r\n", video_count);
    print(message);

    // Foreach image check its created date
    int index = -1;
    const unsigned int MAX_TIME_STR_LEN = 100;
    wchar_t time_str[MAX_TIME_STR_LEN];
    std::vector<std::wstring>::const_iterator cIt = files_image->begin();
    for (; cIt != files_image->end(); ++cIt){
        index++;
        StringCbPrintf(message, 512, L"\r\n[%d/%d]\r\n处理照片： %s ...\r\n", index, image_count, cIt->c_str());
        print(message);

        unsigned int y = 0;
        unsigned int m = 0;
        unsigned int d = 0;
        if (!ZH::UTIL::File::getPhotoTakenTime(*cIt, y, m, d)){
            print(L"拍摄时间: 查询不到，这不是照片，而是图片!\r\n");
            process_image_video(*cIt, 0, 0, 0);
            continue;
        }

        StringCbPrintf(time_str, MAX_TIME_STR_LEN, L"拍摄时间: %d 年 %d 月 %d 日", y, m, d);
        process_image_video(*cIt, y, m, d);
        print((wchar_t*)time_str);
    }

    ZH::UTIL::FreeVector(files_image);
    ZH::UTIL::FreeVector(files_video);

    // Enable button
    GetDlgItem(IDOK)->EnableWindow(true);
}

bool CPhotoManagerDlg::process_image_video(const std::wstring file_name, unsigned int y, unsigned int m, unsigned int d)
{
    if (y == 0){



    }


    return true;
}


