#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTabWidget>
#include <QStringList>
#include <QListWidget>
#include <QTableWidget>
#include <QList>

#include <QRegExp>
#include <QValidator>
#include <QRegExpValidator>
#include <QIntValidator>

#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QBuffer>
#include <QSqlTableModel>

#include "ConnectionToDataBaseDialog.h"
#include "Core.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	Core* m_core;
	bool m_isTabHidding;
	QWidget* m_adminTab;
	QString m_lastPictureFilename;

	ConnectionToDataBaseDialog connectionDialog;

	volatile bool m_isLogin;

	QString m_currentAlbum;
	QString m_currentComposition;

	QList<Core::AlbumInfoMiniType*> m_shoppingCart;

signals:
	void quitSignal();

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

private:
	Ui::MainWindow *ui;	

	void loadData();

	void updateRockbandListWidget(QStringList rockbandList);
//	void updateAlbumTableWidget(QList<Core::AlbumInfoMiniType*> albumList);

	void updateRockbandListWidget(QList<Core::RockbandInfoType*> rockbandList);
	void updateAlbumTableWidget(QList<Core::AlbumInfoType*> albumList);
	void updateCompositionTableWidget(QList<Core::CompositionInfoType*> compositionList);

	void updateInformationLabelByRockband(Core::RockbandInfoType* rockbandInfo);
	void updateInformationLabelByAlbum(Core::AlbumInfoType* albumInfo);
	void updateInformationLabelByComposition(Core::CompositionInfoType* compositionInfo, Core::CompositionFileInfoType* compositionFileInfo);

	void updateCoverLabel(Core::AlbumInfoType* albumInfo);

	void clearRockbandListWidget();
	void clearAlbumTableWidget();
	void clearCompositionTableWidget();
	void clearRockbandInformationLabel();
	void clearAlbumInformationLabel();
	void clearCompositionInformationLabel();
	void clearCoverLabel();
	void clearPurchaseAlbumButton();
	void clearPurchaseCompositionButton();

	void clearAllAdminWidgets();
	void clearAddRockband();
	void clearAddAlbum();
	void clearAddComposition();
	void clearAddArtist();
	void clearUpdateRockband();
	void clearUpdateAlbum();
	void clearUpdateComposition();
	void clearUpdateArtist();
	void clearAddArtistToRockband();
	void clearDeleteArtistFromRockband();

	void addRockbandAction();
	void addArtistAction();
	void addAlbumAction();
	void addCompositionAction();
	void addArtistToRockbandAction();

	void updateRockbandAction();
	void updateArtistAction();
	void updateAlbumAction();
	void updateCompositionAction();
	void deleteArtistFromRockbandAction();

	void clearOrderTableWidget();

private slots:
	void registrationSlot();

	void connectToDBSlot();
	void connectionFailedSlot(const QString& error);
	void connectionEstablishedSlot(const bool isAdmin);

	void rockbandItemChangedSlot(QListWidgetItem* current, QListWidgetItem* previous);
	void albumItemChangedSlot(QTableWidgetItem* current, QTableWidgetItem* previous);
	void albumItemChangedSlot(int selectedRow);

	void compositionItemChangedSlot(QTableWidgetItem* current, QTableWidgetItem* previous);
	void compositionItemChangedSlot(int selectedRow);

	void clearAllWidgets();

	void hideEndOfActivitySlot(int state);
	void hideEndOfActivity_2_Slot(int state);

	void selectAdminActionSlot();

	void addRockbandPreviousActionSlot();
	void addRockbandFinishActionSlot();
	void addArtistPreviousActionSlot();
	void addArtistFinishActionSlot();
	void addAlbumFinishActionSlot();
	void addAlbumPreviousActionSlot();
	void addCompositionFinishActionSlot();
	void addCompositionPreviousActionSlot();
	void addArtistToRockbandFinishActionSlot();
	void addArtistToRockbandPreviousActionSlot();

	void updateRockbandPreviousActionSlot();
	void updateRockbandFinishActionSlot();
	void updateArtistPreviousActionSlot();
	void updateArtistFinishActionSlot();
	void updateAlbumFinishActionSlot();
	void updateAlbumPreviousActionSlot();
	void updateCompositionFinishActionSlot();
	void updateCompositionPreviousActionSlot();
	void deleteArtistFromRockbandFinishActionSlot();
	void deleteArtistFromRockbandPreviousActionSlot();

	void openChooseCoverFileDialogSlot();

	void updateRockbandElementsSlot(QString currentText);
	void updateAlbumElementsSlot(QString currentText);
	void updateCompositionElementsSlot(QString currentText);
	void updateDeleteArtistElementSlot(QString currentText);
	void updateArtistElementSlot(QString currentText);

	void purchaseAlbum();
	void unpurchaseAlbum();

	void updateOrderPage(int current);
	void confirmOrder();
};

#endif // MAINWINDOW_H
