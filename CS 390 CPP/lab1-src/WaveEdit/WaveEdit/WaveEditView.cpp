
// WaveEditView.cpp : implementation of the CWaveEditView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"
#include "WaveEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::stack<WaveFile> undoStack;
std::stack<WaveFile> redoStack;

// CWaveEditView

IMPLEMENT_DYNCREATE(CWaveEditView, CScrollView)

BEGIN_MESSAGE_MAP(CWaveEditView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOOLS_PLAYSELECTED, &CWaveEditView::OnToolsPlayselected)
	ON_COMMAND(ID_EDIT_CUT, &CWaveEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CWaveEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWaveEditView::OnEditPaste)
	ON_COMMAND(ID_SCALING_ZOOMIN, &CWaveEditView::OnScalingZoomin)
	ON_COMMAND(ID_SCALING_ZOOMOUT, &CWaveEditView::OnScalingZoomout)
	ON_COMMAND(ID_TOOLS_ECHO32783, &CWaveEditView::OnToolsEcho)
	ON_COMMAND(ID_TOOLS_SPEEDUP32784, &CWaveEditView::OnToolsSpeedup)
	ON_COMMAND(ID_TOOLS_SPEEDDOWN32785, &CWaveEditView::OnToolsSpeeddown)
	ON_COMMAND(ID_TOOLS_REVERSE, &CWaveEditView::OnToolsReverse)
	ON_COMMAND(ID_EDIT_UNDO, &CWaveEditView::OnEditUndo)
	ON_COMMAND(ID_EDIT_SREDO, &CWaveEditView::OnEditRedo)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

WaveFile * clipboard;
bool zoomIn = false;
bool zoomInDone = false;
bool zoomOut = false;
bool zoomOutDone = true;

CWaveEditView::CWaveEditView()
{
	// TODO: add construction code here
	mousePressed = false;
	selectionStart = 0;
	selectionEnd = 0;
}

CWaveEditView::~CWaveEditView()
{
}

BOOL CWaveEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CWaveEditView::OnInitialUpdate() {
	CScrollView::OnInitialUpdate();
	// Initial scroll sizes
	CSize sizeTotal;
	sizeTotal.cx = 10000;
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// CWaveEditView drawing

void CWaveEditView::OnDraw(CDC* pDC)
{
	CWaveEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WaveFile * wave = &pDoc->wave;
	if (wave->hdr == NULL) {
		return;
	}
	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);

	COLORREF color = RGB(255, 200, 200);
	CPen pen1(PS_SOLID, 0, color);
	pDC->SelectObject(&pen1);
	CBrush brush1(color);
	pDC->SelectObject(&brush1);
	// Draw selection if any
	if (selectionStart != selectionEnd) {
		pDC->Rectangle(selectionStart, 0, selectionEnd, rect.Height());
	}

	// Set color in pen and brush for wave
	color = RGB(0, 255, 0);
	CPen pen2(PS_SOLID, 0, color);
	pDC->SelectObject(&pen2);
	CBrush brush2(color);
	pDC->SelectObject(&brush2);
	// Draw the wave
	pDC->MoveTo(0, 0);
	int x;
	for (x = 0; x < rect.Width(); x++) {
		// Assuming the whole file will be fit in the window, for every x value in the window
		// we need to find the equivalent sample in the wave file.
		float val = wave->get_sample((int)(x*wave->lastSample / rect.Width()));
		// We need to fit the sound also in the y axis. The y axis goes from 0 in the
		// top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
		// we scale it in that way.
		int y = (int)((val + 32768) * (rect.Height() - 1) / (32767 + 32768));

		//This is where the scaling is being done. So we can zoom in and out here.
		if (zoomIn) {
			pDC->LineTo(2 * x, rect.Height() - y);
		}
		else if (zoomOut) {
			pDC->LineTo(x / 2, rect.Height() - y);
		}
		else if (zoomOutDone || zoomInDone) {
			pDC->LineTo(x, rect.Height() - y);
		}
	}

	// TODO: add draw code for native data here
}


// CWaveEditView printing

BOOL CWaveEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaveEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaveEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWaveEditView diagnostics

#ifdef _DEBUG
void CWaveEditView::AssertValid() const
{
	CView::AssertValid();
}

void CWaveEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaveEditDoc* CWaveEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveEditDoc)));
	return (CWaveEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaveEditView message handlers


void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = true;
	selectionStart = point.x;
	selectionEnd = point.x;
	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = false;
	this->selectionEnd = point.x;
	this->RedrawWindow();
	CScrollView::OnLButtonUp(nFlags, point);
}

void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);

	if (mousePressed) {
		selectionEnd = point.x;
		RedrawWindow();
	}
}

void CWaveEditView::OnToolsPlayselected()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc) {
		return;
	}

	WaveFile * wave = &pDoc->wave;

	if (wave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	double startms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionEnd / rect.Width();

	WaveFile * nWave = wave->get_fragment(startms, endms);

	nWave->play();
}

void CWaveEditView::OnEditCut()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	WaveFile * wave = &pDoc->wave;

	if (wave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	double startms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionEnd / rect.Width();

	clipboard = wave->get_fragment(startms, endms);

	WaveFile * nWave = wave->remove_fragment(startms, endms);

	//Add file before change to undoStack
	undoStack.push(pDoc->wave);

	pDoc->wave = *nWave;

	this->RedrawWindow();
}


