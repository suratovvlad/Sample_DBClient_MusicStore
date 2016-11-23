#ifndef CONNECTIONTODATABASEDIALOG_H
#define CONNECTIONTODATABASEDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class ConnectionToDataBaseDialog;
}

class ConnectionToDataBaseDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionToDataBaseDialog(QWidget *parent = 0);
	~ConnectionToDataBaseDialog();

	QString getUsername();
	QString getPassword();

private:
	Ui::ConnectionToDataBaseDialog *ui;

signals:
	void registrationSignal();
};

#endif // CONNECTIONTODATABASEDIALOG_H
