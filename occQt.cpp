/*
*    Copyright (c) 2014 eryar All Rights Reserved.
*
*           File : occQt.cpp
*         Author : eryar@163.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : Qt main window for OpenCASCADE.
*/

#include "occQt.h"
#include "occView.h"

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <BRepTools.hxx>

#include <AIS_Shape.hxx>

#include <Geom_TrimmedCurve.hxx>

#include <GC_MakeSegment.hxx>

#include <GC_MakeArcOfCircle.hxx>

#include <Geom_Plane.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <QDialog>
#include <QInputDialog>
#include <QFormLayout>
#include <Qlabel>
#include <QDialogButtonBox>
#include <Qdebug>

#include <BRepOffsetAPI_ThruSections.hxx>

#include <QMessageBox> 

#include <GProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

#include <AIS_InteractiveObject.hxx>

#include <qcombobox.h> 

#include <gp_Vec.hxx>

#include <QFileDialog> 

#include <QUrl> 

#include <STEPControl_Reader.hxx>

#include <IGESControl_Reader.hxx>

#include <STEPControl_Writer.hxx>

#include <IGESControl_Writer.hxx>


occQt::occQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	myOccView = new OccView(this);

	setCentralWidget(myOccView);

	createActions();
	createMenus();
	createToolBars();
}

occQt::~occQt()
{

}

void occQt::createActions(void)
{
	mExitAction = new QAction(tr("Exit"), this);
	mExitAction->setShortcut(tr("Ctrl+Q"));
	mExitAction->setIcon(QIcon(":/Resources/close.png"));
	mExitAction->setStatusTip(tr("Exit the application"));
	connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

	importBrepFilesAction = new QAction(tr("BREF"), this);
	importBrepFilesAction->setStatusTip(tr("Import BREF files"));
	connect(importBrepFilesAction, SIGNAL(triggered()), this, SLOT(importBrepFiles()));


	importIgesFilesAction = new QAction(tr("IGES"), this);
	importIgesFilesAction->setStatusTip(tr("Import IGES files"));
	connect(importIgesFilesAction, SIGNAL(triggered()), this, SLOT(importIgesFiles()));

	importStepFilesAction = new QAction(tr("STEP"), this);
	importStepFilesAction->setStatusTip(tr("Import STEP files"));
	connect(importStepFilesAction, SIGNAL(triggered()), this, SLOT(importStepFiles()));

	exportBrepFilesAction = new QAction(tr("BREF"), this);
	exportBrepFilesAction->setStatusTip(tr("Export BREF files"));
	connect(exportBrepFilesAction, SIGNAL(triggered()), this, SLOT(exportBrepFiles()));

	exportIgesFilesAction = new QAction(tr("IGES"), this);
	exportIgesFilesAction->setStatusTip(tr("Export IGES files"));
	connect(exportIgesFilesAction, SIGNAL(triggered()), this, SLOT(exportIgesFiles()));

	exportStepFilesAction = new QAction(tr("STEP"), this);
	exportStepFilesAction->setStatusTip(tr("Export STEP files"));
	connect(exportStepFilesAction, SIGNAL(triggered()), this, SLOT(exportStepFiles()));

	mViewZoomAction = new QAction(tr("Zoom"), this);
	mViewZoomAction->setIcon(QIcon(":/Resources/Zoom.png"));
	mViewZoomAction->setStatusTip(tr("Zoom the view"));
	connect(mViewZoomAction, SIGNAL(triggered()), myOccView, SLOT(zoom()));

	mViewPanAction = new QAction(tr("Pan"), this);
	mViewPanAction->setIcon(QIcon(":/Resources/Pan.png"));
	mViewPanAction->setStatusTip(tr("Pan the view"));
	connect(mViewPanAction, SIGNAL(triggered()), myOccView, SLOT(pan()));

	mViewRotateAction = new QAction(tr("Rotate"), this);
	mViewRotateAction->setIcon(QIcon(":/Resources/Rotate.png"));
	mViewRotateAction->setStatusTip(tr("Rotate the view"));
	connect(mViewRotateAction, SIGNAL(triggered()), myOccView, SLOT(rotate()));

	mViewResetAction = new QAction(tr("Reset"), this);
	mViewResetAction->setIcon(QIcon(":/Resources/Home.png"));
	mViewResetAction->setStatusTip(tr("Reset the view"));
	connect(mViewResetAction, SIGNAL(triggered()), myOccView, SLOT(reset()));

	mViewFitallAction = new QAction(tr("Fit All"), this);
	mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
	mViewFitallAction->setStatusTip(tr("Fit all "));
	connect(mViewFitallAction, SIGNAL(triggered()), myOccView, SLOT(fitAll()));

	mMakeBoxAction = new QAction(tr("Box"), this);
	mMakeBoxAction->setIcon(QIcon(":/Resources/box.png"));
	mMakeBoxAction->setStatusTip(tr("Make a box"));
	connect(mMakeBoxAction, SIGNAL(triggered()), this, SLOT(makeBox()));

	mMakeConeAction = new QAction(tr("Cone"), this);
	mMakeConeAction->setIcon(QIcon(":/Resources/cone.png"));
	mMakeConeAction->setStatusTip(tr("Make a cone"));
	connect(mMakeConeAction, SIGNAL(triggered()), this, SLOT(makeCone()));

	mMakeSphereAction = new QAction(tr("Sphere"), this);
	mMakeSphereAction->setStatusTip(tr("Make a sphere"));

	mMakeSphereAction->setIcon(QIcon(":/Resources/sphere.png"));
	connect(mMakeSphereAction, SIGNAL(triggered()), this, SLOT(makeSphere()));

	mMakeCylinderAction = new QAction(tr("Cylinder"), this);
	mMakeCylinderAction->setStatusTip(tr("Make a cylinder"));
	mMakeCylinderAction->setIcon(QIcon(":/Resources/cylinder.png"));
	connect(mMakeCylinderAction, SIGNAL(triggered()), this, SLOT(makeCylinder()));

	mMakeBottleAction = new QAction(tr("Bottle"), this);
	mMakeBottleAction->setStatusTip(tr("Make a bottle"));
	mMakeBottleAction->setIcon(QIcon(":/Resources/bottle.png"));
	connect(mMakeBottleAction, SIGNAL(triggered()), this, SLOT(makeBottle()));

	mBoxAction = new QAction(tr("Box"), this);
	mBoxAction->setStatusTip(tr("Make a box"));
	mBoxAction->setIcon(QIcon(":/Resources/box.png"));
	connect(mBoxAction, SIGNAL(triggered()), this, SLOT(makeBoxFromUserInputs()));

	camber = new QAction(tr("Dialog Input camber"), this);
	camber->setStatusTip(tr("Input camber"));
	camber->setIcon(QIcon(":/Resources/cylinder.png"));
	connect(camber, SIGNAL(triggered()), this, SLOT(ShowCamberInputDialog()));

	mMakeTorusAction = new QAction(tr("Torus"), this);
	mMakeTorusAction->setStatusTip(tr("Make a torus"));
	mMakeTorusAction->setIcon(QIcon(":/Resources/torus.png"));
	connect(mMakeTorusAction, SIGNAL(triggered()), this, SLOT(makeTorus()));

	mFilletAction = new QAction(tr("Fillet"), this);
	mFilletAction->setIcon(QIcon(":/Resources/fillet.png"));
	mFilletAction->setStatusTip(tr("Test Fillet algorithm"));
	connect(mFilletAction, SIGNAL(triggered()), this, SLOT(makeFillet()));

	mChamferAction = new QAction(tr("Chamfer"), this);
	mChamferAction->setIcon(QIcon(":/Resources/chamfer.png"));
	mChamferAction->setStatusTip(tr("Test chamfer algorithm"));
	connect(mChamferAction, SIGNAL(triggered()), this, SLOT(makeChamfer()));

	mExtrudeAction = new QAction(tr("Extrude"), this);
	mExtrudeAction->setIcon(QIcon(":/Resources/extrude.png"));
	mExtrudeAction->setStatusTip(tr("Test extrude algorithm"));
	connect(mExtrudeAction, SIGNAL(triggered()), this, SLOT(makeExtrude()));

	mRevolveAction = new QAction(tr("Revolve"), this);
	mRevolveAction->setIcon(QIcon(":/Resources/revolve.png"));
	mRevolveAction->setStatusTip(tr("Test revol algorithm"));
	connect(mRevolveAction, SIGNAL(triggered()), this, SLOT(makeRevol()));

	mLoftAction = new QAction(tr("Loft"), this);
	mLoftAction->setIcon(QIcon(":/Resources/loft.png"));
	mLoftAction->setStatusTip(tr("Test loft algorithm"));
	connect(mLoftAction, SIGNAL(triggered()), this, SLOT(makeLoft()));

	mCutAction = new QAction(tr("Cut"), this);
	mCutAction->setIcon(QIcon(":/Resources/cut.png"));
	mCutAction->setStatusTip(tr("Boolean operation cut"));
	connect(mCutAction, SIGNAL(triggered()), this, SLOT(testCut()));

	mFuseAction = new QAction(tr("Fuse"), this);
	mFuseAction->setIcon(QIcon(":/Resources/fuse.png"));
	mFuseAction->setStatusTip(tr("Boolean operation fuse"));
	connect(mFuseAction, SIGNAL(triggered()), this, SLOT(testFuse()));

	mCommonAction = new QAction(tr("Common"), this);
	mCommonAction->setIcon(QIcon(":/Resources/common.png"));
	mCommonAction->setStatusTip(tr("Boolean operation common"));
	connect(mCommonAction, SIGNAL(triggered()), this, SLOT(testCommon()));

	myHelixAction = new QAction(tr("Helix"), this);
	myHelixAction->setIcon(QIcon(":/Resources/helix.png"));
	myHelixAction->setStatusTip(tr("Make helix shapes"));
	connect(myHelixAction, SIGNAL(triggered()), this, SLOT(testHelix()));

	mAboutAction = new QAction(tr("About"), this);
	mAboutAction->setStatusTip(tr("About the application"));
	mAboutAction->setIcon(QIcon(":/Resources/lamp.png"));
	connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));

	mZoomAction = new QAction(tr("Zoom"), this);
	mZoomAction->setIcon(QIcon(":/Resources/Zoom.png"));
	mZoomAction->setStatusTip(tr("Zoom"));
	connect(mZoomAction, SIGNAL(triggered()), myOccView, SLOT(zoomObjects()));

	mRotateAction = new QAction(tr("Rotate"), this);
	mRotateAction->setIcon(QIcon(":/Resources/Rotate.png"));
	mRotateAction->setStatusTip(tr("Rotate"));
	connect(mRotateAction, SIGNAL(triggered()), myOccView, SLOT(rotateObjects()));

	mPanAction = new QAction(tr("Pan"), this);
	mPanAction->setIcon(QIcon(":/Resources/Pan.png"));
	mPanAction->setStatusTip(tr("Pan"));
	connect(mPanAction, SIGNAL(triggered()), myOccView, SLOT(panObjects()));

	addFilletAction = new QAction(tr("Fillet"), this);
	addFilletAction->setStatusTip(tr("Fillet"));
	connect(addFilletAction, SIGNAL(triggered()), this, SLOT(filletOption()));

	addDrillAction = new QAction(tr("Drill"), this);
	addDrillAction->setStatusTip(tr("Drill"));
	connect(addDrillAction, SIGNAL(triggered()), this, SLOT(drillOption()));
}

