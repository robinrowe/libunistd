// QtHelpers.h
// Created by Robin Rowe on 11/19/2015.
// Copyright (c) 2015 Robin.Rowe@CinePaint.org. 
// License open source MIT

#ifndef QtHelpers_h
#define QtHelpers_h

#include <QPixmap>
#include <QImageReader>
#include <QDebug>

inline
bool QtLoadImage(const char* filename,QLabel& image)
{   QPixmap qPixmap;
    if(!qPixmap.load(filename))
    {   qDebug()<<"No image "<<filename;
        qDebug() << QImageReader::supportedImageFormats();
        return false;
    }
    image.setPixmap(qPixmap.scaled(image.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    image.setAlignment(Qt::AlignCenter);
    qDebug()<<"Loaded: "<<filename;
    return true;
}

#endif

