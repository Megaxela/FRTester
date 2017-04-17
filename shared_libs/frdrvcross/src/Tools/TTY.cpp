//
// Created by Megaxela on 22.03.2016.
//

#include "Tools/TTY.h"

#include "Tools/Platform.h"

#ifdef OS_LINUX

    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

#endif

#include <thread>
#include "Tools/Logger.h"
#include "Tools/byte.h"
#include "Tools/Time.h"

//
//#ifndef Log
//    #define Log(x)
//#endif
//#ifndef Error
//    #define Error(x)
//#endif
//
//static int TIMEOUT = 1000;
#ifdef OS_WINDOWS
TTY::TTY() :
        m_afterConnectionWaitingTime(100),
        m_Handle(INVALID_HANDLE_VALUE)
{

}
#else
TTY::TTY() :
        m_afterConnectionWaitingTime(100),
        m_fileDescriptor(0)
{

}
#endif

TTY::~TTY()
{
    disconnect();
}

bool TTY::isInitialized() const
{
#ifdef OS_WINDOWS
    return m_Handle != INVALID_HANDLE_VALUE;
#endif
#ifdef OS_LINUX
    return m_fileDescriptor > 0;
#endif
}

int TTY::connect(const std::string &port, int baudrate)
{
    disconnect();

#ifdef OS_WINDOWS
//    if (!stdex::begins(port, "COM"))
//    {
//        return 7;
//    }

    Log("Подключаемся к порту \"" + port + "\".");
    m_Handle = CreateFileA(
            ("\\\\.\\" + port).c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );

    if(m_Handle == INVALID_HANDLE_VALUE)
    {
        Log("Не удалось подключиться к объекту. Ошибка CreateFileA: " + std::to_string(GetLastError()));
        return 6;
    }

    Log("Подключение прошло успешно. Настраиваем подключение.");
    if (!SetCommMask(m_Handle, EV_RXCHAR))
    {
        Error("Не удалось установить маску. Ошибка SetCommMask: " + std::to_string(GetLastError()));
        return 5;
    }

    if (!SetupComm(m_Handle, 1500, 1500))
    {
        Error("Не удалось настроить скорости передачи данных. Ошибка SetupComm: " + std::to_string(GetLastError()));
        return 4;
    }

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = static_cast<DWORD>(10000);
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = static_cast<DWORD>(10000);

    if (!SetCommTimeouts(m_Handle, &CommTimeOuts))
    {
        Error("Не удалось настроить таймауты. Ошибка SetCommTimeouts: " + std::to_string(GetLastError()));
        CloseHandle(m_Handle);
        m_Handle = INVALID_HANDLE_VALUE;
        return 3;
    }

    DCB ComDCM;

    memset(&ComDCM, 0, sizeof(ComDCM));
    ComDCM.DCBlength = sizeof(DCB);

    if (!GetCommState(m_Handle, &ComDCM))
    {
        Error("Не удалось получить состояние порта. Ошибка GetCommState: " + std::to_string(GetLastError()));
        return 2;
    }

    ComDCM.BaudRate = DWORD(baudrate);
    ComDCM.ByteSize = 8;
    ComDCM.Parity = NOPARITY;
//    ComDCM.Parity = EVENPARITY;
    ComDCM.StopBits = ONESTOPBIT;
//    ComDCM.StopBits = TWOSTOPBITS;
    ComDCM.fAbortOnError = TRUE;
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE;
    ComDCM.fRtsControl = RTS_CONTROL_DISABLE;
    ComDCM.fBinary = TRUE;
    ComDCM.fParity = FALSE;
    ComDCM.fInX = FALSE;
    ComDCM.fOutX = FALSE;
    ComDCM.XonChar = 0;
    ComDCM.XoffChar = (unsigned char) 0xFF;
    ComDCM.fErrorChar = FALSE;
    ComDCM.fNull = FALSE;
    ComDCM.fOutxCtsFlow = FALSE;
    ComDCM.fOutxDsrFlow = FALSE;
    ComDCM.XonLim = 128;
    ComDCM.XoffLim = 128;

    if (!SetCommState(m_Handle, &ComDCM))
    {
        Error("Не удалось задать состояние порта. Ошибка SetCommState: " + std::to_string(GetLastError()));
        CloseHandle(m_Handle);
        m_Handle = INVALID_HANDLE_VALUE;
        return 1;
    }

    sleep(m_afterConnectionWaitingTime);

    Log("Соединение завершено успешно.");
    return 0;
#endif
#ifdef OS_LINUX
    Log("Подключаемся к интерфейсу \"" + port + "\".");
    m_fileDescriptor = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (m_fileDescriptor <= 0)
    {
        int err = errno;
        Error("Не удалось открыть интерфейс. Ошибка open #" +
                   std::to_string(err) + ": " +
                   std::string(strerror(err)));
        return err;
    }

    // Setting up file
    struct termios tty;
    struct termios tty_old;
    memset(&tty, 0, sizeof(tty));

    // Получаем аттрибуты
    if (tcgetattr(m_fileDescriptor, &tty) != 0)
    {
        int err = errno;
        Error("Не удалось получить атрибуты. Ошибка tcgetattr #" +
               std::to_string(err) +
               std::string(strerror(err)));
        return err;
    }

    // Сохраняем старые параметры
    tty_old = tty;

    speed_t baudRateDef = B0;
    switch (baudrate)
    {
    case 0:
        break;
    case 50:
        baudRateDef =  B50;
        break;
    case 75:
        baudRateDef =  B75;
        break;
    case 110:
        baudRateDef =  B110;
        break;
    case 134:
        baudRateDef =  B134;
        break;
    case 150:
        baudRateDef =  B150;
        break;
    case 200:
        baudRateDef =  B200;
        break;
    case 300:
        baudRateDef =  B300;
        break;
    case 600:
        baudRateDef =  B600;
        break;
    case 1200:
        baudRateDef =  B1200;
        break;
    case 1800:
        baudRateDef =  B1800;
        break;
    case 2400:
        baudRateDef =  B2400;
        break;
    case 4800:
        baudRateDef =  B4800;
        break;
    case 9600:
        baudRateDef =  B9600;
        break;
    case 19200:
        baudRateDef =  B19200;
        break;
    case 38400:
        baudRateDef =  B38400;
        break;
    case 57600:
        baudRateDef =  B57600;
        break;
    case 115200:
        baudRateDef =  B115200;
        break;
    case 230400:
        baudRateDef =  B230400;
        break;
    case 460800:
        baudRateDef =  B460800;
        break;
    case 500000:
        baudRateDef =  B500000;
        break;
    case 576000:
        baudRateDef =  B576000;
        break;
    case 921600:
        baudRateDef =  B921600;
        break;
    case 1000000:
        baudRateDef =  B1000000;
        break;
    case 1152000:
        baudRateDef =  B1152000;
        break;
    case 1500000:
        baudRateDef =  B1500000;
        break;
    case 2000000:
        baudRateDef =  B2000000;
        break;
    case 2500000:
        baudRateDef =  B2500000;
        break;
    case 3000000:
        baudRateDef =  B3000000;
        break;
    case 3500000:
        baudRateDef =  B3500000;
        break;
    case 4000000:
        baudRateDef =  B4000000;
        break;
    default:
        Error("Неизвестный baud rate (" + std::to_string(baudrate) + ").");
        return -1;
    }

    // Устанавливаем скорость обмена данными
    cfsetospeed(&tty, (speed_t) baudRateDef);
    cfsetispeed(&tty, (speed_t) baudRateDef);

    tty.c_cflag |= (CLOCAL | CREAD);

    // 8 бит
    tty.c_cflag |= CS8;

    // Не обрабатывать данные
    tty.c_lflag &= ~(ICANON | ECHO | ISIG);

    // Устанавливаем параметры порта
    tty.c_cflag &= ~PARODD;

    // 1 стоп бит
    tty.c_cflag &= ~CSTOPB;

    // Отключаем контроль порта
    tty.c_cflag &= ~CRTSCTS;

    //
    tty.c_cflag &= ~PARENB;

    tty.c_cc[VMIN] = 1;

    tty.c_cc[VTIME] = 2;

    cfmakeraw(&tty);

    sleep(2); // Ошибка в ядре linux.
    tcflush(m_fileDescriptor, TCIOFLUSH);
    if (tcsetattr(m_fileDescriptor, TCSANOW, &tty) != 0)
    {
        int err = errno;
        Error("Не удалось установить атрибуты. Ошибка tcsetattr #" +
                   std::to_string(err) + ": " +
                   std::string(strerror(err)));
        return err;
    }

    if(m_fileDescriptor > 0)
    {
        sleep(m_afterConnectionWaitingTime);
    }

    Log("Соединение с интерфейсом успешно открыто и настроено.");

    return 0;
#endif
}

