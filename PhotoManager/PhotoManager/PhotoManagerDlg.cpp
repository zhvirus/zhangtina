
// PhotoManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoManager.h"
#include "PhotoManagerDlg.h"
#include "afxdialogex.h"
#include "Util/File.h"
#include "Util/ModuleFree.h"
#include "Util/Thread.h"
#include "Strsafe.h"
#include "time.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd* detailWnd  = nullptr;
CWnd* summaryWnd = nullptr;
CWnd* activeWnd  = nullptr;


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
    , m_active_output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT4, m_output2);
    DDX_Text(pDX, IDC_EDIT3, m_summary);
    DDX_Control(pDX, IDC_PROGRESS1, m_progress);
    DDX_Text(pDX, IDC_STATIC_ACTIVITY, m_active_output);
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

    this->SetBackgroundColor(0xdddddd);

    //this->m_progress.SetBkColor(RGB(255, 112, 0));

    detailWnd = this->GetDlgItem(IDC_EDIT4);
    summaryWnd = this->GetDlgItem(IDC_EDIT3);
    activeWnd = this->GetDlgItem(IDC_STATIC_ACTIVITY);

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
}

void CPhotoManagerDlg::print(char* msg)
{
    m_output2 += msg;
}

void CPhotoManagerDlg::print_active(wchar_t* msg)
{
    m_active_output = msg;
}

bool summary::scanFolder(const std::wstring& folder)
{
    {
        imageFiles.clear();
        videoFiles.clear();
        unrecognizedFiles.clear();
    }

    std::vector<std::wstring> allFiles;
    ZH::UTIL::File::collect_files(folder, L".*", allFiles, true);

    for (auto i = 0U; i < allFiles.size(); ++i){
        const std::wstring& file = allFiles[i];
        if (ZH::UTIL::File::fileNameIsImage(file)){
            imageFiles.push_back(file);
        }
        else if (ZH::UTIL::File::fileNameIsVideo(file)){
            videoFiles.push_back(file);
        }
        else{
            unrecognizedFiles.push_back(file);
        }
    }

    image_total = (unsigned int)imageFiles.size();
    video_total = (unsigned int)videoFiles.size();

    return true;
}

