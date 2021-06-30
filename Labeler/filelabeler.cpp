#include "filelabeler.h"
#include "ui_filelabeler.h"
#include <QFileDialog>
#include <QDateTime>
#include <QDir>

FileLabeler::FileLabeler(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileLabeler)
{
    ui->setupUi(this);
    QString path = QDir::homePath();
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    ui->line_folder->setText(path);
    ui->line_prefix->setText(date + "_");
    ui->label_name->setText(date + "_" + "001.mp4");
}

FileLabeler::~FileLabeler()
{
    delete ui;
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
    QStringList settings = path.entryList(QStringList() << "*.xml" << "*.XML",QDir::Files);
    if (settings.size()) {
        settings_info = " Configuration files found.";
    }
    if (files.size()) {
        int amount = files.count();
        ui->statusbar->showMessage(QString::number(amount) + " files successfully found." + settings_info);
    } else {
        ui->statusbar->showMessage("Files not found. Please, select another folder.");
    }
}

void FileLabeler::rename_files(const QStringList &files, const QStringList &settings)
{
    int i {};
    QString prefix = ui->line_prefix->text();
    QString suffix = ui->line_suffix->text();
    QDir::setCurrent(ui->line_folder->text());
    for (auto &name: files) {
        QFile file;
        QString number {QString::number(++i).rightJustified(3, '0')};
        file.setFileName(name);
        file.rename(prefix + number + suffix + ".mp4");
    }
    ui->statusbar->showMessage(QString::number(i) + " files successfully renamed. ");
}

void FileLabeler::on_line_prefix_textEdited(const QString &arg1)
{
    QString suffix = ui->line_suffix->text();
    ui->label_name->setText(arg1 + "001" + suffix + ".mp4");
}


void FileLabeler::on_line_suffix_textEdited(const QString &arg1)
{
    QString prefix = ui->line_prefix->text();
    ui->label_name->setText(prefix + "001" + arg1 + ".mp4");

}


void FileLabeler::on_pushButt_rename_released()
{
    QDir path = ui->line_folder->text();
    QStringList files = path.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);
    QStringList settings = path.entryList(QStringList() << "*.xml" << "*.XML",QDir::Files);
    FileLabeler::rename_files(files, settings);
}