void occQt::createMenus(void)
{

	mFileMenu = menuBar()->addMenu(tr("&File"));
	mFileMenu->addAction(mExitAction);

	importMenu = mFileMenu->addMenu("Import");
	importMenu->addAction(importBrepFilesAction);
	importMenu->addAction(importIgesFilesAction);
	importMenu->addAction(importStepFilesAction);

	exportMenu = mFileMenu->addMenu("Export");
	exportMenu->addAction(exportBrepFilesAction);
	exportMenu->addAction(exportIgesFilesAction);
	exportMenu->addAction(exportStepFilesAction);

	mViewMenu = menuBar()->addMenu(tr("&View"));
	mViewMenu->addAction(mViewZoomAction);
	mViewMenu->addAction(mViewPanAction);
	mViewMenu->addAction(mViewRotateAction);
	mViewMenu->addSeparator();
	mViewMenu->addAction(mViewResetAction);
	mViewMenu->addAction(mViewFitallAction);

	mPrimitiveMenu = menuBar()->addMenu(tr("&Primitive"));
	mPrimitiveMenu->addAction(mMakeBoxAction);
	mPrimitiveMenu->addAction(mMakeConeAction);
	mPrimitiveMenu->addAction(mMakeSphereAction);
	mPrimitiveMenu->addAction(mMakeCylinderAction);
	mPrimitiveMenu->addAction(mMakeTorusAction);
	mPrimitiveMenu->addAction(camber);
	mPrimitiveMenu->addAction(mMakeBottleAction);
	mPrimitiveMenu->addAction(mBoxAction);

	mMakeBoxMenu = menuBar()->addMenu(tr("&MakeBox"));
	mMakeBoxMenu->addAction(mBoxAction);

	mModelingMenu = menuBar()->addMenu(tr("&Modeling"));
	mModelingMenu->addAction(mFilletAction);
	mModelingMenu->addAction(mChamferAction);
	mModelingMenu->addAction(mExtrudeAction);
	mModelingMenu->addAction(mRevolveAction);
	mModelingMenu->addAction(mLoftAction);
	mModelingMenu->addSeparator();
	mModelingMenu->addAction(mCutAction);
	mModelingMenu->addAction(mFuseAction);
	mModelingMenu->addAction(mCommonAction);
	mModelingMenu->addSeparator();
	mModelingMenu->addAction(myHelixAction);

	mEditingMenu = menuBar()->addMenu(tr("&Editing"));
	mEditingMenu->addAction(mZoomAction);
	mEditingMenu->addAction(mRotateAction);
	mEditingMenu->addAction(mPanAction);

	mHelpMenu = menuBar()->addMenu(tr("&Help"));
	mHelpMenu->addAction(mAboutAction);


}

