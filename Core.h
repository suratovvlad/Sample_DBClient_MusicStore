#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QList>

#include <QtXml/qxml.h>
#include <QXmlStreamWriter>
#include <QDateTime>

class Core : public QObject
{
	Q_OBJECT

private:
	QString m_username;
	QString m_password;
	QSqlDatabase m_database;
	QSqlQuery* m_sqlQuery;

	QString m_lastError;

public:
	struct RockbandInfoType : public QObject
	{
		QString	title;
		quint32	foundationYear;
		quint32	endOfActivityYear;
		bool	isEndOfActivity;
		RockbandInfoType(QObject* parent = NULL) :
			title(""),
			foundationYear(0),
			endOfActivityYear(0),
			isEndOfActivity(false),
			QObject(parent)
		{}
	};

	struct AlbumInfoType : public QObject
	{
		quint32		year;
		QString		title;
		QString		volumeType;
		QString		recordType;
		QByteArray	cover;
		float		price;
		AlbumInfoType(QObject* parent = NULL) :
			year(0),
			title(""),
			volumeType(""),
			recordType(""),
			cover(),
			price(0.0),
			QObject(parent)
		{}
//		bool albumInfoTypeCompare(Core::AlbumInfoType* left, Core::AlbumInfoType* right, quint8 field)
//		{
//			if (field < 0) {
//				field = 0;
//			}
//			if (field > 5) {
//				field = 5;
//			}
//			bool retval = false;

//			switch (field) {
//				case 0:
//					retval = left->year < right->year;
//				case 1:
//					retval = left->title.toLower() < right->title.toLower();
//				case 2:
//					retval = left->volumeType.toLower() < right->volumeType.toLower();
//				case 3:
//					retval = left->recordType.toLower() < right->recordType.toLower();
//				case 5:
//					retval = left->price < right->price;
//				default:
//					retval = false;
//			}
//			return
//		}
	};

	struct CompositionInfoType : public QObject
	{
		quint32	number;
		QString	title;
		QString	genre;
		QString	duration;
		float	price;
		CompositionInfoType(QObject* parent = NULL) :
			number(0),
			title(""),
			genre(""),
			duration(""),
			price(0.0),
			QObject(parent)
		{}
	};

	struct ArtistInfoType : public QObject
	{
		QString	name;
		QString instruments;
		ArtistInfoType(QObject* parent = NULL) :
			name(""),
			instruments(""),
			QObject(parent)
		{}
	};

	struct CompositionFileInfoType : public QObject
	{
		QString	codecType;
		quint32	bitRate;
		float	fileSize;
		CompositionFileInfoType(QObject* parent = NULL) :
			codecType(""),
			bitRate(0),
			fileSize(0.0),
			QObject(parent)
		{}
	};

	struct AlbumInfoMiniType : public QObject
	{
		QString		title;
		float		price;
		AlbumInfoMiniType(QObject* parent = NULL) :
			title(""),
			price(0.0),
			QObject(parent)
		{}
	};

	QString lastError()
	{
		return m_lastError;
	}

public:
	explicit Core(QObject *parent = 0);
	virtual ~Core();
	void changeConnection(const QString& username, const QString& password);
	void tryingToInsert();

	QStringList getRockbandStringList();
	QStringList getArtistStringList();
	QStringList getArtistStringListByRockband(const QString& rockbandTitle);
	QStringList getAlbumStringList();
	QStringList getCompositionStringList();

/*
	QList<Core::AlbumInfoMiniType*> getAlbumMiniList(const QString& rockband)
	{
		m_sqlQuery->prepare("SELECT year, title, price FROM album WHERE Rockband_idRockband IN"
							"(SELECT idRockband FROM rockband WHERE title = :title) ORDER BY year");
		m_sqlQuery->bindValue(":title", rockband);
		m_sqlQuery->exec();

		if (m_sqlQuery->lastError().isValid()) {
			qDebug() << m_sqlQuery->lastError().text();
		}

		QList<AlbumInfoMiniType*> albumList;
		while (m_sqlQuery->next()) {
			AlbumInfoMiniType* albumInfo = new AlbumInfoMiniType;
			albumInfo->year = m_sqlQuery->value(0).toUInt();
			qDebug() << "year = " << albumInfo->year;
			albumInfo->title = m_sqlQuery->value(1).toString();
			qDebug() << "title = " << albumInfo->title;
			albumInfo->price	= m_sqlQuery->value(2).toFloat();
			qDebug() << "price = " << albumInfo->price;
			albumList.append(albumInfo);
		}
		return albumList;
	}
*/
	QList<Core::RockbandInfoType*> getRockbandList();
	QList<Core::AlbumInfoType*> getAlbumList(const QString& rockband);
	QList<Core::CompositionInfoType*> getCompositionList(const QString& album);
	QList<Core::ArtistInfoType*> getArtistList(const QString& rockband);

