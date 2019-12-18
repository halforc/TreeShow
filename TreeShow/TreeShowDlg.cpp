// TreeShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeShow.h"
#include "TreeShowDlg.h"
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
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CTreeShowDlg 对话框

CTreeShowDlg::CTreeShowDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CTreeShowDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTreeShowDlg::~CTreeShowDlg()
{
}

void CTreeShowDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST, m_list);
    DDX_Control(pDX, IDC_TREE, m_tree);
}

BEGIN_MESSAGE_MAP(CTreeShowDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, &CTreeShowDlg::OnItemexpandedTree)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTreeShowDlg::OnSelchangedTree)
    ON_NOTIFY(NM_RCLICK, IDC_TREE, &CTreeShowDlg::OnNMRClickTree_RCLICK)
END_MESSAGE_MAP()

// CTreeShowDlg 消息处理程序

BOOL CTreeShowDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if(pSysMenu != NULL) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if(!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_ImageList.Create(32, 32, ILC_COLOR32, 10, 30);     //创建图像序列CImageList对象
    HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //图标句柄
    m_ImageList.Add(hIcon);                          //图标添加到图像序列
    m_list.SetImageList(&m_ImageList, LVSIL_NORMAL);  //为树形控件设置图像序列
    m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
    //m_hRoot = m_tree.InsertItem("TreeShow");         //插入根节点
    //GetLogicalDrives(m_hRoot);                       //自定义函数 获取驱动
    //GetDriveDir(m_hRoot);                            //自定义函数 获取驱动子项

    FindDir(nullptr, m_strPath);
    //m_tree.Expand(m_hRoot, TVE_EXPAND);               //展开或折叠子项列表 TVE_EXPAND展开列表
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTreeShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTreeShowDlg::OnPaint()
{
    if(IsIconic()) {
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
    } else {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTreeShowDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//*******************************************************
//添加自定义函数

//函数功能:获取驱动器 参数:路径名
void CTreeShowDlg::GetLogicalDrives(HTREEITEM hParent)
{
    //获取系统分区驱动器字符串信息
    size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //驱动器总长度
    //char* pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //建立数组
    char data[240];
    char* pDriveStrings = data;
    GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
    size_t szDriveString = strlen(pDriveStrings);                        //驱动大小
    while(szDriveString > 0) {
        m_tree.InsertItem(pDriveStrings, hParent);       //在父节点hParent添加盘符
        pDriveStrings += szDriveString + 1;             //pDriveStrings即C:\ D:\ E:\盘
        szDriveString = strlen(pDriveStrings);
    }
}

//函数功能:获取驱动盘符下所有子项文件夹
void CTreeShowDlg::GetDriveDir(HTREEITEM hParent)
{
    HTREEITEM hChild = m_tree.GetChildItem(hParent);   //获取指定位置中的子项
    while(hChild) {
        CString strText = m_tree.GetItemText(hChild);  //检索列表中项目文字
        if(strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符
            strText += _T("\\");
        strText += "*.*";
        //将当前目录下文件枚举并InsertItem树状显示
        CFileFind file;                                       //定义本地文件查找
        BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件
        while(bContinue) {
            bContinue = file.FindNextFile();                  //查找下一个文件
            if(file.IsDirectory() && !file.IsDots())          //找到文件为内容且不为点"."
                m_tree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹
        }
        GetDriveDir(hChild);                                  //递归调用
        hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件(TVGN_NEXT)下一兄弟项
    }
}

//函数功能:展开事件函数
void CTreeShowDlg::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    // TODO: 在此添加控件通知处理程序代码
    TVITEM item = pNMTreeView->itemNew;                  //发送\接受关于树形视图项目信息
    if(item.hItem == m_hRoot)
        return;
    HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //获取指定位置中的子项
    while(hChild) {
        AddSubDir(hChild);                               //添加子目录
        hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //获取树形控件TVGN_NEXT下兄弟项
    }
    *pResult = 0;
}

//函数功能:添加子目录
void CTreeShowDlg::AddSubDir(HTREEITEM hParent)
{
    CString strPath = GetFullPath(hParent);     //获取全路径
    if(strPath.Right(1) != "\\")
        strPath += "\\";
    strPath += "*.*";
    CFileFind file;
    BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
    while(bContinue) {
        bContinue = file.FindNextFile();        //查找下一个文件
        if(file.IsDirectory() && !file.IsDots())
            m_tree.InsertItem(file.GetFileName(), hParent);
    }
}

//函数功能:获取树项目全根路径
CString CTreeShowDlg::GetFullPath(HTREEITEM hCurrent)
{
    CString strTemp;
    CString strReturn = "";
    while(hCurrent != m_hRoot) {
        strTemp = m_tree.GetItemText(hCurrent);    //检索列表中项目文字
        if(strTemp.Right(1) != "\\")
            strTemp += "\\";
        strReturn = strTemp + strReturn;
        hCurrent = m_tree.GetParentItem(hCurrent); //返回父项目句柄
    }
    return /*m_strPath + _T("\\") + */strReturn;
}

//函数功能:选中事件显示图标
void CTreeShowDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    //LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    // TODO: 在此添加控件通知处理程序代码
    m_list.DeleteAllItems();
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    TVITEM item = pNMTreeView->itemNew;
    if(item.hItem == m_hRoot)
        return;
    CString str = GetFullPath(item.hItem);
    if(str.Right(1) != "\\")
        str += "\\";
    str += "*.*";
    CFileFind file;
    BOOL bContinue = file.FindFile(str);
    while(bContinue) {
        bContinue = file.FindNextFile();
        if(!file.IsDirectory() && !file.IsDots()) {
            SHFILEINFO info;
            CString temp = str;
            int index = temp.Find("*.*");
            temp.Delete(index, 3);
            SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
            int i = m_ImageList.Add(info.hIcon);
            m_list.InsertItem(i, info.szDisplayName, i);
        }
    }
    *pResult = 0;
}

void CTreeShowDlg::ShowFile(HTREEITEM tree_Root, CString str_Dir)
{
    CFileFind FileFind;

    //临时变量，用以记录返回的树节点
    HTREEITEM tree_Temp;

    //判断输入目录最后是否存在'\'，不存在则补充
    if(str_Dir.Right(1) != "\\")
        str_Dir += "\\";
    str_Dir += "*.*";
    BOOL res = FileFind.FindFile(str_Dir);
    while(res) {
        tree_Temp = tree_Root;
        res = FileFind.FindNextFile();
        if(FileFind.IsDirectory() && !FileFind.IsDots())//目录是文件夹
        {
            CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
            CString strTitle = FileFind.GetFileName();//得到目录名，做为树控的结点
            tree_Temp = m_tree.InsertItem(strTitle, 0, 0, tree_Root);
            ShowFile(tree_Temp, strPath);
        } else if(!FileFind.IsDirectory() && !FileFind.IsDots())//如果是文件
        {
            CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
            CString strTitle = FileFind.GetFileName();//得到文件名，做为树控的结点
            m_tree.InsertItem(strTitle, 0, 0, tree_Temp);
        }
    }
    FileFind.Close();
}

void CTreeShowDlg::FindDir(HTREEITEM pItem, const CString& dirPath)//HTREEITEM 为一个CtreeCtel节点，此处实现将文件夹映射到CtreeCtrl控件中去

{
    CFileFind tempFind;
    BOOL bFound = tempFind.FindFile(dirPath + "\\*.*");

    HTREEITEM pItem2;
    if(pItem == NULL) {
        pItem2 = m_tree.InsertItem(dirPath, 0, 1, TVI_ROOT, TVI_LAST);//插入根节点
    } else {
        pItem2 = m_tree.InsertItem(dirPath.Right(dirPath.GetLength() - dirPath.ReverseFind('\\') - 1), 0, 1, pItem, TVI_LAST);//插入子节点
    }
    while(bFound) {
        bFound = tempFind.FindNextFile();
        if(tempFind.IsDots())
            continue;

        tempFind.IsDirectory() ?
            FindDir(pItem2, tempFind.GetFilePath()) : //找到的是文件夹
            FindFile(pItem2, tempFind.GetFileName()); //找到的是文件
    }
    tempFind.Close();
}

void CTreeShowDlg::FindFile(HTREEITEM pItem, const CString& filePath)
{
    m_tree.InsertItem(filePath, 0, 1, pItem, TVI_LAST); //插入子节点
}

BOOL CTreeShowDlg::DeleteDirectory(const CString& DirName)
{
    CFileFind tempFind;
    BOOL IsFinded = tempFind.FindFile(DirName + "\\*.*");
    while(IsFinded) {
        IsFinded = tempFind.FindNextFile();

        if(tempFind.IsDots())
            continue;

        if(tempFind.IsDirectory()) {
            DeleteDirectory(tempFind.GetFilePath());
        } else {
            DeleteFile(tempFind.GetFilePath());
        }
    }
    tempFind.Close();
    if(!RemoveDirectory(DirName)) {
        MessageBox(_T("删除目录失败！"), _T("警告信息"), MB_OK);
        return FALSE;
    }
    return TRUE;
}

void CTreeShowDlg::OnNMRClickTree_RCLICK(NMHDR* pNMHDR, LRESULT* pResult)
{
    CPoint point;
    GetCursorPos(&point);  //获取鼠标坐标
    m_tree.ScreenToClient(&point);//映射到CtreeCtrl中
    UINT nFlags;
    HTREEITEM hItems = m_tree.HitTest(point, &nFlags);//根据坐标获取节点
    CString m_treeChangeFile = m_tree.GetItemText(hItems);
    hItems = m_tree.GetParentItem(hItems);
    while(hItems) //循环获取父节点，获取节点全路径
    {
        m_treeChangeFile = m_tree.GetItemText(hItems) + L"\\" + m_treeChangeFile;

        hItems = m_tree.GetParentItem(hItems);
    }

    // 菜单显示
    CMenu menu;
    menu.LoadMenu(IDR_MENU_TREE);
    CMenu* pPopup = menu.GetSubMenu(0);
    POINT	 pt;
    ::GetCursorPos((LPPOINT)&pt);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

    *pResult = 0;
}