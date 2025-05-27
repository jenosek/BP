#ifndef GS_H
#define GS_H

#include <QObject>
#include <QtSerialPort>
#include <QImage>

class GS : public QObject
{
    Q_OBJECT
    QSerialPort* vcom;
    uint16_t img_width;
    uint16_t img_height;
    uint8_t bytes_per_pixel;


public:
    explicit GS(QObject *parent = nullptr);

    void save_img();
    void set_resolution(uint16_t img_width, uint16_t img_height, uint8_t bytes_per_pixel);

signals:
};

#endif // GS_H
