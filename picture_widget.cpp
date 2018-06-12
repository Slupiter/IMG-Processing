#include "picture_widget.h"
#include <QHBoxLayout> //水平排版
#include <QVBoxLayout>//垂直排版


Picture_Widget::Picture_Widget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hlayout=new QHBoxLayout(this);
    m_labimage=new QLabel(tr("请打开文件*png*jpg*bmp"));
    hlayout->addStretch();
    hlayout->addWidget(m_labimage);
      hlayout->addStretch();
    setLayout(hlayout);
}

void Picture_Widget::paintEvent(QImage img)
{
    m_labimage->resize(QSize(img.width(),img.height()));
    QPixmap pix;
    pix=pix.fromImage(img);
    m_labimage->setPixmap(pix);

}
