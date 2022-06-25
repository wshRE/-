
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include "CTreeFile.h"


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	void DelChild(HTREEITEM hmyItem);               //清空子节点
	CString GetItemPath(HTREEITEM hItem);           //获取路径
	void PathFileReadANDShow(CString csPath, HTREEITEM hItem); //获取路径下的文件并插入
	bool IsFolderEmpty(CString strPath); //判断文件夹是否为空
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	CTreeFile m_tFileTree;  //文件树
	afx_msg void OnItemexpandingTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_lTreeFile;   //列表框
};
