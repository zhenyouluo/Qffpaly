#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMainWindow>
#include <QSlider>
#include "audioworks.h"

class QFile;
class QAudioOutput;
class QAudioFormat;
class QTimer;
class ClickedSlider;

class AudioPlayer : public QMainWindow
{
    Q_OBJECT

public:
    AudioPlayer(QWidget* parent = 0);
    ~AudioPlayer();

    //init audioworks, start demuxer and decoder thread
    bool openAudioFile();

    //stop demux and decode threads, destroy audioworks
    void closeAudioFile();

    void stop();

    virtual void keyPressEvent(QKeyEvent* e);
    virtual void closeEvent(QCloseEvent* e);
	void openAudioOutput();
public slots:
 //   void seek();
 //   void setVolum(int volum);
//    void suspend();
 //   void resume();
private slots:
	void eventLoop();

private:
	/*audio play device*/
    QAudioOutput *audioOutput;
    QIODevice* audioDevice;
    std::shared_ptr<AudioBuffer> audioBuffer;
    std::shared_ptr<AudioDecoder> audioDecoder;
    std::shared_ptr<Demuxer> demuxer;

    QTimer* sendTimer;
	
    std::shared_ptr<AudioWorks> aw;
    //ThreadObj* demuxer;
    //ThreadObj* audioDecoder;

    /*ui*/
    ClickedSlider* progressSlider;
    ClickedSlider* volumSlider;
    QPressButton* openButton;
};

#endif // AUDIOPLAYER_H
