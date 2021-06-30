#ifndef FILELABELER_H
#define FILELABELER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FileLabeler; }
QT_END_NAMESPACE

class FileLabeler : public QMainWindow
{
    Q_OBJECT

public:
    FileLabeler(QWidget *parent = nullptr);
    ~FileLabeler();

private slots:
    void on_pushButt_folder_released();

    void scan_folder();

    void rename_files(const QStringList &files, const QStringList &settings);

    void on_line_prefix_textEdited(const QString &arg1);

    void on_line_suffix_textEdited(const QString &arg1);

    void on_pushButt_rename_released();

private:
    Ui::FileLabeler *ui;
};
#endif // FILELABELER_H