	Core::AlbumInfoType* getAlbumInfoByTitle(const QString& albumTitle);
	Core::RockbandInfoType* getRockbandInfoByTitle(const QString& rockbandTitle);
	Core::CompositionInfoType* getCompositionInfoByTitle(const QString& compositionTitle);
	Core::CompositionFileInfoType* getCompositionFileInfoByTitle(const QString& compositionTitle);
	Core::ArtistInfoType* getArtistInfoByName(const QString& artist);

	Core::AlbumInfoMiniType* getAlbumInfoMiniByTitle(const QString &albumTitle)
	{
		QSqlQuery sqlQuery(m_database);
		sqlQuery.prepare("SELECT title, price FROM album WHERE title=:title");
		sqlQuery.bindValue(":title", albumTitle);
		sqlQuery.exec();

		if (sqlQuery.lastError().isValid()) {
			m_lastError = sqlQuery.lastError().text();
			qDebug() << sqlQuery.lastError().text();
		}

		Core::AlbumInfoMiniType* albumInfo = new Core::AlbumInfoMiniType();
		while (sqlQuery.next()) {
			albumInfo->title = sqlQuery.value("title").toString();
			qDebug() << "title =" << albumInfo->title;
			albumInfo->price = sqlQuery.value("price").toFloat();
			qDebug() << "price =" << albumInfo->price;
		}

		return albumInfo;
	}

	Core::AlbumInfoType* getAlbumInfo();
	Core::RockbandInfoType* getRockbandInfo();
	Core::CompositionInfoType* getCompositionInfo();
	Core::CompositionFileInfoType* getCompositionFileInfo();

	QString getRockbandTitleByAlbumTitle(const QString& albumTitle);
	QString getAlbumTitleByCompositionTitle(const QString& compositionTitle);

	QStringList getRockbandStringListByArtist(const QString& artist);

	bool addRockband(Core::RockbandInfoType* rockbandInfo);
	bool addArtist(Core::ArtistInfoType* artistInfo, const QString& rockbandTitle);
	bool addArtistHasRockband(const QString& artist, const QString& rockband);
	bool addAlbum(Core::AlbumInfoType* albumInfo, const QString& rockbandTitle);
	bool addComposition(Core::CompositionFileInfoType* compositionFileInfo, Core::CompositionInfoType* compositionInfo, const QString& albumtitle);
	bool addCompositionFileInfo(Core::CompositionFileInfoType* compositionFileInfo, const QString& compositionTitle);

	bool updateRockband(Core::RockbandInfoType* rockbandInfo, const QString& oldTitle);
	bool updateAlbum(Core::AlbumInfoType* albumInfo, const QString& oldTitle, const QString& newRockband);
	bool updateComposition(Core::CompositionFileInfoType* compositionFileInfo, Core::CompositionInfoType* compositionInfo,
						   const QString& oldTitle, const QString& newAlbum);

	bool deleteArtist(const QString& artist, const QString& rockband);

	bool updateArtist(Core::ArtistInfoType* artistInfo, const QString& oldName);


	bool registrateUser(const QString& user, const QString& passwd);
	QStringList getUsersStringList();

	bool purchaseAlbum(Core::AlbumInfoType* albumInfo);
	bool unpurchaseAlbum(Core::AlbumInfoType* albumInfo);

	QList<Core::AlbumInfoMiniType*> getAlbumMiniListForOrderTable();

	bool confirmOrder(QList<Core::AlbumInfoMiniType*> m_shoppingCart);

	void saveOrderToXML(QList<Core::AlbumInfoMiniType*> albumList);


private:
	QString getUserPassword(const QString& user);
signals:
	void connectionFailedSignal(const QString& error);
	void connectionEstablishedSignal(const bool isAdmin);
};

#endif // CORE_H
