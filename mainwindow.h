#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>
#include "picture_widget.h"
#include "gamma_dlg.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString m_filename;
    QImage m_srcImg;
    QImage m_grayImg;
    double n_Gamma;


   QImage cvMat2QImage(const Mat& mat);
   Mat QImage2cvMat(QImage image);
   void EqHist(QImage& image);
   void Gamma(QImage& image);
   void Dithering(QImage& img);
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private:
    Ui::MainWindow *ui;
    //菜单
    QMenu* m_filemenu;
    QAction* m_openfile;
    QAction* m_savefile;
    QAction* m_exit;

    QMenu* m_processmenu;
    QAction* m_EqHist;
    QAction* m_Gamma;
    QAction* m_Dithering;

    QMenu* m_QualityEvmenu;
    QAction* m_MSE;
  //  QAction* m_PSNR;


    //工具栏
    QToolButton* m_btnclear;
    QToolButton* m_btnbigger;
    QToolButton* m_btnsmaller;
    QToolBar* m_toolbar;
    //主窗体/自建类
    Picture_Widget* m_pQMainWidget;
    gamma_Dlg* m_pGammaDLG;


    //槽函数
public slots:
   void onOpenFile();
   void slot_saveImage();

   void slot_clear();

   void slot_EqHist();
   void onOpenFileGamma();
   void slot_Dithering();
    void slot_Evaluate();
  // void slot_Gamma();
};

#endif // MAINWINDOW_H
