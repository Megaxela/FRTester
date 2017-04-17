//
// Created by megaxela on 04.04.17.
//

#include "include/Windows/Controllers/CommandsControllers/FNFiscalisationTabController.h"
#include <ui_mainwindow.h>
#include <QCheckBox>
#include <QMessageBox>
#include <DriverHolder.h>
#include <Windows/Controllers/CommandsTabController.h>
#include <shared_libs/frdrvcross/include/Tools/Logger.h>

FNFiscalisationTabController::FNFiscalisationTabController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent),
    m_taxMode(0),
    m_workMode(0)
{

}

void FNFiscalisationTabController::setupConnections()
{
    connect(ui()->commandsFNFiscalisationGenerateINNCSPushButton,
            &QPushButton::clicked,
            this,
            &FNFiscalisationTabController::onGenerateINNClicked);

    connect(ui()->commandsFNFiscalisationGenerateRNMPushButton,
            &QPushButton::clicked,
            this,
            &FNFiscalisationTabController::onGenerateRNMClicked);

    connect(ui()->commandsFNFiscalisationRegisterPushButton,
            &QPushButton::clicked,
            this,
            &FNFiscalisationTabController::onRegistrationButtonClicked);

    connect(ui()->commandsFNFiscalisationReRegisterPushButton,
            &QPushButton::clicked,
            this,
            &FNFiscalisationTabController::onReRegistrationButtonClicked);

    connect(ui()->commandsFNFiscalisationClearPushButton,
            &QPushButton::clicked,
            this,
            &FNFiscalisationTabController::onResetButtonClicked);

    // Налоги
    connect(ui()->commandsFNFiscalisationTax1CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 1); }); // 0b1

    connect(ui()->commandsFNFiscalisationTax2CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 2); }); // 0b01

    connect(ui()->commandsFNFiscalisationTax3CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 4); }); // 0b001

    connect(ui()->commandsFNFiscalisationTax4CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 8); }); // 0b0001

    connect(ui()->commandsFNFiscalisationTax5CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 16); }); // 0b00001

    connect(ui()->commandsFNFiscalisationTax6CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onTaxCheckboxClicked(checked, 32); }); // 0b000001

    // Режим работы
    connect(ui()->commandsFNFiscalisationWorkMode1CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 1); }); //0b1

    connect(ui()->commandsFNFiscalisationWorkMode2CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 2); }); //0b01

    connect(ui()->commandsFNFiscalisationWorkMode3CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 4); }); //0b001

    connect(ui()->commandsFNFiscalisationWorkMode4CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 8); }); //0b0001

    connect(ui()->commandsFNFiscalisationWorkMode5CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 16); }); //0b00001

    connect(ui()->commandsFNFiscalisationWorkMode6CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 32); }); //0b000001

    connect(ui()->commandsFNFiscalisationWorkMode7CheckBox,
            &QCheckBox::clicked,
            [=](bool checked){ onWorkModeCheckboxClicked(checked, 64); }); //0b0000001
}

void FNFiscalisationTabController::configureWidgets()
{

}

void FNFiscalisationTabController::onTaxCheckboxClicked(bool checked, uint8_t orBits)
{
    if (checked)
    {
        m_taxMode |= orBits;
    }
    else
    {
        m_taxMode ^= orBits;
    }

    ui()->commandsFNFiscalisationTaxValueSpinBox->setValue(m_taxMode);
}

void FNFiscalisationTabController::onWorkModeCheckboxClicked(bool checked, uint8_t orBits)
{
    if (checked)
    {
        m_workMode |= orBits;
    }
    else
    {
        m_workMode ^= orBits;
    }


    ui()->commandsFNFiscalisationWorkModeValueSpinBox->setValue(m_workMode);
}

void FNFiscalisationTabController::onGenerateINNClicked()
{
    QString inn = ui()->commandsFNFiscalisationINNLineEdit->text();

    if (inn.length() != 9 && inn.length() != 10)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Для генерации требуется указать первые 9 (или 10 для физ. лица) "
                "цифр ИНН."
        );

        return;
    }

    for (auto& c : inn)
    {
        if (!c.isNumber())
        {
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "ИНН должен состоять из цифр."
            );

            return;
        }
    }

    if (inn.length() == 9)
    {
        inn += QString::number(
                ((2  * inn[0].digitValue() +
                  4  * inn[1].digitValue() +
                  10 * inn[2].digitValue() +
                  3  * inn[3].digitValue() +
                  5  * inn[4].digitValue() +
                  9  * inn[5].digitValue() +
                  4  * inn[6].digitValue() +
                  6  * inn[7].digitValue() +
                  8  * inn[8].digitValue()) % 11) % 10
        );
    }
    else if (inn.length() == 10)
    {
        inn += QString::number(
                ((7  * inn[0].digitValue() +
                  2  * inn[1].digitValue() +
                  4  * inn[2].digitValue() +
                  10 * inn[3].digitValue() +
                  3  * inn[4].digitValue() +
                  5  * inn[5].digitValue() +
                  9  * inn[6].digitValue() +
                  4  * inn[7].digitValue() +
                  6  * inn[8].digitValue() +
                  8  * inn[9].digitValue()) % 11) % 10
        );

        inn += QString::number(
                ((3  * inn[0 ].digitValue() +
                  7  * inn[1 ].digitValue() +
                  2  * inn[2 ].digitValue() +
                  4  * inn[3 ].digitValue() +
                  10 * inn[4 ].digitValue() +
                  3  * inn[5 ].digitValue() +
                  5  * inn[6 ].digitValue() +
                  9  * inn[7 ].digitValue() +
                  4  * inn[8 ].digitValue() +
                  6  * inn[9 ].digitValue() +
                  8  * inn[11].digitValue()) % 11) % 10
        );
    }

    ui()->commandsFNFiscalisationINNLineEdit->setText(inn);
}