unsigned int __stdcall doWork(void* param)
{
    time_t startT = time(0);
    CPhotoManagerDlg* pDlg = (CPhotoManagerDlg*)param;

    // clear messages
    pDlg->m_output2 = L"";

    summary SUM;

    // Check source directory
    const std::wstring srcFolder(pDlg->m_srcPath.GetBuffer());
    if (!ZH::UTIL::File::exist(srcFolder)){
        pDlg->print(L"照片源目录不存在!! 请别乱搞啊, 亲! - error\r\n");
        return 1;
    }

    // Check dest directory
    const std::wstring dstFolder(pDlg->m_dstPath.GetBuffer());
    if (!ZH::UTIL::File::exist(dstFolder)){
        pDlg->print(L"照片目标目录不存在!! 请别乱搞啊, 亲! - error\r\n");
        return 1;
    }

    // Disable button
    pDlg->GetDlgItem(IDOK)->EnableWindow(false);
    pDlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
    pDlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(false);

    // Scan files from the source folder
    if (!SUM.scanFolder(srcFolder)){
        pDlg->print(L"扫描分析源文件夹失败! - error\r\n");
        return 1;
    }

    wchar_t message[512];
    {
        // Echo image files count
        StringCbPrintf(message, 512, L"一共帮亲找到 %d 张照片\r\n", SUM.image_total);
        pDlg->print(message);

        // Echo video files count
        StringCbPrintf(message, 512, L"一共帮亲找到 %d 个视频\r\n", SUM.video_total);
        pDlg->print(message);

        // Echo unrec files count
        StringCbPrintf(message, 512, L"一共帮亲找到 %d 个未识别文件\r\n", SUM.unrecognizedFiles.size());
        pDlg->print(message);
    }

    pDlg->m_progress.SetRange(0, SUM.image_total + SUM.video_total+1);
    pDlg->m_progress.SetStep(1);
    pDlg->m_progress.SetPos(1);

    // For each image check its created date
    int image_index = 0;
    const unsigned int MAX_TIME_STR_LEN = 100;
    std::vector<std::wstring>::const_iterator cIt = SUM.imageFiles.begin();
    for (; cIt != SUM.imageFiles.end(); ++cIt){
        pDlg->m_progress.SetPos(image_index+2);
        image_index++;
        //pDlg->m_progress.SetBkColor(RGB(255, 112, 0));
        unsigned int y = 0;
        unsigned int m = 0;
        unsigned int d = 0;
        unsigned int h = 0;
        unsigned int min = 0;
        unsigned int s = 0;
        std::wstring device;
        if (!ZH::UTIL::File::getPhotoTakenInfo(*cIt, device, y, m, d, h, min, s)){
            pDlg->process_image_video(*cIt, L"", 0, 0, 0, 0, 0, 0, SUM, true);
        }
        else{
            pDlg->process_image_video(*cIt, device, y, m, d, h, min, s, SUM, true);
        }

        // Print Summary
        {
            wchar_t summary_str[10000];
            StringCbPrintf(summary_str, 10000,
                L"一共发现照片张数:      %d\r\n"
                L"忽略已备份张数:        %d\r\n"
                L"复制照片张数:          %d\r\n"
                L"复制照片文件大小:      %.3f MB\r\n\r\n"
                L"一共发现视频个数:      %d\r\n"
                L"忽略已备份视频个数:    %d\r\n"
                L"复制视频个数:          %d\r\n"
                L"复制视频文件大小:      %.3f MB\r\n\r\n"
                , SUM.image_total, SUM.image_skipped, SUM.image_copied, SUM.image_file_size / 1000000.0f,
                SUM.video_total, SUM.video_skipped, SUM.video_copied, SUM.video_file_size / 1000000.0f);

            pDlg->m_summary = summary_str;
        }

        pDlg->UpdateData(false);
    }

    // Foreach video check its created date
    int video_index = 0;
    cIt = SUM.videoFiles.begin();
    for (; cIt != SUM.videoFiles.end(); ++cIt){
        pDlg->m_progress.SetPos(image_index+2+video_index+1);
        video_index++;

        pDlg->process_image_video(*cIt, L"", 0, 0, 0, 0, 0, 0, SUM, false);

        // Print Summary
        {
            wchar_t summary_str[10000];
            StringCbPrintf(summary_str, 10000,
                L"一共发现照片张数:      %d\r\n"
                L"忽略已备份张数:        %d\r\n"
                L"复制照片张数:          %d\r\n"
                L"复制照片文件大小:      %.3f MB\r\n\r\n"
                L"一共发现视频个数:      %d\r\n"
                L"忽略已备份视频个数:    %d\r\n"
                L"复制视频个数:          %d\r\n"
                L"复制视频文件大小:      %.3f MB\r\n\r\n"
                , SUM.image_total, SUM.image_skipped, SUM.image_copied, SUM.image_file_size / 1000000.0f,
                SUM.video_total, SUM.video_skipped, SUM.video_copied, SUM.video_file_size / 1000000.0f);

            pDlg->m_summary = summary_str;
        }

        pDlg->UpdateData(false);
    }

    // Append unrecognized files
    const unsigned int unrecCount = (unsigned int)SUM.unrecognizedFiles.size();
    if (unrecCount > 0)
    {
        std::wstringstream ss;
        ss << L"\r\n未识别文件 " << unrecCount << L" 个:\r\n\r\n";
        for (auto str : SUM.unrecognizedFiles){
            std::wstring bName;
            ZH::UTIL::File::basename(str, bName);
            ss << bName << L"\r\n";
        }

        CString unrec = ss.str().c_str();
        pDlg->m_summary += unrec;
        pDlg->UpdateData(false);
    }

    pDlg->print_active(L"亲！ 全部帮你整理好了哦！~请看右上角的 [处理结果] ^_^！");

    // time
    {
        unsigned int sec = static_cast<unsigned int>(time(0) - startT);
        unsigned int hour = sec / 3600;
        sec -= 3600 * hour;
        unsigned int min  = sec / 60;
        sec -= 60 * min;
        
        if (hour > 0){
            StringCbPrintf(message, 512, L"\r\n\r\n用了 % 时 %d 分 %d 秒。\r\n", hour, min, sec);
        }
        else if (min > 0){
            StringCbPrintf(message, 512, L"\r\n\r\n用了 %d 分 %d 秒。\r\n", min, sec);
        }
        else{
            StringCbPrintf(message, 512, L"\r\n\r\n用了 %d 秒。\r\n", sec);
        }
        pDlg->print(message);
    }

    pDlg->UpdateData(false);

    // Enable button
    pDlg->GetDlgItem(IDOK)->EnableWindow(true);
    pDlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
    pDlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);

    return 0;
}

