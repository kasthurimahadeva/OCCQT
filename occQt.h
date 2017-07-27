/*
*    Copyright (c) 2013 to current year. All Rights Reserved.
*
*           File : occQt.cpp
*         Author : eryar@163.com
*           Date : 2014-11-29 10:18
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : OpenCASCADE in Qt.
*
*/

#ifndef OCCQT_H
#define OCCQT_H


#include "ui_occQt.h"

#include <AIS_InteractiveContext.hxx>

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

#include <STEPControl_Reader.hxx>

#include <IGESControl_Reader.hxx>
class OccView;

//! Qt main window which include OpenCASCADE for its central widget.
class occQt : public QMainWindow
{
	Q_OBJECT

public:
	//! constructor/destructor.
	occQt(QWidget *parent = 0);
	~occQt();



protected:
	//! create all the actions.
	void createActions(void);

	//! create all the menus.
	void createMenus(void);

	//! create the toolbar.
	void createToolBars(void);

	private slots:
	//! show about box.
	void about(void);

	//! import files
	void importBrepFiles(void);
	void importIgesFiles(void);
	void importStepFiles(void);

	//! export files
	void exportBrepFiles(void);
	void exportIgesFiles(void);
	void exportStepFiles(void);

	//! make box test.
	void makeBox(void);

	//! make cone test.
	void makeCone(void);

	//! make sphere test.
	void makeSphere(void);

	//! make cylinder test.
	void makeCylinder(void);

	//! make bottle test.
	void makeBottle(void);

	void displayShape(TopoDS_Shape shape);

	void removeAIS_Shape(TopoDS_Shape shape);

	TopoDS_Shape getLastShape();

	//! make torus test.
	void makeTorus(void);

	//! fillet test.
	void makeFillet(void);

	//! chamfer test.
	void makeChamfer(void);

	//! test extrude algorithm.
	void makeExtrude(void);

	//! test revol algorithm.
	void makeRevol(void);

	//! test loft algorithm.
	void makeLoft(void);

	//! test boolean operation cut.
	void testCut(void);

	//! test boolean operation fuse.
	void testFuse(void);

	//! test boolean operation common.
	void testCommon(void);

	//! test helix shapes.
	void testHelix(void);

	//! make box test
	void makeBoxFromUserInputs(void);

	void filletOption(void);

	void drillOption(void);


private:
	Ui::occQtClass ui;

	//! make cylindrical helix.
	void makeCylindricalHelix(void);

	//! make conical helix.
	void makeConicalHelix(void);

	//! make toroidal helix.
	void makeToroidalHelix(void);

private:
	//! the exit action.
	QAction* mExitAction;

	//!import file actions
	QAction* importBrepFilesAction;
	QAction* importIgesFilesAction;
	QAction* importStepFilesAction;

	//!export file actions
	QAction* exportBrepFilesAction;
	QAction* exportIgesFilesAction;
	QAction* exportStepFilesAction;

	//! the actions for the view: pan, reset, fitall.
	QAction* mViewZoomAction;
	QAction* mViewPanAction;
	QAction* mViewRotateAction;
	QAction* mViewResetAction;
	QAction* mViewFitallAction;

	//! the actions to test the OpenCASCADE modeling algorithms.
	QAction* mMakeBoxAction;
	QAction* mMakeConeAction;
	QAction* mMakeSphereAction;
	QAction* mMakeCylinderAction;
	QAction* mMakeTorusAction;
	QAction* camber;
	QAction* mMakeBottleAction;

	//! make a fillet box.
	QAction* mFilletAction;
	QAction* mChamferAction;
	QAction* mExtrudeAction;
	QAction* mRevolveAction;
	QAction* mLoftAction;

	//! boolean operations.
	QAction* mCutAction;
	QAction* mFuseAction;
	QAction* mCommonAction;

	//! helix shapes.
	QAction* myHelixAction;

	//! make box
	QAction* mBoxAction;


	//! show the about info action.
	QAction* mAboutAction;

	//! zoom
	QAction* mZoomAction;

	//! Rotate
	QAction* mRotateAction;

	//! Pan
	QAction* mPanAction;

	QAction* addFilletAction;

	QAction* addDrillAction;

	//! Select Face
	QAction* mSelectFaceAction;

	//! the menus of the application.
	QMenu* mFileMenu;
	QMenu* importMenu;
	QMenu* exportMenu;
	QMenu* mViewMenu;
	QMenu* mPrimitiveMenu;
	QMenu* mModelingMenu;
	QMenu* mHelpMenu;
	QMenu* mMakeBoxMenu;
	QMenu* mEditingMenu;

	//! the toolbars of the application.
	QToolBar* mViewToolBar;
	QToolBar* mNavigateToolBar;
	QToolBar* mPrimitiveToolBar;
	QToolBar* mModelingToolBar;
	QToolBar* mHelpToolBar;
	QToolBar* mMakeBoxToolBar;
	QToolBar* mEditingToolBar;

	// wrapped the widget for occ.
	OccView* myOccView;


	TopoDS_Shape box;
	TopoDS_Shape filletBox;
	TopoDS_Shape drillBox;
	TopoDS_Shape onlyDrilledBox;

	QVector<Handle(AIS_Shape)> aisShapeVector;

	bool isFilleted;
	bool isDrilled;
	TopoDS_Shape lastShape;

	TopoDS_Shape brepFileShape;
	TopoDS_Shape igesFileShape;
	TopoDS_Shape stepFileShape;
};

#endif // OCCQT_H
