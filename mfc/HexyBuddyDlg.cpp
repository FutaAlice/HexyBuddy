
// HexyBuddyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexyBuddy.h"
#include "HexyBuddyDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"
#include "shlwapi.h"
#include <functional>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexyBuddyDlg dialog
struct scopedGuard
{
public:
	scopedGuard(std::function<void()> cleanRoutine):
		func(cleanRoutine)
	{
	}
	~scopedGuard()
	{
		func();
	}
private:
	std::function<void()> func;
};


CHexyBuddyDlg::CHexyBuddyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexyBuddyDlg::IDD, pParent),
	targetWnd(0),
	targetProcessID(0),
	boardSize(0),
	pawnNum(0),
	gameOver(0),
	boardRatio(0.0),
	canvasCenterX(0),
	canvasCenterY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(hexygonCenterPairList, 0, sizeof(hexygonCenterPairList));
	memset(globalPosList, 0, sizeof(globalPosList));
}

void CHexyBuddyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAME_OVER, gameOver);
	DDX_Text(pDX, IDC_EDIT_BOARD_SIZE, boardSize);
	DDX_Text(pDX, IDC_EDIT_PAWN_NUM, pawnNum);
	DDX_Text(pDX, IDC_EDIT_BOARD_RATIO, boardRatio);
	DDX_Control(pDX, IDC_LIST_PAWN_POS_LIST, listPawnPos);
	DDX_Control(pDX, IDC_LIST_HEXYGON_CENTER_POS_PAIR_LIST, listHexygonPos);
	DDX_Control(pDX, IDC_COMBO_COL, comboBoxCol);
	DDX_Control(pDX, IDC_COMBO_ROW, comboBoxRow);
}

BEGIN_MESSAGE_MAP(CHexyBuddyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CHexyBuddyDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CHexyBuddyDlg::OnBnClickedButtonMove)
END_MESSAGE_MAP()


// CHexyBuddyDlg message handlers
#define WM_MSG_UPDATE_DATA WM_USER+0x400

BOOL CHexyBuddyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	EnableDebugPrivilege(GetCurrentProcess());
// 	SetTimer(WM_MSG_UPDATE_DATA, 200, 0);

	listPawnPos.InsertColumn(0, L"落子顺序",0,100);
	listPawnPos.InsertColumn(1, L"落子位置", 0, 150);

	listHexygonPos.InsertColumn(0, L"位置", 0, 100);
	listHexygonPos.InsertColumn(1, L"中心坐标", 0, 150);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexyBuddyDlg::OnPaint()
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
HCURSOR CHexyBuddyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHexyBuddyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==WM_MSG_UPDATE_DATA)
	{
		updateData();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CHexyBuddyDlg::updateData()
{
	if (!targetWnd || !targetProcessID || !IsWindow(targetWnd))
	{
		SetDlgItemText(IDC_STATIC_STATUS, L"未检测到Hexy程序，请确认已运行Hexy");
		findTargetWndAndPID();
		return;
	}
	SetDlgItemText(IDC_STATIC_STATUS, L"检测到Hexy程序");

	HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, targetProcessID);
	scopedGuard guard([&](){CloseHandle(hProc); });

	if (hProc)
	{
#define ARG(var) reinterpret_cast<void *>(&var),sizeof(var),var##_offset
		readRemoteMemory(hProc, ARG(pawnNum));
		readRemoteMemory(hProc, ARG(gameOver));
		readRemoteMemory(hProc, ARG(boardSize));
		readRemoteMemory(hProc, ARG(boardRatio));
		readRemoteMemory(hProc, ARG(canvasCenterX));
		readRemoteMemory(hProc, ARG(canvasCenterY));
		readRemoteMemory(hProc, ARG(globalPosList));
		readRemoteMemory(hProc, ARG(hexygonCenterPairList));

		CString s;
		listPawnPos.DeleteAllItems();
		for (int i = 0; i < pawnNum; ++i)
		{
			s.Format(L"%d", i + 1);
			listPawnPos.InsertItem(i, s);
			s.Format(L"%c-%2d", 'A' + globalPosList[i] % (boardSize + 2) - 1, globalPosList[i] / (boardSize + 2));
			listPawnPos.SetItemText(i, 1, s);
		}

		listHexygonPos.DeleteAllItems();
		for (int i = 0; i < (boardSize + 2)*(boardSize + 2); ++i)
		{
			s.Format(L"%d", i + 1);
			listHexygonPos.InsertItem(i, s);
			s.Format(L"(%.5lf,%.5lf)", hexygonCenterPairList[i * 2], hexygonCenterPairList[i * 2+1]);
			listHexygonPos.SetItemText(i, 1, s);
		}

		s.Format(L"Center:(%d,%d)", canvasCenterX, canvasCenterY);
		SetDlgItemText(IDC_STATIC_CANVAS_CENTER, s);
		fillComboBox();
		UpdateData(FALSE);
	}
}
#define TARGET_IAMGE_NAME	L"Hexy.exe"
#define TARGET_WINDOW_CAPTION	L"Hexy"

BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param)
{
	TCHAR wndCaption[MAX_PATH] = TEXT("");
	GetWindowText(hwnd, wndCaption, MAX_PATH);
	if (!wcscmp(wndCaption,TARGET_WINDOW_CAPTION))
	{
		CHexyBuddyDlg *dlg = reinterpret_cast<CHexyBuddyDlg *>(param);
		dlg->targetWnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

void CHexyBuddyDlg::findTargetWndAndPID()
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	scopedGuard guard([&](){CloseHandle(hProcess); });
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return;
	}
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcess, &entry))
	{
		return;
	}
	while (Process32Next(hProcess, &entry))
	{
		if (!wcscmp(TARGET_IAMGE_NAME,entry.szExeFile))
		{
			targetProcessID = entry.th32ProcessID;
		}
	}
	EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(this));

}

void CHexyBuddyDlg::EnableDebugPrivilege(HANDLE processHandle)
{
	HANDLE hToken;
	// 	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(processHandle, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		return;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
	}
}

bool CHexyBuddyDlg::readRemoteMemory(HANDLE hProc, void *dest, size_t cap, size_t address)
{
	size_t cnt = 0;
	if (!ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(address), dest, cap, reinterpret_cast<SIZE_T *>(&cnt)) || cnt != cap)
	{
		return false;
	}
	return true;
}


void CHexyBuddyDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here
	updateData();
}


void CHexyBuddyDlg::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	int col = comboBoxCol.GetCurSel();
	int row = comboBoxRow.GetCurSel();
	if (col+1<1 || col+1>boardSize || row+1<1 || row+1>boardSize)
	{
		return;
	}
	int pos = (row+1)*(boardSize+2) + (col+1);
	double x = hexygonCenterPairList[2 * pos] * boardRatio + canvasCenterX;
	double y = hexygonCenterPairList[2 * pos + 1] * boardRatio + canvasCenterY;

	LPARAM mousePos = short(x) | short(y) << 16;

	::SendMessage(targetWnd, WM_LBUTTONDOWN, 0, mousePos);
	::SendMessage(targetWnd, WM_LBUTTONUP, 0, mousePos);
}

void CHexyBuddyDlg::fillComboBox()
{
	comboBoxCol.ResetContent();
	comboBoxRow.ResetContent();
	CString s;
	for (int i = 0; i < boardSize;++i)
	{
		s.Format(L"%c", 'A' + i);
		comboBoxCol.AddString(s);
		s.Format(L"%d", i + 1);
		comboBoxRow.AddString(s);
	}

}
