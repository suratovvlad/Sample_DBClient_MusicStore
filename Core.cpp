#include "Core.h"
#include <QFileDialog>
#include <QImage>
#include <QFile>



Core::Core(QObject *parent) :
	QObject(parent)
{
	m_username = m_password = "";
	m_database = QSqlDatabase::addDatabase("QMYSQL", "mydb");

	m_database.setHostName("localhost");
	m_database.setDatabaseName("musicstore");
	m_database.setUserName(m_username);
	m_database.setPassword(m_password);

	m_sqlQuery = new QSqlQuery(m_database);

	m_lastError = "";
}

Core::~Core()
{
	if (m_database.isOpen()) {
		m_database.close();
	}
}

void Core::changeConnection(const QString &username, const QString &password)
{
	if (m_database.isOpen()) {
		m_database.close();
	}

	if (username != "root") {
		m_database.setUserName("user");
		m_database.setPassword("user");

		if (!m_database.open()) {
			qDebug() << m_database.lastError().text();
			emit connectionFailedSignal(m_database.lastError().text());
			return;
		}

		emit connectionEstablishedSignal(false);
	}
	else {
		m_database.setUserName(username);
		m_database.setPassword(password);

		if (!m_database.open()) {
			qDebug() << m_database.lastError().text();
			emit connectionFailedSignal(m_database.lastError().text());
			return;
		}

		emit connectionEstablishedSignal(true);
		//return;
	}

	m_username = username;
	m_password = password;

	QString savedpasswd = getUserPassword(m_username);
	if (savedpasswd != m_password) {
		emit connectionFailedSignal("Wrong password or login");
		return;
	}


}

void Core::tryingToInsert()
{
	/*
	m_sqlQuery->exec("SELECT idRockBand, title FROM rockband WHERE title = \"Guns N' Roses\"");
	quint32 idRockBand;
	while (m_sqlQuery->next()) {
		idRockBand = m_sqlQuery->value(0).toUInt();
		qDebug() << "idRockBand = " << idRockBand;
	}


	m_sqlQuery->prepare("INSERT INTO album (title, year, volumeType, recordType, price, Rockband_idRockband)"
						"VALUES (:title, :year, :volumeType, :recordType, :price, :Rockband_idRockband)");
	m_sqlQuery->bindValue(":title", "Welcome To The Jungle");
	m_sqlQuery->bindValue(":year", 1985);
	m_sqlQuery->bindValue(":volumeType", "LP");
	m_sqlQuery->bindValue(":recordType", "Studio Album");
	m_sqlQuery->bindValue(":price", 14.99);
	m_sqlQuery->bindValue(":Rockband_idRockband", idRockBand);

	if (!m_sqlQuery->exec()) {
		qDebug() << m_sqlQuery->lastError().text();
	}*/

/*
	QString filename = QFileDialog::getOpenFileName();
	QImage image(filename);
//	ui->coverLabel->setPixmap(QPixmap::fromImage(image));
	qDebug() << filename;
	// load image to bytearray
	QByteArray ba;
	QFile f(filename);
	if(f.open(QIODevice::ReadOnly)) {
		ba = f.readAll();
		f.close();
	}

	// Writing the image into table
//	QSqlDatabase::database().transaction();
//	QSqlQuery query;

	m_sqlQuery->prepare("UPDATE album SET cover=(:cover) WHERE title = \"Appetite for Destruction\"");
	m_sqlQuery->bindValue(":cover", ba);
	m_sqlQuery->exec();
	if( m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
//		QSqlDatabase::database().rollback();
	} else {
//		QSqlDatabase::database().commit();
	}
	*/


}

QStringList Core::getRockbandStringList()
{
	m_sqlQuery->prepare("SELECT title FROM rockband");
	m_sqlQuery->exec();
	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	QStringList rockbandList;
	while (m_sqlQuery->next()) {
		rockbandList << m_sqlQuery->value("title").toString();
		qDebug() << "title = " << rockbandList.last();
	}
	return rockbandList;
}

QStringList Core::getArtistStringList()
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT name FROM artist");
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList artistList;
	while (sqlQuery.next()) {
		artistList << sqlQuery.value("name").toString();
		qDebug() << "name = " << artistList.last();
	}
	return artistList;
}

