#include "Wizard.h"
#include "ui_Wizard.h"

Wizard::Wizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::Wizard)
{
	ui->setupUi(this);
}

Wizard::~Wizard()
{
	delete ui;
}
