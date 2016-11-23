SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `MusicStore` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `MusicStore` ;

-- -----------------------------------------------------
-- Table `MusicStore`.`Rockband`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Rockband` (
  `idRockband` INT NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(45) NULL,
  `foundationYear` SMALLINT NULL,
  `endOfActivityYear` SMALLINT NULL,
  `isEndOfActivity` TINYINT(1) NULL,
  PRIMARY KEY (`idRockband`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Album`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Album` (
  `idAlbum` INT NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(45) NULL,
  `cover` LONGBLOB NULL,
  `year` SMALLINT NULL,
  `volumeType` VARCHAR(45) NULL,
  `recordType` VARCHAR(45) NULL,
  `price` FLOAT UNSIGNED NULL,
  `Rockband_idRockband` INT NOT NULL,
  PRIMARY KEY (`idAlbum`),
  INDEX `fk_Album_Rockband1_idx` (`Rockband_idRockband` ASC),
  CONSTRAINT `fk_Album_Rockband1`
    FOREIGN KEY (`Rockband_idRockband`)
    REFERENCES `MusicStore`.`Rockband` (`idRockband`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Composition`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Composition` (
  `idComposition` INT NOT NULL AUTO_INCREMENT,
  `number` INT UNSIGNED NULL,
  `title` VARCHAR(45) NULL,
  `genre` VARCHAR(45) NULL,
  `duration` TIME NULL,
  `price` FLOAT NULL,
  `Album_idAlbum` INT NOT NULL,
  PRIMARY KEY (`idComposition`),
  INDEX `fk_Composition_Album1_idx` (`Album_idAlbum` ASC),
  CONSTRAINT `fk_Composition_Album1`
    FOREIGN KEY (`Album_idAlbum`)
    REFERENCES `MusicStore`.`Album` (`idAlbum`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Artist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Artist` (
  `idArtist` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  `instruments` VARCHAR(100) NULL,
  PRIMARY KEY (`idArtist`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`CompositionFile`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`CompositionFile` (
  `idCompositionFile` INT NOT NULL AUTO_INCREMENT,
  `codecType` VARCHAR(45) NULL,
  `bitRate` INT UNSIGNED NULL,
  `fileSize` FLOAT UNSIGNED NULL,
  `Composition_idComposition` INT NOT NULL,
  PRIMARY KEY (`idCompositionFile`, `Composition_idComposition`),
  INDEX `fk_CompositionFile_Composition1_idx` (`Composition_idComposition` ASC),
  CONSTRAINT `fk_CompositionFile_Composition1`
    FOREIGN KEY (`Composition_idComposition`)
    REFERENCES `MusicStore`.`Composition` (`idComposition`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Artist_has_Rockband`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Artist_has_Rockband` (
  `Artist_idArtist` INT NOT NULL,
  `Rockband_idRockband` INT NOT NULL,
  PRIMARY KEY (`Artist_idArtist`, `Rockband_idRockband`),
  INDEX `fk_Artist_has_Rockband_Rockband1_idx` (`Rockband_idRockband` ASC),
  INDEX `fk_Artist_has_Rockband_Artist1_idx` (`Artist_idArtist` ASC),
  CONSTRAINT `fk_Artist_has_Rockband_Artist1`
    FOREIGN KEY (`Artist_idArtist`)
    REFERENCES `MusicStore`.`Artist` (`idArtist`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Artist_has_Rockband_Rockband1`
    FOREIGN KEY (`Rockband_idRockband`)
    REFERENCES `MusicStore`.`Rockband` (`idRockband`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Users` (
  `idUsers` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  `password` VARCHAR(45) NULL,
  PRIMARY KEY (`idUsers`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Orders`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `MusicStore`.`Orders` (
  `idOrders` INT NOT NULL AUTO_INCREMENT,
  `Users_idUsers` INT NOT NULL,
  `Album_idAlbum` INT NOT NULL,
  PRIMARY KEY (`idOrders`, `Users_idUsers`, `Album_idAlbum`),
  INDEX `fk_Orders_Users1_idx` (`Users_idUsers` ASC),
  INDEX `fk_Orders_Album1_idx` (`Album_idAlbum` ASC),
  CONSTRAINT `fk_Orders_Users1`
    FOREIGN KEY (`Users_idUsers`)
    REFERENCES `MusicStore`.`Users` (`idUsers`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Orders_Album1`
    FOREIGN KEY (`Album_idAlbum`)
    REFERENCES `MusicStore`.`Album` (`idAlbum`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

INSERT INTO `rockband` (`idRockband`,`title`,`foundationYear`,`endOfActivityYear`,`isEndOfActivity`) VALUES (1,'Guns N\' Roses',1985,0,0);
INSERT INTO `rockband` (`idRockband`,`title`,`foundationYear`,`endOfActivityYear`,`isEndOfActivity`) VALUES (2,'System Of A Down',1995,0,0);
INSERT INTO `rockband` (`idRockband`,`title`,`foundationYear`,`endOfActivityYear`,`isEndOfActivity`) VALUES (3,'Nirvana',1987,1994,1);
INSERT INTO `rockband` (`idRockband`,`title`,`foundationYear`,`endOfActivityYear`,`isEndOfActivity`) VALUES (4,'Linkin Park',1996,0,0);

INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (1,'Axl Rose','vocals, piano');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (2,'Slash','lead guitar, rhythm guitar, acoustic guitar');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (3,'Izzy Stradlin','rhythm guitar, backing vocals, lead vocals');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (4,'Duff McKagan','bass, backing vocals, acoustic guitar');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (5,'Steven Adler','drums, percussion');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (6,'Kurt Cobain','lead vocals, guitar');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (7,'Krist Novoselic','bass guitar');
INSERT INTO `artist` (`idArtist`,`name`,`instruments`) VALUES (8,'Dave Grohl','drums, backing vocals');

INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (1,1);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (2,1);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (3,1);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (4,1);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (5,1);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (6,3);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (7,3);
INSERT INTO `artist_has_rockband` (`Artist_idArtist`,`Rockband_idRockband`) VALUES (8,3);

INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (1,'Appetite for Destruction',NULL,1987,'LP','Studio Album',14.99,1);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (2,'Bleach',NULL,1989,'LP','Studio Album',12.99,3);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (3,'Chinese Democracy',NULL,2008,'LP','Studio Album',18.99,1);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (4,'Use Your Illusion I',NULL,1991,'LP','Studio Album',14.99,1);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (5,'Nevermind',NULL,1991,'LP','Studio Album',14.99,3);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (6,'In Utero',NULL,1993,'LP','Studio Album',15.99,3);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (7,'System of a Down',NULL,1998,'LP','Studio Album',14.99,2);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (8,'Toxicity',NULL,2001,'LP','Studio Album',14.99,2);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (9,'Hypnotize',NULL,2005,'LP','Studio Album',14.99,2);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (10,'Steal This Album',NULL,2002,'LP','Studio Album',14.99,2);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (11,'Living Things',NULL,2012,'LP','Studio Album',14.99,4);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (12,'Hybrid Theory',NULL,2000,'LP','Studio Album',12.49,4);
INSERT INTO `album` (`idAlbum`,`title`,`cover`,`year`,`volumeType`,`recordType`,`price`,`Rockband_idRockband`) VALUES (13,'Meteora',NULL,2003,'LP','Studio Album',12.49,4);

INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (1,1,'Welcome to the Jungle','Hard Rock','00:04:34',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (2,2,'It\'s So Easy','Hard Rock','00:03:23',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (3,3,'Nightrain','Hard Rock','00:04:29',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (4,4,'Out ta Get Me','Hard Rock','00:04:25',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (5,5,'Mr. Brownstone','Hard Rock','00:03:49',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (6,6,'Paradise City','Hard Rock','00:06:46',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (7,7,'My Michelle','Hard Rock','00:03:40',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (8,8,'Think About You','Hard Rock','00:03:52',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (9,9,'Sweet Child o\' Mine','Hard Rock','00:05:55',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (10,10,'You\'re Crazy','Hard Rock','00:03:17',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (11,11,'Anything Goes','Hard Rock','00:03:26',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (12,12,'Rocket Queen','Hard Rock','00:06:13',1.49,1);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (13,3,'About a Girl','Grunge','00:02:48',1.19,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (14,3,'Come as You Are','Grunge','00:03:38',1.29,5);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (15,1,'Smells Like Teen Spirit','Grunge','00:05:01',1.69,5);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (16,5,'Lithium','Grunge','00:04:16',1.29,5);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (17,2,'In Bloom','Grunge','00:04:14',1.19,5);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (18,4,'Breed','Grunge','00:03:03',1.29,5);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (19,4,'Rape Me','Grunge','00:02:50',1.99,6);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (20,1,'Serve the Servants','Grunge','00:03:36',1.29,6);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (21,2,'Scentless Apprentice','Grunge','00:03:48',1.29,6);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (22,3,'Heart-Shaped Box','Grunge','00:04:41',1.99,6);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (23,1,'Blew','Grunge','00:02:55',1.49,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (24,2,'Floyd the Barber','Grunge','00:02:18',1.49,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (25,3,'About a Girl','Grunge','00:02:48',1.59,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (26,4,'School','Grunge','00:02:42',1.59,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (27,5,'Love Buzz','Grunge','00:03:35',1.29,2);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (28,1,'Suite-Pee','Alternative Metal','00:02:31',1.49,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (29,2,'Know','Alternative Metal','00:02:56',1.49,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (30,3,'Sugar','Alternative Metal','00:02:33',1.59,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (31,4,'Suggestions','Alternative Metal','00:02:44',1.29,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (32,5,'Spiders','Alternative Metal','00:03:35',1.29,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (33,6,'DDevil','Alternative Metal','00:01:43',1.19,7);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (34,1,'Prison Song','Alternative Metal','00:03:21',1.49,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (35,2,'Needles','Alternative Metal','00:03:13',1.49,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (36,3,'Deer Dance','Alternative Metal','00:02:55',1.19,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (37,12,'Toxicity','Alternative Metal','00:03:39',1.99,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (38,13,'Psycho','Alternative Metal','00:03:45',1.99,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (39,14,'Aerials','Alternative Metal','00:06:11',2.49,8);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (40,2,'Innervision','Alternative Metal','00:02:33',1.49,10);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (41,12,'Fuck the System','Alternative Metal','00:02:12',1.49,10);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (42,1,'Attack','Alternative Metal','00:03:06',1.59,9);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (43,3,'Kill Rock \'n Roll','Alternative Metal','00:02:28',1.49,9);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (44,4,'Hypnotize','Alternative Metal','00:03:09',1.49,9);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (45,11,'Lonely Day','Alternative Metal','00:02:47',1.99,9);
INSERT INTO `composition` (`idComposition`,`number`,`title`,`genre`,`duration`,`price`,`Album_idAlbum`) VALUES (46,12,'Soldier Side','Alternative Metal','00:03:40',1.49,9);

INSERT INTO `compositionfile` (`idCompositionFile`,`codecType`,`bitRate`,`fileSize`,`Composition_idComposition`) VALUES (1,'MP3',320,10.47,1);
