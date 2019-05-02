
// integrationDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "integration.h"
#include "integrationDlg.h"
#include "afxdialogex.h"

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


// CintegrationDlg 대화 상자



CintegrationDlg::CintegrationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTEGRATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CintegrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_CHART, m_chartViewer);
}

BEGIN_MESSAGE_MAP(CintegrationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CintegrationDlg 메시지 처리기

BOOL CintegrationDlg::OnInitDialog()
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

	//------------------------------------------------------------------
	//  표
	//------------------------------------------------------------------

	BOOL m_bEditable = FALSE;
	BOOL m_bListMode = TRUE;
	int m_nRows = 5;
	int m_nCols = 6;
	int m_nFixRows = 1;
	int m_nFixCols = 0;

	m_Grid.SetEditable(m_bEditable);
	m_Grid.SetListMode(m_bListMode);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xFF)); // 원소들 배경색

	m_Grid.SetRowCount(m_nRows);
	m_Grid.SetColumnCount(m_nCols);
	m_Grid.SetFixedRowCount(m_nFixRows);
	m_Grid.SetFixedColumnCount(m_nFixCols);

	DWORD dwTextStyle = DT_RIGHT | DT_VCENTER | DT_CENTER | DT_SINGLELINE;
	CString title[6] = { L"이름", L"도착시간", L"BT", L"WT", L"TT", L"NTT" };

	// 릴레이션 스키마
	for (int row = 0; row < m_Grid.GetRowCount(); row++) {
		for (int col = 0; col < m_Grid.GetColumnCount(); col++) {
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows) {
				Item.nFormat = DT_CENTER;
				Item.strText.Format(title[col]);
			}

			m_Grid.SetItem(&Item);

			if (row == 0)
				m_Grid.SetItemBkColour(row, col, RGB(200, 200, 200)); // 배경색
			//m_Grid.SetItemFgColour(row, col, RGB(100, 100, 100)); // 글자색
		}
	}

	// 릴레이션 인스턴스
	for (int row = 1; row < m_Grid.GetRowCount(); row++) {
		for (int col = 0; col < m_Grid.GetColumnCount(); col++) {
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			Item.nFormat = dwTextStyle;
			// 인스턴스 값들 설정
			Item.strText.Format(L"%d",col);

			m_Grid.SetItem(&Item);

		}
	}
	// Make cell 1,1 read-only
	//m_Grid.SetItemState(1, 1, m_Grid.GetItemState(1, 1) | GVIS_READONLY);

	// 프로세스 개수
	m_Grid.AutoSize(5);

	// 줄마다 크기 설정
	m_Grid.SetRowHeight(0, 3 * m_Grid.GetRowHeight(0) / 2);
	//m_Grid.SetColumnWidth(2,10);

	m_Edit = (CEdit*)GetDlgItem(IDC_GRID);

	// 평균 TT구하기
	float tt = 55.55f;
	CString ttString;
	ttString.Format(_T("평균 TT : %f"), tt);
	m_Edit->SetWindowText(ttString);


	//------------------------------------------------------------------
	//  그래프
	//------------------------------------------------------------------

	// 프로세스 이름(이름 개수대로 생성)
	const char *labels[] = { "Process1", "Process2", "Process3" };

	// The task index, start date, end date and color for each bar
	double taskNo[] = { 1, 0, 2, 1 };
	double startData[] = { 0, 3, 8, 5 };
	double endData[] = { 3, 5, 13, 8 };
	// 그래프바 색깔
	int colors[] = { 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff, 0x0000ff };

	// 사이즈와 배경색 조절(x,y,배경색,테두리색, 그림자길이(3d느낌))
	XYChart *c = new XYChart(600, 300, 0xffffcc, 0xffffff, 0);

	// 타이틀 이름, 폰트, 사이즈, 색, 타이틀배경색
	c->addTitle("Process Progressing !", "timesbi.ttf", 15, 0xffffff)->setBackground(0x555555);

	// Set the plotarea at (140, 55) and of size 460 x 200 pixels. Use alternative white/grey
	// background. Enable both horizontal and vertical grids by setting their colors to grey
	// (c0c0c0). Set vertical major grid (represents month boundaries) 2 pixels in width
	c->setPlotArea(100, 55, 460, 100, 0xffffff, 0xeeeeee, Chart::LineColor, 0xc0c0c0, 0xc0c0c0
	)->setGridWidth(2, 1, 1, 1);

	// swap the x and y axes to create a horziontal box-whisker chart
	c->swapXY();

	// x축 최대 길이 설정 및 점선 거리
	c->yAxis()->setDateScale(0, 15, 1);

	// Set the y-axis to shown on the top (right + swapXY = top)
	c->setYAxisOnRight();

	// Set the labels on the x axis
	c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

	// Reverse the x-axis scale so that it points downwards.
	c->xAxis()->setReverse();

	// Set the horizontal ticks and grid lines to be between the bars
	c->xAxis()->setTickOffset(0.5);

	// Add a multi-color box-whisker layer to represent the gantt bars
	BoxWhiskerLayer *layer = c->addBoxWhiskerLayer2(DoubleArray(startData, (int)(sizeof(startData) / sizeof(startData[0]))), DoubleArray(endData, (int)(sizeof(endData) / sizeof(endData[0]))),
		DoubleArray(), DoubleArray(), DoubleArray(), IntArray(colors, (int)(sizeof(colors) / sizeof(
			colors[0]))));
	layer->setXData(DoubleArray(taskNo, (int)(sizeof(taskNo) / sizeof(taskNo[0]))));
	layer->setBorderColor(Chart::SameAsMainColor);

	// Divide the plot area height ( = 200 in this chart) by the number of tasks to get the height
	// of each slot. Use 80% of that as the bar height.
	layer->setDataWidth(100 * 4 / 5 / (int)(sizeof(labels) / sizeof(labels[0])));

	// 항목 이름 나타내기
	LegendBox *legendBox = c->addLegend2(100, 250, Chart::AutoGrid, "arialbd.ttf", 10);
	legendBox->setWidth(461);
	legendBox->setBackground(0xdddddd);

	legendBox->addKey("Process", 0x00cc00);
	legendBox->addKey("Process1", 0x0000cc);
	legendBox->addKey("Process2", 0xcc0000);



	// Output the chart
	c->makeChart();
	m_chartViewer.setChart(c);

	delete c;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CintegrationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CintegrationDlg::OnPaint()
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
HCURSOR CintegrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

