#include "ConnectionToDataBaseDialog.h"
#include "ui_ConnectionToDataBaseDialog.h"

ConnectionToDataBaseDialog::ConnectionToDataBaseDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConnectionToDataBaseDialog)
{
	ui->setupUi(this);

	ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
	ui->usernameLineEdit->setFocus();

	QRegExp regexp("[A-Za-z0-9]*");
	QValidator* stringValidator = new QRegExpValidator(regexp, this);
	ui->usernameLineEdit->setValidator(stringValidator);
	ui->passwordLineEdit->setValidator(stringValidator);

	connect(ui->regButton, SIGNAL(clicked()), this, SIGNAL(registrationSignal()));
}

ConnectionToDataBaseDialog::~ConnectionToDataBaseDialog()
{
	delete ui;
}

QString ConnectionToDataBaseDialog::getUsername()
{
	qDebug() << ui->usernameLineEdit->text();
	return ui->usernameLineEdit->text();
}

QString ConnectionToDataBaseDialog::getPassword()
{
	qDebug() << ui->passwordLineEdit->text();
	return ui->passwordLineEdit->text();
}
