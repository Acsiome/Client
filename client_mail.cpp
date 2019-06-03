#include "stdafx.h"
#include <iostream>
#pragma comment (lib,"ws2_32.lib")

int main(int arg, TCHAR* argv[])
{
    const uint32_t PORT = 6666;
    const std::string IP = "127.0.0.1";
    unsigned long SERVERADDRES= inet_addr(IP.c_str());


    setlocale (LC_CTYPE, "Russian");
    std::vector<char> buff;
    std::cout<<"Клиент v 1.0 \n";
    WSADATA ws;
    if (FAILED(WSAStartup (MAKEWORD(1, 1), &ws) ))
    {
        std::cout<<"Ошибка WSAStartup %d \n"<< WSAGetLastError();
        return -1;
    }
     SOCKET my_sock;
    if ((my_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
    {
        std::cout<<"Ошибка сокета \n"<< WSAGetLastError();
        WSACleanup();
        return -1;
    }

    // Шаг 3 - установка соединения
    // заполнение структуры sockaddr_in - указание адреса и порта сервера
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT *hst;

    // преобразование IP адреса из символьного в сетевой формат
    if (SERVERADDRES != INADDR_NONE)
        dest_addr.sin_addr.s_addr = SERVERADDRES;
    else
    {
        // попытка получить IP адрес по доменному имени сервера
        if (hst = gethostbyname(IP.c_str()))
            // hst->h_addr_list содержит не массив адресов,
            // а массив указателей на адреса
            ((unsigned long *)&dest_addr.sin_addr)[0] =
            ((unsigned long **)hst->h_addr_list)[0][0];
        else
        {
            std::cout<<"Invalid address \n"<<" "<<IP;
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
    }
// Адрес сервера получен. Пытаемся восстановить соединение
if (connect (my_sock, (sockaddr*) &dest_addr, sizeof (dest_addr)))
{
    std::cout<<"Ошибка соединения \n"<<WSAGetLastError();
    closesocket(my_sock);
    WSACleanup();
    return -1;
}
std::cout<<"Соединение с "<<IP<<" успешно установлено \n";

//Чтение и передача сообщений
if ( (recv(my_sock, buff.data(), buff.size(), 0)) != SOCKET_ERROR)
    {
    std::cout<<"Введите логин \n";
    std::string log, pass, res;
    std::getline(std::cin, log);
    std::cout<<"Введите пароль \n";
    std::getline(std::cin, pass);
    res=log+'~'+pass;

    for (char c : res)
    buff.push_back(c);
    send(my_sock,buff.data(),buff.size(),0);

    if (!strcmp(buff.data(), "quit \n"))
    {
         std::cout<<"Выход";
        closesocket(my_sock);
        WSACleanup();
        return 0;
    }
     send(my_sock, buff.data(), buff.size(), 0);
}
std::cout<<"Recy Error"<<WSAGetLastError();
WSACleanup();
closesocket(my_sock);
system("pause");
return -1;

}
