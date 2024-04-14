-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema agenzia_viaggi
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema agenzia_viaggi
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `agenzia_viaggi` ;
USE `agenzia_viaggi` ;

-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Programma Di Viaggio`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Programma Di Viaggio` (
  `ID Programma` INT NOT NULL AUTO_INCREMENT,
  `Nome Programma` VARCHAR(45) NOT NULL,
  `Numero Giorni` INT NOT NULL,
  PRIMARY KEY (`ID Programma`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Viaggio`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Viaggio` (
  `Data di Partenza` DATE NOT NULL,
  `ID Programma` INT NOT NULL,
  `Data di Rientro` DATE NOT NULL,
  `Costo` DOUBLE NOT NULL,
  `Stato Viaggio` ENUM('in programma', 'attivo', 'terminato', 'prenotabile') NOT NULL DEFAULT 'prenotabile',
  `Num. Partecipanti Confermati` INT NULL,
  PRIMARY KEY (`Data di Partenza`, `ID Programma`),
  INDEX `fk_Viaggio_1_idx` (`ID Programma` ASC) VISIBLE,
  CONSTRAINT `fk_Viaggio_1`
    FOREIGN KEY (`ID Programma`)
    REFERENCES `agenzia_viaggi`.`Programma Di Viaggio` (`ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Località`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Località` (
  `Nome` VARCHAR(45) NOT NULL,
  `Stato` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Nome`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Tappa`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Tappa` (
  `Giorno Arrivo` INT NOT NULL,
  `Ora Arrivo` VARCHAR(45) NOT NULL,
  `ID Programma` INT NOT NULL,
  `Giorno Partenza` INT NOT NULL,
  `Ora Partenza` VARCHAR(45) NOT NULL,
  `Trattamento` ENUM('FB', 'HB', 'BB', 'OB', 'NB') NOT NULL,
  `Nome Località` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Giorno Arrivo`, `Ora Arrivo`, `ID Programma`),
  INDEX `fk_Tappa_1_idx` (`ID Programma` ASC) VISIBLE,
  INDEX `fk_Tappa_2_idx` (`Nome Località` ASC) VISIBLE,
  CONSTRAINT `fk_Tappa_1`
    FOREIGN KEY (`ID Programma`)
    REFERENCES `agenzia_viaggi`.`Programma Di Viaggio` (`ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Tappa_2`
    FOREIGN KEY (`Nome Località`)
    REFERENCES `agenzia_viaggi`.`Località` (`Nome`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Partecipante`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Partecipante` (
  `Codice Fiscale` CHAR(16) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Cognome` VARCHAR(45) NOT NULL,
  `E-mail` VARCHAR(45) NOT NULL,
  `Data Nascita` DATE NOT NULL,
  `Nazionalità` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Codice Fiscale`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Prenotazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Prenotazione` (
  `ID Prenotazione` INT NOT NULL AUTO_INCREMENT,
  `Data` VARCHAR(45) NOT NULL,
  `Codice Utente` VARCHAR(45) NULL,
  `Data di Partenza` DATE NOT NULL,
  `ID Programma` INT NOT NULL,
  PRIMARY KEY (`ID Prenotazione`),
  INDEX `fk_Prenotazione_1_idx` (`Data di Partenza` ASC, `ID Programma` ASC) VISIBLE,
  CONSTRAINT `fk_Prenotazione_1`
    FOREIGN KEY (`Data di Partenza` , `ID Programma`)
    REFERENCES `agenzia_viaggi`.`Viaggio` (`Data di Partenza` , `ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Impegno`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Impegno` (
  `Codice Fiscale Partecipante` CHAR(16) NOT NULL,
  `ID Prenotazione` INT NOT NULL,
  PRIMARY KEY (`Codice Fiscale Partecipante`, `ID Prenotazione`),
  INDEX `fk_Impegno_1_idx` (`ID Prenotazione` ASC) VISIBLE,
  CONSTRAINT `fk_Impegno_1`
    FOREIGN KEY (`ID Prenotazione`)
    REFERENCES `agenzia_viaggi`.`Prenotazione` (`ID Prenotazione`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Impegno_2`
    FOREIGN KEY (`Codice Fiscale Partecipante`)
    REFERENCES `agenzia_viaggi`.`Partecipante` (`Codice Fiscale`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Guida`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Guida` (
  `Codice Fiscale` CHAR(16) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Cognome` VARCHAR(45) NOT NULL,
  `E-mail` VARCHAR(45) NOT NULL,
  `Data Nascita` DATE NOT NULL,
  PRIMARY KEY (`Codice Fiscale`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Accompagno`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Accompagno` (
  `Data di Partenza` DATE NOT NULL,
  `ID Programma` INT NOT NULL,
  `Codice Fiscale Guida` CHAR(16) NOT NULL,
  PRIMARY KEY (`Data di Partenza`, `ID Programma`, `Codice Fiscale Guida`),
  INDEX `fk_Accompagno_2_idx` (`Codice Fiscale Guida` ASC) VISIBLE,
  CONSTRAINT `fk_Accompagno_1`
    FOREIGN KEY (`Data di Partenza` , `ID Programma`)
    REFERENCES `agenzia_viaggi`.`Viaggio` (`Data di Partenza` , `ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Accompagno_2`
    FOREIGN KEY (`Codice Fiscale Guida`)
    REFERENCES `agenzia_viaggi`.`Guida` (`Codice Fiscale`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Albergo`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Albergo` (
  `Indirizzo` VARCHAR(45) NOT NULL,
  `Nome Località` VARCHAR(45) NOT NULL,
  `Costo Per Notte Per Persona` DOUBLE NOT NULL,
  `Referente` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Fax` VARCHAR(45) NOT NULL,
  `E-mail` VARCHAR(45) NOT NULL,
  `Telefono` VARCHAR(45) NOT NULL,
  `Capienza max. persone` INT NOT NULL,
  PRIMARY KEY (`Indirizzo`, `Nome Località`),
  INDEX `fk_Albergo_1_idx` (`Nome Località` ASC) VISIBLE,
  CONSTRAINT `fk_Albergo_1`
    FOREIGN KEY (`Nome Località`)
    REFERENCES `agenzia_viaggi`.`Località` (`Nome`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Pernottamenti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Pernottamenti` (
  `Data di Partenza` DATE NOT NULL,
  `ID Programma` INT NOT NULL,
  `Indirizzo` VARCHAR(45) NOT NULL,
  `Nome Località` VARCHAR(45) NOT NULL,
  `Numero Notti` INT NOT NULL,
  PRIMARY KEY (`Data di Partenza`, `ID Programma`, `Indirizzo`, `Nome Località`),
  INDEX `fk_Pernottamenti_1_idx` (`Data di Partenza` ASC, `ID Programma` ASC) VISIBLE,
  INDEX `fk_Pernottamenti_2_idx` (`Indirizzo` ASC, `Nome Località` ASC) VISIBLE,
  CONSTRAINT `fk_Pernottamenti_1`
    FOREIGN KEY (`Data di Partenza` , `ID Programma`)
    REFERENCES `agenzia_viaggi`.`Viaggio` (`Data di Partenza` , `ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Pernottamenti_2`
    FOREIGN KEY (`Indirizzo` , `Nome Località`)
    REFERENCES `agenzia_viaggi`.`Albergo` (`Indirizzo` , `Nome Località`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Autobus Privato`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Autobus Privato` (
  `Targa` CHAR(7) NOT NULL,
  `Nome Deposito` VARCHAR(45) NOT NULL,
  `Capienza max. persone` INT NOT NULL,
  `Costo giornaliero` DOUBLE NOT NULL,
  PRIMARY KEY (`Targa`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Trasporto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Trasporto` (
  `Data di Partenza` DATE NOT NULL,
  `ID Programma` INT NOT NULL,
  `Targa` CHAR(7) NOT NULL,
  PRIMARY KEY (`Data di Partenza`, `ID Programma`, `Targa`),
  INDEX `fk_Trasporto_1_idx` (`Targa` ASC) VISIBLE,
  CONSTRAINT `fk_Trasporto_1`
    FOREIGN KEY (`Targa`)
    REFERENCES `agenzia_viaggi`.`Autobus Privato` (`Targa`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Trasporto_2`
    FOREIGN KEY (`Data di Partenza` , `ID Programma`)
    REFERENCES `agenzia_viaggi`.`Viaggio` (`Data di Partenza` , `ID Programma`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `agenzia_viaggi`.`Segretari`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `agenzia_viaggi`.`Segretari` (
  `username` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`username`))
ENGINE = InnoDB;

USE `agenzia_viaggi` ;

-- -----------------------------------------------------
-- procedure login_segretario
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `login_segretario` (in var_username varchar(45), in var_pass varchar(45), out var_role INT)
BEGIN
SELECT IF( EXISTS(
             SELECT *
             FROM Segretari
             WHERE `username` = var_username and `password` = md5(var_pass)), 1, 0) into var_role;
         #return 1 if the user exist, else 0
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_localita
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_localita` (IN var_nome varchar(45),IN var_stato varchar(45))
BEGIN
INSERT INTO `agenzia_viaggi`.`Località` (`Nome`, `Stato`) 
VALUES (var_nome, var_stato);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_guida
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_guida` (IN var_cf char(16),IN var_nome varchar(45),IN var_cognome char(45),IN var_email varchar(45),IN var_data date)
BEGIN
INSERT INTO `agenzia_viaggi`.`Guida` (`Codice Fiscale`, `Nome`, `Cognome`, `E-mail`, `Data Nascita`) 
VALUES (var_cf, var_nome, var_cognome, var_email, var_data);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_albergo
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_albergo` (IN var_ind varchar(45),IN var_loc varchar(45),IN var_cpn DOUBLE,IN var_ref varchar(45),IN var_nome varchar(45),IN var_fax varchar(45),IN var_email varchar(45),IN var_tel varchar(45),IN var_capmax int)
BEGIN
insert into `agenzia_viaggi`.`Albergo` (`Indirizzo`, `Nome Località`, `Costo Per Notte Per Persona`, `Referente`, `Nome`, `Fax`, `E-mail`, `Telefono`, `Capienza max. persone`) 
values (var_ind, var_loc, var_cpn, var_ref, var_nome, var_fax, var_email, var_tel, var_capmax);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_tappa
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_tappa` (in var_giorno_arrivo int, in var_ora_arrivo time, in var_id_programma int, in var_giorno_partenza int, in var_ora_partenza time, in var_trattamento enum('FB', 'HB', 'BB', 'OB', 'NB'), in var_nome_località varchar(45))
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    start transaction;
INSERT INTO `agenzia_viaggi`.`Tappa` (`Giorno Arrivo`, `Ora Arrivo`, `ID Programma`, `Giorno Partenza`, `Ora Partenza`, `Trattamento`, `Nome Località`) 
VALUES (var_giorno_arrivo, var_ora_arrivo, var_id_programma, var_giorno_partenza, var_ora_partenza, var_trattamento, var_nome_località);
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_programma_di_viaggio
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_programma_di_viaggio` (in var_nome varchar(45),in var_numero_giorni int, out id_programma int)
BEGIN
INSERT INTO `agenzia_viaggi`.`Programma Di Viaggio` (`Nome Programma`, `Numero Giorni`) 
VALUES (var_nome,var_numero_giorni);
set id_programma=last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_autobus_privato
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_autobus_privato` (in var_targa char(7), in var_deposito char(45) , in var_capmax int,in  var_costo_g double)
BEGIN
INSERT INTO `agenzia_viaggi`.`Autobus Privato` (`Targa`, `Nome Deposito`, `Capienza max. persone`, `Costo giornaliero`) 
VALUES (var_targa, var_deposito,var_capmax, var_costo_g);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_viaggio
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_viaggio`(in var_data_p date, in var_id_programma int, in var_data_r date, in var_costo double)
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    start transaction;
INSERT INTO `agenzia_viaggi`.`Viaggio` (`Data di Partenza`, `ID Programma`, `Data di Rientro`, `Costo`) 
VALUES (var_data_p, var_id_programma, var_data_r, var_costo);
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_programmi_di_viaggio_cliente
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_programmi_di_viaggio_cliente` ()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    -- anche se il costo minimo potrebbe non essere corretto si è deciso di non appesantire questa procedura con un livello di isolamento più stretto
    -- anche tenendo in considerazione la sua frequenza elevata
	set transaction read only;
    start transaction;
	select `Nome Programma`,`Numero Giorni`,p.`ID Programma`, `Costo`
    from `Programma Di Viaggio` as p
    join `Viaggio` on p.`ID Programma`=`Viaggio`.`ID Programma`
    WHERE `Stato Viaggio`='prenotabile' AND
    Costo <= all( select Costo 
						from Viaggio
						where `ID Programma`=p.`ID Programma`);
    commit;                    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_viaggi_input_stato
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_viaggi_input_stato` (in var_stato ENUM('in programma', 'attivo', 'terminato', 'prenotabile'))
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
 SELECT `Data di Partenza`,`Data di Rientro`,`Costo`,`Num. Partecipanti Confermati`,`ID Programma` FROM `Viaggio`
 WHERE `Stato Viaggio`=var_stato;
 commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_itinerario_input_id
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `get_itinerario_input_id` (in var_id_programma int)
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    start transaction;
 SELECT `Nome Località`,`Giorno Arrivo`,`Ora Arrivo`,`Giorno Partenza`,`Ora Partenza`,`Trattamento` FROM `Tappa` 
 WHERE `ID Programma`=var_id_programma  ORDER BY `Giorno Arrivo`,`Ora Arrivo`;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_programmi_viaggi_segretari
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_programmi_viaggi_segretari` ()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select `Nome Programma`,`Numero Giorni`,`ID Programma` from `Programma Di Viaggio`;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_guida
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `assegna_guida` (in var_cf CHAR(16),in id_programma_in int,in data_partenza_in date)
BEGIN   
declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
set transaction isolation level serializable;
start transaction;
	INSERT INTO `agenzia_viaggi`.`Accompagno` (`Data di Partenza`, `ID Programma`, `Codice Fiscale Guida`) 
	VALUES (data_partenza_in, id_programma_in, var_cf);
commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_autobus_privato
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `assegna_autobus_privato` (in id_programma_in int,in data_partenza_in date,in targa char(7))
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level serializable;
    start transaction;
 INSERT INTO `agenzia_viaggi`.`Trasporto` (`Data di Partenza`, `ID Programma`, `Targa`) 
 VALUES (data_partenza_in, id_programma_in, targa);
 commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_albergo
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `assegna_albergo` (in id_programma_in int,in data_partenza_in date,in indirizzo varchar(45),in nome_localita varchar(45),in numero_notti int)
BEGIN    
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level serializable;
    start transaction;
INSERT INTO `agenzia_viaggi`.`Pernottamenti` (`Data di Partenza`, `ID Programma`, `Indirizzo`, `Nome Località`, `Numero Notti`) 
VALUES (data_partenza_in, id_programma_in, indirizzo, nome_localita, numero_notti);
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_assegnamenti_guide
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_assegnamenti_guide` ()
BEGIN
declare cur_date date;
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;
    
    select curdate()into cur_date;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select `Codice Fiscale`,`Viaggio`.`Data di Partenza`,`Data di Rientro`
    from Guida 
    join Accompagno on `Codice Fiscale`=`Codice Fiscale Guida`
    join Viaggio on (Accompagno.`Data di Partenza`=Viaggio.`Data di Partenza` and Accompagno.`ID Programma`=Viaggio.`ID Programma`)
    where `Viaggio`.`Data di Partenza` >= cur_date;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_guide_all
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_guide_all`()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select *
    from Guida ;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_assegnamenti_autobus
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_assegnamenti_autobus` ()
BEGIN
declare cur_date date;
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;
    
    select curdate()into cur_date;
    
    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select Trasporto.`Targa`,`Viaggio`.`Data di Partenza`,`Data di Rientro`
    from `Autobus Privato` 
    join Trasporto on Trasporto.`Targa`=`Autobus Privato`.`Targa`
    join Viaggio on (Trasporto.`Data di Partenza`=Viaggio.`Data di Partenza` and Trasporto.`ID Programma`=Viaggio.`ID Programma`)
    where `Viaggio`.`Data di Partenza`>= cur_date;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_autobus_all
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_autobus_all`()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select *
    from `agenzia_viaggi`.`Autobus Privato` ;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_alberghi_all
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_alberghi_all`()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
	select *
    from Albergo ;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_prenotazione` (in data_p date, in id_programma int,in num_persone int,out codice varchar(45),out newid int,out costo_tot_prenotazione double)
BEGIN

    declare costo_viaggio double;
	declare dataprenotazione date;
    declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;
    
    select left(UUID(), 8) into codice; -- 8 character pseudo-random string 
    
    select curdate() into dataprenotazione;

    
	set transaction isolation level read committed;
    start transaction;
    
    insert into `agenzia_viaggi`.`Prenotazione`(`Data di Partenza`,`ID Programma`,`Codice Utente`,`Data`)
    values (data_p,id_programma,md5(codice),dataprenotazione);
    
    select Costo 
    from Viaggio 
    where `ID Programma`=id_programma AND `Data di Partenza`=data_p into  costo_viaggio;
    
    commit;
    set newid=last_insert_id();
    set costo_tot_prenotazione=costo_viaggio*num_persone;
END;$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_partecipante
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `aggiungi_partecipante` (in id_prenotazione int, IN var_cf char(16),IN var_nome varchar(45),IN var_cognome char(45),IN var_email varchar(45),IN var_data_n date,IN nazionalita varchar(45))
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	start transaction;
IF EXISTS( SELECT *
		   FROM Partecipante  
		   WHERE `Codice Fiscale`= var_cf) THEN 
	update Partecipante set `Nome`=var_nome,`Cognome`=var_cognome,`E-mail`=var_email,`Data Nascita`=var_data_n,`Nazionalità`=nazionalita
    where `Codice Fiscale`= var_cf;
ELSE 
    insert into Partecipante VALUES (var_cf,var_nome,var_cognome,var_email,var_data_n,nazionalita);
END IF;
            
insert into Impegno VALUES (var_cf,id_prenotazione);
commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure disdici_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `disdici_prenotazione` (in id_prenotazione int,in codice varchar(45))
BEGIN
declare supp int;
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    start transaction;
SELECT IF( EXISTS(
             SELECT *
             FROM Prenotazione  
             WHERE `ID Prenotazione`= id_prenotazione and `Codice Utente`=md5(codice)
             ), 1, 0) into supp;
             
IF (supp=0) THEN 
	signal sqlstate '45001' set message_text="Dati inserti non validi. Prenotazione non disdicibile o inesistente";
END IF;

    
DELETE FROM Impegno WHERE `ID Prenotazione`=id_prenotazione;
DELETE FROM Prenotazione WHERE `ID Prenotazione`=id_prenotazione;
	commit;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_prenotazione_info
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `get_prenotazione_info` (in id_prenotazione int, in codice varchar(45))
BEGIN
declare supp int;

declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
SELECT IF(EXISTS(
             SELECT *
             FROM Prenotazione  
             WHERE `ID Prenotazione`= id_prenotazione and `Codice Utente`=md5(codice)
             ), 1, 0) into supp;
             
IF (supp=0) THEN 
	signal sqlstate '45001' set message_text="Dati inserti non validi. Prenotazione non disdicibile o inesistente";
END IF;
select `Nome`,`Cognome`
from Impegno 
join Partecipante on `Codice Fiscale`=`Codice Fiscale Partecipante`
where Impegno.`ID Prenotazione` = id_prenotazione;

select `Data`,`Nome Programma`,Viaggio.`Data di Partenza`,`Costo`
from Prenotazione 
join `Programma Di Viaggio` on Prenotazione.`ID Programma`=`Programma Di Viaggio`.`ID Programma`
join Viaggio on Viaggio.`ID Programma`=`Programma Di Viaggio`.`ID Programma`
where `ID Prenotazione` = id_prenotazione;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure report_viaggio
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `report_viaggio` (in id_programma int,in data_p date)
BEGIN
declare num_partecipanti int;
declare num_giorni int;
declare costo_viaggio_per_persona double;
declare costo_autobus_giornaliero_tot double;
declare costo_albergo_per_persona_tot double;
declare profitto double;
declare stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;
drop temporary table if exists `ListaCosti`;
	create temporary table `ListaCosti` (
		`Costo Per Persona` double,
		`Costo Totale Autobus` double,
		`Costo Totale Alberghi` double,
		`Profitto` double,
        `Num Partecipanti` int
	);
    
    set transaction isolation level read committed;
    start transaction;
    -- si osservi che il report può essere generato solo per viaggi 'terminati', a questi non è possibile l'assegnazione di alberghi/auotbus privati
    -- ne è possibile che avvenga  alcuna prenotazione. Di conseguenza non esistono problemi di phantom
    
select `Stato Viaggio`
from Viaggio
where `ID Programma`=id_programma and `Data di Partenza`= data_p into stato_viaggio; 
    
if(stato_viaggio<>'terminato')then 
	signal sqlstate '45001' set message_text="Un report può essere generato solo per viaggi terminati.";
end if; 

select `Nome`,`Cognome`,`Data Nascita`,`E-mail`,`Nazionalità`
from Partecipante
join Impegno on `Codice Fiscale`=`Codice Fiscale Partecipante`
join Prenotazione on Impegno.`ID Prenotazione`=Prenotazione.`ID Prenotazione`
join Viaggio on  Viaggio.`ID Programma`=Prenotazione.`ID Programma` AND Viaggio.`Data di Partenza`=Prenotazione.`Data di Partenza`
where Viaggio.`ID Programma`=id_programma and Viaggio.`Data di Partenza`=data_p;
        
    
select Costo 
from Viaggio
where`ID Programma`=id_programma and `Data di Partenza`=data_p 
into costo_viaggio_per_persona;

select  `Num. Partecipanti Confermati` 
from Viaggio
where `ID Programma`=id_programma and `Data di Partenza`= data_p
into num_partecipanti;

select `Numero Giorni`
from Viaggio
join `Programma Di Viaggio` on Viaggio.`ID Programma`=`Programma Di Viaggio`.`ID Programma` 
where Viaggio.`ID Programma`=id_programma and Viaggio.`Data di Partenza`=data_p
into num_giorni;

select sum(`Costo Giornaliero`) 
from Trasporto
join `Autobus Privato` on Trasporto.`Targa`=`Autobus Privato`.`Targa`
where `ID Programma`=id_programma and `Data di Partenza`=data_p
into costo_autobus_giornaliero_tot;

select sum(tot)
from(
select `Costo Per Notte Per Persona`*`Numero Notti` as tot
from Pernottamenti
join Albergo on Pernottamenti.`Indirizzo`=Albergo.`Indirizzo` and  Pernottamenti.`Nome Località`=Albergo.`Nome Località`
where `ID Programma`=id_programma and `Data di Partenza`=data_p)src 
into costo_albergo_per_persona_tot;

set profitto=(num_partecipanti*costo_viaggio_per_persona)-(costo_albergo_per_persona_tot*num_partecipanti)-(costo_autobus_giornaliero_tot*num_giorni);

insert into `ListaCosti` VALUES (costo_viaggio_per_persona,costo_autobus_giornaliero_tot*num_giorni,costo_albergo_per_persona_tot*num_partecipanti,profitto,num_partecipanti);
select * from `ListaCosti`;

	commit;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_viaggi_prenotabili_input_id
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_viaggi_prenotabili_input_id` (in var_id_programma int)
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
 SELECT `Data di Partenza`,`Data di Rientro`,`Costo` FROM `Viaggio`
 WHERE `ID Programma`=var_id_programma AND (`Stato Viaggio`='prenotabile');
 commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure lista_localita_all
-- -----------------------------------------------------

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE PROCEDURE `lista_localita_all` ()
BEGIN
declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;

    set transaction isolation level read committed;
	set transaction read only;
    start transaction;
 select *
 from `Località`
 order by `Stato`;
 commit;
END$$

DELIMITER ;
CREATE USER 'loginTA' IDENTIFIED BY 'login';

GRANT EXECUTE ON procedure `agenzia_viaggi`.`login_segretario` TO 'loginTA';
CREATE USER 'segretarioTA' IDENTIFIED BY 'segretario';

GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_localita` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_guida` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_albergo` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_programma_di_viaggio` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_tappa` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_autobus_privato` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_viaggio` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_programmi_di_viaggio_cliente` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_programmi_viaggi_segretari` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`assegna_guida` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_viaggi_input_stato` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`assegna_albergo` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`assegna_autobus_privato` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_assegnamenti_guide` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_guide_all` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_autobus_all` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_assegnamenti_autobus` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_alberghi_all` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`report_viaggio` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`get_itinerario_input_id` TO 'segretarioTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_localita_all` TO 'segretarioTA';
CREATE USER 'clienteTA' IDENTIFIED BY 'cliente';

GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_programmi_di_viaggio_cliente` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_viaggi_input_stato` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`get_itinerario_input_id` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_prenotazione` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`aggiungi_partecipante` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`disdici_prenotazione` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`get_prenotazione_info` TO 'clienteTA';
GRANT EXECUTE ON procedure `agenzia_viaggi`.`lista_viaggi_prenotabili_input_id` TO 'clienteTA';

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
USE `agenzia_viaggi`;

DELIMITER $$
USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Programma Di Viaggio_BEFORE_INSERT` BEFORE INSERT ON `Programma Di Viaggio` FOR EACH ROW
BEGIN
if (NEW.`Numero Giorni`<1 or NEW.`Numero Giorni`>7)  THEN 
	signal sqlstate '45001' set message_text="Non è stato inserito un numero di giorni valido";
END IF;
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Viaggio_BEFORE_INSERT` BEFORE INSERT ON `Viaggio` FOR EACH ROW
BEGIN
	declare num_giorni int;
    declare num_giorni_input int;
    declare cur_date date;
    declare supp int;
    select curdate() into cur_date;
    
    SELECT IF(NEW.`Data di Partenza`> DATE(DATE_ADD(cur_date, INTERVAL 20 DAY)), 1, 0) into supp;
    
    IF(supp=0)THEN 
		signal sqlstate '45001' set message_text="Per favore, inserisci una data di partenza valida, deve essere almeno successiva di 20 giorni alla data odierna ";
	END IF;
    
    select `Numero Giorni`
    from `Programma Di Viaggio`
    where `ID Programma`=NEW.`ID Programma` into num_giorni;
    
    SELECT DATEDIFF(NEW.`Data di Rientro`,NEW.`Data di Partenza`) into num_giorni_input;
    
	IF(num_giorni_input<>num_giorni)THEN 
		signal sqlstate '45001' set message_text="Date di partenza e rientro non compatibili con il numero di giorni del viaggio";
	END IF;
    
    
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Località_BEFORE_INSERT` BEFORE INSERT ON `Località` FOR EACH ROW
BEGIN
declare supp  int;
	SELECT IF( NEW.`Stato` in 
    ('Italia','Albania','Andorra','Armenia','Austria','Azerbaigian','Bielorussia','Belgio','Bosnia-Erzegovina','Bulgaria','Cipro','Croazia','Danimarca','Estonia','Ex-Repubblica jugoslava di Macedonia','Finlandia','Francia','Georgia','Germania','Grecia','Irlanda','Islanda','Lettonia','Liechtenstein','Lituania','Lussemburgo','Malta','Moldavia','Monaco','Montenegro','Norvegia','Paesi Bassi','Polonia','Portogallo','Regno Unito','Repubblica ceca','Romania','Federazione di Russia','San Marino','Serbia','Slovacchia','Slovenia','Spagna','Svezia','Svizzera','Turchia','Ucraina','Ungheria' )
			, 1, 0) into supp;
IF (supp=0) THEN 
	signal sqlstate '45001' set message_text="Inserire per favore una località italiana o europea";
END IF;
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Tappa_BEFORE_INSERT` BEFORE INSERT ON `Tappa` FOR EACH ROW
BEGIN
declare num_giorni int;
IF(NEW.`Giorno Partenza`< NEW.`Giorno Arrivo`) THEN 
	signal sqlstate '45001' set message_text="Giorno di Arrivo e Giorno di Partenza inseriti non sono compatibili";
END IF;
IF(NEW.`Giorno Arrivo`< NEW.`Giorno Partenza` AND NEW.`Trattamento`='NB') THEN 
	signal sqlstate '45001' set message_text="La tappa prevede un pernottamento ma non è stato specificato un trattamento valido.";
END IF;
IF(NEW.`Giorno Arrivo`= NEW.`Giorno Partenza` AND NEW.`Trattamento`<>'NB') THEN 
	signal sqlstate '45001' set message_text="La tappa non prevede un pernottamento, non è quindi corretto specificare un trattamento.";
END IF;
select `Numero Giorni`
from `Programma Di Viaggio`
where `ID Programma`=NEW.`ID Programma` into num_giorni;

IF(NEW.`Giorno Arrivo`>num_giorni)  THEN 
	signal sqlstate '45001' set message_text="Giorno di arrivo non valido, supera il numero  di giorni previsti dal piano di viaggio";
END IF;   
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Partecipante_BEFORE_INSERT` BEFORE INSERT ON `Partecipante` FOR EACH ROW
BEGIN
if (NEW.`Codice Fiscale` not regexp'^[A-Z]{6}[0-9]{2}[A-Z][0-9]{2}[A-Z][0-9]{3}[A-Z]$') then
signal sqlstate '45001' set message_text = 'Formato codice fiscale errato';
end if;
if (NEW.`E-mail` not regexp'^[a-zA-Z0-9][a-zA-Z0-9._-]*@[a-zA-Z0-9][a-zA-Z0-9._-]*\\.[a-zA-Z]{2,4}$') then
signal sqlstate '45001' set message_text = 'Formato email  errato';
end if;
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Prenotazione_BEFORE_INSERT` BEFORE INSERT ON `Prenotazione` FOR EACH ROW
BEGIN
	
declare stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');
    
select `Stato Viaggio`
from Viaggio
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` into stato_viaggio; 
    
if(stato_viaggio<>'prenotabile')then 
	signal sqlstate '45001' set message_text="Non è più possibile prenotarsi a questo viaggio.";
end if; 

END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Impegno_BEFORE_INSERT` BEFORE INSERT ON `Impegno` FOR EACH ROW
BEGIN
declare supp int;
SELECT IF(NEW.`ID Prenotazione`in(
             SELECT `ID Prenotazione`
             FROM Prenotazione  
             WHERE `Codice Utente` is not null ), 1, 0) into supp;
if(supp=0)then 
	signal sqlstate '45001' set message_text="Mi dispiace, le prenotazioni per questo viaggio sono terminate.";
end if; 
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Guida_BEFORE_INSERT` BEFORE INSERT ON `Guida` FOR EACH ROW
BEGIN
if (NEW.`Codice Fiscale` not regexp'^[A-Z]{6}[0-9]{2}[A-Z][0-9]{2}[A-Z][0-9]{3}[A-Z]$') then
signal sqlstate '45001' set message_text = 'Formato codice fiscale errato';
end if;
if (NEW.`E-mail` not regexp'^[a-zA-Z0-9][a-zA-Z0-9._-]*@[a-zA-Z0-9][a-zA-Z0-9._-]*\\.[a-zA-Z]{2,4}$') then
signal sqlstate '45001' set message_text = 'Formato email  errato';
end if;
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Accompagno_BEFORE_INSERT` BEFORE INSERT ON `Accompagno` FOR EACH ROW
BEGIN
DECLARE done INT DEFAULT FALSE;
DECLARE data_rientro_in DATE;
DECLARE viaggio_id INT;
DECLARE viaggio_date DATE;
DECLARE data_rientro DATE;
declare assegnamento_guida int;
declare stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');

DECLARE cur CURSOR FOR
	SELECT `ID Programma`,`Data di Partenza` FROM `Accompagno` 
	WHERE `Codice Fiscale Guida` = NEW.`Codice Fiscale Guida`;
DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
SELECT `Data di Rientro` FROM `Viaggio` WHERE `Data di Partenza`=NEW.`Data di Partenza` AND `ID Programma`=NEW.`ID Programma` INTO data_rientro_in;

select `Stato Viaggio`
from Viaggio
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` into stato_viaggio; 
    
if(stato_viaggio<>'in programma')then 
	signal sqlstate '45001' set message_text="Lo stato attuale del viaggio non permette l assegnamento";
end if; 

select count(*)
	from `Accompagno` 
	where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` into assegnamento_guida; 
    
if(assegnamento_guida<>0)then 
  	signal sqlstate '45001' set message_text="Il viaggio selezionato ha già  una guida assegnata";
end if;

OPEN cur;
	read_loop: loop
		FETCH cur INTO viaggio_id,viaggio_date;
        IF done THEN
			LEAVE read_loop;
		END IF;
        SELECT `Data di Rientro` FROM `Viaggio` WHERE `Data di Partenza`=viaggio_date AND `ID Programma`=viaggio_id INTO data_rientro;
        IF((NEW.`Data di Partenza` BETWEEN viaggio_date AND data_rientro) OR (data_rientro_in BETWEEN viaggio_date AND data_rientro))THEN 
			signal sqlstate '45001' set message_text="La guida è stata già assegnata in questo periodo";
		END IF;
	END LOOP;
	CLOSE cur;
END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Pernottamenti_BEFORE_INSERT` BEFORE INSERT ON `Pernottamenti` FOR EACH ROW
BEGIN
declare stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');
declare count_pernottamenti int;
declare num_notti int; 
declare assegnamento_albergo int;
declare capienza_max int;
declare num_partecipanti int;

select `Stato Viaggio`
	from Viaggio
    where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` into stato_viaggio; 
    
if(stato_viaggio<>'in programma')then 
	signal sqlstate '45001' set message_text="Lo stato attuale del viaggio non permette l'assegnamento";
end if;  

select count(*)
	from Tappa
	where `ID Programma`=NEW.`ID Programma` AND  `Nome Località`=NEW.`Nome Località` AND `Trattamento`<>'NB' 
into count_pernottamenti;

if not(count_pernottamenti>0) then 
	signal sqlstate '45001' set message_text="Non esiste una tappa con pernottamento in questa località per questo viaggio";
	END IF;
    
select `Tappa`.`Giorno Partenza`-`Tappa`.`Giorno Arrivo`
	from Tappa
	where `ID Programma`=NEW.`ID Programma` AND  `Nome Località`=NEW.`Nome Località` AND `Trattamento`<>'NB' 
into num_notti;
    
if (num_notti<>NEW.`Numero Notti`) then 
	signal sqlstate '45001' set message_text="Non è stato inserito un numero di notti valido per questa tappa";
	END IF;
    
select count(*)
from `Pernottamenti` 
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` and `Nome Località`=NEW.`Nome Località`
into assegnamento_albergo; 
 
if (assegnamento_albergo<>0) then 
	signal sqlstate '45001' set message_text="Un albergo è già  stato assegnato per questa tappa";
	END IF;

select  `Capienza max. persone` 
from Albergo
where `Nome Località`=NEW.`Nome Località` and `Indirizzo`=NEW.`Indirizzo`
into capienza_max;

select  `Num. Partecipanti Confermati` 
from Viaggio
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` 
into num_partecipanti;

if(num_partecipanti>capienza_max) then
	signal sqlstate '45001' set message_text="L albergo scelto non dispone di una capienza sufficente ad ospitare tutti i partecipanti al viaggio";
	END IF;

END$$

USE `agenzia_viaggi`$$
CREATE DEFINER = CURRENT_USER TRIGGER `agenzia_viaggi`.`Trasporto_BEFORE_INSERT` BEFORE INSERT ON `Trasporto` FOR EACH ROW
BEGIN
DECLARE done INT DEFAULT FALSE;
DECLARE data_rientro_in DATE;
DECLARE viaggio_id INT;
DECLARE viaggio_date DATE;
DECLARE data_rientro DATE;
DECLARE capienza_attuale int;
DECLARE num_partecipanti int;
DECLARE stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');

DECLARE cur CURSOR FOR
	SELECT `ID Programma`,`Data di Partenza` FROM `Trasporto` 
	WHERE `Targa` = NEW.`Targa`;
DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

SELECT `Stato Viaggio`
FROM Viaggio
WHERE `ID Programma`=NEW.`ID Programma` AND `Data di Partenza`= NEW.`Data di Partenza` 
INTO stato_viaggio; 
    
IF(stato_viaggio<>'in programma')THEN 
	signal sqlstate '45001' set message_text="Lo stato attuale del viaggio non permette l assegnamento";
END IF; 
    
SELECT `Data di Rientro` 
FROM `Viaggio` 
WHERE `Data di Partenza`=NEW.`Data di Partenza` AND `ID Programma`=NEW.`ID Programma` 
INTO data_rientro_in;

OPEN cur;
	read_loop: loop
		FETCH cur INTO viaggio_id,viaggio_date;
        IF done THEN
			LEAVE read_loop;
		END IF;
        SELECT `Data di Rientro` FROM `Viaggio` WHERE `Data di Partenza`=viaggio_date AND `ID Programma`=viaggio_id INTO data_rientro;
        IF((NEW.`Data di Partenza` BETWEEN viaggio_date AND data_rientro) OR (data_rientro_in BETWEEN viaggio_date AND data_rientro))THEN 
			signal sqlstate '45001' set message_text="L autobus privato è stato già assegnato in questo periodo";
		END IF;
	END LOOP;
	CLOSE cur;

select  `Num. Partecipanti Confermati` 
from Viaggio
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` 
into num_partecipanti;

select sum(`Capienza max. persone`)
from Trasporto
join `Autobus Privato` on Trasporto.Targa=`Autobus Privato`.Targa
where `ID Programma`=NEW.`ID Programma` and `Data di Partenza`= NEW.`Data di Partenza` 
into capienza_attuale;

IF(capienza_attuale >num_partecipanti)THEN 
	signal sqlstate '45001' set message_text="Non sono necessari ulteriori autobus privati per il viaggio selezionato";
END IF; 

END$$
DELIMITER ;
set global event_scheduler = on;
delimiter |
CREATE EVENT IF NOT EXISTS aggiorna_stato
  ON SCHEDULE EVERY 1 DAY STARTS (TIMESTAMP(CURRENT_DATE) + INTERVAL 1 DAY )
  ON COMPLETION PRESERVE
  DO 
   BEGIN
   	DECLARE data_odierna DATE;
    DECLARE done INT DEFAULT FALSE;
   	DECLARE viaggio_id INT;
	DECLARE viaggio_partenza DATE;
   	DECLARE viaggio_rientro DATE;
   	DECLARE num_partecipanti int;
    declare stato_viaggio ENUM('in programma', 'attivo', 'terminato', 'prenotabile');
   	DECLARE cur CURSOR FOR SELECT `ID Programma`,`Data di Partenza`,`Data di Rientro`,`Stato Viaggio` FROM `Viaggio`; 
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
    declare exit handler for sqlexception
    begin
        rollback;  
        resignal; 
    end;
    
	SELECT curdate() into data_odierna;

    set transaction isolation level serializable;
    start transaction;	
    
OPEN cur;
	read_loop: loop
		FETCH cur INTO viaggio_id,viaggio_partenza,viaggio_rientro,stato_viaggio;
        IF done THEN
		LEAVE read_loop;
	END IF;
	
	IF((data_odierna BETWEEN viaggio_partenza AND viaggio_rientro ) AND stato_viaggio <>'attivo')THEN 
        	update Viaggio set `Stato Viaggio`='attivo' 
   		where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza; 
	END IF;
	
	IF(data_odierna > viaggio_rientro AND stato_viaggio <>'terminato')THEN 
        	update Viaggio set `Stato Viaggio`='terminato' 
   		where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza ; 
	END IF;
	
	IF((DATE_ADD(data_odierna, INTERVAL 20 DAY) >= viaggio_partenza) AND data_odierna<viaggio_partenza AND stato_viaggio<>'in programma')THEN 
        	select count(`Codice Fiscale Partecipante`) 
			from  Impegno 
			join Prenotazione on Impegno.`ID Prenotazione`=Prenotazione.`ID Prenotazione`
			join Viaggio on  Viaggio.`ID Programma`=Prenotazione.`ID Programma` AND Viaggio.`Data di Partenza`=Prenotazione.`Data di Partenza`
			where Viaggio.`ID Programma`=viaggio_id and Viaggio.`Data di Partenza`=viaggio_partenza 
		into num_partecipanti;
		
        update Viaggio set `Stato Viaggio`='in programma',`Num. Partecipanti Confermati`=num_partecipanti
       		where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza;
       		
       	update Prenotazione set `Codice Utente`= NULL 	
			where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza;
       	
	END IF;
    
	IF(DATE_ADD( viaggio_rientro, INTERVAL 1 YEAR) <= data_odierna)THEN 
		DELETE FROM Trasporto where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza ;
		DELETE FROM Accompagno where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza ;
        DELETE FROM Pernottamenti where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza ;
        DELETE FROM Impegno where `ID Prenotazione` in ( select `ID Prenotazione` 
														  from Prenotazione
                                                          where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza);
        DELETE FROM Prenotazione  where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza;   
        DELETE FROM Viaggio  where `ID Programma`=viaggio_id AND `Data di Partenza`=viaggio_partenza;   
	END IF;
	
	END LOOP;
    
    commit;
    
CLOSE cur;
   END |
delimiter ;
