/*
 * Copyright (c) 2013-2016 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CDGWINDOW_H
#define CDGWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include "khsettings.h"
#include "scrolltext.h"
#include "cdgvideowidget.h"

namespace Ui {
class DlgCdg;
}

class DlgCdg : public QDialog
{
    Q_OBJECT

private:
    Ui::DlgCdg *ui;
//    QGLCanvas *canvas;
    CdgVideoWidget *canvas;
    ScrollText *ticker;
    bool m_fullScreen;
    QRect m_lastSize;

public:
    explicit DlgCdg(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~DlgCdg();
    void updateCDG(QImage image, bool overrideVisibleCheck = false);
    void makeFullscreen();
    void makeWindowed();
    void setTickerText(QString text);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);

public slots:
    void setFullScreen(bool fullscreen);
    void setFullScreenMonitor(int monitor);
    void tickerFontChanged();
    void tickerHeightChanged();
    void tickerSpeedChanged();
    void tickerTextColorChanged();
    void tickerBgColorChanged();
    void tickerEnableChanged();
    void presentBgImage();

};

#endif // CDGWINDOW_H
