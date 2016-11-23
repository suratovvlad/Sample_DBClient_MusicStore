#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Wizard.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_lastPictureFilename = "";

	m_adminTab = NULL;
	m_isTabHidding = true;
	m_adminTab = ui->adminTab;
	ui->tabWidget->removeTab(0);

	m_core = new Core();

	m_currentAlbum = "";
	m_currentComposition = "";

	connect(m_core, SIGNAL(connectionFailedSignal(const QString&)), this, SLOT(connectionFailedSlot(const QString&)));
	connect(m_core, SIGNAL(connectionEstablishedSignal(const bool)), this, SLOT(connectionEstablishedSlot(const bool)));
	connectToDBSlot();


	connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));


	connect(ui->actionConnect_to_DB, SIGNAL(triggered()), this, SLOT(connectToDBSlot()));

	connect(ui->rockbandListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
			this, SLOT(rockbandItemChangedSlot(QListWidgetItem*, QListWidgetItem*)));


	connect(ui->albumTableWidget, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
			this, SLOT(albumItemChangedSlot(QTableWidgetItem*,QTableWidgetItem*)));
	connect(ui->albumTableWidget->verticalHeader(), SIGNAL(sectionClicked(int)),
			this, SLOT(albumItemChangedSlot(int)));

	connect(ui->compositionTableWidget, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
			this, SLOT(compositionItemChangedSlot(QTableWidgetItem*,QTableWidgetItem*)));
	connect(ui->compositionTableWidget->verticalHeader(), SIGNAL(sectionClicked(int)),
			this, SLOT(compositionItemChangedSlot(int)));


	ui->stackedWidget->setCurrentIndex(0);
	QRegExp regExp("[0-9]*");
	QValidator* numberValidator = new QRegExpValidator(regExp, this);
	ui->foundationYearLineEdit->setValidator(numberValidator);
	ui->endOfActivityYearLineEdit->setValidator(numberValidator);
	ui->foundationYearLineEdit_4->setValidator(numberValidator);
	ui->endOfActivityYearLineEdit_4->setValidator(numberValidator);
	ui->albumYearLineEdit->setValidator(numberValidator);
	ui->albumYearLineEdit_2->setValidator(numberValidator);
	ui->compositionBitRateLineEdit->setValidator(numberValidator);
	ui->compositionBitRateLineEdit_2->setValidator(numberValidator);

	regExp.setPattern("[ \tA-Za-zА-Яа-я0-9\'.,!?:]*");
	QValidator* stringValidator = new QRegExpValidator(regExp, this);
	ui->rockbandTitleLineEdit->setValidator(stringValidator);
	ui->rockbandTitleLineEdit_4->setValidator(stringValidator);
	ui->albumTitleLineEdit->setValidator(stringValidator);
	ui->albumTitleLineEdit_2->setValidator(stringValidator);
	ui->compositionTitleLineEdit->setValidator(stringValidator);
	ui->compositionTitleLineEdit_2->setValidator(stringValidator);
	ui->artistNameLineEdit->setValidator(stringValidator);
	ui->artistNameLineEdit_5->setValidator(stringValidator);
	ui->instumentsLineEdit->setValidator(stringValidator);
	ui->instumentsLineEdit_5->setValidator(stringValidator);

	QValidator* doubleValidator = new QDoubleValidator(this);
	ui->albumPriceLineEdit->setValidator(doubleValidator);
	ui->albumPriceLineEdit_2->setValidator(doubleValidator);

	ui->endOfActivityYearLabel->setHidden(true);
	ui->endOfActivityYearLineEdit->setHidden(true);

	ui->endOfActivityYearLabel_4->setHidden(true);
	ui->endOfActivityYearLineEdit_4->setHidden(true);

	// TODO: make input masks

	connect(ui->firstNextPushButton, SIGNAL(clicked()), this, SLOT(selectAdminActionSlot()));

	connect(ui->addRockbandPreviousButton, SIGNAL(clicked()), this, SLOT(addRockbandPreviousActionSlot()));
	connect(ui->addRockbandFinishButton, SIGNAL(clicked()), this, SLOT(addRockbandFinishActionSlot()));
	connect(ui->isEndOfActivityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(hideEndOfActivitySlot(int)));

	connect(ui->addArtistPreviousButton, SIGNAL(clicked()), this, SLOT(addArtistPreviousActionSlot()));
	connect(ui->addArtistFinishButton, SIGNAL(clicked()), this, SLOT(addArtistFinishActionSlot()));

	connect(ui->addAlbumPreviousButton, SIGNAL(clicked()), this, SLOT(addAlbumPreviousActionSlot()));
	connect(ui->addAlbumFinishButton, SIGNAL(clicked()), this, SLOT(addAlbumFinishActionSlot()));

	connect(ui->addCompositionPreviousButton, SIGNAL(clicked()), this, SLOT(addCompositionPreviousActionSlot()));
	connect(ui->addCompositionFinishButton, SIGNAL(clicked()), this, SLOT(addCompositionFinishActionSlot()));

	connect(ui->addArtistToRockbandPreviousButton, SIGNAL(clicked()), this, SLOT(addArtistToRockbandPreviousActionSlot()));
	connect(ui->addArtistToRockbandFinishButton, SIGNAL(clicked()), this, SLOT(addArtistToRockbandFinishActionSlot()));

	connect(ui->updateRockbandPreviousButton, SIGNAL(clicked()), this, SLOT(updateRockbandPreviousActionSlot()));
	connect(ui->updateRockbandFinishButton, SIGNAL(clicked()), this, SLOT(updateRockbandFinishActionSlot()));

	connect(ui->updateAlbumPreviousButton, SIGNAL(clicked()), this, SLOT(updateAlbumPreviousActionSlot()));
	connect(ui->updateAlbumFinishButton, SIGNAL(clicked()), this, SLOT(updateAlbumFinishActionSlot()));

	connect(ui->updateArtistPreviousButton, SIGNAL(clicked()), this, SLOT(updateArtistPreviousActionSlot()));
	connect(ui->updateArtistFinishButton, SIGNAL(clicked()), this, SLOT(updateArtistFinishActionSlot()));

	connect(ui->updateComposionPreviousButton, SIGNAL(clicked()), this, SLOT(updateCompositionPreviousActionSlot()));
	connect(ui->updateComposionFinishButton, SIGNAL(clicked()), this, SLOT(updateCompositionFinishActionSlot()));

	connect(ui->deleteArtistFromRockbandPreviousButton, SIGNAL(clicked()), this, SLOT(deleteArtistFromRockbandPreviousActionSlot()));
	connect(ui->deleteArtistFromRockbandFinishButton, SIGNAL(clicked()), this, SLOT(deleteArtistFromRockbandFinishActionSlot()));

	connect(ui->isEndOfActivityCheckBox_4, SIGNAL(stateChanged(int)), this, SLOT(hideEndOfActivity_2_Slot(int)));

	connect(ui->coverBrowseButton, SIGNAL(clicked()), this, SLOT(openChooseCoverFileDialogSlot()));
	connect(ui->coverBrowseButton_2, SIGNAL(clicked()), this, SLOT(openChooseCoverFileDialogSlot()));

	connect(ui->chooseRockbandComboBox_7, SIGNAL(currentTextChanged(QString)), this, SLOT(updateRockbandElementsSlot(QString)));
	connect(ui->chooseAlbumComboBox_2, SIGNAL(currentTextChanged(QString)), this, SLOT(updateAlbumElementsSlot(QString)));

	connect(ui->chooseCompositionComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateCompositionElementsSlot(QString)));

	connect(ui->chooseArtistComboBox_2, SIGNAL(currentTextChanged(QString)), this, SLOT(updateDeleteArtistElementSlot(QString)));
	connect(ui->chooseArtistComboBox_3, SIGNAL(currentTextChanged(QString)), this, SLOT(updateArtistElementSlot(QString)));


	connect(ui->purchaseAlbumPushButton, SIGNAL(clicked()), this, SLOT(purchaseAlbum()));
	connect(ui->purchaseCompositionPushButton, SIGNAL(clicked()), this, SLOT(unpurchaseAlbum()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateOrderPage(int)));

	connect(ui->confirmOrderPushButton, SIGNAL(clicked()), this, SLOT(confirmOrder()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::connectToDBSlot()
{

	connect(&connectionDialog, SIGNAL(registrationSignal()), this, SLOT(registrationSlot()));

	if (connectionDialog.exec() != QDialog::Accepted) {
		emit quitSignal();
		return;
	}
	clearAllWidgets();

	m_core->changeConnection(connectionDialog.getUsername(), connectionDialog.getPassword());

//	clearAllWidgets();
///	m_core->changeConnection("root", "Fcnhjabpbrf95");
///	ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::registrationSlot()
{
	QString user = connectionDialog.getUsername();
	QString passwd = connectionDialog.getPassword();


	QStringList users = m_core->getUsersStringList();

	foreach (QString existUser, users) {
		if (existUser == user) {
			QMessageBox::warning(this, "Warning", "User already exists");
			return;
		}
	}

	if (!m_core->registrateUser(user, passwd)) {
		QMessageBox::warning(this, "Warning", m_core->lastError());
		return;
	}

	QMessageBox::information(this, "Information", "Registration successfull");
}

void MainWindow::connectionFailedSlot(const QString& error)
{
	QMessageBox::warning(this, tr("Connection Error"), error);
	QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Reopen connectioin"), tr("Do you want to reconnect?"), QMessageBox::Yes|QMessageBox::No);

	if (reply != QMessageBox::Yes) {
		connectionEstablishedSlot(false);
		return;
	}

	qDebug() << "reconnecting...";
	connectToDBSlot();
}

void MainWindow::connectionEstablishedSlot(const bool isAdmin)
{
	loadData();

	if (isAdmin) {
		if (!m_isTabHidding) {
			return;
		}
		ui->tabWidget->insertTab(0, m_adminTab, "Admin");
	}
	else {
		if (m_isTabHidding) {
			return;
		}
		m_adminTab = ui->adminTab;
		ui->tabWidget->removeTab(0);
	}
	m_isTabHidding = !isAdmin;	
}

void MainWindow::loadData()
{
	//Clear All Widgets
	clearAllWidgets();

	//Update Rockband List Widget
	QStringList rockbandList = m_core->getRockbandStringList();
	updateRockbandListWidget(rockbandList);
}

void MainWindow::rockbandItemChangedSlot(QListWidgetItem *current, QListWidgetItem* previous)
{
	if (!current) {
		return;
	}
	qDebug() << "Now selected in rockband = " << current->text();

	//Clear Widgets
	clearRockbandInformationLabel();
	clearAlbumTableWidget();
	clearAlbumInformationLabel();
	clearCompositionTableWidget();
	clearCompositionInformationLabel();
	clearCoverLabel();
	clearPurchaseAlbumButton();
	clearPurchaseCompositionButton();

	//Update Album Table Widget
	QList<Core::AlbumInfoType*> albumList = m_core->getAlbumList(current->text());
	updateAlbumTableWidget(albumList);
	albumList.clear();

//	QList<Core::AlbumInfoMiniType*> albumList = m_core->getAlbumMiniList(current->text());
//	updateAlbumTableWidget(albumList);

	//Update Information Label
	Core::RockbandInfoType* rockbandInfo = m_core->getRockbandInfoByTitle(current->text());
	updateInformationLabelByRockband(rockbandInfo);

}

void MainWindow::albumItemChangedSlot(int selectedRow)
{
	QTableWidgetItem* current = ui->albumTableWidget->item(selectedRow, 1);
	if (!current) {
		return;
	}
	qDebug() << "Now selected in album = " << current->text();

	//Clear Widgets
	clearAlbumInformationLabel();
	clearCompositionTableWidget();
	clearCompositionInformationLabel();
	clearCoverLabel();
	clearPurchaseAlbumButton();
	clearPurchaseCompositionButton();

	//Update Composition Table Widget
	QList<Core::CompositionInfoType*> compositionList = m_core->getCompositionList(current->text());
	updateCompositionTableWidget(compositionList);
	compositionList.clear();

	Core::AlbumInfoType* albumInfo = m_core->getAlbumInfoByTitle(current->text());

	//Update cover
	updateCoverLabel(albumInfo);

	//Update Information Label
	updateInformationLabelByAlbum(albumInfo);

	//Activate Purchase Album Button
	ui->purchaseAlbumPushButton->setEnabled(true);
	ui->purchaseCompositionPushButton->setEnabled(true);

	m_currentAlbum = current->text();
}

void MainWindow::albumItemChangedSlot(QTableWidgetItem *current, QTableWidgetItem *previous)
{
	if (!current) {
		return;
	}
	emit ui->albumTableWidget->verticalHeader()->sectionClicked(current->row());
}

void MainWindow::clearAllWidgets()
{
	clearRockbandListWidget();
	clearAlbumTableWidget();
	clearCompositionTableWidget();

	clearRockbandInformationLabel();
	clearAlbumInformationLabel();
	clearCompositionTableWidget();

	clearPurchaseAlbumButton();
	clearPurchaseCompositionButton();

	clearCoverLabel();
	clearOrderTableWidget();
}

void MainWindow::clearRockbandListWidget()
{
	ui->rockbandListWidget->clear();
}

void MainWindow::clearRockbandInformationLabel()
{
	ui->rockbandInformationLabel->setText("Information...");
}

void MainWindow::clearAlbumTableWidget()
{
	QStringList table_header;
	table_header << "Year" << "Title" << "Price";

	ui->albumTableWidget->clear();
	ui->albumTableWidget->setColumnCount(table_header.count());
	ui->albumTableWidget->setHorizontalHeaderLabels(table_header);
	ui->albumTableWidget->setRowCount(0);

	ui->albumTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->albumTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->albumTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

	m_currentAlbum = "";
	ui->purchaseAlbumPushButton->setEnabled(false);
	ui->purchaseCompositionPushButton->setEnabled(false);
}

void MainWindow::clearAlbumInformationLabel()
{
	ui->albumInformationLabel->setText("Information...");
}

void MainWindow::clearCompositionTableWidget()
{
	QStringList table_header;
	table_header << "Number" << "Title" << "Genre" << "Duration" /*<< "Price*/;

	ui->compositionTableWidget->clear();
	ui->compositionTableWidget->setColumnCount(table_header.count());
	ui->compositionTableWidget->setHorizontalHeaderLabels(table_header);
	ui->compositionTableWidget->setRowCount(0);

	ui->compositionTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->compositionTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->compositionTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::clearCompositionInformationLabel()
{
	ui->compositionInformationLabel->setText("Information...");
}

void MainWindow::clearCoverLabel()
{
	QString filename = "./resources/no-cover.jpg";
	QImage image(filename);
	ui->coverLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::clearPurchaseAlbumButton()
{
	ui->purchaseAlbumPushButton->setEnabled(false);
}

void MainWindow::clearPurchaseCompositionButton()
{
	ui->purchaseCompositionPushButton->setEnabled(false);
}

void MainWindow::updateCoverLabel(Core::AlbumInfoType *albumInfo)
{
	if (albumInfo->cover == QByteArray()) {
		return;
	}
	QPixmap picture;
	picture.loadFromData(albumInfo->cover);
	ui->coverLabel->setPixmap(picture);
}

void MainWindow::updateRockbandListWidget(QStringList rockbandList)
{
	rockbandList.sort();
	ui->rockbandListWidget->addItems(rockbandList);
}

void MainWindow::updateRockbandListWidget(QList<Core::RockbandInfoType *> rockbandList)
{
	QStringList rockbandStringList;

	foreach (Core::RockbandInfoType* rockbandInfo, rockbandList) {
		rockbandStringList.append(rockbandInfo->title);
	}

	rockbandStringList.sort();
	ui->rockbandListWidget->addItems(rockbandStringList);
}
/*
void MainWindow::updateAlbumTableWidget(QList<Core::AlbumInfoMiniType*> albumList)
{
	ui->albumTableWidget->setRowCount(albumList.count());
	foreach (Core::AlbumInfoMiniType* albumInfo, albumList) {
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 0,
									  new QTableWidgetItem(QString::number(albumInfo->year)));
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 1,
									  new QTableWidgetItem(albumInfo->title));
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 2,
									  new QTableWidgetItem(QString::number(albumInfo->price)));
	}
}
*/
void MainWindow::updateAlbumTableWidget(QList<Core::AlbumInfoType *> albumList)
{
	ui->albumTableWidget->setRowCount(albumList.count());
	foreach (Core::AlbumInfoType* albumInfo, albumList) {
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 0,
									  new QTableWidgetItem(QString::number(albumInfo->year)));
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 1,
									  new QTableWidgetItem(albumInfo->title));
		ui->albumTableWidget->setItem(albumList.indexOf(albumInfo), 2,
									  new QTableWidgetItem(QString::number(albumInfo->price)));
	}
}

void MainWindow::updateCompositionTableWidget(QList<Core::CompositionInfoType *> compositionList)
{
	ui->compositionTableWidget->setRowCount(compositionList.count());
	foreach (Core::CompositionInfoType* compositionInfo, compositionList) {
		ui->compositionTableWidget->setItem(compositionList.indexOf(compositionInfo), 0,
									  new QTableWidgetItem(QString::number(compositionInfo->number)));
		ui->compositionTableWidget->setItem(compositionList.indexOf(compositionInfo), 1,
									  new QTableWidgetItem(compositionInfo->title));
		ui->compositionTableWidget->setItem(compositionList.indexOf(compositionInfo), 2,
									  new QTableWidgetItem(compositionInfo->genre));
		ui->compositionTableWidget->setItem(compositionList.indexOf(compositionInfo), 3,
									  new QTableWidgetItem(compositionInfo->duration));
		/*ui->compositionTableWidget->setItem(compositionList.indexOf(compositionInfo), 4,
									  new QTableWidgetItem(QString::number(compositionInfo->price)));*/
	}
}

void MainWindow::updateInformationLabelByRockband(Core::RockbandInfoType *rockbandInfo)
{
	QString text = "Group: " + rockbandInfo->title;
	text += "\nFoundation year: " + QString::number(rockbandInfo->foundationYear);
	if (rockbandInfo->isEndOfActivity) {
		text += "\nBand ends activity in " + QString::number(rockbandInfo->endOfActivityYear);
	}

	QList<Core::ArtistInfoType*> artistInfo = m_core->getArtistList(rockbandInfo->title);

	text += "\n\nMembers:\n";
	foreach (Core::ArtistInfoType* artist, artistInfo) {
		text += "\n" + artist->name + " : " + artist->instruments;
	}

	ui->rockbandInformationLabel->setText(text);
}

void MainWindow::updateInformationLabelByAlbum(Core::AlbumInfoType *albumInfo)
{
	QString text = "Album: " + albumInfo->title;
	text += "\nYear: " + QString::number(albumInfo->year);
	text += "\nVolume Type: " + albumInfo->volumeType;
	text += "\nRecord Type: " + albumInfo->recordType;
	text += "\n\nPrice: " + QString::number(albumInfo->price);

	ui->albumInformationLabel->setText(text);
}


void MainWindow::compositionItemChangedSlot(int selectedRow)
{
	QTableWidgetItem* current = ui->compositionTableWidget->item(selectedRow, 1);
	if (!current) {
		return;
	}
	qDebug() << "Now selected in composition = " << current->text();

	//Clear Widgets
	clearCompositionInformationLabel();
	clearPurchaseCompositionButton();

	//Update Information Label
	Core::CompositionInfoType* compositionInfo = m_core->getCompositionInfoByTitle(current->text());
	Core::CompositionFileInfoType* compositionFileInfo = m_core->getCompositionFileInfoByTitle(current->text());
	updateInformationLabelByComposition(compositionInfo, compositionFileInfo);

//	//Activate Purchase Composition Button
//	ui->purchaseCompositionPushButton->setEnabled(true);
}

void MainWindow::compositionItemChangedSlot(QTableWidgetItem *current, QTableWidgetItem *previous)
{
	if (!current) {
		return;
	}
	qDebug() << "Now selected in album = " << current->text();

	emit ui->compositionTableWidget->verticalHeader()->sectionClicked(current->row());
}

void MainWindow::updateInformationLabelByComposition(Core::CompositionInfoType *compositionInfo,
													 Core::CompositionFileInfoType *compositionFileInfo)
{
	QString text = "Number: " + QString::number(compositionInfo->number);
	text += "\nTitle: " + compositionInfo->title;
	text += "\nGenre: " + compositionInfo->genre;
	text += "\nDuration: " + compositionInfo->duration;
	text += "\n\nFile Info:";
	text += "\n\tCodec Type: " + compositionFileInfo->codecType;
	text += "\n\tBit Rate: " + QString::number(compositionFileInfo->bitRate);
	text += "\n\tFile Size: " + QString::number(compositionFileInfo->fileSize);
	text += "\n\nPrice: " + QString::number(compositionInfo->price);

	ui->compositionInformationLabel->setText(text);

}

void MainWindow::clearAllAdminWidgets()
{
	clearAddArtist();
	clearAddRockband();
	clearAddAlbum();
	clearAddComposition();

	clearUpdateRockband();
	clearUpdateAlbum();
	clearUpdateComposition();
	clearUpdateArtist();
	clearAddArtistToRockband();
	clearDeleteArtistFromRockband();
}

void MainWindow::clearAddArtist()
{
	ui->artistNameLineEdit->clear();
	ui->instumentsLineEdit->clear();
	ui->chooseRockbandComboBox->clear();
}

void MainWindow::clearAddRockband()
{
	ui->foundationYearLineEdit->clear();
	ui->endOfActivityYearLineEdit->clear();
	ui->rockbandTitleLineEdit->clear();
	ui->isEndOfActivityCheckBox->setChecked(false);
}

void MainWindow::clearAddAlbum()
{
	ui->albumTitleLineEdit->clear();
	ui->albumPriceLineEdit->clear();
	ui->albumYearLineEdit->clear();
	ui->chooseRockbandComboBox_5->clear();

	QString filename = "./resources/no-cover.jpg";
	QImage image(filename);
	ui->coverLabel_2->setPixmap(QPixmap::fromImage(image));

	m_lastPictureFilename = "";
}

void MainWindow::clearAddComposition()
{
	ui->compositionBitRateLineEdit->clear();
	ui->compositionDurationTimeEdit->setTime(QTime(0,0,0));
	ui->compositionNumberSpinBox->setValue(1);
	ui->fileSizeDoubleSpinBox->setValue(0.00);
	ui->compositionTitleLineEdit->clear();
	ui->compositionPriceLineEdit->clear();

	ui->chooseAlbumComboBox->clear();

}

void MainWindow::clearUpdateRockband()
{
	ui->chooseRockbandComboBox_7->clear();

	ui->rockbandTitleLineEdit_4->clear();
	ui->foundationYearLineEdit_4->clear();
	ui->endOfActivityYearLineEdit_4->clear();
	ui->isEndOfActivityCheckBox_4->setChecked(false);
}

void MainWindow::clearUpdateAlbum()
{
	ui->chooseAlbumComboBox_2->clear();

	ui->albumPriceLineEdit_2->clear();
	ui->albumTitleLineEdit_2->clear();
	ui->albumYearLineEdit_2->clear();
	ui->chooseRockbandComboBox_8->clear();

	QString filename = "./resources/no-cover.jpg";
	QImage image(filename);
	ui->coverLabel_4->setPixmap(QPixmap::fromImage(image));

	m_lastPictureFilename = "";
}

void MainWindow::clearUpdateComposition()
{
	ui->chooseCompositionComboBox->clear();

	ui->chooseAlbumComboBox_3->clear();
	ui->compositionBitRateLineEdit_2->clear();
	ui->compositionDurationTimeEdit_2->setTime(QTime(0,0,0));
	ui->compositionNumberSpinBox_2->setValue(1);
	ui->fileSizeDoubleSpinBox_2->setValue(0.00);
	ui->compositionTitleLineEdit_2->clear();
	ui->compositionPriceLineEdit_2->clear();

}

void MainWindow::clearUpdateArtist()
{
	ui->chooseArtistComboBox_3->clear();
	ui->artistNameLineEdit_5->clear();
	ui->instumentsLineEdit_5->clear();
}

void MainWindow::clearAddArtistToRockband()
{
	ui->chooseArtistComboBox->clear();
	ui->chooseRockbandComboBox_6->clear();
}

void MainWindow::clearDeleteArtistFromRockband()
{
	ui->chooseArtistComboBox_2->clear();
	ui->chooseRockbandComboBox_9->clear();
}

void MainWindow::selectAdminActionSlot()
{
	QListWidgetItem* currentItem = ui->adminActionListWidget->currentItem();
	if (!currentItem) {
		QMessageBox::warning(this, tr("Warning"), tr("Choose action"));
		return;
	}
	quint32 index = ui->adminActionListWidget->currentRow();
	qDebug() << "Current action:" << currentItem->text();
	qDebug() << "Current action index:" << index;

	switch (index)
	{
		case 0:
			addRockbandAction();
			break;
		case 1:
			addAlbumAction();
			break;
		case 2:
			addCompositionAction();
			break;
		case 3:
			addArtistAction();
			break;
		case 4:
			addArtistToRockbandAction();
			break;
		case 5:
			deleteArtistFromRockbandAction();
			break;
		case 6:
			updateRockbandAction();
			break;
		case 7:
			updateAlbumAction();
			break;
		case 8:
			updateCompositionAction();
			break;
		case 9:
			updateArtistAction();
			break;
	}
}

//
void MainWindow::addRockbandAction()
{
	ui->stackedWidget->setCurrentIndex(1);

	/*
	QRegExp regExp("");
	QValidator* titleValidator = new QRegExpValidator(regExp, this);

	ui->rockbandTitleLineEdit->setValidator();*/
}

void MainWindow::addRockbandPreviousActionSlot()
{
	clearAddRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addRockbandFinishActionSlot()
{
	QString title = ui->rockbandTitleLineEdit->text();
	quint16 foundationYear = ui->foundationYearLineEdit->text().toUInt();
	bool	isEndOfActivity = ui->isEndOfActivityCheckBox->isChecked();
	quint16 endOfActivityYear = ui->endOfActivityYearLineEdit->text().toUInt();

	//Rechecking input here
	if (ui->rockbandTitleLineEdit->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->foundationYearLineEdit->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (isEndOfActivity) {
		if (ui->endOfActivityYearLineEdit->text() == QString("")) {
			QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
			return;
		}
	}

	QStringList rockbandList = m_core->getRockbandStringList();
	foreach (QString rockband, rockbandList) {
		if (QString::compare(rockband, title, Qt::CaseInsensitive) == 0) {
			QMessageBox::warning(this, tr("Warning"), tr("Rock band exisits.\nPlease use update action in case of modify data"));
			return;
		}
	}
	rockbandList.clear();

	//Preparing struct
	Core::RockbandInfoType* rockbandInfo = new Core::RockbandInfoType;
	rockbandInfo->deleteLater();

	rockbandInfo->title = title;
	rockbandInfo->foundationYear = foundationYear;
	rockbandInfo->isEndOfActivity = isEndOfActivity;
	if (isEndOfActivity) {
		rockbandInfo->endOfActivityYear = endOfActivityYear;
	}
	else {
		rockbandInfo->endOfActivityYear = 0;
	}

	if (!m_core->addRockband(rockbandInfo)) {
		QMessageBox::warning(this, tr("warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Rock band added successfully"));

	clearAddRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::hideEndOfActivitySlot(int state)
{
	ui->endOfActivityYearLabel->setHidden(!((bool)state));
	ui->endOfActivityYearLineEdit->setHidden(!((bool)state));
}

void MainWindow::hideEndOfActivity_2_Slot(int state)
{
	ui->endOfActivityYearLabel_4->setHidden(!((bool)state));
	ui->endOfActivityYearLineEdit_4->setHidden(!((bool)state));
}

//
void MainWindow::addArtistAction()
{
	ui->stackedWidget->setCurrentIndex(2);

	QStringList rockbandList = m_core->getRockbandStringList();

//	ui->chooseRockbandComboBox->addItem("None");

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox->addItem(rockband);
	}

	rockbandList.clear();
}

void MainWindow::addArtistPreviousActionSlot()
{
	clearAddArtist();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addArtistFinishActionSlot()
{
	QString name = ui->artistNameLineEdit->text();
	QString instruments = ui->instumentsLineEdit->text();

	//Rechecking input here
	if (name == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (instruments == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}

	QString rockband = ui->chooseRockbandComboBox->currentText();
	qDebug() << "Current rockband:" << rockband;

	QStringList artistList = m_core->getArtistStringList();
	foreach (QString artist, artistList) {
		if (QString::compare(artist, name, Qt::CaseInsensitive) == 0) {
			QMessageBox::warning(this, tr("Warning"), tr("Artist exisits.\nPlease use update action in case of modify data"));
			return;
		}
	}
	artistList.clear();

	//Preparing struct
	Core::ArtistInfoType* artistInfo = new Core::ArtistInfoType;
	artistInfo->deleteLater();

	artistInfo->name = name;
	artistInfo->instruments = instruments;

	if (!m_core->addArtist(artistInfo, rockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	if (rockband != "None") {
		if (!m_core->addArtistHasRockband(name, rockband)) {
			QMessageBox::warning(this, tr("Warning"), m_core->lastError());
			return;
		}
	}

	QMessageBox::information(this, tr("Information"), tr("Artist added successfully"));

	clearAddArtist();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::addAlbumAction()
{
	ui->stackedWidget->setCurrentIndex(3);

	QStringList rockbandList = m_core->getRockbandStringList();

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_5->addItem(rockband);
	}

	rockbandList.clear();
}

void MainWindow::addAlbumPreviousActionSlot()
{
	clearAddAlbum();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addAlbumFinishActionSlot()
{
	QString title = ui->albumTitleLineEdit->text();
	quint32 year = ui->albumYearLineEdit->text().toUInt();
	float	price = ui->albumPriceLineEdit->text().toFloat();

	//Rechecking input here
	if (title == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->albumYearLineEdit->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->albumPriceLineEdit->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (price < 0.0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive price"));
		return;
	}

	QString rockband = ui->chooseRockbandComboBox_5->currentText();

	QByteArray ba;
	QFile f(m_lastPictureFilename);
	if(f.open(QIODevice::ReadOnly)) {
		ba = f.readAll();
		f.close();
	}

	if (ba.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}

	Core::AlbumInfoType* albumInfo = new Core::AlbumInfoType();
	albumInfo->deleteLater();

	albumInfo->title = title;
	albumInfo->year = year;
	albumInfo->price = price;
	albumInfo->volumeType = ui->volumeTypeComboBox->currentText();
	albumInfo->recordType = ui->recordTypeComboBox->currentText();
	albumInfo->cover = ba;

	if (!m_core->addAlbum(albumInfo, rockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Album added successfully"));

	clearAddAlbum();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::addCompositionAction()
{
	ui->stackedWidget->setCurrentIndex(4);

	QStringList albumList = m_core->getAlbumStringList();

	foreach(QString album, albumList){
		ui->chooseAlbumComboBox->addItem(album);
	}

	albumList.clear();
}

void MainWindow::addCompositionPreviousActionSlot()
{
	clearAddComposition();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addCompositionFinishActionSlot()
{
	QString title = ui->compositionTitleLineEdit->text();
	QString duration = ui->compositionDurationTimeEdit->text();
	qDebug() << duration;
	quint32 number = ui->compositionNumberSpinBox->text().toUInt();
	float fileSize = ui->fileSizeDoubleSpinBox->text().toFloat();
	float price = ui->compositionPriceLineEdit->text().toFloat();
	quint32 bitrate = ui->compositionBitRateLineEdit->text().toUInt();

	QString album = ui->chooseAlbumComboBox->currentText();
	qDebug() << album;

	//Rechecking input here
	if (title == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (duration == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->compositionPriceLineEdit->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (price < 0.0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive price"));
		return;
	}
	if (bitrate == 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter right bit rate"));
		return;
	}
	if (fileSize < 0.00) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive file size"));
		return;
	}
	if (number == 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter right omposition number"));
		return;
	}

	Core::CompositionInfoType* compositionInfo = new Core::CompositionInfoType();
	compositionInfo->deleteLater();

	compositionInfo->title = title;
	compositionInfo->duration = duration;
	compositionInfo->number = number;
	compositionInfo->price = price;
	compositionInfo->genre = ui->chooseGenreComboBox->currentText();

	Core::CompositionFileInfoType* compositionFileInfo = new Core::CompositionFileInfoType();
	compositionFileInfo->deleteLater();

	compositionFileInfo->bitRate = bitrate;
	compositionFileInfo->fileSize = fileSize;
	compositionFileInfo->codecType = ui->chooseCodecComboBox->currentText();

	if (!m_core->addComposition(compositionFileInfo, compositionInfo, album)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

//	if (!m_core->addCompositionFileInfo(compositionFileInfo, title)) {
//		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
//		return;
//	}

	QMessageBox::information(this, tr("Information"), tr("Composition added successfully"));

	clearAddComposition();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::addArtistToRockbandAction()
{
	ui->stackedWidget->setCurrentIndex(5);

	QStringList rockbandList = m_core->getRockbandStringList();

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_6->addItem(rockband);
	}

	rockbandList.clear();

	QStringList artistList = m_core->getArtistStringList();

	foreach(QString artist, artistList){
		ui->chooseArtistComboBox->addItem(artist);
	}

	artistList.clear();
}

void MainWindow::addArtistToRockbandPreviousActionSlot()
{
	clearAddArtistToRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addArtistToRockbandFinishActionSlot()
{
	QString artist = ui->chooseArtistComboBox->currentText();
	QString rockband = ui->chooseRockbandComboBox_6->currentText();

	if (!m_core->addArtistHasRockband(artist, rockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Information added successfully"));


	clearAddArtistToRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::updateRockbandAction()
{
	ui->stackedWidget->setCurrentIndex(6);

	QStringList rockbandList = m_core->getRockbandStringList();

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_7->addItem(rockband);
	}

	rockbandList.clear();

	ui->chooseRockbandComboBox_7->setCurrentIndex(0);
}

void MainWindow::updateRockbandPreviousActionSlot()
{
	clearUpdateRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateRockbandFinishActionSlot()
{
	QString rockband = ui->chooseRockbandComboBox_7->currentText();
	Core::RockbandInfoType* rockbandInfo = m_core->getRockbandInfoByTitle(rockband);

	QString title = ui->rockbandTitleLineEdit_4->text();
	quint16 foundationYear = ui->foundationYearLineEdit_4->text().toUInt();
	bool	isEndOfActivity = ui->isEndOfActivityCheckBox_4->isChecked();
	quint16 endOfActivityYear = ui->endOfActivityYearLineEdit_4->text().toUInt();

	//Rechecking input here
	if (ui->rockbandTitleLineEdit_4->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->foundationYearLineEdit_4->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (isEndOfActivity) {
		if (ui->endOfActivityYearLineEdit_4->text() == QString("")) {
			QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
			return;
		}
	}

	if (rockbandInfo->title != rockband) {
		QMessageBox::warning(this, tr("Warning"), "Some error");
		return;
	}

	if (title == rockbandInfo->title
			&& foundationYear == rockbandInfo->foundationYear
			&& isEndOfActivity == rockbandInfo->isEndOfActivity
			&& endOfActivityYear == rockbandInfo->endOfActivityYear)
	{
		QMessageBox::warning(this, tr("Warning"), "Nothing change");
		return;
	}

	rockband = rockbandInfo->title;

	rockbandInfo->title = title;
	rockbandInfo->foundationYear = foundationYear;
	rockbandInfo->isEndOfActivity = isEndOfActivity;
	if (isEndOfActivity) {
		rockbandInfo->endOfActivityYear = endOfActivityYear;
	}

	if (!m_core->updateRockband(rockbandInfo, rockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Rockband updated successfully"));

	clearUpdateRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::updateArtistAction()
{
	ui->stackedWidget->setCurrentIndex(10);
	QStringList artistList = m_core->getArtistStringList();

	foreach(QString artist, artistList){
		ui->chooseArtistComboBox_3->addItem(artist);
	}

	artistList.clear();

	//SET CURRENT VALUES
}

void MainWindow::updateArtistPreviousActionSlot()
{
	clearUpdateArtist();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateArtistFinishActionSlot()
{
	QString artist = ui->chooseArtistComboBox_3->currentText();
	Core::ArtistInfoType* artistInfo = m_core->getArtistInfoByName(artist);

	QString name = ui->artistNameLineEdit_5->text();
	QString instruments = ui->instumentsLineEdit_5->text();

	if (name == artistInfo->name
			&& instruments == artistInfo->instruments)
	{
		QMessageBox::warning(this, tr("Warning"), "Nothing change");
		return;
	}

	artistInfo->name = name;
	artistInfo->instruments = instruments;

	if (!m_core->updateArtist(artistInfo, artist)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Artist updated successfully"));

	clearUpdateArtist();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::updateAlbumAction()
{
	ui->stackedWidget->setCurrentIndex(7);
	QStringList albumList = m_core->getAlbumStringList();

	foreach(QString album, albumList){
		ui->chooseAlbumComboBox_2->addItem(album);
	}

	albumList.clear();

	QStringList rockbandList = m_core->getRockbandStringList();

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_8->addItem(rockband);
	}

	rockbandList.clear();

	ui->chooseAlbumComboBox_2->setCurrentIndex(0);
}

void MainWindow::updateAlbumPreviousActionSlot()
{
	clearUpdateAlbum();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateAlbumFinishActionSlot()
{
	QString album = ui->chooseAlbumComboBox_2->currentText();
	Core::AlbumInfoType* albumInfo = m_core->getAlbumInfoByTitle(album);

	QString title = ui->albumTitleLineEdit_2->text();
	quint32 year = ui->albumYearLineEdit_2->text().toUInt();
	float	price = ui->albumPriceLineEdit_2->text().toFloat();

	QString volumeType = ui->volumeTypeComboBox_2->currentText();
	QString recordType = ui->recordTypeComboBox_2->currentText();

	QByteArray cover;
	QBuffer buffer(&cover);
	buffer.open(QIODevice::WriteOnly);
	ui->coverLabel_4->pixmap()->save(&buffer, "PNG");

	//Rechecking input here
	if (title == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->albumYearLineEdit_2->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->albumPriceLineEdit_2->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (price < 0.0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive price"));
		return;
	}

	QByteArray ba;
	QFile f(m_lastPictureFilename);
	if(f.open(QIODevice::ReadOnly)) {
		ba = f.readAll();
		f.close();
	}

	if (ba.isEmpty()) {
		ba = albumInfo->cover;
	}

	if (ba.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}

	if (title == albumInfo->title
			&& year == albumInfo->year
			&& price == albumInfo->price
			&& volumeType == albumInfo->volumeType
			&& recordType == albumInfo->recordType
			&& cover == albumInfo->cover)
	{
		QMessageBox::warning(this, tr("Warning"), "Nothing change");
		return;
	}

	Core::AlbumInfoType *newAlbumInfo = new Core::AlbumInfoType;
	newAlbumInfo->deleteLater();

	newAlbumInfo->title = title;
	newAlbumInfo->year = year;
	newAlbumInfo->price = price;
	newAlbumInfo->volumeType = volumeType;
	newAlbumInfo->recordType = recordType;
	newAlbumInfo->cover = ba;

	QString newRockband = ui->chooseRockbandComboBox_8->currentText();

	if (!m_core->updateAlbum(newAlbumInfo, album, newRockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Album updated successfully"));

	clearUpdateAlbum();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::updateCompositionAction()
{
	ui->stackedWidget->setCurrentIndex(8);
	QStringList compositionList = m_core->getCompositionStringList();

	foreach(QString composition, compositionList){
		ui->chooseCompositionComboBox->addItem(composition);
	}

	compositionList.clear();

	QStringList albumList = m_core->getAlbumStringList();

	foreach(QString album, albumList){
		ui->chooseAlbumComboBox_3->addItem(album);
	}

	albumList.clear();

	//SET CURRENT VALUES
	ui->chooseCompositionComboBox->setCurrentIndex(0);
}

void MainWindow::updateCompositionPreviousActionSlot()
{
	clearUpdateComposition();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateCompositionFinishActionSlot()
{
	QString composition = ui->chooseCompositionComboBox->currentText();
	Core::CompositionInfoType* compositionInfo = m_core->getCompositionInfoByTitle(composition);
	Core::CompositionFileInfoType* compositionFileInfo = m_core->getCompositionFileInfoByTitle(composition);

	QString title = ui->compositionTitleLineEdit_2->text();
	QString duration = ui->compositionDurationTimeEdit_2->text();
	qDebug() << duration;
	quint32 number = ui->compositionNumberSpinBox_2->text().toUInt();
	float fileSize = ui->fileSizeDoubleSpinBox_2->text().toFloat();
	float price = ui->compositionPriceLineEdit_2->text().toFloat();
	quint32 bitrate = ui->compositionBitRateLineEdit_2->text().toUInt();

	QString codec = ui->chooseCodecComboBox_2->currentText();
	QString genre = ui->chooseGenreComboBox_2->currentText();

	QString album = ui->chooseAlbumComboBox_3->currentText();
	qDebug() << album;

	//Rechecking input here
	if (title == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (duration == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (ui->compositionPriceLineEdit_2->text() == QString("")) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter full information"));
		return;
	}
	if (price < 0.0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive price"));
		return;
	}
	if (bitrate == 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter right bit rate"));
		return;
	}
	if (fileSize < 0.00) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter positive file size"));
		return;
	}
	if (number == 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Enter right omposition number"));
		return;
	}

	if (title == compositionInfo->title
			&& number == compositionInfo->number
			&& price == compositionInfo->price
			&& genre == compositionInfo->genre
			&& duration == compositionInfo->genre
			&& codec == compositionFileInfo->codecType
			&& bitrate == compositionFileInfo->bitRate
			&& fileSize == compositionFileInfo->fileSize)
	{
		QMessageBox::warning(this, tr("Warning"), "Nothing change");
		return;
	}

	compositionInfo->title = title;
	compositionInfo->duration = duration;
	compositionInfo->number = number;
	compositionInfo->price = price;
	compositionInfo->genre = genre;

	compositionFileInfo->bitRate = bitrate;
	compositionFileInfo->fileSize = fileSize;
	compositionFileInfo->codecType = codec;

	if (!m_core->updateComposition(compositionFileInfo, compositionInfo, composition, album)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Composition updated successfully"));


	clearUpdateComposition();
	ui->stackedWidget->setCurrentIndex(0);
}

//
void MainWindow::deleteArtistFromRockbandAction()
{
	ui->stackedWidget->setCurrentIndex(9);
/*
	QStringList rockbandList = m_core->getRockbandStringList();

	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_9->addItem(rockband);
	}

	rockbandList.clear();

*/
	QStringList artistList = m_core->getArtistStringList();

	foreach(QString artist, artistList){
		ui->chooseArtistComboBox_2->addItem(artist);
	}

	artistList.clear();

	//SET CURRENT VALUES
}

void MainWindow::deleteArtistFromRockbandPreviousActionSlot()
{
	clearDeleteArtistFromRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::deleteArtistFromRockbandFinishActionSlot()
{
	QString artist = ui->chooseArtistComboBox_2->currentText();
	QString rockband = ui->chooseRockbandComboBox_9->currentText();

	if (!m_core->deleteArtist(artist, rockband)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Artist deleted successfully"));

	clearDeleteArtistFromRockband();
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::openChooseCoverFileDialogSlot()
{
	m_lastPictureFilename = QFileDialog::getOpenFileName(this, tr("Choose image"),
														 "", tr("Images (*.png *.jpg)"));

	if (m_lastPictureFilename == "") {
		return;
	}

	// load image to bytearray
	QByteArray ba;
	QFile f(m_lastPictureFilename);
	if(f.open(QIODevice::ReadOnly)) {
		ba = f.readAll();
		f.close();
	}

	QPixmap picture;
	picture.loadFromData(ba);
	ui->coverLabel_2->setPixmap(picture);
	ui->coverLabel_4->setPixmap(picture);
}


void MainWindow::updateRockbandElementsSlot(QString currentText)
{
	Core::RockbandInfoType* rockbandInfo = m_core->getRockbandInfoByTitle(currentText);

	if (rockbandInfo->title != currentText) {
		QMessageBox::warning(this, tr("Warning"), "Some error");
		return;
	}

	if (rockbandInfo->title == "") {
		return;
	}

	ui->rockbandTitleLineEdit_4->setText(rockbandInfo->title);
	ui->foundationYearLineEdit_4->setText(QString::number(rockbandInfo->foundationYear));

	ui->isEndOfActivityCheckBox_4->setChecked(rockbandInfo->isEndOfActivity);

	if (rockbandInfo->isEndOfActivity) {
		ui->endOfActivityYearLineEdit_4->setText(QString::number(rockbandInfo->endOfActivityYear));
	}
}

void MainWindow::updateAlbumElementsSlot(QString currentText)
{
	Core::AlbumInfoType* albumInfo = m_core->getAlbumInfoByTitle(currentText);

	if (albumInfo->title != currentText) {
		QMessageBox::warning(this, tr("Warning"), "Some error");
		return;
	}

	if (albumInfo->title == "") {
		return;
	}


	ui->albumTitleLineEdit_2->setText(albumInfo->title);
	ui->albumYearLineEdit_2->setText(QString::number(albumInfo->year));
	ui->albumPriceLineEdit_2->setText(QString::number(albumInfo->price));
	ui->volumeTypeComboBox_2->setCurrentText(albumInfo->volumeType);
	ui->recordTypeComboBox_2->setCurrentText(albumInfo->recordType);

	QString rockband = m_core->getRockbandTitleByAlbumTitle(currentText);
	ui->chooseRockbandComboBox_8->setCurrentText(rockband);

	if (albumInfo->cover == QByteArray()) {
		QString filename = "./resources/no-cover.jpg";
		QImage image(filename);
		ui->coverLabel_4->setPixmap(QPixmap::fromImage(image));
		return;
	}
	QPixmap picture;
	picture.loadFromData(albumInfo->cover);
	ui->coverLabel_4->setPixmap(picture);
}

void MainWindow::updateCompositionElementsSlot(QString currentText)
{
	Core::CompositionInfoType* compositionInfo = m_core->getCompositionInfoByTitle(currentText);
	Core::CompositionFileInfoType* compositionFileInfo = m_core->getCompositionFileInfoByTitle(currentText);


	if (compositionInfo->title != currentText) {
		QMessageBox::warning(this, tr("Warning"), "Some error");
		return;
	}

	if (compositionInfo->title == "") {
		return;
	}

	ui->compositionBitRateLineEdit_2->setText(QString::number(compositionFileInfo->bitRate));
	ui->compositionNumberSpinBox_2->setValue(compositionInfo->number);
	ui->compositionPriceLineEdit_2->setText(QString::number(compositionInfo->price));
	ui->compositionTitleLineEdit_2->setText(compositionInfo->title);
	ui->chooseCodecComboBox_2->setCurrentText(compositionFileInfo->codecType);
	ui->chooseGenreComboBox_2->setCurrentText(compositionInfo->genre);
	ui->fileSizeDoubleSpinBox_2->setValue(compositionFileInfo->fileSize);

	QStringList strlist = compositionInfo->duration.split(":");
	QTime time(strlist.at(0).toInt(), strlist.at(1).toInt(), strlist.at(2).toInt());
	ui->compositionDurationTimeEdit_2->setTime(time);


	QString album = m_core->getAlbumTitleByCompositionTitle(currentText);
	ui->chooseAlbumComboBox_3->setCurrentText(album);
}

void MainWindow::updateDeleteArtistElementSlot(QString currentText)
{
	QStringList rockbandList = m_core->getRockbandStringListByArtist(currentText);

	ui->chooseRockbandComboBox_9->clear();
	foreach(QString rockband, rockbandList){
		ui->chooseRockbandComboBox_9->addItem(rockband);
	}

}

void MainWindow::updateArtistElementSlot(QString currentText)
{
	Core::ArtistInfoType* artistInfo = m_core->getArtistInfoByName(currentText);

	if (artistInfo->name == "") {
		return;
	}

	ui->artistNameLineEdit_5->setText(artistInfo->name);
	ui->instumentsLineEdit_5->setText(artistInfo->instruments);
}

void MainWindow::purchaseAlbum()
{
	Core::AlbumInfoMiniType* albumInfo = m_core->getAlbumInfoMiniByTitle(m_currentAlbum);

	m_shoppingCart.append(albumInfo);
	QMessageBox::information(this, tr("Information"), tr("Added to cart successfully"));

	//if (!m_core->purchaseAlbum(albumInfo)) {
	//	QMessageBox::warning(this, tr("Warning"), m_core->lastError());
	//	return;
	//}

	//QMessageBox::information(this, tr("Information"), tr("Purchase successfully"));
}

void MainWindow::unpurchaseAlbum()
{
	Core::AlbumInfoMiniType* albumInfo = m_core->getAlbumInfoMiniByTitle(m_currentAlbum);

	if (m_shoppingCart.isEmpty()) {
		QMessageBox::information(this, tr("Information"), tr("Cart is empty"));
		return;
	}

	foreach (Core::AlbumInfoMiniType* album, m_shoppingCart) {
		if (album->title == albumInfo->title) {
			if (!m_shoppingCart.removeOne(album)) {
				QMessageBox::warning(this, tr("Warning"), "Some error");
				return;
			}
			break;
		}
		else {
			QMessageBox::warning(this, tr("Warning"), "Not found in cart");
			return;
		}
	}

	QMessageBox::information(this, tr("Information"), tr("Removed from cart successfully"));
}

void MainWindow::clearOrderTableWidget()
{
	QStringList table_header;
	table_header << "Album" << "Price";

	ui->ordersTableWidget->clear();
	ui->ordersTableWidget->setColumnCount(table_header.count());
	ui->ordersTableWidget->setHorizontalHeaderLabels(table_header);
	ui->ordersTableWidget->setRowCount(0);

	ui->ordersTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->ordersTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->ordersTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::updateOrderPage(int current)
{
	ui->ordersTableWidget->setRowCount(m_shoppingCart.count());
	float cost = 0;
	foreach (Core::AlbumInfoMiniType* album, m_shoppingCart) {
		ui->ordersTableWidget->setItem(m_shoppingCart.indexOf(album), 0,
									  new QTableWidgetItem(album->title));
		ui->ordersTableWidget->setItem(m_shoppingCart.indexOf(album), 1,
									  new QTableWidgetItem(QString::number(album->price)));
		cost += album->price;
	}
	ui->lineEdit->setText(QString::number(cost));
}

void MainWindow::confirmOrder()
{
	if (m_shoppingCart.isEmpty()) {
		QMessageBox::information(this, tr("Information"), tr("Cart is empty"));
		return;
	}

	/// TO XML
	m_core->saveOrderToXML(m_shoppingCart);

	if (!m_core->confirmOrder(m_shoppingCart)) {
		QMessageBox::warning(this, tr("Warning"), m_core->lastError());
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Complete!"));

	m_shoppingCart.clear();

	updateOrderPage(0);
}