QStringList Core::getArtistStringListByRockband(const QString& rockbandTitle)
{
	QSqlQuery sqlQuery(m_database);

	//SELECT * FROM artist WHERE idArtist IN (SELECT Artist_idArtist FROM artist_has_rockband WHERE Rockband_idRockband IN (SELECT idRockband FROM rockband WHERE title = 'Guns N\' Roses'));
	sqlQuery.prepare("SELECT name FROM artist WHERE idArtist IN"
					 "(SELECT Artist_idArtist FROM artist_has_rockband WHERE Rockband_idRockband IN"
					 "(SELECT idRockband FROM rockband WHERE title = :title))");
	sqlQuery.bindValue(":title", rockbandTitle);
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList artistList;
	while (sqlQuery.next()) {
		artistList << sqlQuery.value("name").toString();
		qDebug() << "name = " << artistList.last();
	}
	return artistList;
}

QStringList Core::getAlbumStringList()
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT title FROM album");
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList albumList;
	while (sqlQuery.next()) {
		albumList << sqlQuery.value("title").toString();
		qDebug() << "title = " << albumList.last();
	}
	return albumList;
}

QStringList Core::getCompositionStringList()
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT title FROM composition");
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList albumList;
	while (sqlQuery.next()) {
		albumList << sqlQuery.value("title").toString();
		qDebug() << "title = " << albumList.last();
	}
	return albumList;
}

QStringList Core::getRockbandStringListByArtist(const QString &artist)
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT title FROM rockband WHERE idRockband IN"
					 "(SELECT Rockband_idRockband FROM artist_has_rockband WHERE Artist_idArtist IN"
					 "(SELECT idArtist FROM artist WHERE name = :name));");
	sqlQuery.bindValue(":name", artist);
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList rockbandList;
	while (sqlQuery.next()) {
		rockbandList << sqlQuery.value("title").toString();
		qDebug() << "title = " << rockbandList.last();
	}
	return rockbandList;
}

QList<Core::RockbandInfoType*> Core::getRockbandList()
{
	m_sqlQuery->prepare("SELECT title, foundationYear, endOfActivityYear, isEndOfActivity FROM rockband");
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	QList<RockbandInfoType*> rockbandList;
	while (m_sqlQuery->next()) {
		RockbandInfoType* rockbandInfo = getRockbandInfo();
		rockbandList.append(rockbandInfo);
	}
	return rockbandList;
}

QList<Core::AlbumInfoType*> Core::getAlbumList(const QString& rockband)
{
	//Inserted query
	m_sqlQuery->prepare("SELECT year, title, volumeType, recordType, cover, price FROM album WHERE Rockband_idRockband IN"
						"(SELECT idRockband FROM rockband WHERE title = :title) ORDER BY year");
	m_sqlQuery->bindValue(":title", rockband);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	QList<AlbumInfoType*> albumList;
	while (m_sqlQuery->next()) {
		AlbumInfoType* albumInfo = getAlbumInfo();
		albumList.append(albumInfo);
	}
	return albumList;
}

QList<Core::CompositionInfoType*> Core::getCompositionList(const QString& album)
{
	//Inserted query
	m_sqlQuery->prepare("SELECT number, title, genre, duration, price FROM composition WHERE Album_idAlbum IN"
						"(SELECT idAlbum FROM album WHERE title = :title) ORDER BY number");
	m_sqlQuery->bindValue(":title", album);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	QList<CompositionInfoType*> compositionList;
	while (m_sqlQuery->next()) {
		CompositionInfoType* compositionInfo = getCompositionInfo();
		compositionList.append(compositionInfo);
	}
	return compositionList;
}

QList<Core::ArtistInfoType*> Core::getArtistList(const QString& rockband)
{
	//Inserted query
	m_sqlQuery->prepare("SELECT name, instruments FROM artist WHERE idArtist IN"
						"(SELECT Artist_idArtist FROM artist_has_rockband WHERE Rockband_idRockband IN"
						"(SELECT idRockband FROM rockband WHERE title = :title))");
	m_sqlQuery->bindValue(":title", rockband);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	QList<Core::ArtistInfoType*> artistList;
	while (m_sqlQuery->next()) {
		ArtistInfoType* artistInfo = new ArtistInfoType();
		artistInfo->deleteLater();

		artistInfo->name = m_sqlQuery->value("name").toString();
		qDebug() << "name = " << artistInfo->name;

		artistInfo->instruments = m_sqlQuery->value("instruments").toString();
		qDebug() << "instruments = " << artistInfo->instruments;

		artistList.append(artistInfo);
	}
	return artistList;
}

