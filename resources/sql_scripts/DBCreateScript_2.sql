SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

DROP SCHEMA IF EXISTS `MusicStore` ;
CREATE SCHEMA IF NOT EXISTS `MusicStore` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `MusicStore` ;

-- -----------------------------------------------------
-- Table `MusicStore`.`Rockband`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MusicStore`.`Rockband` ;

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
DROP TABLE IF EXISTS `MusicStore`.`Album` ;

CREATE TABLE IF NOT EXISTS `MusicStore`.`Album` (
  `idAlbum` INT NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(45) NULL,
  `cover` LONGBLOB NULL,
  `year` SMALLINT NULL,
  `volumeType` VARCHAR(45) NULL,
  `recordType` VARCHAR(45) NULL,
  `price` FLOAT UNSIGNED NULL,
  `Rockband_idRockband` INT NOT NULL,
  PRIMARY KEY (`idAlbum`, `Rockband_idRockband`),
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
DROP TABLE IF EXISTS `MusicStore`.`Composition` ;

CREATE TABLE IF NOT EXISTS `MusicStore`.`Composition` (
  `idComposition` INT NOT NULL AUTO_INCREMENT,
  `number` INT UNSIGNED NULL,
  `title` VARCHAR(45) NULL,
  `genre` VARCHAR(45) NULL,
  `duration` TIME NULL,
  `price` FLOAT NULL,
  `Album_idAlbum` INT NOT NULL,
  PRIMARY KEY (`idComposition`, `Album_idAlbum`),
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
DROP TABLE IF EXISTS `MusicStore`.`Artist` ;

CREATE TABLE IF NOT EXISTS `MusicStore`.`Artist` (
  `idArtist` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  `instruments` VARCHAR(100) NULL,
  PRIMARY KEY (`idArtist`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`CompositionFile`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MusicStore`.`CompositionFile` ;

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
DROP TABLE IF EXISTS `MusicStore`.`Artist_has_Rockband` ;

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
DROP TABLE IF EXISTS `MusicStore`.`Users` ;

CREATE TABLE IF NOT EXISTS `MusicStore`.`Users` (
  `idUsers` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  `password` VARCHAR(45) NULL,
  PRIMARY KEY (`idUsers`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MusicStore`.`Orders`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MusicStore`.`Orders` ;

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
