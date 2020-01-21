#include "viewpersonalhtmldialog.h"

/*
    Copyright © 2014-2019 by The qTox Project Contributors

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details. ViewPersonalHtmlDialog

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "ui_viewpersonalhtmldialog.h"

#include <QApplication>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include "src/core/core.h"
#include "src/core/corefile.h"
#include "src/model/message.h"
#include <deque>
#include "src/model/friend.h"
#include "src/model/friendmessagedispatcher.h"
#include <QDebug>
#include "src/persistence/settings.h"
#include "src/model/status.h"
#include "src/core/icorefriendmessagesender.h"
#include "src/model/friend.h"
#include "src/model/friendmessagedispatcher.h"
#include "src/model/message.h"
#include <QObject>
#include <deque>
#include "src/nexus.h"
#include "src/model/friend.h"
#include "src/friendlist.h"
#include "src/core/toxstring.h"

const double ViewPersonalHtmlDialog::reasonablePasswordLength = 8.;

ViewPersonalHtmlDialog::ViewPersonalHtmlDialog(QString body, QString extraButton, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ViewPersonalHtmlDialog)
    , body(body + "\n\n")
    , messageDispatcher(messageDispatcher)
{
     ui->setupUi(this);
}

ViewPersonalHtmlDialog::~ViewPersonalHtmlDialog()
{
    delete ui;
}

void ViewPersonalHtmlDialog::on_pushButton_clicked()
{
    //view html with Browser
    //QString htmlPath = "/home/grj/grj/minitox-master/page.out";
    //QDesktopServices::openUrl(QUrl("file:///"+htmlPath));

    //send file with tox
    /*QString path ="/home/grj/grj/minitox-master/page_test.out";
    QFile file(path);
    QString fileName = QFileInfo(path).fileName();
    qint64 filesize = file.size();
    Core* coreTemp = Core::getInstance();
    coreTemp->getCoreFile()->sendFile(1, fileName, path, filesize);*/


    //send message with tox
    //init
    f = std::unique_ptr<Friend>(new Friend(3, ToxPk()));
    f->setStatus(Status::Status::Online);
    sharedProcessorParams =
        std::unique_ptr<MessageProcessor::SharedParams>(new MessageProcessor::SharedParams());
    messageProcessor = std::unique_ptr<MessageProcessor>(new MessageProcessor(*sharedProcessorParams));
    Core* core = nullptr;
    core = Nexus::getCore();
    friendMessageDispatcher = std::unique_ptr<FriendMessageDispatcher>(
        new FriendMessageDispatcher(*f, *messageProcessor, *core));
    connect(friendMessageDispatcher.get(), &FriendMessageDispatcher::messageSent, this,
            &ViewPersonalHtmlDialog::onMessageSent);
    connect(friendMessageDispatcher.get(), &FriendMessageDispatcher::messageComplete, this,
            &ViewPersonalHtmlDialog::onMessageComplete);
    connect(friendMessageDispatcher.get(), &FriendMessageDispatcher::messageReceived, this,
            &ViewPersonalHtmlDialog::onMessageReceived);
    outgoingMessages = std::map<DispatchedMessageId, Message>();
    receivedMessages = std::deque<Message>();
    friendMessageDispatcher->sendMessage(false, "html");

    //receive file with tox
    QString path ="/home/grj/grj/minitox-master/page_test.out";
    Core* coreTemp = Core::getInstance();
    coreTemp->getCoreFile()->acceptFileRecvRequest(3, 65536, path); //fileInfo.friendId, fileInfo.fileNum, filepath
    qDebug("getCoreFile()->acceptFileRecvRequest");

}