Core::AlbumInfoType* Core::getAlbumInfoByTitle(const QString& albumTitle)
{
	m_sqlQuery->prepare("SELECT year, title, volumeType, recordType, cover, price FROM album WHERE title = :title");
	m_sqlQuery->bindValue(":title", albumTitle);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	Core::AlbumInfoType* albumInfo = new Core::AlbumInfoType();
	albumInfo->deleteLater();
	while (m_sqlQuery->next()) {
		delete albumInfo;
		albumInfo = getAlbumInfo();
	}
	return albumInfo;
}

Core::RockbandInfoType* Core::getRockbandInfoByTitle(const QString& rockbandTitle)
{
	m_sqlQuery->prepare("SELECT title, foundationYear, endOfActivityYear, isEndOfActivity FROM rockband WHERE title = :title");
	m_sqlQuery->bindValue(":title", rockbandTitle);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	Core::RockbandInfoType* rockbandInfo = new Core::RockbandInfoType();
	rockbandInfo->deleteLater();
	while (m_sqlQuery->next()) {
		delete rockbandInfo;
		rockbandInfo = getRockbandInfo();
	}
	return rockbandInfo;
}

Core::CompositionInfoType* Core::getCompositionInfoByTitle(const QString& compositionTitle)
{
	m_sqlQuery->prepare("SELECT number, title, genre, duration, price FROM composition WHERE title = :title");
	m_sqlQuery->bindValue(":title", compositionTitle);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	Core::CompositionInfoType* compositionInfo = new Core::CompositionInfoType();
	compositionInfo->deleteLater();
	while (m_sqlQuery->next()) {
		delete compositionInfo;
		compositionInfo = getCompositionInfo();
	}
	return compositionInfo;
}

Core::CompositionFileInfoType* Core::getCompositionFileInfoByTitle(const QString& compositionTitle)
{
	m_sqlQuery->prepare("SELECT codecType, bitRate, fileSize FROM compositionfile WHERE Composition_idComposition IN"
						"(SELECT idComposition FROM composition WHERE title = :title)");
	m_sqlQuery->bindValue(":title", compositionTitle);
	m_sqlQuery->exec();

	if (m_sqlQuery->lastError().isValid()) {
		qDebug() << m_sqlQuery->lastError().text();
	}

	Core::CompositionFileInfoType* compositionFileInfo = NULL;
	while (m_sqlQuery->next()) {
		compositionFileInfo = getCompositionFileInfo();
	}
	return compositionFileInfo == NULL ? new Core::CompositionFileInfoType() : compositionFileInfo;
}

Core::RockbandInfoType* Core::getRockbandInfo()
{
	Core::RockbandInfoType* rockbandInfo = new Core::RockbandInfoType();

	rockbandInfo->title = m_sqlQuery->value(0).toString();
	qDebug() << "title = " << rockbandInfo->title;

	rockbandInfo->foundationYear = m_sqlQuery->value(1).toUInt();
	qDebug() << "foundationYear = " << rockbandInfo->foundationYear;

	rockbandInfo->endOfActivityYear = m_sqlQuery->value(2).toUInt();
	qDebug() << "endOfActivityYear = " << rockbandInfo->endOfActivityYear;

	rockbandInfo->isEndOfActivity = m_sqlQuery->value(2).toBool();
	qDebug() << "isEndOfActivity = " << rockbandInfo->isEndOfActivity;

	rockbandInfo->deleteLater();

	return rockbandInfo;
}

Core::AlbumInfoType* Core::getAlbumInfo()
{
	Core::AlbumInfoType* albumInfo = new Core::AlbumInfoType();

	albumInfo->year = m_sqlQuery->value(0).toUInt();
	qDebug() << "year = " << albumInfo->year;

	albumInfo->title = m_sqlQuery->value(1).toString();
	qDebug() << "title = " << albumInfo->title;

	albumInfo->volumeType = m_sqlQuery->value(2).toString();
	qDebug() << "volumeType = " << albumInfo->volumeType;

	albumInfo->recordType = m_sqlQuery->value(3).toString();
	qDebug() << "recordType = " << albumInfo->recordType;

	albumInfo->cover = m_sqlQuery->value(4).toByteArray();
	qDebug() << "cover = " << albumInfo->cover;

	albumInfo->price = m_sqlQuery->value(5).toFloat();
	qDebug() << "price = " << albumInfo->price;

	albumInfo->deleteLater();

	return albumInfo;
}