void TTY::disconnect()
{
#ifdef OS_WINDOWS
    if (m_Handle != INVALID_HANDLE_VALUE)
    {
        if (!CloseHandle(m_Handle))
        {
            Error("Не удалось закрыть соединение. Ошибка CloseHandle: " + std::to_string(GetLastError()));
            return;
        }

        m_Handle = INVALID_HANDLE_VALUE;
    }
#else
    if (m_fileDescriptor)
        close(m_fileDescriptor);
    m_fileDescriptor = 0;
#endif
}

uint32_t TTY::write(const ByteArray &dataArray) const
{
#ifdef OS_WINDOWS
    if (m_Handle == INVALID_HANDLE_VALUE)
        throw TTYException("[TTY::write] Writing without open connection.");

    DWORD feedback;
    if (!WriteFile(m_Handle, dataArray.data(), (DWORD) dataArray.size(), &feedback, 0) || feedback != (DWORD) dataArray.size())
    {
        Error("Ошибка записи в TTY. Ошибка WriteFile: " + std::to_string(GetLastError()));
        CloseHandle(m_Handle);
        m_Handle = INVALID_HANDLE_VALUE;
        throw TTYException("[TTY::write] Error while writing.");
    }

    return dataArray.length();
    // In some cases it's worth uncommenting
    //FlushFileBuffers(m_Handle);
#endif
#ifdef OS_LINUX
    ssize_t dataWrote = ::write(m_fileDescriptor, dataArray.data(), dataArray.size());
    if (dataWrote < dataArray.size())
    {
        Error("Было отправлено только " +
                   std::to_string(dataWrote) +
                   " байт, вместо " +
                   std::to_string(dataArray.size()));
    }

    return (uint32_t) dataWrote;
#endif
}