void occQt::createToolBars(void)
{

	mNavigateToolBar = addToolBar(tr("&Navigate"));
	mNavigateToolBar->addAction(mViewZoomAction);
	mNavigateToolBar->addAction(mViewPanAction);
	mNavigateToolBar->addAction(mViewRotateAction);

	mViewToolBar = addToolBar(tr("&View"));
	mViewToolBar->addAction(mViewResetAction);
	mViewToolBar->addAction(mViewFitallAction);

	mPrimitiveToolBar = addToolBar(tr("&Primitive"));
	mPrimitiveToolBar->addAction(mMakeBoxAction);
	mPrimitiveToolBar->addAction(mMakeConeAction);
	mPrimitiveToolBar->addAction(mMakeSphereAction);
	mPrimitiveToolBar->addAction(mMakeCylinderAction);
	mPrimitiveToolBar->addAction(mMakeTorusAction);
	mPrimitiveToolBar->addAction(camber);
	mPrimitiveToolBar->addAction(mMakeBottleAction);


	mModelingToolBar = addToolBar(tr("&Modeling"));
	mModelingToolBar->addAction(mFilletAction);
	mModelingToolBar->addAction(mChamferAction);
	mModelingToolBar->addAction(mExtrudeAction);
	mModelingToolBar->addAction(mRevolveAction);
	mModelingToolBar->addAction(mLoftAction);
	mModelingToolBar->addSeparator();
	mModelingToolBar->addAction(mCutAction);
	mModelingToolBar->addAction(mFuseAction);
	mModelingToolBar->addAction(mCommonAction);
	mModelingToolBar->addSeparator();
	mModelingToolBar->addAction(myHelixAction);

	mMakeBoxToolBar = addToolBar(tr("&MakeBox"));
	mMakeBoxToolBar->addAction(mBoxAction);

	mEditingToolBar = addToolBar(tr("&Editing"));
	mEditingToolBar->addAction(mZoomAction);
	mEditingMenu->addAction(mRotateAction);
	mEditingMenu->addAction(mPanAction);

	mHelpToolBar = addToolBar(tr("Help"));
	mHelpToolBar->addAction(mAboutAction);
}

void occQt::about()
{
	QMessageBox::about(this, tr("About occQt"),
		tr("<h2>occQt 2.0</h2>"
			"<p>Copyright &copy; 2014 eryar@163.com"
			"<p>occQt is a demo applicaton about Qt and OpenCASCADE."));
}

void occQt::importBrepFiles()
{
	QString importBrepFile = QFileDialog::getOpenFileName(this, "Select a file to open...", "D:/", "Brep files(*.brep)");
	std::filebuf aFileBuf;
	std::istream importStream(&aFileBuf);
	if (!aFileBuf.open(importBrepFile.toStdString(), ios::in))
	{
		return;
	}

	BRep_Builder importBrepBuilder;
	BRepTools::Read(brepFileShape, importStream, importBrepBuilder);
	myOccView->removeAllShapes();
	displayShape(brepFileShape);
}

void occQt::importIgesFiles()
{
	QString importIgesFile = QFileDialog::getOpenFileName(this, "Select a file to open....", "D:/", "Iges files(*.iges)");
	IGESControl_Reader igesFileReader;
	igesFileReader.ReadFile(importIgesFile.toStdString().c_str());
	igesFileReader.TransferRoots();
	igesFileShape = igesFileReader.OneShape();
	myOccView->removeAllShapes();
	displayShape(igesFileShape);
}

void occQt::importStepFiles()
{
	QString importStepFile = QFileDialog::getOpenFileName(this, "Select a file to open...", "D:/", "Step files(*.step)");
	STEPControl_Reader stepFileReader;
	stepFileReader.ReadFile(importStepFile.toStdString().c_str());
	stepFileReader.TransferRoots();
	stepFileShape = stepFileReader.OneShape();
	myOccView->removeAllShapes();
	displayShape(stepFileShape);
}

void occQt::exportBrepFiles()
{
	QString exportBrepFile = QFileDialog::getSaveFileName(this, "Save this file to...", "D:/", "Brep files(*.brep)");
	std::filebuf aFileBuf;
	std::ostream exportStream(&aFileBuf);

	//BRep_Builder exportBrepBuilder;
	if (!(brepFileShape.IsNull()))
	{
		BRepTools::Write(brepFileShape, exportBrepFile.toStdString().c_str());
	}

	else
	{
		QMessageBox::warning(this, "Warning!!!", "There is no .brep file to save");
	}
	
}

void occQt::exportIgesFiles()
{
	QString exportIgesFile = QFileDialog::getSaveFileName(this, "Save the file...", "D:/", "Iges files(*.iges)");
	if (!(igesFileShape.IsNull()))
	{
		IGESControl_Writer igesFileWriter;
		igesFileWriter.AddShape(igesFileShape);
		igesFileWriter.Write(exportIgesFile.toStdString().c_str(), true);
	}

	else
	{
		QMessageBox::warning(this, "Warning!!!", "There is no .iges file to save");
	}
}

void occQt::exportStepFiles()
{
	QString exportStepFile = QFileDialog::getSaveFileName(this, "Save the file...", "D:/", "Step files(*.step)");
	if (!(stepFileShape.IsNull()))
	{
		STEPControl_Writer stepFileWriter;
		stepFileWriter.Transfer(stepFileShape, STEPControl_AsIs);
		stepFileWriter.Write(exportStepFile.toStdString().c_str());
	}

	else
	{
		QMessageBox::warning(this, "Warning!!!", "There is no .step file to save");
	}
}

void occQt::makeBox()
{
	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();

	BRepTools::Write(aTopoBox, "C:/QTSample/Box.brep");

	Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

	anAisBox->SetColor(Quantity_NOC_AZURE);

	myOccView->getContext()->Display(anAisBox);
}

void occQt::makeCone()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

	TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
	Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

	anAisReducer->SetColor(Quantity_NOC_BISQUE);

	anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
	TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
	Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

	anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

	myOccView->getContext()->Display(anAisReducer);
	myOccView->getContext()->Display(anAisCone);
}

void occQt::makeSphere()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0));

	TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();
	Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);

	anAisSphere->SetColor(Quantity_NOC_BLUE1);

	myOccView->getContext()->Display(anAisSphere);
}