Core::CompositionInfoType* Core::getCompositionInfo()
{
	Core::CompositionInfoType* compositionInfo = new Core::CompositionInfoType();

	compositionInfo->number = m_sqlQuery->value(0).toUInt();
	qDebug() << "number = " << compositionInfo->number;

	compositionInfo->title = m_sqlQuery->value(1).toString();
	qDebug() << "title = " << compositionInfo->title;

	compositionInfo->genre = m_sqlQuery->value(2).toString();
	qDebug() << "genre = " << compositionInfo->genre;

	compositionInfo->duration = m_sqlQuery->value(3).toString();
	qDebug() << "duration = " << compositionInfo->duration;

	compositionInfo->price = m_sqlQuery->value(4).toFloat();
	qDebug() << "price = " << compositionInfo->price;

	compositionInfo->deleteLater();

	return compositionInfo;
}

Core::CompositionFileInfoType* Core::getCompositionFileInfo()
{
	Core::CompositionFileInfoType* compositionFileInfo = new Core::CompositionFileInfoType();

	compositionFileInfo->codecType = m_sqlQuery->value(0).toString();
	qDebug() << "codecType = " << compositionFileInfo->codecType;

	compositionFileInfo->bitRate = m_sqlQuery->value(1).toUInt();
	qDebug() << "bitRate = " << compositionFileInfo->bitRate;

	compositionFileInfo->fileSize = m_sqlQuery->value(2).toFloat();
	qDebug() << "fileSize = " << compositionFileInfo->fileSize;

	compositionFileInfo->deleteLater();

	return compositionFileInfo;
}

Core::ArtistInfoType* Core::getArtistInfoByName(const QString &artist)
{
	Core::ArtistInfoType* artistInfo = new Core::ArtistInfoType();
	artistInfo->deleteLater();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT name, instruments FROM artist WHERE name = :name");
	sqlQuery.bindValue(":name", artist);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	while (sqlQuery.next()) {
		artistInfo->name = sqlQuery.value("name").toString();
		qDebug() << "name:" << artistInfo->name;

		artistInfo->instruments = sqlQuery.value("instruments").toString();
		qDebug() << "instruments:" << artistInfo->instruments;
	}


	return artistInfo;
}

bool Core::addRockband(Core::RockbandInfoType *rockbandInfo)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("INSERT INTO rockband (title, foundationYear, endOfActivityYear, isEndOfActivity)"
					 "VALUES (:title, :foundationYear, :endOfActivityYear, :isEndOfActivity)");
	sqlQuery.bindValue(":title", rockbandInfo->title);
	sqlQuery.bindValue(":foundationYear", rockbandInfo->foundationYear);
	sqlQuery.bindValue(":endOfActivityYear", rockbandInfo->endOfActivityYear);
	sqlQuery.bindValue(":isEndOfActivity", rockbandInfo->isEndOfActivity);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}
	m_database.commit();
	return true;
}

bool Core::addArtist(Core::ArtistInfoType *artistInfo, const QString& rockbandTitle)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);

	//Add to artist table
	sqlQuery.prepare("INSERT INTO artist (name, instruments)"
					 "VALUES (:name, :instruments)");
	sqlQuery.bindValue(":name", artistInfo->name);
	sqlQuery.bindValue(":instruments", artistInfo->instruments);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::addArtistHasRockband(const QString &artist, const QString &rockband)
{

	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
//	SELECT T1.idRockband, T2.idArtist FROM  (SELECT idRockband FROM rockband WHERE title = 'Guns N\' Roses') AS T1,	(SELECT idArtist FROM artist WHERE name = 'Axl Rose') AS T2;
	sqlQuery.prepare("SELECT T1.idRockband, T2.idArtist FROM"
					 "(SELECT idRockband FROM rockband WHERE title = :title) AS T1,"
					 "(SELECT idArtist FROM artist WHERE name = :name) AS T2");
	sqlQuery.bindValue(":title", rockband);
	sqlQuery.bindValue(":name", artist);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idRockband	= 0;
	quint32 idArtist	= 0;
	while (sqlQuery.next()) {
		idRockband = sqlQuery.value("idRockband").toUInt();
		qDebug() << "Rockband id:" << idRockband;

		idArtist = sqlQuery.value("idArtist").toUInt();
		qDebug() << "Artist id:" << idArtist;
	}

	sqlQuery.prepare("INSERT INTO artist_has_rockband (Artist_idArtist, Rockband_idRockband)"
					 "VALUES (:Artist_idArtist, :Rockband_idRockband)");
	sqlQuery.bindValue(":Artist_idArtist", idArtist);
	sqlQuery.bindValue(":Rockband_idRockband", idRockband);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}
	m_database.commit();
	return true;
}

