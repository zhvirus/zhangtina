// RayTracingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RayTracing.h"
#include "RayTracingDlg.h"
#include	"Sphere.h"
#include	"Polygon.h"
#include	"Light.h"
#include	"PointLight.h"

using namespace Raytracing;
using namespace Gdiplus;




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int progress=0;
bool isThreadAlive=false;
int CRayTracingDlg::m_imageSize=600;
RayTrace CRayTracingDlg::Ray;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRayTracingDlg �Ի���




CRayTracingDlg::CRayTracingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRayTracingDlg::IDD, pParent)
	, m_sphere_center_x(0)
	, m_sphere_center_y(0)
	, m_sphere_center_z(0)
	, m_sphere_radius(1)
	, m_sphere_refractrate(1.5)
	, m_sphere_material_r(1)
	,m_sphere_material_g(1)
	,m_sphere_material_b(1)
	, m_polygon_x(0)
	, m_polygon_y(0)
	, m_polygon_z(0)
	,m_polygon_r(1)
	,m_polygon_g(1)
	,m_polygon_b(1)
	, m_polygon_refractRate(1.5)
	, m_light_x(0)
	, m_light_y(0)
	, m_light_z(0)
	, m_light_r(255)
	, m_light_g(255)
	, m_light_b(255)
	, m_light_a(255)
	,m_light_a0(1)
	,m_light_a1(0.0001)
	,m_light_a2(0)
	,m_light_ka(1)
	,m_light_kd(1)
	,m_light_ks(1)
	,tex(NULL)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CRayTracingDlg::~CRayTracingDlg()
{

if(tex!=NULL)
{
delete tex;
tex=NULL;

}


}


void CRayTracingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sphere_center_x);
	DDX_Text(pDX, IDC_EDIT2, m_sphere_center_y);
	DDX_Text(pDX, IDC_EDIT3, m_sphere_center_z);
	DDX_Text(pDX, IDC_EDIT43, m_sphere_material_r);
	DDX_Text(pDX, IDC_EDIT53, m_sphere_material_g);
	DDX_Text(pDX, IDC_EDIT63, m_sphere_material_b);
	DDV_MinMaxFloat(pDX, m_sphere_material_r, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_sphere_material_g, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_sphere_material_b, 0.0, 1.0);


	DDX_Text(pDX, IDC_EDIT7, m_sphere_radius);
	DDV_MinMaxFloat(pDX, m_sphere_radius, 1, 5000);
	DDX_Text(pDX, IDC_EDIT83, m_sphere_refractrate);
	DDV_MinMaxFloat(pDX, m_sphere_refractrate, 1, 5);
	DDX_Control(pDX, IDC_CHECK1, m_sphere_check);
	DDX_Control(pDX, IDC_LIST_MAIN, m_object_list);
	DDX_Text(pDX, IDC_EDIT4, m_polygon_x);
	DDX_Text(pDX, IDC_EDIT5, m_polygon_y);
	DDX_Text(pDX, IDC_EDIT6, m_polygon_z);

	DDX_Text(pDX, IDC_EDIT97, m_polygon_r);
	DDX_Text(pDX, IDC_EDIT107, m_polygon_g);
	DDX_Text(pDX, IDC_EDIT117, m_polygon_b);
	DDV_MinMaxFloat(pDX, m_polygon_r, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_polygon_g, 0.0, 1.0);
	DDV_MinMaxFloat(pDX, m_polygon_b, 0.0, 1.0);

	DDX_Control(pDX, IDC_COMBO1, m_polygon_list);
	DDX_Text(pDX, IDC_EDIT127, m_polygon_refractRate);
	DDV_MinMaxFloat(pDX, m_polygon_refractRate, 1, 2);
	DDX_Control(pDX, IDC_CHECK2, m_polygon_check);
	DDX_Control(pDX, IDC_LIST1, m_light_list);
	DDX_Text(pDX, IDC_EDIT8, m_light_x);
	DDX_Text(pDX, IDC_EDIT9, m_light_y);
	DDX_Text(pDX, IDC_EDIT10, m_light_z);
	DDX_Text(pDX, IDC_EDIT11, m_light_r);
	DDV_MinMaxFloat(pDX, m_light_r, 0, 255);
	DDX_Text(pDX, IDC_EDIT12, m_light_g);
	DDV_MinMaxFloat(pDX, m_light_g, 0, 255);
	DDX_Text(pDX, IDC_EDIT13, m_light_b);
	DDV_MinMaxFloat(pDX, m_light_b, 0, 255);
	DDX_Text(pDX, IDC_EDIT20, m_light_a);
	DDV_MinMaxFloat(pDX, m_light_a, 0, 255);

	DDX_Text(pDX, IDC_EDIT14, m_light_a0);
	DDV_MinMaxFloat(pDX, m_light_a0, 0.0f, 100.0f);

	DDX_Text(pDX, IDC_EDIT15, m_light_a1);
	DDV_MinMaxFloat(pDX, m_light_a1, 0.0f, 100.0f);

	DDX_Text(pDX, IDC_EDIT16, m_light_a2);
	DDV_MinMaxFloat(pDX, m_light_a2, 0.0f, 100.0f);

	DDX_Text(pDX, IDC_EDIT17, m_light_ka);
	DDV_MinMaxFloat(pDX, m_light_ka, 0.0f, 1.0f);

	DDX_Text(pDX, IDC_EDIT18, m_light_kd);
	DDV_MinMaxFloat(pDX, m_light_kd, 0.0f, 1.0f);

	DDX_Text(pDX, IDC_EDIT19, m_light_ks);
	DDV_MinMaxFloat(pDX, m_light_ks, 0.0f, 1.0f);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDOK, m_start);
	DDX_Control(pDX, IDC_BUTTON3, m_b1);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON9, m_b3);
	DDX_Control(pDX, IDC_BUTTON10, m_b4);
	DDX_Control(pDX, IDC_BUTTON1, m_b5);
	DDX_Control(pDX, IDC_BUTTON27, m_b6);
	DDX_Control(pDX, IDC_BUTTON11, m_b7);
	DDX_Control(pDX, IDC_BUTTON7, m_b8);
	//DDX_Control(pDX, IDC_BUTTON8, m_b9);
	DDX_Control(pDX, IDC_CHECK3, m_polygon_check2);
	DDX_Text(pDX, IDC_EDIT21, m_imageSize);
	DDV_MinMaxInt(pDX, m_imageSize, 20, 20000);
	DDX_Control(pDX, IDC_CHECK4, m_sphere_check2);
	DDX_Control(pDX, IDC_EDIT22, m_polygon_texture);
}