void occQt::makeCylinder()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 30.0, 0.0));

	TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0).Shape();
	Handle(AIS_Shape) anAisCylinder = new AIS_Shape(aTopoCylinder);

	anAisCylinder->SetColor(Quantity_NOC_RED);

	anAxis.SetLocation(gp_Pnt(8.0, 30.0, 0.0));
	TopoDS_Shape aTopoPie = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0, M_PI_2 * 3.0).Shape();
	Handle(AIS_Shape) anAisPie = new AIS_Shape(aTopoPie);

	anAisPie->SetColor(Quantity_NOC_TAN);

	myOccView->getContext()->Display(anAisCylinder);
	myOccView->getContext()->Display(anAisPie);
}

void occQt::displayShape(TopoDS_Shape shape)
{

	Handle(AIS_Shape) ais_Shape = new AIS_Shape(shape);

	ais_Shape->SetColor(Quantity_NOC_GREEN);

	myOccView->getContext()->Display(ais_Shape);
	aisShapeVector.append(ais_Shape);

}

void occQt::removeAIS_Shape(TopoDS_Shape shape)
{
	if (aisShapeVector.isEmpty() != true)
	{
		Handle(AIS_Shape) ais_Shape = aisShapeVector.takeLast();
		myOccView->getContext()->Remove(ais_Shape);

	}
}

TopoDS_Shape occQt::getLastShape()
{
	TopoDS_Shape shape;
	Handle(AIS_Shape) aisShape;
	if (aisShapeVector.isEmpty() != true)
	{
		aisShape = aisShapeVector.takeLast();
	}

	shape = aisShape->Shape();
	return shape;
}

void occQt::makeBottle()
{
	Standard_Real height = 70;
	Standard_Real width = 50;
	Standard_Real thickness = 30;

	gp_Pnt point1((-width / 2.), 0, 0);
	gp_Pnt point2((-width / 2.), (-thickness / 4.), 0);
	gp_Pnt point3(0, (-thickness / 2.), 0);
	gp_Pnt point4((width / 2.), (-thickness / 4.), 0);
	gp_Pnt point5((width / 2.), 0, 0);

	Handle(Geom_TrimmedCurve) arcOfCircle = GC_MakeArcOfCircle(point2, point3, point4);
	Handle(Geom_TrimmedCurve) segment1 = GC_MakeSegment(point1, point2);
	Handle(Geom_TrimmedCurve) segment2 = GC_MakeSegment(point4, point5);

	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(segment1);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(arcOfCircle);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(segment2);

	TopoDS_Wire wire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3);

	gp_Pnt origin(0, 0, 0);
	gp_Dir dir(1, 0, 0);
	gp_Ax1 axis(origin, dir);

	gp_Trsf trsf;
	trsf.SetMirror(axis);

	BRepBuilderAPI_Transform brepTrsf(wire, trsf);

	TopoDS_Shape mirroredShape = brepTrsf.Shape();

	TopoDS_Wire mirroredWire = TopoDS::Wire(mirroredShape);

	BRepBuilderAPI_MakeWire makeWire;
	makeWire.Add(wire);
	makeWire.Add(mirroredWire);
	TopoDS_Wire wireProfile = makeWire.Wire();

	TopoDS_Face faceProfile = BRepBuilderAPI_MakeFace(wireProfile);

	gp_Vec prismVec(0, 0, height);
	TopoDS_Shape body = BRepPrimAPI_MakePrism(faceProfile, prismVec);

	BRepFilletAPI_MakeFillet makeFillet(body);

	TopExp_Explorer edgeExplorer(body, TopAbs_EDGE);
	while (edgeExplorer.More()) {
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		makeFillet.Add((thickness / 12.), edge);
		edgeExplorer.Next();
	}

	body = makeFillet.Shape();

	gp_Pnt neckLocation(0, 0, height);
	gp_Dir neckAxis = gp::DZ();
	gp_Ax2 neckAx2(neckLocation, neckAxis);

	Standard_Real neckRadius = thickness / 4.;
	Standard_Real neckHeight = height / 10.;
	BRepPrimAPI_MakeCylinder makeCylinder(neckAx2, neckRadius, neckHeight);
	TopoDS_Shape neck = makeCylinder.Shape();

	body = BRepAlgoAPI_Fuse(body, neck);

	TopoDS_Face faceToRemove;
	Standard_Real zMax = -1;

	for (TopExp_Explorer faceExplorer(body, TopAbs_FACE);
		faceExplorer.More();
		faceExplorer.Next()) {
		TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

		Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

		if (surface->DynamicType() == STANDARD_TYPE(Geom_Plane))
		{
			Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);

			gp_Pnt point = plane->Location();
			Standard_Real z = point.Z();

			if (z > zMax)
			{
				zMax = z;
				faceToRemove = face;
			}
		}
	}

	TopTools_ListOfShape facesToRemove;
	facesToRemove.Append(faceToRemove);

	body = BRepOffsetAPI_MakeThickSolid(body, facesToRemove, (-thickness / 50), 1.e-3);

	Handle(Geom_CylindricalSurface) cylinder1 = new Geom_CylindricalSurface(neckAx2, neckRadius * 0.99);
	Handle(Geom_CylindricalSurface) cylinder2 = new Geom_CylindricalSurface(neckAx2, neckRadius * 1.05);

	gp_Pnt2d point2d((2. *M_PI), (neckHeight / 2.));
	gp_Dir2d dir2d((2. * M_PI), (neckHeight / 4.));
	gp_Ax2d ax2d(point2d, dir2d);

	Standard_Real major = (2. * M_PI);
	Standard_Real minor = (neckHeight / 10);

	Handle_Geom2d_Ellipse ellipse1 = new Geom2d_Ellipse(ax2d, major, minor);
	Handle_Geom2d_Ellipse ellipse2 = new Geom2d_Ellipse(ax2d, major, minor / 4);

	Handle(Geom2d_TrimmedCurve) arc1 = new Geom2d_TrimmedCurve(ellipse1, 0, M_PI);
	Handle(Geom2d_TrimmedCurve) arc2 = new Geom2d_TrimmedCurve(ellipse2, 0, M_PI);

	gp_Pnt2d ellipsePoint1 = ellipse1->Value(0);
	gp_Pnt2d ellipsePoint2 = ellipse1->Value(M_PI);

	Handle(Geom2d_TrimmedCurve) segment = GCE2d_MakeSegment(ellipsePoint1, ellipsePoint2);

	TopoDS_Edge edge1OnSurface1 = BRepBuilderAPI_MakeEdge(arc1, cylinder1);
	TopoDS_Edge edge2OnSurface1 = BRepBuilderAPI_MakeEdge(segment, cylinder1);
	TopoDS_Edge edge1OnSurface2 = BRepBuilderAPI_MakeEdge(arc2, cylinder2);
	TopoDS_Edge edge2OnSurface2 = BRepBuilderAPI_MakeEdge(segment, cylinder2);

	TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(edge1OnSurface1, edge2OnSurface1);
	TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(edge1OnSurface2, edge2OnSurface2);

	BRepLib::BuildCurves3d(threadingWire1);
	BRepLib::BuildCurves3d(threadingWire2);

	BRepOffsetAPI_ThruSections tool(Standard_True);
	tool.AddWire(threadingWire1);
	tool.AddWire(threadingWire2);
	tool.CheckCompatibility(Standard_False);
	TopoDS_Shape threading = tool.Shape();

	TopoDS_Compound compound;
	BRep_Builder builder;
	builder.MakeCompound(compound);
	builder.Add(compound, body);
	builder.Add(compound, threading);

	Handle(AIS_Shape) anAisBottle = new AIS_Shape(compound);

	anAisBottle->SetColor(Quantity_NOC_THISTLE);

	myOccView->getContext()->Display(anAisBottle);
}

