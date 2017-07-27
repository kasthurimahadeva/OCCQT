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
#include <AIS_Shape.hxx>
#include <AIS_TextLabel.hxx>


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

    //! make box test.
    void makeBox(void);

    //! make cone test.
    void makeCone(void);

    //! make sphere test.
    void makeSphere(void);

    //! make cylinder test.
    void makeCylinder(void);

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

	void makeBottle(void);

	void makeBoxWithInput(void);

	



private:
    Ui::occQtClass ui;

    //! make cylindrical helix.
    void makeCylindricalHelix(void);

    //! make conical helix.
    void makeConicalHelix(void);

    //! make toroidal helix.
    void makeToroidalHelix(void);

	void displayASape(TopoDS_Shape topoDSShape);

	void makeTextLabel(const char* lableText, const gp_Pnt gpPnt, Standard_Real Xoffset,
		Standard_Real Yoffset,
		Standard_Real Zoffset);

	void removeDisplaiedAISShape();
	void removeDisplaiedAISLables();

private:
    //! the exit action.
    QAction* mExitAction;

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
	QAction* mMakeBottle;
	QAction* mMakeBoxWithInput;

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

    //! show the about info action.
    QAction* mAboutAction;

    //! the menus of the application.
    QMenu* mFileMenu;
    QMenu* mViewMenu;
    QMenu* mPrimitiveMenu;
    QMenu* mModelingMenu;
    QMenu* mHelpMenu;

    //! the toolbars of the application.
    QToolBar* mViewToolBar;
    QToolBar* mNavigateToolBar;
    QToolBar* mPrimitiveToolBar;
    QToolBar* mModelingToolBar;
    QToolBar* mHelpToolBar;

    // wrapped the widget for occ.
    OccView* myOccView;

	//vector<Handle(AIS_Shape)> handleAISShapes;
	QVector<Handle(AIS_Shape)> handleAISShapesVector;
	QVector<Handle(AIS_TextLabel)> handleAISLablesVector;
};

#endif // OCCQT_H
