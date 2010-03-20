/**
 * \file SerialPlugin.cpp
 * \author Denis Martinez
 */

#include "SerialPlugin.h"

#include <QDebug>

#include "SerialWidget.h"
#include "IDEApplication.h"

bool SerialPlugin::setup(IDEApplication *app)
{
    mApp = app;
    mName = tr("Serial");

    widget = new SerialWidget;
    app->mainWindow()->utilityTabWidget()->addTab(widget, name());

    connect(widget, SIGNAL(openRequested()), this, SLOT(open()));
    connect(widget, SIGNAL(closeRequested()), this, SLOT(close()));

    return true;
}

void SerialPlugin::open()
{
    if (mSerial.data() != NULL)
        mSerial->close();

    QString port = mApp->settings()->devicePort();
    if (port.isEmpty())
    {
        widget->setStatus(tr("Unable to open, you must select a device first."));
        return;
    }

    int baudRate = widget->baudRate();

    mSerial.reset(new Serial(port, baudRate));
    if (! mSerial->open(QIODevice::ReadWrite))
    {
        widget->setStatus(tr("Open failed: %0").arg(mSerial->errorString()));
        return;
    }

    widget->setStatus(tr("Serial port opened successfully."));
}

void SerialPlugin::close()
{
    if (mSerial.data() != NULL)
        mSerial->close();

    widget->setStatus(tr("Serial port closed."));
}

Q_EXPORT_PLUGIN2(serial, SerialPlugin)