void occQt::makeTorus()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));

	TopoDS_Shape aTopoTorus = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
	Handle(AIS_Shape) anAisTorus = new AIS_Shape(aTopoTorus);

	anAisTorus->SetColor(Quantity_NOC_YELLOW);

	anAxis.SetLocation(gp_Pnt(8.0, 40.0, 0.0));
	TopoDS_Shape aTopoElbow = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0, M_PI_2).Shape();
	Handle(AIS_Shape) anAisElbow = new AIS_Shape(aTopoElbow);

	anAisElbow->SetColor(Quantity_NOC_THISTLE);

	myOccView->getContext()->Display(anAisTorus);
	myOccView->getContext()->Display(anAisElbow);
}

void occQt::makeFillet()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 50.0, 0.0));

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
	BRepFilletAPI_MakeFillet MF(aTopoBox);

	// Add all the edges to fillet.
	for (TopExp_Explorer ex(aTopoBox, TopAbs_EDGE); ex.More(); ex.Next())
	{
		MF.Add(1.0, TopoDS::Edge(ex.Current()));
	}

	Handle(AIS_Shape) anAisShape = new AIS_Shape(MF.Shape());
	anAisShape->SetColor(Quantity_NOC_VIOLET);

	myOccView->getContext()->Display(anAisShape);
}

void occQt::makeChamfer()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(8.0, 50.0, 0.0));

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
	BRepFilletAPI_MakeChamfer MC(aTopoBox);
	TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;

	TopExp::MapShapesAndAncestors(aTopoBox, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);

	for (Standard_Integer i = 1; i <= aEdgeFaceMap.Extent(); ++i)
	{
		TopoDS_Edge anEdge = TopoDS::Edge(aEdgeFaceMap.FindKey(i));
		TopoDS_Face aFace = TopoDS::Face(aEdgeFaceMap.FindFromIndex(i).First());

		MC.Add(0.6, 0.6, anEdge, aFace);
	}

	Handle(AIS_Shape) anAisShape = new AIS_Shape(MC.Shape());
	anAisShape->SetColor(Quantity_NOC_TOMATO);

	myOccView->getContext()->Display(anAisShape);
}

void occQt::makeBoxFromUserInputs()
{

	Standard_Real boxLength;
	Standard_Real boxWidth;
	Standard_Real boxHeight;

	QDialog dialog;
	QFormLayout form(&dialog);
	form.addRow(new QLabel("Enter your box details"));

	QList<QLineEdit *> fields;

	QLineEdit *length = new QLineEdit(&dialog);
	QString lengthLabel = QString("Length");
	form.addRow(lengthLabel, length);
	fields << length;

	QLineEdit *width = new QLineEdit(&dialog);
	QString widthLabel = QString("Width");
	form.addRow(widthLabel, width);
	fields << width;

	QLineEdit *height = new QLineEdit(&dialog);
	QString heightLabel = QString("Height");
	form.addRow(heightLabel, height);
	fields << height;

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);

	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	int result = dialog.exec();

	while (true)
	{
		if (result == dialog.Accepted)
		{
			boxLength = length->text().toDouble();
			boxWidth = width->text().toDouble();
			boxHeight = height->text().toDouble();
		}
		else
		{
			return;
		}

		if ((length->text() == NULL) || (width->text() == NULL) || (height->text() == NULL))
		{
			QMessageBox::warning(this, tr("Warning!!!"), tr("Fill the all values"));
			result = dialog.exec();
		}

		else if ((boxLength <= 0) || (boxWidth <= 0) || (boxHeight) <= 0)
		{
			QMessageBox::warning(this, tr("Warning!!!"), tr("Box parameters should be greater than zero"));
			result = dialog.exec();
		}

		else
		{
			break;
		}
	}


	gp_Pnt point1(0, 0, 0);
	gp_Pnt point2(boxLength, 0, 0);
	gp_Pnt point3(0, boxWidth, 0);
	gp_Pnt point4(boxLength, boxWidth, 0);
	gp_Pnt point5(0, 0, boxHeight);
	gp_Pnt point6(boxLength, 0, boxHeight);
	gp_Pnt point7(0, boxWidth, boxHeight);
	gp_Pnt point8(boxLength, boxWidth, boxHeight);

	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(point1, point2);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(point2, point4);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(point3, point4);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(point1, point3);



	TopoDS_Wire wire1 = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);

	TopoDS_Face face1 = BRepBuilderAPI_MakeFace(wire1);

	gp_Vec build(0, 0, boxHeight);

	box = BRepPrimAPI_MakePrism(face1, build);

	displayShape(box);
	lastShape = box;

	isFilleted = false;
	isDrilled = false;

	mMakeBoxMenu->addAction(addFilletAction);
	mMakeBoxMenu->addAction(addDrillAction);

	mMakeBoxToolBar->addAction(addFilletAction);
	mMakeBoxToolBar->addAction(addDrillAction);

}

void occQt::filletOption()
{

	Standard_Real filletRadius;

	if (isDrilled && isFilleted)
	{
		filletBox = onlyDrilledBox;
	}

	else if (isDrilled)
	{
		filletBox = drillBox;
	}

	else
	{
		filletBox = box;
	}

	GProp_GProps edgeProps;
	double minLength = std::numeric_limits<double>::infinity();

	TopExp_Explorer edgeExplorer(filletBox, TopAbs_EDGE);

	double length;

	while (edgeExplorer.More()) {
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		BRepGProp::LinearProperties(edge, edgeProps);
		length = edgeProps.Mass();
		if (length < minLength)
		{
			minLength = length;
		}
		edgeExplorer.Next();
	}

	bool ok;
	filletRadius = QInputDialog::getDouble(this, tr("Enter the fillet radius"), tr("Fillet Radius : "), 0, 0, ((minLength / 2) - 0.0001), 4, &ok);

	if (filletRadius > 0)
	{
		BRepFilletAPI_MakeFillet makeFillet(filletBox);

		TopExp_Explorer filletEdgeExplorer(filletBox, TopAbs_EDGE);

		while (filletEdgeExplorer.More()) {
			TopoDS_Edge filletEdge = TopoDS::Edge(filletEdgeExplorer.Current());
			makeFillet.Add(filletRadius, filletEdge);
			filletEdgeExplorer.Next();
		}

		filletBox = makeFillet.Shape();

		removeAIS_Shape(lastShape);
		displayShape(filletBox);

		lastShape = filletBox;
		isFilleted = true;
	}
}


