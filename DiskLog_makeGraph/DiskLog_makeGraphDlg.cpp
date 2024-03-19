
// DiskLog_makeGraphDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DiskLog_makeGraph.h"
#include "DiskLog_makeGraphDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CDiskLogmakeGraphDlg 대화 상자



CDiskLogmakeGraphDlg::CDiskLogmakeGraphDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISKLOG_MAKEGRAPH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiskLogmakeGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC_Path, m_StaticPath);
}

BEGIN_MESSAGE_MAP(CDiskLogmakeGraphDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CDiskLogmakeGraphDlg::OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDiskLogmakeGraphDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CDiskLogmakeGraphDlg 메시지 처리기

BOOL CDiskLogmakeGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDiskLogmakeGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDiskLogmakeGraphDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDiskLogmakeGraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDiskLogmakeGraphDlg::OnBnClickedButtonConvert()
{
    if (m_FilePath.empty())
    {
        AfxMessageBox(L"변환할 파일을 선택하세요.");
        return;
    }

    std::wstring filePath = m_FilePath;
    std::wstring savePath;
    size_t found = filePath.find_last_of(L"/\\");
    if (found != std::wstring::npos) {
        savePath = filePath.substr(0, found);
    }
    savePath += L"\\";

    // 파일 이름 추출 코드
    std::wstring fileName = L"";
    for (int i = filePath.size() - 1; i >= 0; i--)
    {
        if (filePath[i] == '\\')
        {
            for (int j = i + 1; j < filePath.size(); j++)
            {
                if (filePath[j] == '.')
                    break;

                fileName += filePath[j];
            }
            break;
        }
    }

    std::wstring graphFileName = fileName + L" graph" + L".csv";

    if (std::filesystem::exists(savePath + graphFileName))
    {
        AfxMessageBox(L"파일이 이미 존재합니다.");
        return;
    }

    std::wifstream file(filePath);

    if (file.is_open())
    {
        std::wofstream outputFile(savePath + graphFileName);
        if (outputFile.is_open())
        {
            std::wstring line;
            //csv파일에 열쓰기
            bool writingColumn = false;
            bool writingFirst = true;
            while (getline(file, line))
            {
                if (line == L"---------------------------------------------------")
                {
                    if (!writingColumn)
                    {
                        writingColumn = true;
                        continue;
                    }
                    else
                    {
                        writingColumn = false;
                        break;
                    }
                }

                if (writingColumn && writingFirst)
                {
                    outputFile << L"Time,";
                    writingFirst = false;
                    continue;
                }
                else if (writingColumn && !writingFirst)
                {
                    bool start = false;
                    std::wstring temp = L"";
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (i == 0)
                        {
                            if (line[i] == 'n')
                            {
                                outputFile << L"GPU";
                                break;
                            }
                        }

                        if (line[i] == ':' && line[i - 1] == ' ' && line[i + 1] == ' ')
                        {
                            break;
                        }

                        temp += line[i];
                    }
                    outputFile << temp;
                    outputFile << L" , ";
                }
            }

            line.clear();
            outputFile << L"\n";

            bool writingRow = false;
            int LogLine = 0;

            file.clear();
            file.seekg(0, std::ios::beg);

            while (getline(file, line))
            {
                if (line == L"---------------------------------------------------")
                {
                    if (!writingRow)
                    {
                        writingRow = true;
                        LogLine = 0;
                        continue;
                    }
                    else
                    {
                        writingRow = false;
                        outputFile << L"\n";
                        continue;
                    }
                }

                if (writingRow)
                {
                    bool start = false;
                    std::wstring temp = L"";
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (LogLine == 0)
                        {
                            if (i == 0)
                                continue;

                            if (i == line.size() - 1)
                            {
                                break;
                            }

                            temp += line[i];
                            continue;
                        }

                        if (line[i] == ':' && line[i - 1] == ' ' && line[i + 1] == ' ')
                        {
                            start = true;
                            continue;
                        }

                        if (start)
                            temp += line[i];
                    }

                    if (LogLine != 0)
                    {
                        const wchar_t charsToRemove[] = { ' ', '"' };
                        for (wchar_t c : charsToRemove) {
                            temp.erase(std::remove(temp.begin(), temp.end(), c), temp.end());
                        }
                    }

                    outputFile << temp;
                    outputFile << ',';
                }

                LogLine++;
            }

            outputFile.close();
            file.close();
            
            AfxMessageBox(L"파일을 생성하였습니다.");
        }
        else
        {
            AfxMessageBox(L" 파일을 열 수 없습니다.");
        }
    }
    else
    {
        AfxMessageBox(L"해당 경로의 파일을 열 수 없습니다.");
    }
}

void CDiskLogmakeGraphDlg::OnBnClickedButtonBrowse()
{
    CFileDialog fileDlg(TRUE);
    INT_PTR result = fileDlg.DoModal();
    if (result == IDOK)
    {
        m_FilePath = fileDlg.GetPathName();

        LPCTSTR filePath = m_FilePath.c_str();

        m_StaticPath.SetWindowText(filePath);

    }
}