ByteArray TTY::read(uint32_t size, uint32_t timeoutMcs) const
{
    LogStream() << "Запрашиваю " << size << " байт с таймаутом в " << timeoutMcs / 1000.0 << " миллисекунд." << std::endl;

    auto beginReadTime = Time::get<std::chrono::microseconds>();
#ifdef OS_WINDOWS
    byte* response = new byte[size];
    memset(response, 0, size * sizeof(byte));

    DWORD feedback = 0;

    DWORD dataRead = 0;

    auto beginReadTime = Time::get<std::chrono::microseconds>();

    while (dataRead < size)
    {
        if (!ReadFile(m_Handle, response + dataRead, size - dataRead, &feedback, NULL))
        {
            Error("Не удалось считать данных из TTY. Ошибка ReadFile: " + std::to_string(GetLastError()));
            CloseHandle(m_Handle);
            m_Handle = INVALID_HANDLE_VALUE;
            throw TTYException("[TTY::read] Can't read from COM.");
        }

        dataRead += feedback;

        if (Time::get<std::chrono::microseconds>() - beginReadTime > timeoutMcs)
        {
            Error("Timeout чтения.");
            break;
        }
    }

#endif
#ifdef OS_LINUX
    // Добавляем наш дескриптов в файл
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    FD_ZERO(&read_fds); // Очищаем набор
    FD_ZERO(&write_fds); // Очищаем набор
    FD_ZERO(&except_fds); // Очищаем набор

    FD_SET(m_fileDescriptor, &read_fds);

    byte* response = new byte[size];
    // Закомментированно, поскольку компилятор и так
    // скорее всего удалит этот memset.
    // Если вдруг пригорит - надо использовать memset_s.
//    memset(response, 0, size * sizeof(byte));

    size_t dataRead = 0;

    // Запоминаем время начала чтения
    while (dataRead < size)
    {
        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = (__suseconds_t) (timeoutMcs - (Time::getMicroseconds() - beginReadTime));

        int r = select(m_fileDescriptor + 1,
                       &read_fds,
                       &write_fds,
                       &except_fds,
                       &timeout);

        if (r == -1)
        {
            Error("Ошибка select. #" +
                       std::to_string(errno) + ": " +
                       strerror(errno));

            delete[] response;
            return ByteArray();
        }
        else if (r == 1)
        {
            dataRead += ::read(m_fileDescriptor,
                               response + dataRead,
                               size - dataRead);
        }
        else
        {
            Error("Таймаут чтения.");
            break;
        }
    }

#endif
    if (dataRead == size)
    {
        LogStream() << "Чтение успешно завершено за "
                    << (Time::getMicroseconds() - beginReadTime) / 1000.0
                    << " миллисекунд."
                    << std::endl;
    }
    else
    {
        ErrorStream() << "Чтение не полностью завершено за "
                      << (Time::getMicroseconds() - beginReadTime) / 1000.0
                      << " миллисекунд."
                      << std::endl;
    }

    auto result =  ByteArray(response, static_cast<uint32_t>(dataRead));

    delete[] response;

    return result;
}

void TTY::sleep(int ms) const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void TTY::setAfterConnectionDelay(int ms)
{
    m_afterConnectionWaitingTime = ms;
}

int TTY::afterConnectionDelay()
{
    return m_afterConnectionWaitingTime;
}

TTY::time_t TTY::getTimeMs()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    return static_cast<time_t>(ms.count());
}