void occQt::drillOption()
{

	drillBox = lastShape;

	if (isDrilled)
	{
		onlyDrilledBox = onlyDrilledBox;
	}

	else
	{
		onlyDrilledBox = box;
	}

	QString selectedAxis;

	QList<gp_Pnt> *xAxisCenterPoints = new QList<gp_Pnt>;
	QList<gp_Pnt> *yAxisCenterPoints = new QList<gp_Pnt>;
	QList<gp_Pnt> *zAxisCenterPoints = new QList<gp_Pnt>;

	gp_Ax1 planeAxis;
	gp_Pnt centerPoint;

	GProp_GProps faceCenterPointsProps;
	GProp_GProps edgeLengthProps;

	Standard_Real holeRadius;
	Standard_Real holeDepth;

	double length;

	double xAxisMinLength;
	double yAxisMinLength;
	double zAxisMinLength;

	QDialog dialog;
	QFormLayout form(&dialog);
	form.addRow(new QLabel("Select a axis"));

	QList<QComboBox *> axis;

	QComboBox* axisComboBox = new QComboBox(&dialog);
	QStringList axisList = (QStringList() << "X Axis" << "Y Axis" << "Z Axis");
	axisComboBox->addItems(axisList);
	QString lengthLabel = QString("Axis : ");
	form.addRow(lengthLabel, axisComboBox);
	axis << axisComboBox;

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);

	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	int holeAxisAccepted = dialog.exec();

	if (holeAxisAccepted == QDialog::Accepted)
	{
		selectedAxis = axisComboBox->currentText();

		TopExp_Explorer faceExplorer(drillBox, TopAbs_FACE);


		while (faceExplorer.More())
		{
			double minLength = std::numeric_limits<double>::infinity();
			TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

			Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

			if (surface->DynamicType() == STANDARD_TYPE(Geom_Plane))
			{
				Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);

				planeAxis = plane->Axis();
				BRepGProp::SurfaceProperties(face, faceCenterPointsProps);

				centerPoint = faceCenterPointsProps.CentreOfMass();

				TopExp_Explorer edgeExplorer(face, TopAbs_EDGE);
				while (edgeExplorer.More())
				{
					TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
					BRepGProp::LinearProperties(edge, edgeLengthProps);
					length = edgeLengthProps.Mass();

					if (length < minLength)
					{
						minLength = length;
					}

					edgeExplorer.Next();

				}

				if (planeAxis.IsParallel(gp::OX(), 1.e-3))
				{
					xAxisCenterPoints->append(centerPoint);

					xAxisMinLength = minLength;
				}

				else if (planeAxis.IsParallel(gp::OY(), 1.e-3))
				{
					yAxisCenterPoints->append(centerPoint);

					yAxisMinLength = length;
				}

				else if (planeAxis.IsParallel(gp::OZ(), 1.e-3))
				{
					zAxisCenterPoints->append(centerPoint);

					zAxisMinLength = length;
				}
			}

			faceExplorer.Next();
		}

		QDialog holeDialog;
		QFormLayout form(&holeDialog);
		form.addRow(new QLabel("Enter your hole details"));

		QList<QLineEdit *> parameters;

		QLineEdit *radius = new QLineEdit(&holeDialog);
		QString radiusLabel = QString("Radius");
		form.addRow(radiusLabel, radius);
		parameters << radius;

		QLineEdit *depth = new QLineEdit(&holeDialog);
		QString depthLabel = QString("Depth");
		form.addRow(depthLabel, depth);
		parameters << depth;

		QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &holeDialog);
		form.addRow(&buttonBox);

		QObject::connect(&buttonBox, SIGNAL(accepted()), &holeDialog, SLOT(accept()));
		QObject::connect(&buttonBox, SIGNAL(rejected()), &holeDialog, SLOT(reject()));

		int holeDetailsAccepted = holeDialog.exec();

		while (true)
		{
			if (holeDetailsAccepted == QDialog::Accepted)
			{
				holeRadius = radius->text().toDouble();
				holeDepth = depth->text().toDouble();

				if (selectedAxis == "X Axis")
				{
					if (holeRadius <= (xAxisMinLength / 2))
					{
						gp_Pnt xAxisHoleLocation(xAxisCenterPoints->at(1));
						gp_Vec xAxisholeDirectionVector(xAxisCenterPoints->at(1), xAxisCenterPoints->at(0));
						xAxisholeDirectionVector.Normalize();
						gp_Dir xAxisholeDirection = gp_Dir(xAxisholeDirectionVector);
						gp_Ax2 xAxisholeAx2(xAxisHoleLocation, xAxisholeDirection);

						BRepPrimAPI_MakeCylinder xAxisHoleCylinder(xAxisholeAx2, holeRadius, holeDepth);
						TopoDS_Shape xAxisHole = xAxisHoleCylinder.Shape();

						drillBox = BRepAlgoAPI_Cut(drillBox, xAxisHole);
						onlyDrilledBox = BRepAlgoAPI_Cut(onlyDrilledBox, xAxisHole);
						break;
					}

					else
					{
						QMessageBox::warning(this, tr("Wraning!!!"), tr("Hole Radius is lesser than the half of the length"));
						holeDetailsAccepted = holeDialog.exec();

					}


				}

				else if (selectedAxis == "Y Axis")
				{

					if (holeRadius <= (yAxisMinLength / 2))
					{
						gp_Pnt yAxisHoleLocation(yAxisCenterPoints->at(0));
						gp_Vec yAxisholeDirectionVector(yAxisCenterPoints->at(0), yAxisCenterPoints->at(1));
						yAxisholeDirectionVector.Normalize();
						gp_Dir yAxisholeDirection = gp_Dir(yAxisholeDirectionVector);
						gp_Ax2 yAxisholeAx2(yAxisHoleLocation, yAxisholeDirection);

						BRepPrimAPI_MakeCylinder yAxisHoleCylinder(yAxisholeAx2, holeRadius, holeDepth);
						TopoDS_Shape yAxisHole = yAxisHoleCylinder.Shape();

						drillBox = BRepAlgoAPI_Cut(drillBox, yAxisHole);
						onlyDrilledBox = BRepAlgoAPI_Cut(onlyDrilledBox, yAxisHole);
						break;
					}

					else
					{
						QMessageBox::warning(this, tr("Wraning!!!"), tr("Hole Radius is lesser than the half of the length"));
						holeDetailsAccepted = holeDialog.exec();

					}

				}

				else if (selectedAxis == "Z Axis")
				{
					if (holeRadius <= (zAxisMinLength / 2))
					{
						gp_Pnt zAxisHoleLocation(zAxisCenterPoints->at(1));
						gp_Vec zAxisholeDirectionVector(zAxisCenterPoints->at(1), zAxisCenterPoints->at(0));
						zAxisholeDirectionVector.Normalize();
						gp_Dir zAxisholeDirection = gp_Dir(zAxisholeDirectionVector);
						gp_Ax2 zAxisholeAx2(zAxisHoleLocation, zAxisholeDirection);

						BRepPrimAPI_MakeCylinder zAxisHoleCylinder(zAxisholeAx2, holeRadius, holeDepth);
						TopoDS_Shape zAxisHole = zAxisHoleCylinder.Shape();

						drillBox = BRepAlgoAPI_Cut(drillBox, zAxisHole);
						onlyDrilledBox = BRepAlgoAPI_Cut(onlyDrilledBox, zAxisHole);
						break;
					}

					else
					{
						QMessageBox::warning(this, tr("Wraning!!!"), tr("Hole Radius is lesser than the half of the length"));
						holeDetailsAccepted = holeDialog.exec();

					}

				}
			}

			else
			{
				return;
			}
		}

		removeAIS_Shape(lastShape);
		displayShape(drillBox);

		lastShape = drillBox;
		isDrilled = true;
	}
}