bool Core::addAlbum(Core::AlbumInfoType *albumInfo, const QString &rockbandTitle)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idRockband FROM rockband WHERE title = :title");
	sqlQuery.bindValue(":title", rockbandTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idRockband = 0;
	while (sqlQuery.next()) {
		idRockband = sqlQuery.value("idRockband").toUInt();
		qDebug() << "Rockband id:" << idRockband;
	}

	sqlQuery.prepare("INSERT INTO album (title, year, volumeType, recordType, price, Rockband_idRockband, cover)"
					 "VALUES (:title, :year, :volumeType, :recordType, :price, :Rockband_idRockband, :cover)");
	sqlQuery.bindValue(":title", albumInfo->title);
	sqlQuery.bindValue(":year", albumInfo->year);
	sqlQuery.bindValue(":volumeType", albumInfo->volumeType);
	sqlQuery.bindValue(":recordType", albumInfo->recordType);
	sqlQuery.bindValue(":price", albumInfo->price);
	sqlQuery.bindValue(":Rockband_idRockband", idRockband);
	sqlQuery.bindValue(":cover", albumInfo->cover);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}
	m_database.commit();
	return true;
}

bool Core::addComposition(Core::CompositionFileInfoType *compositionFileInfo, Core::CompositionInfoType *compositionInfo, const QString &albumtitle)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idAlbum FROM album WHERE title = :title");
	sqlQuery.bindValue(":title", albumtitle);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idAlbum = 0;
	while (sqlQuery.next()) {
		idAlbum = sqlQuery.value("idAlbum").toUInt();
		qDebug() << "Album id:" << idAlbum;
	}

	sqlQuery.prepare("INSERT INTO composition (number, title, genre, duration, price, Album_idAlbum)"
					 "VALUES (:number, :title, :genre, :duration, :price, :Album_idAlbum)");
	sqlQuery.bindValue(":number", compositionInfo->number);
	sqlQuery.bindValue(":title", compositionInfo->title);
	sqlQuery.bindValue(":genre", compositionInfo->genre);
	sqlQuery.bindValue(":duration", compositionInfo->duration);
	sqlQuery.bindValue(":price", compositionInfo->price);
	sqlQuery.bindValue(":Album_idAlbum", idAlbum);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	sqlQuery.prepare("SELECT idComposition FROM composition WHERE title = :title");
	sqlQuery.bindValue(":title", compositionInfo->title);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idComposition = 0;
	while (sqlQuery.next()) {
		idComposition = sqlQuery.value("idComposition").toUInt();
		qDebug() << "Composition id:" << idComposition;
	}

	sqlQuery.prepare("INSERT INTO compositionfile (codecType, bitRate, fileSize, Composition_idComposition)"
					 "VALUES (:codecType, :bitRate, :fileSize, :Composition_idComposition)");
	sqlQuery.bindValue(":codecType", compositionFileInfo->codecType);
	sqlQuery.bindValue(":bitRate", compositionFileInfo->bitRate);
	sqlQuery.bindValue(":fileSize", compositionFileInfo->fileSize);
	sqlQuery.bindValue(":Composition_idComposition", idComposition);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::addCompositionFileInfo(Core::CompositionFileInfoType *compositionFileInfo, const QString &compositionTitle)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idComposition FROM composition WHERE title = :title");
	sqlQuery.bindValue(":title", compositionTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idComposition = 0;
	while (sqlQuery.next()) {
		idComposition = sqlQuery.value("idComposition").toUInt();
		qDebug() << "Composition id:" << idComposition;
	}

	sqlQuery.prepare("INSERT INTO compositionfile (codecType, bitRate, fileSize, Composition_idComposition)"
					 "VALUES (:codecType, :bitRate, :fileSize, :Composition_idComposition)");
	sqlQuery.bindValue(":codecType", compositionFileInfo->codecType);
	sqlQuery.bindValue(":bitRate", compositionFileInfo->bitRate);
	sqlQuery.bindValue(":fileSize", compositionFileInfo->fileSize);
	sqlQuery.bindValue(":Composition_idComposition", idComposition);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::updateRockband(Core::RockbandInfoType *rockbandInfo, const QString& oldTitle)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("UPDATE rockband SET title=:title, foundationYear=:foundationYear, isEndOfActivity=:isEndOfActivity, endOfActivityYear=:endOfActivityYear WHERE title=:oldtitle");
	sqlQuery.bindValue(":title", rockbandInfo->title);
	sqlQuery.bindValue(":foundationYear", rockbandInfo->foundationYear);
	sqlQuery.bindValue(":isEndOfActivity", rockbandInfo->isEndOfActivity);
	sqlQuery.bindValue(":endOfActivityYear", rockbandInfo->endOfActivityYear);
	sqlQuery.bindValue(":oldtitle", oldTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

QString Core::getRockbandTitleByAlbumTitle(const QString &albumTitle)
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT title FROM rockband WHERE idRockband IN"
					 "(SELECT Rockband_idRockband FROM album WHERE title = :title)");
	sqlQuery.bindValue(":title", albumTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		return "";
	}

	QString rockbandTitle = 0;
	while (sqlQuery.next()) {
		rockbandTitle = sqlQuery.value("title").toString();
		qDebug() << "rockbandTitle:" << rockbandTitle;
	}

	return rockbandTitle;
}

