SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE TABLE IF NOT EXISTS `MusicStore`.`Users` (
  `idUsers` INT(11) NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL DEFAULT NULL,
  `password` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`idUsers`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;

CREATE TABLE IF NOT EXISTS `MusicStore`.`Orders` (
  `idOrders` INT(11) NOT NULL AUTO_INCREMENT,
  `numberOfComposition` INT(11) NULL DEFAULT NULL,
  `numberOfAlbums` INT(11) NULL DEFAULT NULL,
  `Users_idUsers` INT(11) NOT NULL,
  `Composition_idComposition` INT(11) NOT NULL,
  `Album_idAlbum` INT(11) NOT NULL,
  PRIMARY KEY (`idOrders`, `Users_idUsers`, `Composition_idComposition`, `Album_idAlbum`),
  INDEX `fk_Orders_Users1_idx` (`Users_idUsers` ASC),
  INDEX `fk_Orders_Composition1_idx` (`Composition_idComposition` ASC),
  INDEX `fk_Orders_Album1_idx` (`Album_idAlbum` ASC),
  CONSTRAINT `fk_Orders_Users1`
    FOREIGN KEY (`Users_idUsers`)
    REFERENCES `MusicStore`.`Users` (`idUsers`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Orders_Composition1`
    FOREIGN KEY (`Composition_idComposition`)
    REFERENCES `MusicStore`.`Composition` (`idComposition`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Orders_Album1`
    FOREIGN KEY (`Album_idAlbum`)
    REFERENCES `MusicStore`.`Album` (`idAlbum`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
