
// HexyBuddyDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CHexyBuddyDlg dialog
class CHexyBuddyDlg : public CDialogEx
{
	friend BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param);

// Construction
public:
	CHexyBuddyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HEXYBUDDY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HWND targetWnd;
	DWORD targetProcessID;
	
	int boardSize;
	int globalPosList[128];
	int pawnNum;
	int gameOver;
	int canvasCenterX;
	int canvasCenterY;
	double boardRatio;
	double hexygonCenterPairList[13*13 * 2];

	enum
	{
		pawnNum_offset = 0x01B0C5D0,
		boardSize_offset = 0x0042303C,
		boardRatio_offset = 0x01B0BFC8,
		gameOver_offset = 0x01B0CCE0,
		canvasCenterX_offset = 0x01b0c328,
		canvasCenterY_offset = 0x01b0c32c,
		globalPosList_offset = 0x01B073A0,
		hexygonCenterPairList_offset = 0x01B075C0,
	};


	void updateData();
	void findTargetWndAndPID();
	void EnableDebugPrivilege(HANDLE processHandle);
	bool readRemoteMemory(HANDLE hProc, void *dest,size_t cap,size_t address);
	void fillComboBox();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl listPawnPos;
	CListCtrl listHexygonPos;
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonMove();
	CComboBox comboBoxCol;
	CComboBox comboBoxRow;
};
