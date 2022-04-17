#include "filelabeler.h"
#include "ui_filelabeler.h"
#include <QFileDialog>
#include <QDateTime>
#include <QIntValidator>
#include <QDir>
#include <QFile>
#include <QtXml>
#include <QtDebug>

FileLabeler::FileLabeler(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileLabeler)
{
    ui->setupUi(this);
    ui->line_number->setValidator( new QIntValidator(0, 9999, this));
    // ui->line_prefix->setValidator( new QRegExpValidator( QRegExp( "[A-Z][a-z]{1,9}" ) ))
    FileLabeler::default_parameters();

    connect(ui->line_folder, &QLineEdit::textEdited, this, &FileLabeler::scan_folder);
    connect(ui->line_prefix, &QLineEdit::textEdited, this, &FileLabeler::check_checkers);
    connect(ui->line_suffix, &QLineEdit::textEdited, this, &FileLabeler::check_checkers);
    connect(ui->line_number, &QLineEdit::textEdited, this, &FileLabeler::check_checkers);
    connect(ui->checkBox_Date, &QCheckBox::stateChanged, this, &FileLabeler::check_checkers);
    connect(ui->checkBox_FPS, &QCheckBox::stateChanged, this, &FileLabeler::check_checkers);
    connect(ui->checkBox_Res, &QCheckBox::stateChanged, this, &FileLabeler::check_checkers);
}

FileLabeler::~FileLabeler()
{
    delete ui;
}


void FileLabeler::default_parameters()
{
    QString path = QDir::homePath();
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString default_number = "001";
    ui->line_folder->setText(path);
    ui->line_prefix->setText(date + "_");
    ui->line_suffix->setText("");
    ui->line_number->setText(default_number);
    ui->label_name->setText(date + "_" + default_number + ".mp4");
}


void FileLabeler::on_pushButt_folder_released()
{
    QString current_path = ui->line_folder->text();
    QString path = QFileDialog::getExistingDirectory(this, "Select a folder with files to rename", current_path);
    if (path.size()) {
        ui->line_folder->setText(path);
    }
    FileLabeler::scan_folder();
}


void FileLabeler::scan_folder()
{
    QDir path = ui->line_folder->text();
    QString settings_info = " No configuration files found.";
    QStringList files = path.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);
    QStringList meta = path.entryList(QStringList() << "*.xml" << "*.XML",QDir::Files);
    if (meta.size()) {
        ui->checkBox_Date->setEnabled(true);
        ui->checkBox_FPS->setEnabled(true);
        ui->checkBox_Res->setEnabled(true);
        settings_info = " Configuration files found.";
    } else {
        ui->checkBox_Date->setEnabled(false);
        ui->checkBox_FPS->setEnabled(false);
        ui->checkBox_Res->setEnabled(false);
        ui->checkBox_Date->setChecked(false);
        ui->checkBox_FPS->setChecked(false);
        ui->checkBox_Res->setChecked(false);
    }
    if (files.size()) {
        int amount = files.count();
        ui->statusbar->showMessage(QString::number(amount) + " video files found." + settings_info);
    } else {
        ui->statusbar->showMessage("Files not found. Please, select another folder.");
    }
}


void FileLabeler::check_checkers()
{
    QString prefix = ui->line_prefix->text();
    QString suffix = ui->line_suffix->text();
    QString starting_number = ui->line_number->text();
    QString date = QDate::currentDate().toString("yyyy-MM-dd_");
    QString fps = "25p_";
    QString resolution = "1080_";

    if (ui->checkBox_Date->isChecked()) prefix += date;
    if (ui->checkBox_FPS->isChecked()) prefix += fps;
    if (ui->checkBox_Res->isChecked()) prefix += resolution;

    ui->label_name->setText(prefix + starting_number + suffix + ".mp4");
}


void FileLabeler::rename_files(const QStringList &files, const QStringList &meta)
{
    int starting_number = ui->line_number->text().toInt();
    int number_length = ui->line_number->text().size();
    int amount {};
    QDir::setCurrent(ui->line_folder->text());
    for (auto &name: files) {
        QString prefix = ui->line_prefix->text();
        QString suffix = ui->line_suffix->text();
        QStringList current_meta {meta.filter(name.left(5))};
        if (current_meta.size()) {
            FileLabeler::xml_reader(current_meta[0], prefix, suffix);
        }
        QFile file;
        QString current_number {QString::number(starting_number++).rightJustified(number_length, '0')};
        file.setFileName(name);
        file.rename(prefix + current_number + suffix + ".mp4");
        ++amount;
    }
    ui->statusbar->showMessage(QString::number(amount) + " files successfully renamed. ");
}


void FileLabeler::xml_reader(const QString &xml_name, QString &prefix, QString &suffix)
{
    qDebug() << "XML name is " << xml_name;
    QDomDocument xml;
    QFile file(xml_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }
    xml.setContent(&file);
    file.close();

    QDomNode node = xml.documentElement().firstChild();

    while (!node.isNull())
    {
       QString current_tag {node.toElement().tagName()};
       if (current_tag == "CreationDate") {
           QString date {node.toElement().attribute(("value"))};
           if (ui->checkBox_Date->isChecked()) prefix = prefix + date.left(10) + "_";
       }
       if (current_tag == "VideoFormat") {
           QDomElement child {node.firstChild().toElement()};
           while (!child.isNull()) {
               if (child.tagName() == "VideoFrame") {
                   QString fps {child.attribute(("captureFps"))};
                   if (ui->checkBox_FPS->isChecked()) suffix = suffix + "_" + fps;
               }
               if (child.tagName() == "VideoLayout") {
                   QString res {child.attribute(("numOfVerticalLine"))};
                   if (ui->checkBox_Res->isChecked()) suffix = suffix + "_" + res;
               }
               child = child.nextSibling().toElement();
           }
       }
       node = node.nextSibling();
    }
}


void FileLabeler::on_pushButt_rename_released()
{
    QDir path = ui->line_folder->text();
    QStringList files = path.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);
    QStringList meta = path.entryList(QStringList() << "*.xml" << "*.XML",QDir::Files);
    FileLabeler::rename_files(files, meta);
}


void FileLabeler::on_pushButt_reset_released()
{
    FileLabeler::default_parameters();
    FileLabeler::scan_folder();
}