bool Core::updateAlbum(Core::AlbumInfoType *albumInfo, const QString &oldTitle, const QString& newRockband)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idRockband FROM rockband WHERE title = :title");
	sqlQuery.bindValue(":title", newRockband);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idRockband = 0;
	while (sqlQuery.next()) {
		idRockband = sqlQuery.value("idRockband").toUInt();
		qDebug() << "Rockband id:" << idRockband;
	}

	sqlQuery.prepare("UPDATE album SET title=:title, year=:year, volumeType=:volumeType, recordType=:recordType, price=:price, Rockband_idRockband=:Rockband_idRockband, cover=:cover WHERE title=:oldtitle");
	sqlQuery.bindValue(":title", albumInfo->title);
	sqlQuery.bindValue(":year", albumInfo->year);
	sqlQuery.bindValue(":volumeType", albumInfo->volumeType);
	sqlQuery.bindValue(":recordType", albumInfo->recordType);
	sqlQuery.bindValue(":price", albumInfo->price);
	sqlQuery.bindValue(":Rockband_idRockband", idRockband);
	sqlQuery.bindValue(":cover", albumInfo->cover);
	sqlQuery.bindValue(":oldtitle", oldTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

QString Core::getAlbumTitleByCompositionTitle(const QString &compositionTitle)
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT title FROM album WHERE idAlbum IN"
					 "(SELECT Album_idAlbum FROM composition WHERE title = :title)");
	sqlQuery.bindValue(":title", compositionTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		return "";
	}

	QString albumTitle = 0;
	while (sqlQuery.next()) {
		albumTitle = sqlQuery.value("title").toString();
		qDebug() << "albumTitle:" << albumTitle;
	}

	return albumTitle;
}