void occQt::makeExtrude()
{
	// prism a vertex result is an edge.
	TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(0.0, 60.0, 0.0));
	TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, 5.0));
	Handle(AIS_Shape) anAisPrismVertex = new AIS_Shape(aPrismVertex);

	// prism an edge result is a face.
	TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(5.0, 60.0, 0.0), gp_Pnt(10.0, 60.0, 0.0));
	TopoDS_Shape aPrismEdge = BRepPrimAPI_MakePrism(anEdge, gp_Vec(0.0, 0.0, 5.0));
	Handle(AIS_Shape) anAisPrismEdge = new AIS_Shape(aPrismEdge);

	// prism a wire result is a shell.
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(16.0, 60.0, 0.0));

	TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(anAxis, 3.0));
	TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
	TopoDS_Shape aPrismCircle = BRepPrimAPI_MakePrism(aCircleWire, gp_Vec(0.0, 0.0, 5.0));
	Handle(AIS_Shape) anAisPrismCircle = new AIS_Shape(aPrismCircle);

	// prism a face or a shell result is a solid.
	anAxis.SetLocation(gp_Pnt(24.0, 60.0, 0.0));
	TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(anAxis, 3.0, 2.0));
	TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
	TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
	TopoDS_Shape aPrismEllipse = BRepPrimAPI_MakePrism(aEllipseFace, gp_Vec(0.0, 0.0, 5.0));
	Handle(AIS_Shape) anAisPrismEllipse = new AIS_Shape(aPrismEllipse);

	anAisPrismVertex->SetColor(Quantity_NOC_PAPAYAWHIP);
	anAisPrismEdge->SetColor(Quantity_NOC_PEACHPUFF);
	anAisPrismCircle->SetColor(Quantity_NOC_PERU);
	anAisPrismEllipse->SetColor(Quantity_NOC_PINK);

	myOccView->getContext()->Display(anAisPrismVertex);
	myOccView->getContext()->Display(anAisPrismEdge);
	myOccView->getContext()->Display(anAisPrismCircle);
	myOccView->getContext()->Display(anAisPrismEllipse);
}

void occQt::makeRevol()
{
	gp_Ax1 anAxis;

	// revol a vertex result is an edge.
	anAxis.SetLocation(gp_Pnt(0.0, 70.0, 0.0));
	TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(2.0, 70.0, 0.0));
	TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis);
	Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(aRevolVertex);

	// revol an edge result is a face.
	anAxis.SetLocation(gp_Pnt(8.0, 70.0, 0.0));
	TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(6.0, 70.0, 0.0), gp_Pnt(6.0, 70.0, 5.0));
	TopoDS_Shape aRevolEdge = BRepPrimAPI_MakeRevol(anEdge, anAxis);
	Handle(AIS_Shape) anAisRevolEdge = new AIS_Shape(aRevolEdge);

	// revol a wire result is a shell.
	anAxis.SetLocation(gp_Pnt(20.0, 70.0, 0.0));
	anAxis.SetDirection(gp::DY());

	TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(15.0, 70.0, 0.0), gp::DZ()), 1.5));
	TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
	TopoDS_Shape aRevolCircle = BRepPrimAPI_MakeRevol(aCircleWire, anAxis, M_PI_2);
	Handle(AIS_Shape) anAisRevolCircle = new AIS_Shape(aRevolCircle);

	// revol a face result is a solid.
	anAxis.SetLocation(gp_Pnt(30.0, 70.0, 0.0));
	anAxis.SetDirection(gp::DY());

	TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(gp_Ax2(gp_Pnt(25.0, 70.0, 0.0), gp::DZ()), 3.0, 2.0));
	TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
	TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
	TopoDS_Shape aRevolEllipse = BRepPrimAPI_MakeRevol(aEllipseFace, anAxis, M_PI_4);
	Handle(AIS_Shape) anAisRevolEllipse = new AIS_Shape(aRevolEllipse);

	anAisRevolVertex->SetColor(Quantity_NOC_LIMEGREEN);
	anAisRevolEdge->SetColor(Quantity_NOC_LINEN);
	anAisRevolCircle->SetColor(Quantity_NOC_MAGENTA1);
	anAisRevolEllipse->SetColor(Quantity_NOC_MAROON);

	myOccView->getContext()->Display(anAisRevolVertex);
	myOccView->getContext()->Display(anAisRevolEdge);
	myOccView->getContext()->Display(anAisRevolCircle);
	myOccView->getContext()->Display(anAisRevolEllipse);
}


void occQt::makeLoft()
{
	// bottom wire.
	TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(0.0, 80.0, 0.0), gp::DZ()), 1.5));
	TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);

	// top wire.
	BRepBuilderAPI_MakePolygon aPolygon;
	aPolygon.Add(gp_Pnt(-3.0, 77.0, 6.0));
	aPolygon.Add(gp_Pnt(3.0, 77.0, 6.0));
	aPolygon.Add(gp_Pnt(3.0, 83.0, 6.0));
	aPolygon.Add(gp_Pnt(-3.0, 83.0, 6.0));
	aPolygon.Close();

	BRepOffsetAPI_ThruSections aShellGenerator;
	BRepOffsetAPI_ThruSections aSolidGenerator(true);

	aShellGenerator.AddWire(aCircleWire);
	aShellGenerator.AddWire(aPolygon.Wire());

	aSolidGenerator.AddWire(aCircleWire);
	aSolidGenerator.AddWire(aPolygon.Wire());

	// translate the solid.
	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(18.0, 0.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aSolidGenerator.Shape(), aTrsf);

	Handle(AIS_Shape) anAisShell = new AIS_Shape(aShellGenerator.Shape());
	Handle(AIS_Shape) anAisSolid = new AIS_Shape(aTransform.Shape());

	anAisShell->SetColor(Quantity_NOC_OLIVEDRAB);
	anAisSolid->SetColor(Quantity_NOC_PEACHPUFF);

	myOccView->getContext()->Display(anAisShell);
	myOccView->getContext()->Display(anAisSolid);
}

