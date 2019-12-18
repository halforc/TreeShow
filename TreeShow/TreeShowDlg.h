// TreeShowDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

// CTreeShowDlg 对话框
class CTreeShowDlg : public CDialogEx
{
    // 构造
public:
    CTreeShowDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CTreeShowDlg();
    // 对话框数据
    enum { IDD = IDD_TREESHOW_DIALOG };

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
    CListCtrl m_list;        //列表控件 IDC_LIST
    CTreeCtrl m_tree;        //树形控件 IDC_TREE
    CImageList m_ImageList;  //图像列表(相同大小图像集合)
    HTREEITEM m_hRoot;       //句柄 CTreeCtrl的根结点"我的电脑"
protected:
    CString m_strPath = _T("C:\\Users\\Yang\\Downloads\\TreeShow\\TreeShow");
public:
    void GetLogicalDrives(HTREEITEM hParent);  //获取驱动
    void GetDriveDir(HTREEITEM hParent);       //获取子项
    afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
    void AddSubDir(HTREEITEM hParent);         //添加子目录
    CString GetFullPath(HTREEITEM hCurrent);   //获取树项目全根路径
    afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);

    void ShowFile(HTREEITEM tree_Root, CString str_Dir);

    void FindDir(HTREEITEM pItem, const CString& dirPath);
    void FindFile(HTREEITEM pItem, const CString& filePath);
    BOOL DeleteDirectory(const CString& DirName);

    afx_msg void OnNMRClickTree_RCLICK(NMHDR* pNMHDR, LRESULT* pResult);
};