bool Core::updateComposition(Core::CompositionFileInfoType *compositionFileInfo, Core::CompositionInfoType *compositionInfo, const QString &oldTitle, const QString &newAlbum)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idAlbum FROM album WHERE title = :title");
	sqlQuery.bindValue(":title", newAlbum);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idAlbum = 0;
	while (sqlQuery.next()) {
		idAlbum = sqlQuery.value("idAlbum").toUInt();
		qDebug() << "idAlbum id:" << idAlbum;
	}

	sqlQuery.prepare("UPDATE composition SET number=:number, title=:title, genre=:genre, duration=:duration, price=:price, Album_idAlbum=:Album_idAlbum WHERE title=:oldtitle");
	sqlQuery.bindValue(":number", compositionInfo->number);
	sqlQuery.bindValue(":title", compositionInfo->title);
	sqlQuery.bindValue(":genre", compositionInfo->genre);
	sqlQuery.bindValue(":duration", compositionInfo->duration);
	sqlQuery.bindValue(":price", compositionInfo->price);
	sqlQuery.bindValue(":Album_idAlbum", idAlbum);
	sqlQuery.bindValue(":oldtitle", oldTitle);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	sqlQuery.prepare("SELECT idComposition FROM composition WHERE title = :title");
	sqlQuery.bindValue(":title", compositionInfo->title);
	sqlQuery.exec();


	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idComposition = 0;
	while (sqlQuery.next()) {
		idComposition = sqlQuery.value("idComposition").toUInt();
		qDebug() << "Composition id:" << idComposition;
	}

	sqlQuery.prepare("UPDATE compositionfile SET codecType=:codecType, bitRate=:bitRate, fileSize=:fileSize WHERE Composition_idComposition=:Composition_idComposition");
	sqlQuery.bindValue(":codecType", compositionFileInfo->codecType);
	sqlQuery.bindValue(":bitRate", compositionFileInfo->bitRate);
	sqlQuery.bindValue(":fileSize", compositionFileInfo->fileSize);
	sqlQuery.bindValue(":Composition_idComposition", idComposition);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::deleteArtist(const QString &artist, const QString &rockband)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("DELETE FROM artist_has_rockband WHERE (Artist_idArtist IN (SELECT idArtist FROM artist WHERE name = :name)) AND (Rockband_idRockband IN (SELECT idRockband FROM rockband WHERE title = :title))");
	sqlQuery.bindValue(":name", artist);
	sqlQuery.bindValue(":title", rockband);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::updateArtist(Core::ArtistInfoType *artistInfo, const QString &oldName)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("UPDATE artist SET name=:name, instruments=:instruments WHERE name=:oldName");
	sqlQuery.bindValue(":name", artistInfo->name);
	sqlQuery.bindValue(":instruments", artistInfo->instruments);
	sqlQuery.bindValue(":oldName", oldName);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

QStringList Core::getUsersStringList()
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT name FROM users");
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QStringList userList;
	while (sqlQuery.next()) {
		userList << sqlQuery.value("name").toString();
		qDebug() << "name = " << userList.last();
	}
	return userList;
}

QString Core::getUserPassword(const QString& user)
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT password FROM users WHERE name = :name");
	sqlQuery.bindValue(":name", user);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
	}

	QString password = "";
	while (sqlQuery.next()) {
		password = sqlQuery.value("password").toString();
		qDebug() << "password = " << password;
	}
	return password;
}

bool Core::registrateUser(const QString &user, const QString &passwd)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("INSERT INTO users (name, password) VALUES (:name, :password)");
	sqlQuery.bindValue(":name",user);
	sqlQuery.bindValue(":password", passwd);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

bool Core::purchaseAlbum(AlbumInfoType* albumInfo)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idUsers FROM users WHERE name = :name");
	sqlQuery.bindValue(":name", m_username);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idUsers = 0;
	while (sqlQuery.next()) {
		idUsers = sqlQuery.value("idUsers").toUInt();
		qDebug() << "idUsers = " << idUsers;
	}

	sqlQuery.prepare("SELECT idAlbum FROM album WHERE title = :title");
	sqlQuery.bindValue(":title", albumInfo->title);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idAlbum = 0;
	while (sqlQuery.next()) {
		idAlbum = sqlQuery.value("idAlbum").toUInt();
		qDebug() << "idAlbum = " << idAlbum;
	}

	sqlQuery.prepare("INSERT INTO orders (Users_idUsers, Album_idAlbum) VALUES (:user, :album)");
	sqlQuery.bindValue(":user",idUsers);
	sqlQuery.bindValue(":album", idAlbum);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}


bool Core::unpurchaseAlbum(AlbumInfoType* albumInfo)
{
	m_database.transaction();

	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("DELETE FROM orders WHERE (Users_idUsers IN(SELECT idUsers FROM users WHERE name = :name)) AND (Album_idAlbum IN (SELECT idAlbum FROM album WHERE title = :title)) LIMIT 1");
	sqlQuery.bindValue(":name", m_username);
	sqlQuery.bindValue(":title", albumInfo->title);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	m_database.commit();
	return true;
}