void CPhotoManagerDlg::OnBnClickedOk()
{
    ZH::UTIL::Thread::runSingleThread((ZH::UTIL::ThreadFunc)doWork, (void*)this, false);

}

bool CPhotoManagerDlg::process_image_video(
    const std::wstring& file_name,
    const std::wstring& device_name,
    unsigned int y,
    unsigned int m,
    unsigned int d,
    unsigned int h,
    unsigned int min,
    unsigned int s,
    summary& SUM, bool isImage)
{
    if (y == 0){
        ZH::UTIL::File::getLastWriteTime(file_name, y, m, d);
    }

    // dst dir name
    wchar_t dst_dir[4096];
    StringCbPrintf(dst_dir, 4096, L"%s\\%d年%d月\\%d", m_dstPath.GetString(), y, m, d);

    // create target folder if it doesn't exist
    wchar_t message[4096];
    if (!ZH::UTIL::File::exist(dst_dir)){
        StringCbPrintf(message, 4096, L"创建目录: %s - ", dst_dir);
        print(message);
        if (!ZH::UTIL::File::mkdir(dst_dir)){
            print(L"创建失败!!\r\n");
            return false;
        }
        else{
            print(L"创建成功。\r\n");
        }
    }

    // dst file name
    wchar_t basename[4096];
    {
        std::wstring tmp;
        ZH::UTIL::File::basename(file_name, tmp);
        ZH::UTIL::File::remove_preTag(tmp);

        if (device_name != L""){
            StringCbPrintf(basename, 4096, L"[%s]_[%02d-%02d-%02d]_%s", device_name.c_str(), h, min, s, tmp.c_str());
        }
        else{
            StringCbPrintf(basename, 4096, L"%s", tmp.c_str());
        }
    }

    wchar_t dst_file[4096];
    StringCbPrintf(dst_file, 4096, L"%s\\%s", dst_dir, basename);

    if (ZH::UTIL::File::exist(dst_file)){
        if (ZH::UTIL::File::fileSize(file_name) == ZH::UTIL::File::fileSize(dst_file)){

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
            if (ZH::UTIL::File::fileSize(file_name) == ZH::UTIL::File::fileSize(new_dst_file)){
                if (isImage){
                    SUM.image_skipped++;
                }
                else{
                    SUM.video_skipped++;
                }
                return true;
            }
            StringCbPrintf(new_dst_file, 4096, L"%s/%d_%s", dst_dir, ++index, basename);
        }
        StringCbPrintf(dst_file, 4096, L"%s", new_dst_file);
        StringCbPrintf(message, 4096, L"同名目标文件存在，改名为： %s\r\n", dst_file);
        print(message);

        StringCbPrintf(message, 4096, L"[名字冲突，改名]： %s\r\n", dst_file);
        print_active(message);
    }

    //assert(!ZH::UTIL::File::exist(dst_file));

    // copy file
    const int file_size = ZH::UTIL::File::fileSize(file_name);
    StringCbPrintf(message, 4096, L"复制文件: %s        ======>        %s        文件大小: %.3f MB\r\n", file_name.c_str(), dst_file, (float)file_size/1000000.0f);
    print_active(message);

    if (ZH::UTIL::File::copyFile(file_name.c_str(), dst_file)){
        if (isImage){
            SUM.image_copied++;
            SUM.image_file_size += file_size;
        }
        else{
            SUM.video_copied++;
            SUM.video_file_size += file_size;
        }
    }
    else{
        print(L"复制失败\r\n");
    }

    return true;
}


