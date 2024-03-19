
// DiskLog_makeGraphDlg.h: 헤더 파일
//

#pragma once

#include <vector>
#include <afx.h>
#include <string>


// CDiskLogmakeGraphDlg 대화 상자
class CDiskLogmakeGraphDlg : public CDialogEx
{
// 생성입니다.
public:
	CDiskLogmakeGraphDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISKLOG_MAKEGRAPH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedButtonBrowse();

private:
	std::wstring m_FilePath;
	CStatic m_StaticPath;
};
