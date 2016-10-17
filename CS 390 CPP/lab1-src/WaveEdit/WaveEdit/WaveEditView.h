
// WaveEditView.h : interface of the CWaveEditView class
//

#pragma once
#include <stack>

class CWaveEditView : public CScrollView
{
	bool mousePressed;
	int selectionStart; // Selected sample start
	int selectionEnd; // Selected sample end

protected: // create from serialization only
	CWaveEditView();
	DECLARE_DYNCREATE(CWaveEditView)

// Attributes
public:
	CWaveEditDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWaveEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolsPlayselected();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnScalingZoomin();
	afx_msg void OnScalingZoomout();
	afx_msg void OnToolsEcho();
	afx_msg void OnToolsSpeedup();
	afx_msg void OnToolsSpeeddown();
	afx_msg void OnToolsReverse();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
};

#ifndef _DEBUG  // debug version in WaveEditView.cpp
inline CWaveEditDoc* CWaveEditView::GetDocument() const
   { return reinterpret_cast<CWaveEditDoc*>(m_pDocument); }
#endif

