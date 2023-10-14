#include "../headers/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    about = new About(this);

    ui->statusBar->addWidget(ui->sb_frame);

    client = new TCPclient(this);

    //Доступность полей по умолчанию
    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");

    mo_cat.setFileName(":/images/cat");
    ui->lb_image->setMovie(&mo_cat);

    //При отключении меняем надписи и доступность полей.
    connect(client, &TCPclient::sig_Disconnected, this, [&]
    {

        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);

        ui->sb_label_status->setText("Отключено");
        ui->sb_label_status->setStyleSheet("color: red");

        mo_cat.stop();
    });

 /*
  * Соединяем сигналы со слотами
 */

    connect(client, &TCPclient::sig_sendTime, this, &MainWindow::DisplayTime);
    connect(client, &TCPclient::sig_sendFreeSize, this, &MainWindow::DisplayFreeSpace);
    connect(client, &TCPclient::sig_sendStat, this, &MainWindow::DisplayStat);
    connect(client, &TCPclient::sig_Success, this, &MainWindow::DisplaySuccess);
    connect(client, &TCPclient::sig_Error, this, &MainWindow::DisplayError);
    connect(client, &TCPclient::sig_SendReplyForSetData, this, &MainWindow::SetDataReply);
    connect(client, &TCPclient::sig_connectStatus, this, &MainWindow::DisplayConnectStatus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Группа методо отображения различных данных
 */
void MainWindow::DisplayTime(QDateTime time)
{
    qDebug() << "DEBUG: in MainWindow::DisplayTime";

    ui->tb_result->append("Время на сервере: " + time.toString());
}
void MainWindow::DisplayFreeSpace(uint32_t freeSpace)
{
    qDebug() << "DEBUG: in MainWindow::DisplayFreeSpace";

    ui->tb_result->append("Свободное место на сервере: " + QString::number(freeSpace) + " байт");
}
void MainWindow::SetDataReply(QString replyString)
{
    qDebug() << "DEBUG: in MainWindow::SetDataReply";

    ui->tb_result->append("Строка: '" + replyString + "' успешно получена");

}
void MainWindow::DisplayStat(StatServer stat)
{
    qDebug() << "DEBUG: in MainWindow::DisplayStat";

    /*
        uint32_t incBytes;  //принято байт
        uint32_t sendBytes; //передано байт
        uint32_t revPck;    //принто пакетов
        uint32_t sendPck;   //передано пакетов
        uint32_t workTime;  //Время работы сервера секунд
        uint32_t clients;   //Количество подключенных клиентов
    */

    ui->tb_result->append("Статистика сервера: ");
    ui->tb_result->append(" - принято байт: " + QString::number(stat.incBytes) + " байт");
    ui->tb_result->append(" - передано байт: " + QString::number(stat.sendBytes) + " байт");
    ui->tb_result->append(" - принто пакетов: " + QString::number(stat.revPck) + " ед");
    ui->tb_result->append(" - передано пакетов: " + QString::number(stat.sendPck) + " ед");
    ui->tb_result->append(" - время работы сервера секунд: " + QString::number(stat.workTime) + " секунд");
    ui->tb_result->append(" - количество подключенных клиентов: " + QString::number(stat.clients) + " шт");

}
void MainWindow::DisplayError(uint16_t error)
{
    qDebug() << "DEBUG: in MainWindow::DisplayError";

    switch (error)
    {
    case ERR_NO_FREE_SPACE:
    {
        ui->tb_result->append("Ошибка выполнения, не достаточно свободного места на сервере");
        break;
    }
    case ERR_NO_FUNCT:
    {
        ui->tb_result->append("Ошибка выполнения, функционал не реализован");
        break;
    }
    default:
        break;
    }
}
/*!
 * \brief Метод отображает квитанцию об успешно выполненном сообщениии
 * \param typeMess ИД успешно выполненного сообщения
 */
void MainWindow::DisplaySuccess(uint16_t typeMess)
{
    qDebug() << "DEBUG: in MainWindow::DisplaySuccess";

    switch (typeMess)
    {
    case CLEAR_DATA:
    {
        ui->tb_result->append("Запрос на очистку успешно обработан");

        break;
    }
    default:
        break;
    }

}

/*!
 * \brief Метод отображает статус подключения
 */
void MainWindow::DisplayConnectStatus(uint16_t status)
{
    qDebug() << "DEBUG: in MainWindow::DisplayConnectStatus";

    if(status == ERR_CONNECT_TO_HOST)
    {

        ui->tb_result->append("Ошибка подключения к порту: " + QString::number(ui->spB_port->value()));

    }
    else
    {
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);

        mo_cat.start();
    }
}

/*!
 * \brief Обработчик кнопки подключения/отключения
 */
void MainWindow::on_pb_connect_clicked()
{
    qDebug() << "DEBUG: in MainWindow::on_pb_connect_clicked";

    if (ui->pb_connect->text() == "Подключиться")
    {

        uint16_t port = ui->spB_port->value();

        QString ip = ui->spB_ip4->text() + "." +
                     ui->spB_ip4->text() + "." +
                     ui->spB_ip4->text() + "." +
                     ui->spB_ip4->text();

        client->ConnectToHost(QHostAddress(ip), port);
    }
    else
    {
        client->DisconnectFromHost();
    }
}

/*
 * Для отправки сообщения согласно ПИВ необходимо
 * заполнить заголовок и передать его на сервер. В ответ
 * сервер вернет информацию в соответствии с типом сообщения
*/
void MainWindow::on_pb_request_clicked()
{
   qDebug() << "DEBUG: in MainWindow::on_pb_request_clicked";

   ServiceHeader header;

   header.id = ID;
   header.status = STATUS_SUCCES;
   header.len = 0;   

   switch (ui->cb_request->currentIndex())
   {

        //Получить время
        case 0:
        {
            header.idData = GET_TIME;
            break;
        }
       //Получить свободное место
       case 1:
       {
           header.idData = GET_SIZE;
           break;
       }
       //Получить статистику
       case 2:
       {
           header.idData = GET_STAT;
           break;
       }
       //Отправить данные
       case 3:
       {
           QString str;
           header.idData = SET_DATA;
           str = ui->le_data->text();
           header.len += str.toUtf8().size() + sizeof(int);

           client->SendData(header, str);

           break;
       }
       //Очистить память на сервере
       case 4:
       {
           header.idData = CLEAR_DATA;
           break;
       };
       default:
       ui->tb_result->append("Такой запрос не реализован в текущей версии");
       return;

   }

   if (ui->cb_request->currentIndex() != 3)
        client->SendRequest(header);
}

/*!
 * \brief Обработчик изменения индекса запроса
 */
void MainWindow::on_cb_request_currentIndexChanged(int index)
{
    qDebug() << "DEBUG: in MainWindow::on_cb_request_currentIndexChanged";

    //Разблокируем поле отправления данных только когда выбрано "Отправить данные"
    if (ui->cb_request->currentIndex() == 3)
    {
        ui->le_data->setEnabled(true);
    }
    else
    {
        ui->le_data->setEnabled(false);
    }
}

void MainWindow::on_action_exit_triggered()
{
    qDebug() << "DEBUG: in MainWindow::on_action_exit_triggered";

    this->close();
}

void MainWindow::on_action_about_triggered()
{
    qDebug() << "DEBUG: in MainWindow::on_action_about_triggered";

    about->show();
}


void MainWindow::on_action_connect_triggered()
{
    qDebug() << "DEBUG: in MainWindow::on_action_connect_triggered";

    on_pb_connect_clicked();
}