void CWaveEditView::OnEditCopy()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc) {
		return;
	}

	WaveFile * wave = &pDoc->wave;

	if (wave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	double startms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionEnd / rect.Width();

	clipboard = wave->get_fragment(startms, endms);
}


void CWaveEditView::OnEditPaste()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc) {
		return;
	}

	WaveFile * wave = &pDoc->wave;

	if (wave->hdr == NULL) {
		return;
	}

	if (clipboard == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	double startms = (1000.0 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();

	WaveFile * nWave = wave->paste_fragment(startms, clipboard);

	undoStack.push(pDoc->wave);

	pDoc->wave = *nWave;

	wave->updateHeader();

	this->RedrawWindow();
}

void CWaveEditView::OnScalingZoomin()
{
	// TODO: Add your command handler code here
	if (zoomOut) {
		zoomInDone = true;
		zoomOut = false;
		this->RedrawWindow();
		return;
	}

	zoomIn = true;
	zoomOut = false;

	this->RedrawWindow();
}

void CWaveEditView::OnScalingZoomout()
{
	// TODO: Add your command handler code here
	if (zoomIn) {
		zoomOutDone = true;
		zoomIn = false;
		this->RedrawWindow();
		return;
	}

	zoomOut = true;
	zoomIn = false;

	this->RedrawWindow();
}


void CWaveEditView::OnToolsEcho()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc) {
		return;
	}

	WaveFile * nWave = &pDoc->wave;

	if (nWave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	float atenuation = 0.2f;
	float delay = 100;

	double startms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionEnd / rect.Width();

	undoStack.push(pDoc->wave);

	if (startms != endms) {
		WaveFile * toModify = new WaveFile(nWave->numChannels, nWave->sampleRate, nWave->bitsPerSample);
		toModify = nWave->get_fragment(startms, endms);
		toModify->updateHeader();
		toModify = toModify->echo(atenuation, delay);
		toModify->updateHeader();
		//toModify->play();

		nWave = nWave->replace_fragment(startms, endms, toModify);
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}
	else {
		nWave = nWave->echo(atenuation, delay);
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}

	this->selectionStart = 0;
	this->selectionEnd = 0;

	this->RedrawWindow();
}


void CWaveEditView::OnToolsSpeedup()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc) {
		return;
	}

	WaveFile * nWave = &pDoc->wave;

	if (nWave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	float speed = 2;

	double startms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionEnd / rect.Width();

	undoStack.push(pDoc->wave);

	if (startms != endms) {
		WaveFile * toModify = new WaveFile(nWave->numChannels, nWave->sampleRate, nWave->bitsPerSample);
		toModify = nWave->get_fragment(startms, endms);
		toModify = toModify->changeSpeed(speed);

		nWave = nWave->replace_fragment(startms, endms, toModify);
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}
	else {
		nWave = nWave->changeSpeed(speed);
		pDoc->wave = *nWave;
	}

	this->selectionStart = 0;
	this->selectionEnd = 0;

	this->RedrawWindow();
}


void CWaveEditView::OnToolsSpeeddown()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc) {
		return;
	}

	WaveFile * nWave = &pDoc->wave;

	if (nWave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	float speed = 0.5;

	double startms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionEnd / rect.Width();

	undoStack.push(pDoc->wave);

	if (startms != endms) {
		WaveFile * toModify = new WaveFile(nWave->numChannels, nWave->sampleRate, nWave->bitsPerSample);
		toModify = nWave->get_fragment(startms, endms);
		toModify = toModify->changeSpeed(speed);

		nWave = nWave->replace_fragment(startms, endms, toModify);
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}
	else {
		nWave = nWave->changeSpeed(speed);
		pDoc->wave = *nWave;
	}

	this->selectionStart = 0;
	this->selectionEnd = 0;

	this->RedrawWindow();
}

void CWaveEditView::OnToolsReverse()
{
	// TODO: Add your command handler code here
	CWaveEditDoc * pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc) {
		return;
	}

	WaveFile * nWave = &pDoc->wave;

	if (nWave->hdr == NULL) {
		return;
	}

	CRect rect;
	GetClientRect(rect);

	double startms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionStart / rect.Width();
	double endms = (1000.0 * nWave->lastSample / nWave->sampleRate) * this->selectionEnd / rect.Width();

	undoStack.push(pDoc->wave);

	if (startms != endms) {
		WaveFile * toModify = new WaveFile(nWave->numChannels, nWave->sampleRate, nWave->bitsPerSample);
		toModify = nWave->get_fragment(startms, endms);
		toModify = toModify->reverse();
		toModify->updateHeader();

		nWave = nWave->replace_fragment(startms, endms, toModify);
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}
	else {
		nWave = nWave->reverse();
		nWave->updateHeader();
		pDoc->wave = *nWave;
	}

	this->selectionStart = 0;
	this->selectionEnd = 0;

	this->RedrawWindow();
}


void CWaveEditView::OnEditUndo()
{
	// TODO: Add your command handler code here
	if (undoStack.empty())
		return;

	redoStack.push(undoStack.top());

	CWaveEditDoc * pDoc = GetDocument();

	if (!pDoc)
		return;

	redoStack.push(pDoc->wave);

	pDoc->wave = undoStack.top();

	undoStack.pop();
}




void CWaveEditView::OnEditRedo()
{
	// TODO: Add your command handler code here
	if (redoStack.empty())
		return;

	CWaveEditDoc * pDoc = GetDocument();

	if (!pDoc)
		return;

	undoStack.push(pDoc->wave);

	pDoc->wave = redoStack.top();

	redoStack.pop();
}
