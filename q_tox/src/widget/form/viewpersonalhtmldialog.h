#ifndef VIEWPERSONALHTMLDIALOG_H
#define VIEWPERSONALHTMLDIALOG_H
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



#include <QDialog>
#include "src/model/imessagedispatcher.h"
#include <deque>

#include "src/core/icorefriendmessagesender.h"
#include "src/model/friend.h"
#include "src/model/friendmessagedispatcher.h"
#include "src/model/message.h"


namespace Ui {
class ViewPersonalHtmlDialog;
}

class ViewPersonalHtmlDialog : public QDialog
{
    Q_OBJECT

public:
    enum ReturnCode
    {
        Rejected = QDialog::Rejected,
        Accepted = QDialog::Accepted,
        Tertiary
    };
    explicit ViewPersonalHtmlDialog(QString body, QString extraButton, QWidget* parent = nullptr);
    ~ViewPersonalHtmlDialog();
private slots:
private:
    Ui::ViewPersonalHtmlDialog* ui;
    QString body;
    static const double reasonablePasswordLength;
    IMessageDispatcher& messageDispatcher;
    // All unique_ptrs to make construction/init() easier to manage
    std::unique_ptr<Friend> f;
    std::unique_ptr<MessageProcessor::SharedParams> sharedProcessorParams;
    std::unique_ptr<MessageProcessor> messageProcessor;
    std::unique_ptr<FriendMessageDispatcher> friendMessageDispatcher;
    std::map<DispatchedMessageId, Message> outgoingMessages;
    std::deque<Message> receivedMessages;
protected slots:
    void on_pushButton_clicked();
    void onMessageSent(DispatchedMessageId id, Message message)
    {
        auto it = outgoingMessages.find(id);
        outgoingMessages.emplace(id, std::move(message));
    }

    void onMessageComplete(DispatchedMessageId id)
    {
        auto it = outgoingMessages.find(id);
        outgoingMessages.erase(it);
        qDebug("------onMessageComplete------");
    }

    void onMessageReceived(const ToxPk& sender, Message message)
    {
        qDebug("------onMessageReceived------");
        receivedMessages.push_back(std::move(message));
    }

};
#endif // VIEWPERSONALHTMLDIALOG_H



