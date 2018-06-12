#ifndef GAMMA_DLG_H
#define GAMMA_DLG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
class gamma_Dlg : public QDialog
{
    Q_OBJECT
public:
    explicit gamma_Dlg(QDialog *parent = nullptr);

    double nGamma;
    QLineEdit* putGmma;
    QPushButton* m_btnok;
signals:

public slots:
    void onReturnGamma();
};

#endif // GAMMA_DLG_H
