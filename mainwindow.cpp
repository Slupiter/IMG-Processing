#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <Qstring>
#include <QDebug>
#include <QMessageBox>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //菜单1
    QMenu* m_filemenu = new QMenu;
     m_filemenu = menuBar()->addMenu(tr("文件"));
     m_openfile=new QAction(tr("打开文件"));
     m_filemenu->addAction(m_openfile);
     connect(m_openfile,SIGNAL(triggered(bool)),this,SLOT(onOpenFile()));
     m_savefile=new QAction(tr("保存文件"));
     m_filemenu->addAction(m_savefile);
     connect(m_savefile,SIGNAL(triggered(bool)),this,SLOT(slot_saveImage()));    
     m_exit =new QAction(tr("退出程序"));     
     m_filemenu->addAction(m_exit);
     connect(m_exit,SIGNAL(triggered(bool)),this,SLOT(close()));

     //菜单2
     QMenu* m_processmenu = new QMenu;
      m_processmenu = menuBar()->addMenu(tr("图像处理"));
      m_EqHist=new QAction(tr("直方图均衡化"));
      m_processmenu->addAction(m_EqHist);
      connect(m_EqHist,SIGNAL(triggered(bool)),this,SLOT(slot_EqHist()));
      m_Gamma=new QAction(tr("Gamma校正"));
      m_processmenu->addAction(m_Gamma);
      connect(m_Gamma,SIGNAL(triggered(bool)),this,SLOT(onOpenFileGamma()));
      m_Dithering=new QAction(tr("Dithering加网"));
      m_processmenu->addAction(m_Dithering);
      connect(m_Dithering,SIGNAL(triggered(bool)),this,SLOT(slot_Dithering()));

      //菜单
      QMenu* m_QualityEvmenu = new QMenu;
       m_QualityEvmenu = menuBar()->addMenu(tr("质量评价"));
       m_MSE=new QAction(tr("执行"));
      m_QualityEvmenu->addAction(m_MSE);
       connect(m_MSE,SIGNAL(triggered(bool)),this,SLOT(slot_Evaluate()));

     //  m_PSNR=new QAction(tr("峰值信噪比（PSNR）"));
    //   m_QualityEvmenu->addAction(m_PSNR);

      //工具栏slot_clear()
     m_btnclear=new QToolButton(this);
     m_btnclear->setText("清理");
     connect(m_btnclear,SIGNAL(clicked(bool)),this,SLOT(slot_clear()));
     m_btnbigger=new QToolButton(this);
     m_btnbigger->setText("放大");
     m_btnsmaller=new QToolButton(this);
     m_btnsmaller->setText("缩小");

     m_toolbar=addToolBar("工具");
     m_toolbar->addWidget( m_btnclear);
     m_toolbar->addWidget( m_btnbigger);
     m_toolbar->addWidget( m_btnsmaller);


     m_pQMainWidget=new Picture_Widget(this);
     setCentralWidget(m_pQMainWidget);

}

void MainWindow::onOpenFile()
{
    m_filename=QFileDialog::getOpenFileName(this,tr("选择图像"),"",
                                               tr("Images (*.png *.bmp *.jpg )"));

       m_srcImg.load(m_filename);
       m_pQMainWidget->paintEvent(m_srcImg);

}

void MainWindow::slot_saveImage()
{

    QString savefile = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.jpg *.png *.bmp )")); //选择路径
   string fileAsSave = savefile.toStdString();
    Mat SaveImg = QImage2cvMat(m_srcImg);
    imwrite(fileAsSave,SaveImg);
}

