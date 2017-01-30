#ifndef TAGREADER_H
#define TAGREADER_H

#include <QObject>
#include <gst/gst.h>

class TagReader : public QObject
{
    Q_OBJECT
private:
    QString m_artist;
    QString m_title;
    unsigned int m_duration;
    QString m_path;

public:
    explicit TagReader(QObject *parent = 0);
    QString getArtist();
    QString getTitle();
    unsigned int getDuration();
    void setMedia(QString path);

signals:

public slots:
};

#endif // TAGREADER_H