/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
const unsigned short Crc16Table[256] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

unsigned short Crc16(unsigned char * pcBlock, unsigned short len)
{
    unsigned short crc = 0xFFFF;

    while (len--)
        crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *pcBlock++];

    return crc;
}

void FNFiscalisationTabController::onGenerateRNMClicked()
{
    QString posNumber = ui()->commandsFNFiscalisationPOSNumberLineEdit->text();
    for (auto& c : posNumber)
    {
        if (!c.isNumber())
        {
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "Номер ККТ должен быть числом."
            );

            return;
        }
    }

    if (posNumber.length() > 10)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Номер ККТ должен иметь меньше 10 символов"
        );

        return;
    }

    QString innNumber = ui()->commandsFNFiscalisationINNLineEdit->text();
    if (innNumber.length() != 10 && innNumber.length() != 12)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "ИНН должен иметь длину в 10 или в 12 символов"
        );

        return;
    }

    for (auto& c : posNumber)
    {
        if (!c.isNumber())
        {
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "ИНН должен быть числом."
            );

            return;
        }
    }

    // Запрос заводского номера ККТ
    auto firmwareNumber = DriverHolder::driver().readTableStr(
        commandsTabControllerParent()->password(),
        0x12,
        0x01,
        0x01
    );

    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Не удалось получить заводской номер ККТ. Ошибка: #" +
                QString::number((int) DriverHolder::driver().getLastError()) +
                ' ' +
                QString::fromStdString(
                        FRDriver::Converters::errorToString(
                                (int) DriverHolder::driver().getLastError()
                        )
                )
        );

        return;
    }

    QString concatenatedResult;

    auto count = 10 - posNumber.length();
    for (int32_t i = 0; i < count; ++i)
    {
        posNumber = '0' + posNumber;
    }
    concatenatedResult += posNumber;
    LogStream() << "Номер ККТ: " << posNumber.toStdString() << std::endl;

    count = 12 - innNumber.length();
    for (int32_t i = 0; i < count; ++i)
    {
        innNumber = '0' + innNumber;
    }
    concatenatedResult += innNumber;
    LogStream() << "ИНН: " << innNumber.toStdString() << std::endl;

    count = static_cast<int>(20 - firmwareNumber.length());
    for (int32_t i = 0; i < count; ++i)
    {
        firmwareNumber = '0' + firmwareNumber;
    }
    concatenatedResult += QString::fromStdString(firmwareNumber);
    LogStream() << "Заводской номер ККТ: " << firmwareNumber << std::endl;

    LogStream() << "Результат для вычисления CRC: " << concatenatedResult.toStdString() << std::endl;

    auto result = Crc16((unsigned char *) concatenatedResult.toStdString().c_str(),
                        (unsigned short) concatenatedResult.length());

    QString strResult = QString::number(result);
    for (int32_t i = 0; i < 6 - strResult.length(); ++i)
    {
        strResult = '0' + strResult;
    }

    ui()->commandsFNFiscalisationRNMLineEdit->setText(
            posNumber + strResult
    );
}

void FNFiscalisationTabController::onRegistrationButtonClicked()
{
    // Проверяем значения
    auto inn = ui()->commandsFNFiscalisationINNLineEdit->text();

    if (inn.size() != 10 && inn.size() != 12)
    {
        QMessageBox::information(
                parentWidget(),
                "Ошибка",
                "ИНН должен быть длиной в 10 или 12 символов."
        );

        return;
    }

    auto rnm = ui()->commandsFNFiscalisationRNMLineEdit->text();

    if (rnm.isEmpty())
    {
        QMessageBox::information(
                parentWidget(),
                "Ошибка",
                "РНМ требуется сгенерировать."
        );

        return;
    }

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        return;
    }

    auto pwd = commandsTabControllerParent()->password();

    auto report = DriverHolder::driver().formPOSRegistrationReport(
            pwd,
            inn.toStdString(),
            rnm.toStdString(),
            static_cast<uint8_t>(ui()->commandsFNFiscalisationTaxValueSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsFNFiscalisationWorkModeValueSpinBox->value())
    );

    commandsTabControllerParent()->setLastStatus();
}

void FNFiscalisationTabController::onReRegistrationButtonClicked()
{
    // Проверяем значения
    auto inn = ui()->commandsFNFiscalisationINNLineEdit->text();

    if (inn.size() != 10 && inn.size() != 12)
    {
        QMessageBox::information(
                parentWidget(),
                "Ошибка",
                "ИНН должен быть длиной в 10 или 12 символов."
        );

        return;
    }

    auto rnm = ui()->commandsFNFiscalisationRNMLineEdit->text();

    if (rnm.isEmpty())
    {
        QMessageBox::information(
                parentWidget(),
                "Ошибка",
                "РНМ требуется сгенерировать."
        );

        return;
    }

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        return;
    }

    auto pwd = commandsTabControllerParent()->password();

    auto report = DriverHolder::driver().formPOSRegistrationReport(
            pwd,
            inn.toStdString(),
            rnm.toStdString(),
            static_cast<uint8_t>(ui()->commandsFNFiscalisationTaxValueSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsFNFiscalisationWorkModeValueSpinBox->value())
    );

    commandsTabControllerParent()->setLastStatus();
}

void FNFiscalisationTabController::onResetButtonClicked()
{
    QMessageBox::information(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано, сорян."
    );
}

CommandsTabController *FNFiscalisationTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}