QImage MainWindow::cvMat2QImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Mat MainWindow::QImage2cvMat(QImage image)
{
    Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

void MainWindow::EqHist(QImage& image)
{
    Mat mat = QImage2cvMat(image);
    Mat Grayimage;
    cvtColor(mat, Grayimage,CV_RGB2GRAY);
    m_grayImg =cvMat2QImage(Grayimage);
    Mat resultImg;
    equalizeHist(Grayimage, resultImg);

    image=cvMat2QImage(resultImg);
}


void MainWindow::slot_EqHist()
{
   MainWindow::EqHist(m_srcImg);
    m_pQMainWidget->paintEvent(m_srcImg);
}

void MainWindow::onOpenFileGamma()
{
    m_pGammaDLG =new gamma_Dlg();
    m_pGammaDLG->exec();

    if(m_pGammaDLG->exec()==QDialog::Accepted)
    {
        n_Gamma=m_pGammaDLG->nGamma;

    }
    MainWindow::Gamma(m_srcImg);
      m_pQMainWidget->paintEvent(m_srcImg);
}

void MainWindow::Gamma(QImage& image)
{
    Mat mat = QImage2cvMat(image);
    //灰度化
    Mat Grayimage;
    cvtColor(mat, Grayimage,CV_RGB2GRAY);
    m_grayImg =cvMat2QImage(Grayimage);
    Mat reslutImg(Grayimage.size(),CV_8UC1);
    for(int i = 0; i < Grayimage.rows; i++)
        {
            // 每列
            for(int j = 0; j < Grayimage.cols; j++)
            {
                 reslutImg.at<uchar>(i,j)= 255*pow(Grayimage.at<uchar>(i,j)/255.0,1.0/n_Gamma);

            }
        }
    image=cvMat2QImage(reslutImg);
}
/*
void MainWindow::slot_Gamma()
{
    MainWindow::Gamma(m_srcImg);
     m_pQMainWidget->paintEvent(m_srcImg);
}*/

void MainWindow::Dithering(QImage& img)
{
    Mat image = QImage2cvMat(img);
    Mat Grayimage;
    cvtColor(image, Grayimage,CV_RGB2GRAY);
   m_grayImg =cvMat2QImage(Grayimage);
    Mat reslutImg(Grayimage.size(),CV_8UC1);

    int Matrix[4][4]={{0,8,2,10},{4,12,6,14},{3,11,1,9},{7,15,5,13}};
    int m,n;
    for (int i=0;i<Grayimage.rows;i++)
    {
        m=i%4;
        for (int j=0;j<Grayimage.cols;j++)
        {
            n=j%4;
            if (Grayimage.at<uchar>(i,j)/255.0>=Matrix[m][n]/15.0)
            reslutImg.at<uchar>(i,j)=255;
            else
            reslutImg.at<uchar>(i,j)=0;
        }
    }
    QImage m_ditheringImg=cvMat2QImage(reslutImg);
    img=cvMat2QImage(reslutImg);
}
 void MainWindow::slot_Dithering()
 {
     MainWindow::Dithering(m_srcImg);
     m_pQMainWidget->paintEvent(m_srcImg);
}

 void MainWindow::slot_Evaluate()
 {
     Mat Befmat = QImage2cvMat(m_grayImg);
     Mat Aftmat = QImage2cvMat(m_srcImg);
     double NumD,NumS=0;
     for(int i = 0; i < Befmat.rows; i++)
         {
             for(int j = 0; j < Befmat.cols; j++)
             {
                  NumD=(double)Befmat.at<uchar>(i,j)-(double)Aftmat.at<uchar>(i,j);
                  NumS += NumD*NumD;
             }
         }
     double dMSE=NumS/Befmat.rows/Befmat.cols;
     double dPSNR=10.0 * log10((255*255)/dMSE);
     QString msg = "MSE:";
     msg.append(QString::number(dMSE));
     msg.append("     PSNR:");
     msg.append(QString::number(dPSNR));
 
     QMessageBox::information(this,tr("图像评价"),msg);
 
 }
 
void MainWindow::slot_clear()
{
       m_pQMainWidget->m_labimage->clear();
}
MainWindow::~MainWindow()
{
    delete ui;
}
