
// PhotoManagerDlg.h : header file
//

#pragma once

#include <string>
#include "afxcmn.h"

class summary
{
public:
    summary() :
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


// CPhotoManagerDlg dialog
class CPhotoManagerDlg : public CDialogEx
{
    // Construction
public:
    CPhotoManagerDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    enum { IDD = IDD_PHOTOMANAGER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


    // Implementation
protected:
    HICON m_hIcon;


    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedOk();
    void print(wchar_t* msg);
    void print(char* msg);
    void print_active(wchar_t* msg);
    bool process_image_video(const std::wstring, unsigned int, unsigned int, unsigned int, summary&, bool);
    CString m_output2;
    CString m_summary;
    CString m_srcPath;
    CString m_dstPath;
    CProgressCtrl m_progress;
    CString m_active_output;
};
