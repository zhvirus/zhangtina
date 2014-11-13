
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

class summary
{
public:
    summary():
        image_total(0),
        image_skipped(0),
        image_copied(0),
        video_total(0),
        video_skipped(0),
        video_copied(0)
    {}
    unsigned int image_total;
    unsigned int image_skipped;
    unsigned int image_copied;
    unsigned int video_total;
    unsigned int video_skipped;
    unsigned int video_copied;
};

summary SUM;

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
    , m_summary(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT4, m_output2);
    DDX_Text(pDX, IDC_EDIT3, m_summary);
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
        print(L"��ƬԴĿ¼�����ڣ�����Ҹ�! - error\r\n");
        return;
    }

    // Check dest directory
    const std::wstring dstFolder(m_dstPath.GetBuffer());
    if (!ZH::UTIL::File::exist(dstFolder)){
        print(L"��ƬĿ��Ŀ¼�����ڣ�����Ҹ�! - error\r\n");
        return;
    }

    // Disable button
    GetDlgItem(IDOK)->EnableWindow(false);

    // Collect image files
    print(L"���ڰ��ײ�����Ƭ...");
    std::vector<std::wstring>* files_image = NULL;
    ZH::UTIL::File::collect_files(srcFolder, L"\\.(jpg|jpeg|JPG|JPEG)$", files_image, true);
    unsigned int image_count = (unsigned int)files_image->size();
    wchar_t message[512];
    StringCbPrintf(message, 512, L"һ�������ҵ� %d ����Ƭ\r\n", image_count);
    SUM.image_total = image_count;
    print(message);

    // Collect video files
    print(L"���ڰ��ײ���¼��...");
    std::vector<std::wstring>* files_video = NULL;
    ZH::UTIL::File::collect_files(srcFolder, L"\\.(MOV|AVI|avi|mp4|MP4)$", files_video, true);
    unsigned int video_count = (unsigned int)files_video->size();
    StringCbPrintf(message, 512, L"һ�������ҵ� %d ����Ƶ\r\n", video_count);
    SUM.video_total = video_count;
    print(message);

    // Foreach image check its created date
    int index = -1;
    const unsigned int MAX_TIME_STR_LEN = 100;
    wchar_t time_str[MAX_TIME_STR_LEN];
    std::vector<std::wstring>::const_iterator cIt = files_image->begin();
    for (; cIt != files_image->end(); ++cIt){
        index++;
        StringCbPrintf(message, 512, L"\r\n[%d/%d]\r\n������Ƭ�� %s ...\r\n", index, image_count, cIt->c_str());
        print(message);

        unsigned int y = 0;
        unsigned int m = 0;
        unsigned int d = 0;
        if (!ZH::UTIL::File::getPhotoTakenTime(*cIt, y, m, d)){
            print(L"����ʱ��: ��ѯ�������ⲻ����Ƭ������ͼƬ!\r\n");
            process_image_video(*cIt, 0, 0, 0, true);
            continue;
        }

        StringCbPrintf(time_str, MAX_TIME_STR_LEN, L"����ʱ��: %d �� %d �� %d ��\r\n", y, m, d);
        process_image_video(*cIt, y, m, d, true);
        print((wchar_t*)time_str);
    }

    print(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
    print(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");

    // Foreach video check its created date
    index = -1;
    cIt = files_video->begin();
    for (; cIt != files_video->end(); ++cIt){
        index++;
        StringCbPrintf(message, 512, L"\r\n[%d/%d]\r\n������Ƶ�� %s ...\r\n", index, video_count, cIt->c_str());
        print(message);

        process_image_video(*cIt, 0, 0, 0, false);
    }

    ZH::UTIL::FreeVector(files_image);
    ZH::UTIL::FreeVector(files_video);

    // Enable button
    GetDlgItem(IDOK)->EnableWindow(true);


    // Print Summary
    wchar_t summary_str[10000];
    StringCbPrintf(summary_str, 10000,
        L"һ��������Ƭ������     %d\r\n"
        L"�����ѱ�������:        %d\r\n"
        L"������Ƭ����:          %d\r\n\r\n"
        L"һ��������Ƶ������     %d\r\n"
        L"�����ѱ�����Ƶ����:    %d\r\n"
        L"������Ƶ����:          %d\r\n"
        , SUM.image_total, SUM.image_skipped, SUM.image_copied,
          SUM.video_total, SUM.video_skipped, SUM.video_copied);

    m_summary = summary_str;
    UpdateData(false);

    SUM.image_total = 0;
    SUM.image_copied = 0;
    SUM.image_skipped = 0;
    SUM.video_total = 0;
    SUM.video_skipped = 0;
    SUM.video_copied = 0;
}

bool CPhotoManagerDlg::process_image_video(const std::wstring file_name, unsigned int y, unsigned int m, unsigned int d, bool isImage)
{
    if (y == 0){
        ZH::UTIL::File::getLastWriteTime(file_name, y, m, d);
    }

    // dst dir name
    wchar_t dst_dir[4096];
    StringCbPrintf(dst_dir, 4096, L"%s/%d��%d��/%d", m_dstPath.GetString(), y, m, d);

    // create target folder if it doesn't exist
    wchar_t message[4096];
    if (!ZH::UTIL::File::exist(dst_dir)){
        StringCbPrintf(message, 4096, L"����Ŀ¼�� %s\r\n", dst_dir);
        print(message);
        if (!ZH::UTIL::File::mkdir(dst_dir)){
            print(L"����ʧ�ܣ�����\r\n");
            return false;
        }
        else{
            print(L"�����ɹ���\r\n");
        }
    }

    // dst file name
    wchar_t basename[4096];
    ZH::UTIL::File::basename(file_name, basename);
    wchar_t dst_file[4096];
    StringCbPrintf(dst_file, 4096, L"%s\/%s", dst_dir, basename);

    if (ZH::UTIL::File::exist(dst_file)){
        if (ZH::UTIL::File::fileSize(file_name) == ZH::UTIL::File::fileSize(dst_file)){
            StringCbPrintf(message, 4096, L"�����ѱ���ͼƬ�� %s\r\n", dst_file);
            print(message);
            if (isImage){
                SUM.image_skipped++;
            }
            else{
                SUM.video_skipped++;
            }
            return true;
        }

        // same file name, but different size, we need to rename it.
        unsigned int index = 0;
        wchar_t new_dst_file[4096];
        StringCbPrintf(new_dst_file, 4096, L"%s/%d_%s", dst_dir, index, basename);
        while (ZH::UTIL::File::exist(new_dst_file)){
            StringCbPrintf(new_dst_file, 4096, L"%s/%d_%s", dst_dir, ++index, basename);
        }
        StringCbPrintf(dst_file, 4096, L"%s", new_dst_file);
        StringCbPrintf(message, 4096, L"ͬ��Ŀ���ļ����ڣ�����Ϊ�� %s\r\n", dst_file);
        print(message);
    }

    //assert(!ZH::UTIL::File::exist(dst_file));

    // copy file
    StringCbPrintf(message, 4096, L"�����ļ�: %s �� %s\r\n", file_name.c_str(), dst_file);
    print(message);
    if (ZH::UTIL::File::copyFile(file_name.c_str(), dst_file)){
        print(L"���Ƴɹ�\r\n");
        if (isImage){
            SUM.image_copied++;
        }
        else{
            SUM.video_copied++;
        }
    }
    else{
        print(L"����ʧ��\r\n");
    }


    return true;
}


