
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CMFCApplication1Dlg::DelChild(HTREEITEM hmyItem)
{
	// Delete all of the children of hmyItem.
	if (m_tFileTree.ItemHasChildren(hmyItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_tFileTree.GetChildItem(hmyItem);

		while (hChildItem != NULL)
		{
			hNextItem = m_tFileTree.GetNextItem(hChildItem, TVGN_NEXT);
			m_tFileTree.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

CString CMFCApplication1Dlg::GetItemPath(HTREEITEM hItem)
{
	CString ItemPath;
	ItemPath.Empty();
	CString TemPath(_T(""));

	while (TRUE)
	{
		HTREEITEM hParentItem = m_tFileTree.GetParentItem(hItem);
		if (hParentItem == NULL)//没有父节点
		{
			ItemPath = m_tFileTree.GetItemText(hItem);
			ItemPath.Delete(ItemPath.GetLength()-1,1);
			ItemPath = ItemPath + TemPath;

			break;
		}
		else//有父节点
		{
			TemPath = _T("\\") + m_tFileTree.GetItemText(hItem) + TemPath;
			hItem = m_tFileTree.GetParentItem(hItem);
		}
	}
	return ItemPath + "\\";
}

void CMFCApplication1Dlg::PathFileReadANDShow(CString csPath, HTREEITEM hItem)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(csPath+'*');
	//AfxMessageBox(csPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strFmt = finder.GetFileName();
		//文件类型判断
		if (!finder.IsSystem() && !finder.IsDots()) {//筛出非系统文件
			if (finder.IsDirectory()) {  //如果是文件夹
				HTREEITEM hItem1 = m_tFileTree.InsertItem(strFmt, hItem);
				if (!IsFolderEmpty(csPath + '\\' + strFmt)) {//如果文件夹非空
					m_tFileTree.InsertItem("读取中", hItem1);
				}
			}
		}
	}
}

bool CMFCApplication1Dlg::IsFolderEmpty(CString strPath)
{
	CFileFind ff;
	bool bRet = false;
	CString strFilePaht = strPath + "\\*.*";

	bRet = ff.FindFile(strFilePaht);
	while (bRet)
	{
		bRet = ff.FindNextFile();
		if (!ff.IsDots())
		{
			ff.Close();
			return false;
		}
	}
	ff.Close();
	return true;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tFileTree);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_FILE, &CMFCApplication1Dlg::OnItemexpandingTreeFile)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FILE, &CMFCApplication1Dlg::OnDblclkTreeFile)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//获取全部盘符
	char szBuff[MAXBYTE] = {};
	GetLogicalDriveStrings(MAXBYTE, szBuff);
	//盘符拆分
	int nBuff = 0;
	char szDrive[MAXBYTE] = {};
	int nDrive = 0;
	CFileFind ff;
	HTREEITEM hTree;
	while (szBuff[nBuff]) {
		nDrive = 0;
		while (szBuff[nBuff]) {
			szDrive[nDrive] = szBuff[nBuff];
			nDrive++;
			nBuff++;
		}
		szDrive[nDrive] = '\0';
		
		/*MFC_Dir_A_S(szDrive);*/
		//添加所有盘符并添加读取中的提示
		hTree = m_tFileTree.InsertItem(szDrive);
		m_tFileTree.InsertItem("读取中", hTree);
		nBuff++;
	}

	//设置风格，树带线
	m_tFileTree.ModifyStyle(0,
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_EX_FADEINOUTEXPANDOS
	);




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//展开 或 收缩
void CMFCApplication1Dlg::OnItemexpandingTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	/*规则：
	1.展开：
		1.删掉读取中，获取数据并写入
		2.判断子文件夹下是否有文件夹
			1.有则添加 待写入
			2.没有就不添加
	2.收起：
		1.删除所有子树
		2.添加 读取中
	*/
	HTREEITEM hNode = pNMTreeView->itemNew.hItem;  //获取句柄
	//如果是展开
	if (pNMTreeView->action == TVE_EXPAND) {
		//删除所有子节点
		DelChild(hNode);
		/*标记：插入新的项*/
		CString csPath = GetItemPath(hNode); //获取路径
		//获取文件并插入
		PathFileReadANDShow(csPath, hNode);
	}
	//如果是收起
	else if (pNMTreeView->action == TVE_COLLAPSE) {
		//删除所有子节点
		DelChild(hNode);
		m_tFileTree.InsertItem("读取中", hNode);  //分展开还是收起
	}

	*pResult = 0;
}


//双击
void CMFCApplication1Dlg::OnDblclkTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//1.获取路径
	CString csPath = GetItemPath(m_tFileTree.GetSelectedItem());
	//2.读取路径下的所有文件并显示在文本框
	//AfxMessageBox(csPath);

}
