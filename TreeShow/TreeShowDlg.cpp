// TreeShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TreeShow.h"
#include "TreeShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CTreeShowDlg �Ի���

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

// CTreeShowDlg ��Ϣ�������

BOOL CTreeShowDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_ImageList.Create(32, 32, ILC_COLOR32, 10, 30);     //����ͼ������CImageList����
    HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //ͼ����
    m_ImageList.Add(hIcon);                          //ͼ����ӵ�ͼ������
    m_list.SetImageList(&m_ImageList, LVSIL_NORMAL);  //Ϊ���οؼ�����ͼ������
    m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
    //m_hRoot = m_tree.InsertItem("TreeShow");         //������ڵ�
    //GetLogicalDrives(m_hRoot);                       //�Զ��庯�� ��ȡ����
    //GetDriveDir(m_hRoot);                            //�Զ��庯�� ��ȡ��������

    FindDir(nullptr, m_strPath);
    //m_tree.Expand(m_hRoot, TVE_EXPAND);               //չ�����۵������б� TVE_EXPANDչ���б�
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTreeShowDlg::OnPaint()
{
    if(IsIconic()) {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTreeShowDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//*******************************************************
//����Զ��庯��

//��������:��ȡ������ ����:·����
void CTreeShowDlg::GetLogicalDrives(HTREEITEM hParent)
{
    //��ȡϵͳ�����������ַ�����Ϣ
    size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //�������ܳ���
    //char* pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //��������
    char data[240];
    char* pDriveStrings = data;
    GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
    size_t szDriveString = strlen(pDriveStrings);                        //������С
    while(szDriveString > 0) {
        m_tree.InsertItem(pDriveStrings, hParent);       //�ڸ��ڵ�hParent����̷�
        pDriveStrings += szDriveString + 1;             //pDriveStrings��C:\ D:\ E:\��
        szDriveString = strlen(pDriveStrings);
    }
}

//��������:��ȡ�����̷������������ļ���
void CTreeShowDlg::GetDriveDir(HTREEITEM hParent)
{
    HTREEITEM hChild = m_tree.GetChildItem(hParent);   //��ȡָ��λ���е�����
    while(hChild) {
        CString strText = m_tree.GetItemText(hChild);  //�����б�����Ŀ����
        if(strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�
            strText += _T("\\");
        strText += "*.*";
        //����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ
        CFileFind file;                                       //���屾���ļ�����
        BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�
        while(bContinue) {
            bContinue = file.FindNextFile();                  //������һ���ļ�
            if(file.IsDirectory() && !file.IsDots())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."
                m_tree.InsertItem(file.GetFileName(), hChild); //����̷�·������״�ļ���
        }
        GetDriveDir(hChild);                                  //�ݹ����
        hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //��ȡ���οؼ�(TVGN_NEXT)��һ�ֵ���
    }
}

//��������:չ���¼�����
void CTreeShowDlg::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    TVITEM item = pNMTreeView->itemNew;                  //����\���ܹ���������ͼ��Ŀ��Ϣ
    if(item.hItem == m_hRoot)
        return;
    HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //��ȡָ��λ���е�����
    while(hChild) {
        AddSubDir(hChild);                               //�����Ŀ¼
        hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //��ȡ���οؼ�TVGN_NEXT���ֵ���
    }
    *pResult = 0;
}

//��������:�����Ŀ¼
void CTreeShowDlg::AddSubDir(HTREEITEM hParent)
{
    CString strPath = GetFullPath(hParent);     //��ȡȫ·��
    if(strPath.Right(1) != "\\")
        strPath += "\\";
    strPath += "*.*";
    CFileFind file;
    BOOL bContinue = file.FindFile(strPath);    //���Ұ����ַ������ļ�
    while(bContinue) {
        bContinue = file.FindNextFile();        //������һ���ļ�
        if(file.IsDirectory() && !file.IsDots())
            m_tree.InsertItem(file.GetFileName(), hParent);
    }
}

//��������:��ȡ����Ŀȫ��·��
CString CTreeShowDlg::GetFullPath(HTREEITEM hCurrent)
{
    CString strTemp;
    CString strReturn = "";
    while(hCurrent != m_hRoot) {
        strTemp = m_tree.GetItemText(hCurrent);    //�����б�����Ŀ����
        if(strTemp.Right(1) != "\\")
            strTemp += "\\";
        strReturn = strTemp + strReturn;
        hCurrent = m_tree.GetParentItem(hCurrent); //���ظ���Ŀ���
    }
    return /*m_strPath + _T("\\") + */strReturn;
}

//��������:ѡ���¼���ʾͼ��
void CTreeShowDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    //LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    // TODO: �ڴ���ӿؼ�֪ͨ����������
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

    //��ʱ���������Լ�¼���ص����ڵ�
    HTREEITEM tree_Temp;

    //�ж�����Ŀ¼����Ƿ����'\'���������򲹳�
    if(str_Dir.Right(1) != "\\")
        str_Dir += "\\";
    str_Dir += "*.*";
    BOOL res = FileFind.FindFile(str_Dir);
    while(res) {
        tree_Temp = tree_Root;
        res = FileFind.FindNextFile();
        if(FileFind.IsDirectory() && !FileFind.IsDots())//Ŀ¼���ļ���
        {
            CString strPath = FileFind.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
            CString strTitle = FileFind.GetFileName();//�õ�Ŀ¼������Ϊ���صĽ��
            tree_Temp = m_tree.InsertItem(strTitle, 0, 0, tree_Root);
            ShowFile(tree_Temp, strPath);
        } else if(!FileFind.IsDirectory() && !FileFind.IsDots())//������ļ�
        {
            CString strPath = FileFind.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
            CString strTitle = FileFind.GetFileName();//�õ��ļ�������Ϊ���صĽ��
            m_tree.InsertItem(strTitle, 0, 0, tree_Temp);
        }
    }
    FileFind.Close();
}

void CTreeShowDlg::FindDir(HTREEITEM pItem, const CString& dirPath)//HTREEITEM Ϊһ��CtreeCtel�ڵ㣬�˴�ʵ�ֽ��ļ���ӳ�䵽CtreeCtrl�ؼ���ȥ

{
    CFileFind tempFind;
    BOOL bFound = tempFind.FindFile(dirPath + "\\*.*");

    HTREEITEM pItem2;
    if(pItem == NULL) {
        pItem2 = m_tree.InsertItem(dirPath, 0, 1, TVI_ROOT, TVI_LAST);//������ڵ�
    } else {
        pItem2 = m_tree.InsertItem(dirPath.Right(dirPath.GetLength() - dirPath.ReverseFind('\\') - 1), 0, 1, pItem, TVI_LAST);//�����ӽڵ�
    }
    while(bFound) {
        bFound = tempFind.FindNextFile();
        if(tempFind.IsDots())
            continue;

        tempFind.IsDirectory() ?
            FindDir(pItem2, tempFind.GetFilePath()) : //�ҵ������ļ���
            FindFile(pItem2, tempFind.GetFileName()); //�ҵ������ļ�
    }
    tempFind.Close();
}

void CTreeShowDlg::FindFile(HTREEITEM pItem, const CString& filePath)
{
    m_tree.InsertItem(filePath, 0, 1, pItem, TVI_LAST); //�����ӽڵ�
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
        MessageBox(_T("ɾ��Ŀ¼ʧ�ܣ�"), _T("������Ϣ"), MB_OK);
        return FALSE;
    }
    return TRUE;
}

void CTreeShowDlg::OnNMRClickTree_RCLICK(NMHDR* pNMHDR, LRESULT* pResult)
{
    CPoint point;
    GetCursorPos(&point);  //��ȡ�������
    m_tree.ScreenToClient(&point);//ӳ�䵽CtreeCtrl��
    UINT nFlags;
    HTREEITEM hItems = m_tree.HitTest(point, &nFlags);//���������ȡ�ڵ�
    CString m_treeChangeFile = m_tree.GetItemText(hItems);
    hItems = m_tree.GetParentItem(hItems);
    while(hItems) //ѭ����ȡ���ڵ㣬��ȡ�ڵ�ȫ·��
    {
        m_treeChangeFile = m_tree.GetItemText(hItems) + L"\\" + m_treeChangeFile;

        hItems = m_tree.GetParentItem(hItems);
    }

    // �˵���ʾ
    CMenu menu;
    menu.LoadMenu(IDR_MENU_TREE);
    CMenu* pPopup = menu.GetSubMenu(0);
    POINT	 pt;
    ::GetCursorPos((LPPOINT)&pt);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

    *pResult = 0;
}