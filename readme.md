# Урок 10, Работа с сетью. TCP клиент

Инструкция к заданию

- Скачать исполняемый файл и библиотеки сервера — это необходимо для проверки и отладки.
- Скачать прекод клиента.
- Реализовать функции в клиентском приложении в соответствии с заданием.

## Задание 1

Необходимо разработать функции приложения.

1. Реализовать следующие методы:
- TCPclient::TCPclient
- TCPclient::SendRequest
- TCPclient::ConnectToHost
- TCPclient::DisconnectFromHost
- TCPclient::ProcessingData
- MainWindow::DisplayTime
- MainWindow::DisplayStat
- MainWindow::on_pb_request_clicked
2. В приложении должны использоваться сигналы:
- sig_sendTime
- sig_Disconnected
- sig_connectStatus
- sig_sendStat
3. Необходимо реализовать обработчик и вывод в лог следующих сообщений:
- приёма времени от сервера,
- вывода статистики сервера.
4. Приложение должно подключаться к серверу по введённому пользователем адресу и порту.
5. Приложение должно отключаться от сервера.

CPP-07
14.10.2023
[@J1n4ed](https://github.com/J1n4ed)
