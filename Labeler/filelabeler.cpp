#include "filelabeler.h"
#include "ui_filelabeler.h"
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QtXml>
#include <QtDebug>

FileLabeler::FileLabeler(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileLabeler)
{
    ui->setupUi(this);
    FileLabeler::default_parameters();
}

FileLabeler::~FileLabeler()
{
    delete ui;
}


void FileLabeler::default_parameters()
{
    QString path = QDir::homePath();
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    ui->line_folder->setText(path);
    ui->line_prefix->setText(date + "_");
    ui->line_suffix->setText("");
    ui->label_name->setText(date + "_" + "001.mp4");
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
    QString date = QDate::currentDate().toString("yyyy-MM-dd_");
    QString fps = "25p_";
    QString resolution = "1080_";

    if (ui->checkBox_Date->isChecked()) prefix += date;
    if (ui->checkBox_FPS->isChecked()) prefix += fps;
    if (ui->checkBox_Res->isChecked()) prefix += resolution;

    ui->label_name->setText(prefix + "001" + suffix + ".mp4");
}


void FileLabeler::rename_files(const QStringList &files, const QStringList &meta)
{
    int i {};
    QDir::setCurrent(ui->line_folder->text());
    for (auto &name: files) {
        QString prefix = ui->line_prefix->text();
        QString suffix = ui->line_suffix->text();
        QStringList current_meta {meta.filter(name.left(5))};
        if (current_meta.size()) {
            FileLabeler::xml_reader(current_meta[0], prefix);
            qDebug() << "Prefix after changes is " << prefix;
        }
        QFile file;
        QString number {QString::number(++i).rightJustified(3, '0')};
        file.setFileName(name);
        file.rename(prefix + number + suffix + ".mp4");
    }
    ui->statusbar->showMessage(QString::number(i) + " files successfully renamed. ");
}


void FileLabeler::xml_reader(const QString &xml_name, QString &prefix)
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
                   if (ui->checkBox_FPS->isChecked()) prefix = prefix + fps + "_";
               }
               if (child.tagName() == "VideoLayout") {
                   QString res {child.attribute(("numOfVerticalLine"))};
                   if (ui->checkBox_Res->isChecked()) prefix = prefix + res + "_";
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


void FileLabeler::on_line_prefix_textEdited(const QString &arg1)
{
    FileLabeler::check_checkers();
}


void FileLabeler::on_line_suffix_textEdited(const QString &arg1)
{
    FileLabeler::check_checkers();
}


void FileLabeler::on_checkBox_Date_stateChanged(int arg1)
{
    FileLabeler::check_checkers();
}


void FileLabeler::on_checkBox_FPS_stateChanged(int arg1)
{
    FileLabeler::check_checkers();
}


void FileLabeler::on_checkBox_Res_stateChanged(int arg1)
{
    FileLabeler::check_checkers();
}


void FileLabeler::on_line_folder_textEdited(const QString &arg1)
{
    FileLabeler::scan_folder();
}


void FileLabeler::on_pushButt_reset_released()
{
    FileLabeler::default_parameters();
    FileLabeler::scan_folder();
}
