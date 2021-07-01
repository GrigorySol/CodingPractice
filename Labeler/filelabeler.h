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
    void default_parameters();

    void scan_folder();

    void check_checkers();

    void rename_files(const QStringList &files, const QStringList &settings);

    void xml_reader(const QString &file_name, QString &prefix);

    void on_line_prefix_textEdited(const QString &arg1);

    void on_line_suffix_textEdited(const QString &arg1);

    void on_pushButt_folder_released();

    void on_pushButt_rename_released();

    void on_checkBox_Date_stateChanged(int arg1);

    void on_checkBox_FPS_stateChanged(int arg1);

    void on_checkBox_Res_stateChanged(int arg1);

    void on_line_folder_textEdited(const QString &arg1);

    void on_pushButt_reset_released();

private:
    Ui::FileLabeler *ui;
};
#endif // FILELABELER_H
