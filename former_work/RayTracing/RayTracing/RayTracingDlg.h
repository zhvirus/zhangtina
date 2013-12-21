// RayTracingDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "RayTrace.h"
#include "afxcmn.h"

using namespace Raytracing;

// CRayTracingDlg 对话框
class CRayTracingDlg : public CDialog
{
// 构造
public:
	CRayTracingDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CRayTracingDlg();
// 对话框数据
	enum { IDD = IDD_RAYTRACING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	static RayTrace Ray;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	float m_sphere_center_x;
public:
	float m_sphere_center_y;
public:
	float m_sphere_center_z;
public:
	float m_sphere_material_r;
	float m_sphere_material_g;
	float m_sphere_material_b;

public:
	float m_sphere_radius;
public:
	float m_sphere_refractrate;
public:
	CButton m_sphere_check;
public:
	afx_msg void OnBnClickedButton1();
public:
	CListBox m_object_list;
public:
	int displayObjects(void);
	int displayLights(void);
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	float m_polygon_x;
public:
	float m_polygon_y;
public:
	float m_polygon_z;
public:
	float m_polygon_r;
public:
	float m_polygon_g;
public:
	float m_polygon_b;
public:
	CComboBox m_polygon_list;
public:
	float m_polygon_refractRate;
public:
	CButton m_polygon_check;
public:
	afx_msg void OnBnClickedButton4();

	vector<D3DXVECTOR3> vTemp;
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton27();
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnBnClickedButton7();
public:
	CListBox m_light_list;
public:
	afx_msg void OnBnClickedButton9();
public:
	afx_msg void OnBnClickedButton10();
public:
	float m_light_x;
public:
	float m_light_y;
public:
	float m_light_z;
public:
	float m_light_r;
public:
	float m_light_g;
public:
	float m_light_b;
public:
	float m_light_a;

float m_light_a0;
float m_light_a1;
float m_light_a2;

float m_light_ka;
float m_light_kd;
float m_light_ks;


public:
	afx_msg void OnBnClickedButton11();
public:
	//afx_msg void OnBnClickedButton8();
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
private:
	Bitmap* tex;
	CProgressCtrl m_progress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	static DWORD CALLBACK doThreadTrace(LPVOID param);
	CButton m_start;
	CButton m_b1;
	CButton m_b2;
	CButton m_b3;
	CButton m_b4;
	CButton m_b5;
	CButton m_b6;
	CButton m_b7;
	CButton m_b8;
	//CButton m_b9;
	CButton m_polygon_check2;
	// 生成图像尺寸
	public:
	static int m_imageSize;
public:
	CButton m_sphere_check2;
public:
	afx_msg void OnBnClickedButton12();
public:
	CEdit m_polygon_texture;
};