void occQt::testCut()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 90.0, 0.0));

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
	TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
	TopoDS_Shape aCuttedShape1 = BRepAlgoAPI_Cut(aTopoBox, aTopoSphere);
	TopoDS_Shape aCuttedShape2 = BRepAlgoAPI_Cut(aTopoSphere, aTopoBox);

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
	BRepBuilderAPI_Transform aTransform1(aCuttedShape1, aTrsf);

	aTrsf.SetTranslation(gp_Vec(16.0, 0.0, 0.0));
	BRepBuilderAPI_Transform aTransform2(aCuttedShape2, aTrsf);

	Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
	Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
	Handle(AIS_Shape) anAisCuttedShape1 = new AIS_Shape(aTransform1.Shape());
	Handle(AIS_Shape) anAisCuttedShape2 = new AIS_Shape(aTransform2.Shape());

	anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
	anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
	anAisCuttedShape1->SetColor(Quantity_NOC_TAN);
	anAisCuttedShape2->SetColor(Quantity_NOC_SALMON);

	myOccView->getContext()->Display(anAisBox);
	myOccView->getContext()->Display(anAisSphere);
	myOccView->getContext()->Display(anAisCuttedShape1);
	myOccView->getContext()->Display(anAisCuttedShape2);
}

void occQt::testFuse()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 100.0, 0.0));

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
	TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
	TopoDS_Shape aFusedShape = BRepAlgoAPI_Fuse(aTopoBox, aTopoSphere);

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aFusedShape, aTrsf);

	Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
	Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
	Handle(AIS_Shape) anAisFusedShape = new AIS_Shape(aTransform.Shape());

	anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
	anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
	anAisFusedShape->SetColor(Quantity_NOC_ROSYBROWN);

	myOccView->getContext()->Display(anAisBox);
	myOccView->getContext()->Display(anAisSphere);
	myOccView->getContext()->Display(anAisFusedShape);
}

void occQt::testCommon()
{
	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(0.0, 110.0, 0.0));

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
	TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
	TopoDS_Shape aCommonShape = BRepAlgoAPI_Common(aTopoBox, aTopoSphere);

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aCommonShape, aTrsf);

	Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
	Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
	Handle(AIS_Shape) anAisCommonShape = new AIS_Shape(aTransform.Shape());

	anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
	anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
	anAisCommonShape->SetColor(Quantity_NOC_ROYALBLUE);

	myOccView->getContext()->Display(anAisBox);
	myOccView->getContext()->Display(anAisSphere);
	myOccView->getContext()->Display(anAisCommonShape);
}


void occQt::testHelix()
{
	makeCylindricalHelix();

	makeConicalHelix();

	makeToroidalHelix();
}

void occQt::makeCylindricalHelix()
{
	Standard_Real aRadius = 3.0;
	Standard_Real aPitch = 1.0;

	// the pcurve is a 2d line in the parametric space.
	gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

	Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

	Handle(Geom_CylindricalSurface) aCylinder = new Geom_CylindricalSurface(gp::XOY(), aRadius);

	TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(0.0, 120.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

	Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

	myOccView->getContext()->Display(anAisHelixCurve);

	// sweep a circle profile along the helix curve.
	// there is no curve3d in the pcurve edge, so approx one.
	BRepLib::BuildCurve3d(aHelixEdge);

	gp_Ax2 anAxis;
	anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
	anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

	gp_Circ aProfileCircle(anAxis, 0.3);

	TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
	TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
	TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

	TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

	BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

	if (aPipeMaker.IsDone())
	{
		aTrsf.SetTranslation(gp_Vec(8.0, 120.0, 0.0));
		BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

		Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
		anAisPipe->SetColor(Quantity_NOC_CORAL);
		myOccView->getContext()->Display(anAisPipe);
	}
}

/**
* make conical helix, it is the same as the cylindrical helix,
* the only different is the surface.
*/
void occQt::makeConicalHelix()
{
	Standard_Real aRadius = 3.0;
	Standard_Real aPitch = 1.0;

	// the pcurve is a 2d line in the parametric space.
	gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

	Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

	Handle(Geom_ConicalSurface) aCylinder = new Geom_ConicalSurface(gp::XOY(), M_PI / 6.0, aRadius);

	TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(18.0, 120.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

	Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

	myOccView->getContext()->Display(anAisHelixCurve);

	// sweep a circle profile along the helix curve.
	// there is no curve3d in the pcurve edge, so approx one.
	BRepLib::BuildCurve3d(aHelixEdge);

	gp_Ax2 anAxis;
	anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
	anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

	gp_Circ aProfileCircle(anAxis, 0.3);

	TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
	TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
	TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

	TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

	BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

	if (aPipeMaker.IsDone())
	{
		aTrsf.SetTranslation(gp_Vec(28.0, 120.0, 0.0));
		BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

		Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
		anAisPipe->SetColor(Quantity_NOC_DARKGOLDENROD);
		myOccView->getContext()->Display(anAisPipe);
	}
}

void occQt::makeToroidalHelix()
{
	Standard_Real aRadius = 1.0;
	Standard_Real aSlope = 0.05;

	// the pcurve is a 2d line in the parametric space.
	gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aSlope, 1.0));

	Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

	Handle(Geom_ToroidalSurface) aCylinder = new Geom_ToroidalSurface(gp::XOY(), aRadius * 5.0, aRadius);

	TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 2.0 * M_PI / aSlope).Edge();

	gp_Trsf aTrsf;
	aTrsf.SetTranslation(gp_Vec(45.0, 120.0, 0.0));
	BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

	Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

	myOccView->getContext()->Display(anAisHelixCurve);

	// sweep a circle profile along the helix curve.
	// there is no curve3d in the pcurve edge, so approx one.
	BRepLib::BuildCurve3d(aHelixEdge);

	gp_Ax2 anAxis;
	anAxis.SetDirection(gp_Dir(0.0, 0.0, 1.0));
	anAxis.SetLocation(gp_Pnt(aRadius * 6.0, 0.0, 0.0));

	gp_Circ aProfileCircle(anAxis, 0.3);

	TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
	TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
	TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

	TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

	BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

	if (aPipeMaker.IsDone())
	{
		aTrsf.SetTranslation(gp_Vec(60.0, 120.0, 0.0));
		BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

		Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
		anAisPipe->SetColor(Quantity_NOC_CORNSILK1);
		myOccView->getContext()->Display(anAisPipe);
	}
}