BEGIN_MESSAGE_MAP(CRayTracingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRayTracingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRayTracingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRayTracingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRayTracingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CRayTracingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CRayTracingDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON27, &CRayTracingDlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON6, &CRayTracingDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CRayTracingDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CRayTracingDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CRayTracingDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CRayTracingDlg::OnBnClickedButton11)
	//ON_BN_CLICKED(IDC_BUTTON8, &CRayTracingDlg::OnBnClickedButton8)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON12, &CRayTracingDlg::OnBnClickedButton12)
END_MESSAGE_MAP()


// CRayTracingDlg ��Ϣ�������

BOOL CRayTracingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	Ray.loadObjectFromFile("save.txt");
	displayObjects();
	displayLights();
	m_progress.SetRange(0,100);
	m_progress.SetPos(0);
	SetTimer(911,200,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRayTracingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRayTracingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CRayTracingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRayTracingDlg::OnBnClickedOk()
{	
	m_start.EnableWindow(false);
	m_b1.EnableWindow(false);
	m_b2.EnableWindow(false);
	m_b3.EnableWindow(false);
	m_b4.EnableWindow(false);
	m_b5.EnableWindow(false);
	m_b6.EnableWindow(false);
	m_b7.EnableWindow(false);
	m_b8.EnableWindow(false);
	//m_b9.EnableWindow(false);

	HANDLE hand;
	DWORD threadID;
	UpdateData(true);
	//�����߳̽��й��߸���
	hand = CreateThread(NULL,0,doThreadTrace,NULL,0,&threadID);
	

	
}

void CRayTracingDlg::OnBnClickedButton1()
{
	UpdateData(true);
	D3DXVECTOR3 center(m_sphere_center_x,m_sphere_center_y,m_sphere_center_z);
	float radius = m_sphere_radius;
	D3DXVECTOR3 material(m_sphere_material_r,m_sphere_material_g,m_sphere_material_b);
	float refractRate	=	m_sphere_refractrate;
	bool bb=static_cast<bool>(m_sphere_check.GetCheck());
	bool bb2=static_cast<bool>(m_sphere_check2.GetCheck());
	
	Sphere *sp	=new Sphere(center,radius,material,refractRate,bb,bb2);
	Ray.addObject(sp);

    //���б������ʾ��ǰ�����б�
	displayObjects();



}

int CRayTracingDlg::displayObjects(void)
{

	m_object_list.ResetContent();
	for(int i=0;i<static_cast<int>(Ray.object.size());i++)
	{
		m_object_list.AddString(Ray.object[i]->getString());


	}
	



	return 0;
}

//��ʾ��Դ
int CRayTracingDlg::displayLights(void)
{

	m_light_list.ResetContent();
	for(int i=0;i<static_cast<int>(Ray.light.size());i++)
	{
		m_light_list.AddString(Ray.light[i]->getString());


	}
return 0;

}

//����б�
void CRayTracingDlg::OnBnClickedButton2()
{

	Ray.deleteAllObject();
	displayObjects();

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//ɾ��һ������
void CRayTracingDlg::OnBnClickedButton3()
{
	int k=m_object_list.GetCurSel();

	if(k>=0)
	{
		Ray.deleteObject(k);
		displayObjects();	

	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//����polygon����
void CRayTracingDlg::OnBnClickedButton4()
{	
	UpdateData(true);
	
	vTemp.push_back(D3DXVECTOR3(m_polygon_x,m_polygon_y,m_polygon_z));
	m_polygon_list.ResetContent();

	for(int i=0;i<static_cast<int>(vTemp.size());i++)
	{
		CString temp;
		temp.Format(L"(%.2f %.2f %.2f)",vTemp[i].x,vTemp[i].y,vTemp[i].z);
		m_polygon_list.AddString(temp);

	}
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//��ն���ζ����б�
void CRayTracingDlg::OnBnClickedButton5()
{
	vTemp.clear();
	m_polygon_list.ResetContent();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//����һ�������
void CRayTracingDlg::OnBnClickedButton27()
{
	
	if(static_cast<int>(vTemp.size())>=3)
	{
	UpdateData(true);
	D3DXVECTOR3 x_material(m_polygon_r,m_polygon_g,m_polygon_b);
	
	bool bb=m_polygon_check.GetCheck()==1?true:false;
	bool bb2=m_polygon_check2.GetCheck()==1?true:false;

	Raytracing::Polygon* poly	=	new Raytracing::Polygon(vTemp,x_material,m_polygon_refractRate,bb,bb2);
	
	CString fileName;
	m_polygon_texture.GetWindowText(fileName);
	Bitmap temp(fileName.AllocSysString());
	Bitmap *mapCloe	=	temp.Clone(0,0,temp.GetWidth(),temp.GetHeight(),PixelFormatDontCare);
	if(fileName==L"")
		fileName	=	L"NULL";
	poly->setTexFileName(fileName);
	poly->setTexture(mapCloe);

	Ray.addObject(poly);
	displayObjects();
	
	
	}
	
	
	}

//�������б���Ϊ�ļ�
void CRayTracingDlg::OnBnClickedButton6()
{

//�������ļ��Ի���
CFileDialog cfd(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"AllFiles(*.*)|*.*|",AfxGetMainWnd(),0);
//��ȡ�ļ����ּ�·��
	if(cfd.DoModal()==IDOK)
	{   	

	CString fileName	=	cfd.GetPathName();
	string temp=_com_util::ConvertBSTRToString(BSTR(fileName.GetString()));
	Ray.saveObjectToFile(temp);

	}

	
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//���ļ��ظ������б�
void CRayTracingDlg::OnBnClickedButton7()
{

	//�������ļ��Ի���
CFileDialog cfd(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"AllFiles(*.*)|*.*|",AfxGetMainWnd(),0);
//��ȡ�ļ����ּ�·��
	if(cfd.DoModal()==IDOK)
	{   	

	CString fileName	=	cfd.GetPathName();
	string temp=_com_util::ConvertBSTRToString(BSTR(fileName.GetString()));
	Ray.loadObjectFromFile(temp);
	displayObjects();
	displayLights();

	}





	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//ɾ��һ����Դ
void CRayTracingDlg::OnBnClickedButton9()
{

int k=m_light_list.GetCurSel();

	if(k>=0)
	{
		Ray.deleteLight(k);
		displayLights();	

	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//ɾ�����й�Դ
void CRayTracingDlg::OnBnClickedButton10()
{
	
	m_light_list.ResetContent();
	Ray.deleteAllLights();
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//����һ����Դ
void CRayTracingDlg::OnBnClickedButton11()
{
	UpdateData(true);
	D3DXVECTOR3 position(m_light_x,m_light_y,m_light_z);
	D3DXCOLOR	 color(m_light_r,m_light_g,m_light_b,m_light_a);
	D3DXVECTOR3 att(m_light_a0,m_light_a1,m_light_a2);
	PointLight *here = new PointLight(position,color,att,m_light_ka,m_light_kd,m_light_ks);
	Ray.addLight(here);
	displayLights();
}

////����Ĭ�ϳ���
//void CRayTracingDlg::OnBnClickedButton8()
//{
//	Ray.loadObjectFromFile("default.zhd");
//	displayObjects();
//	displayLights();
//}



int CRayTracingDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
void CRayTracingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==911)
	{
		
		m_progress.SetPos(progress);
		if(isThreadAlive==false)
		{
			m_start.EnableWindow(true);
			m_b1.EnableWindow(true);
			m_b2.EnableWindow(true);
			m_b3.EnableWindow(true);
			m_b4.EnableWindow(true);
			m_b5.EnableWindow(true);
			m_b6.EnableWindow(true);
			m_b7.EnableWindow(true);
			m_b8.EnableWindow(true);
			//m_b9.EnableWindow(true);
	
		
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//���߸��ټ����߳�
DWORD CALLBACK CRayTracingDlg::doThreadTrace(LPVOID param)
{	
	//�̴߳��
	isThreadAlive=true;
	
	//����ͼ��ߴ�
	
	int width		=	m_imageSize;
	int height	=	m_imageSize;

	//����һ����ͼ�� �������ڴ�
	Bitmap dd(width,height,PixelFormat24bppRGB);
	BitmapData *data	=	new BitmapData;
	Rect crt(0,0,dd.GetWidth(),dd.GetHeight());
	dd.LockBits(&crt,ImageLockModeWrite,PixelFormat24bppRGB,data);
	BYTE *start	=	static_cast<BYTE*>(data->Scan0);
	INT stride	=	data->Stride;
	
	//�۾�λ��
	D3DXVECTOR3 eye(0,0,250);
	
	//�Ӿ����ߴ�
	float viewSize	=	150.0f;


	
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{	
			float x	=	viewSize*(static_cast<float>(j)/static_cast<float>(width)-0.5f);	
			float y	=	viewSize*(0.5f-static_cast<float>(i)/static_cast<float>(height));
			float z	=	75.0f;
			D3DXVECTOR3 dir(x-eye.x,y-eye.y,z-eye.z);
			
			//���߸��ټ����ÿһ��������ɫֵ
			int index	=	i*stride+j*3;
			D3DXCOLOR col	=	Ray.trace(eye,dir);
			*(start+index)		=	static_cast<BYTE>(col.b);
			*(start+index+1)	=	static_cast<BYTE>(col.g);
			*(start+index+2)	=	static_cast<BYTE>(col.r);
			
			progress = static_cast<int>(static_cast<float>(i*width+j)/static_cast<float>((width*height))*100.0f);

		}
	progress	=	0;

	dd.UnlockBits(data);
	// Save the altered image.

	
   CLSID pngClsid;
   GetEncoderClsid(L"image/bmp", &pngClsid);


	//�������ļ��Ի���
CFileDialog cfd(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"AllFiles(*.BMP,*.JPG,*.JPEG)|*.*|*.jpg|*.bmp|*.jpeg|",AfxGetMainWnd(),0);
//��ȡ�ļ����ּ�·��
	if(cfd.DoModal()==IDOK)
	{   	

	CString fileName	=	cfd.GetPathName();
   dd.Save(fileName.AllocSysString(), &pngClsid, NULL);

	}

	isThreadAlive	=	false;

	return 0;
}

//ѡ�������ļ�
void CRayTracingDlg::OnBnClickedButton12()
{
//�������ļ��Ի���
CFileDialog cfd(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"AllFiles(*.BMP,*.JPG,*.JPEG)|*.*|*.jpg|*.bmp|*.jpeg|",AfxGetMainWnd(),0);
//��ȡ�ļ����ּ�·��
	if(cfd.DoModal()==IDOK)
	{   	

	CString fileName	=	cfd.GetPathName();

	Bitmap newMap(fileName.AllocSysString());

				if(tex!=NULL)
				{

				delete tex;
				tex=NULL;
				}

				tex = newMap.Clone(0,0,newMap.GetWidth(),newMap.GetHeight(),PixelFormatDontCare);

		if(tex!=NULL)
			m_polygon_texture.SetWindowTextW(fileName);
		else
			m_polygon_texture.SetWindowTextW(L"");


	}// if press OK
}//function