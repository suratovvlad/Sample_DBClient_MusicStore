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

INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('1', '1');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('2', '1');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('3', '1');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('4', '1');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('5', '1');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('6', '3');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('7', '3');
INSERT INTO `musicstore`.`artist_has_rockband` (`Artist_idArtist`, `Rockband_idRockband`) VALUES ('8', '3');
