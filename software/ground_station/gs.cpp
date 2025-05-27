#include "gs.h"

GS::GS(QObject *parent)
    : QObject{parent}
{
    vcom = new QSerialPort();
    vcom->setPortName("COM4");
    vcom->setBaudRate(QSerialPort::BaudRate::Baud115200);
    vcom->setParity(QSerialPort::Parity::NoParity);
    vcom->setDataBits(QSerialPort::DataBits::Data8);
    vcom->setStopBits(QSerialPort::StopBits::OneStop);
    vcom->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    vcom->open(QIODevice::ReadWrite);
}

void GS::set_resolution(uint16_t img_width, uint16_t img_height, uint8_t bytes_per_pixel){

}

void GS::save_img() {
    if (bytes_per_pixel == 1) { // Image is black&white

    }
    else { //Image is
        QImage image(img_width, img_height, QImage::Format_RGB16);
    }


QPixmap.loadFromData((const uchar *) imageBuffer_pointer, (sizeof(imageRows) * sizeof(imageCols));
    pixmap.save(fileName, "JPG");
}