QList<Core::AlbumInfoMiniType*> Core::getAlbumMiniListForOrderTable()
{
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT Album_idAlbum FROM orders WHERE Users_idUsers IN (SELECT idUsers FROM users WHERE name=:user)");
	sqlQuery.bindValue(":user", m_username);
	sqlQuery.exec();
	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();;
	}

	QList<quint32> idAlbumsList;
	while (sqlQuery.next()) {
		idAlbumsList << sqlQuery.value("Album_idAlbum").toUInt();
		qDebug() << "Album_idAlbum = " << idAlbumsList.last();
	}

	QList<Core::AlbumInfoMiniType*> albumList;
	foreach (quint32 idAlbums, idAlbumsList) {
		sqlQuery.prepare("SELECT title, price FROM album WHERE idAlbum=:id");
		sqlQuery.bindValue(":id", idAlbums);
		sqlQuery.exec();

		if (sqlQuery.lastError().isValid()) {
			m_lastError = sqlQuery.lastError().text();
			qDebug() << sqlQuery.lastError().text();;
		}

		Core::AlbumInfoMiniType* albumInfo = new Core::AlbumInfoMiniType();
		albumInfo->deleteLater();
		while (sqlQuery.next()) {
			albumInfo->title = sqlQuery.value("title").toString();
			qDebug() << "title = " << albumInfo->title;

			albumInfo->price = sqlQuery.value("price").toFloat();
			qDebug() << "price = " << albumInfo->price;
		}
		albumList.append(albumInfo);
	}
	return albumList;
}

bool Core::confirmOrder(QList<AlbumInfoMiniType*> m_shoppingCart)
{
	m_database.transaction();
	QSqlQuery sqlQuery(m_database);
	sqlQuery.prepare("SELECT idUsers FROM users WHERE name = :name");
	sqlQuery.bindValue(":name", m_username);
	sqlQuery.exec();

	if (sqlQuery.lastError().isValid()) {
		m_lastError = sqlQuery.lastError().text();
		qDebug() << sqlQuery.lastError().text();
		m_database.rollback();
		return false;
	}

	quint32 idUsers = 0;
	while (sqlQuery.next()) {
		idUsers = sqlQuery.value("idUsers").toUInt();
		qDebug() << "idUsers = " << idUsers;
	}

	foreach (AlbumInfoMiniType* albumInfo, m_shoppingCart) {
		sqlQuery.prepare("SELECT idAlbum FROM album WHERE title = :title");
		sqlQuery.bindValue(":title", albumInfo->title);
		sqlQuery.exec();

		if (sqlQuery.lastError().isValid()) {
			m_lastError = sqlQuery.lastError().text();
			qDebug() << sqlQuery.lastError().text();
			m_database.rollback();
			return false;
		}

		quint32 idAlbum = 0;
		while (sqlQuery.next()) {
			idAlbum = sqlQuery.value("idAlbum").toUInt();
			qDebug() << "idAlbum = " << idAlbum;
		}

		sqlQuery.prepare("INSERT INTO orders (Users_idUsers, Album_idAlbum) VALUES (:user, :album)");
		sqlQuery.bindValue(":user",idUsers);
		sqlQuery.bindValue(":album", idAlbum);
		sqlQuery.exec();

		if (sqlQuery.lastError().isValid()) {
			m_lastError = sqlQuery.lastError().text();
			qDebug() << sqlQuery.lastError().text();
			m_database.rollback();
			return false;
		}
	}

	m_database.commit();
	return true;
}

void Core::saveOrderToXML(QList<Core::AlbumInfoMiniType *> albumList)
{
	QString timestamp = QDateTime::currentDateTime().toString("hh-mm-ss.MM-dd-yyyy");
	QString filename = m_username + "_order." + timestamp + ".xml";
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
	xmlWriter->setDevice(&file);
	xmlWriter->setAutoFormatting(true);
	xmlWriter->writeStartDocument();

	xmlWriter->writeStartElement("musicstore_order");

	xmlWriter->writeStartElement("customer");
	xmlWriter->writeCharacters(m_username);
	xmlWriter->writeEndElement();

	xmlWriter->writeStartElement("timestamp");
	xmlWriter->writeCharacters(timestamp);
	xmlWriter->writeEndElement();

	float cost = 0;
	foreach (Core::AlbumInfoMiniType* album, albumList) {
		xmlWriter->writeStartElement("Album");
		xmlWriter->writeStartElement("title");
		xmlWriter->writeCharacters(album->title);
		xmlWriter->writeEndElement();
		xmlWriter->writeStartElement("price");
		xmlWriter->writeCharacters(QString::number(album->price));
		xmlWriter->writeEndElement();
		xmlWriter->writeEndElement();
		cost += album->price;
	}
	xmlWriter->writeStartElement("total_cost");
	xmlWriter->writeCharacters(QString::number(cost));
	xmlWriter->writeEndElement();

	xmlWriter->writeEndElement();
	xmlWriter->writeEndDocument();
	delete xmlWriter;
	file.close();
}
