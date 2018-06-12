#include "gamma_dlg.h"
#include <QHBoxLayout>
#include <QString>
gamma_Dlg::gamma_Dlg(QDialog *parent) : QDialog(parent)
{
   QHBoxLayout* hlayout=new QHBoxLayout();
   putGmma=new QLineEdit;
   m_btnok=new QPushButton(tr("确定"));
   connect(m_btnok,SIGNAL(clicked(bool)),this,SLOT(onReturnGamma()));

   hlayout->addWidget(putGmma);
   hlayout->addWidget(m_btnok);

   setLayout(hlayout);
}


void gamma_Dlg::onReturnGamma()
{
    QString str;
    str=putGmma->text();
    nGamma=str.toDouble();

    QDialog::accept();
}
