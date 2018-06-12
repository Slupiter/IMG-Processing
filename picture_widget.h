#ifndef PICTURE_WIDGET_H
#define PICTURE_WIDGET_H


#include <QWidget>
#include <QLabel>

class Picture_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Picture_Widget(QWidget *parent = nullptr);
    void paintEvent(QImage img);
     QLabel* m_labimage;
signals:
protected:

public slots:
};

#endif // PICTURE_WIDGET_H
