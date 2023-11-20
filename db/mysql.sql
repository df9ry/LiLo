SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `##DATABASENAME##` ;
CREATE SCHEMA IF NOT EXISTS `##DATABASENAME##` DEFAULT CHARACTER SET utf8 ;
USE `##DATABASENAME##` ;

-- -----------------------------------------------------
-- Table `##DATABASENAME##`.`informations`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `##DATABASENAME##`.`informations` ;

CREATE  TABLE IF NOT EXISTS `##DATABASENAME##`.`informations` (
  `programname` VARCHAR(50) NOT NULL,
  `programversion` VARCHAR(50) NOT NULL DEFAULT '',
  `dbversion` INTEGER NOT NULL DEFAULT 0 )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

INSERT INTO `##DATABASENAME##`.`informations` SET `ProgramName` = 'LOG4OM2', `ProgramVersion` = '',  `DBVersion`  = 1;

-- -----------------------------------------------------
-- Table `##DATABASENAME##`.`log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `##DATABASENAME##`.`log` ;

CREATE  TABLE IF NOT EXISTS `##DATABASENAME##`.`log` (
     `qsoid`                  BIGINT(17)      NOT NULL ,
     `callsign`               VARCHAR(50)     NOT NULL ,
     `band`                   VARCHAR(10)     NOT NULL,
     `mode`                   VARCHAR(30)     NOT NULL,
     `qsodate`                DATETIME        NOT NULL,
     `address`                VARCHAR(500)    DEFAULT '',
     `arrlsect`               VARCHAR(50)     DEFAULT '',
     `age`                    DECIMAL(18, 3),
     `aindex`                 DECIMAL(18, 3),
     `antaz`                  DECIMAL(18, 3),
     `antel`                  DECIMAL(18, 3),
     `antpath`                VARCHAR(50)     DEFAULT '',
     `antenna`                VARCHAR(100)    DEFAULT '',
     `arrlcheck`              VARCHAR(50)     DEFAULT '',
     `bandrx`                 VARCHAR(10)     DEFAULT '',
     `callsignurl`            VARCHAR(300)    DEFAULT '',
     `class`                  VARCHAR(50)     DEFAULT '',
     `cnty`                   VARCHAR(200)    DEFAULT '',
     `comment`                VARCHAR(500)    DEFAULT '',
     `cont`                   VARCHAR(2)      DEFAULT '',
     `contactassociations`    VARCHAR(500)    DEFAULT '',
     `contactedop`            VARCHAR(50)     DEFAULT '',
     `contactreferences`      JSON                      ,
     `contestid`              VARCHAR(50)     DEFAULT '',
     `country`                VARCHAR(100)    DEFAULT '',
     `cqzone`                 INTEGER,
     `distance`               DECIMAL(18, 3),
     `dxcc`                   INTEGER         NOT NULL DEFAULT 0,
     `eqcall`                 VARCHAR(50),
     `email`                  VARCHAR(100)    DEFAULT '',
     `forceinit`              INTEGER         DEFAULT 0,
     `freq`                   DECIMAL(18, 3)  NOT NULL DEFAULT 0,
     `freqrx`                 DECIMAL(18, 3)  NOT NULL DEFAULT 0,
     `gridsquare`             VARCHAR(10)     DEFAULT '',
     `ituzone`                INTEGER,
     `kindex`                 DECIMAL(18, 3),
     `lat`                    DECIMAL(18, 10),
     `lon`                    DECIMAL(18, 10),
     `maxbursts`              DECIMAL(18, 3),
     `msshower`               VARCHAR(50)     DEFAULT '',
     `myassociations`         VARCHAR(500)    DEFAULT '',
     `mydxcc`                 INTEGER,
     `mylat`                  DECIMAL(18, 10),
     `mylon`                  DECIMAL(18, 10),
     `mycity`                 VARCHAR(100)    DEFAULT '',
     `mycnty`                 VARCHAR(200)    DEFAULT '',
     `mycountry`              VARCHAR(100)    DEFAULT '',
     `mycqzone`               INTEGER,
     `mygridsquare`           VARCHAR(10)     DEFAULT '',
     `myituZone`              INTEGER,
     `myname`                 VARCHAR(300)    DEFAULT '',
     `mypostalcode`           VARCHAR(50)     DEFAULT '',
     `myreferences`           JSON                      ,
     `myrig`                  VARCHAR(300)    DEFAULT '',
     `mysig`                  VARCHAR(300)    DEFAULT '',
     `mysiginfo`              VARCHAR(300)    DEFAULT '',
     `mystate`                VARCHAR(200)    DEFAULT '',
     `mystreet`               VARCHAR(200)    DEFAULT '',
     `name`                   VARCHAR(300)    DEFAULT '',
     `notes`                  VARCHAR(2000)   DEFAULT '',
     `nrbursts`               DECIMAL(18, 3),
     `nrpings`                DECIMAL(18, 3),
     `operator`               VARCHAR(50)     DEFAULT '',
     `ownercallsign`          VARCHAR(50)     DEFAULT '',
     `pfx`                    VARCHAR(50)     DEFAULT '',
     `precedence`             VARCHAR(50)     DEFAULT '',
     `programid`              VARCHAR(50)     DEFAULT '', 
     `programversion`         VARCHAR(50)     DEFAULT '', 
     `propmode`               VARCHAR(50)     DEFAULT '',
     `qslmsg`                 VARCHAR(300)    DEFAULT '',
     `qslvia`                 VARCHAR(300)    DEFAULT '',
     `qsocomplete`            VARCHAR(50)     DEFAULT '',
     `qsoconfirmations`       JSON                      ,
     `qsoenddate`             DATETIME        DEFAULT NULL,
     `qsorandom`              INTEGER         NOT NULL DEFAULT 1,
     `qth`                    VARCHAR(300)    DEFAULT '',
     `rstrcvd`                VARCHAR(10)     DEFAULT '',
     `rstsent`                VARCHAR(10)     DEFAULT '',
     `rxpwr`                  DECIMAL(18, 3),
     `satelliteqso`           INTEGER         DEFAULT 0,
     `satmode`                VARCHAR(50)     DEFAULT '',
     `satname`                VARCHAR(300)    DEFAULT '',
     `sfi`                    DECIMAL(18, 3),
     `sig`                    VARCHAR(300)    DEFAULT '',
     `siginfo`                VARCHAR(300)    DEFAULT '',
     `stationcallsign`        VARCHAR(50)     NOT NULL,
     `srx`                    DECIMAL(18, 3),
     `srxstring`              VARCHAR(50)     DEFAULT '',
     `state`                  VARCHAR(200)    DEFAULT '',
     `stx`                    DECIMAL(18, 3),
     `stxstring`              VARCHAR(50)     DEFAULT '',
     `swl`                    INTEGER         DEFAULT 0,
     `txpwr`                  DECIMAL(18, 3),
  PRIMARY KEY (`mode`,`qsoDate`,`band`,`callsign`),
  UNIQUE KEY `QsoId_UNIQUE` (`qsoid`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ANSI'; 
USE ##DATABASENAME## ; 
DROP PROCEDURE IF EXISTS ##DATABASENAME##.drop_user_if_exists ; 
DELIMITER $$ 
CREATE PROCEDURE ##DATABASENAME##.drop_user_if_exists() 
BEGIN 
  DECLARE foo BIGINT DEFAULT 0 ; 
  SELECT COUNT(*) 
  INTO foo 
    FROM mysql.user 
      WHERE User = '##DATABASENAME##User' and Host = '%'; 
   IF foo > 0 THEN 
         REVOKE ALL PRIVILEGES ON *.* FROM '##DATABASENAME##User'@'%';
         DROP USER '##DATABASENAME##User'@'%' ; 
  END IF; 
  
    SELECT COUNT(*) 
  INTO foo 
    FROM mysql.user 
      WHERE User = '##DATABASENAME##User' and Host = 'localhost';
   IF foo > 0 THEN 
         REVOKE ALL PRIVILEGES ON *.* FROM '##DATABASENAME##User'@'localhost';
         DROP USER '##DATABASENAME##User'@'localhost' ; 
  END IF; 
  
END ;$$ 
DELIMITER ; 
CALL ##DATABASENAME##.drop_user_if_exists() ; 
DROP PROCEDURE IF EXISTS ##DATABASENAME##.drop_users_if_exists ; 
SET SQL_MODE=@OLD_SQL_MODE ;

CREATE USER '##DATABASENAME##User'@'localhost' IDENTIFIED BY '##DATABASENAME##User';

grant INDEX on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User'@'localhost';
grant INSERT on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User'@'localhost';
grant SELECT on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User'@'localhost';
grant UPDATE on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User'@'localhost';
grant DELETE on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User'@'localhost';

grant INDEX on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User'@'localhost';
grant INSERT on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User'@'localhost';
grant SELECT on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User'@'localhost';
grant UPDATE on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User'@'localhost';
grant DELETE on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User'@'localhost';

CREATE USER '##DATABASENAME##User' IDENTIFIED BY '##DATABASENAME##User';

grant INDEX on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User' ;
grant INSERT on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User' ;
grant SELECT on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User' ;
grant UPDATE on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User' ;
grant DELETE on TABLE `##DATABASENAME##`.`informations` to '##DATABASENAME##User' ;

grant INDEX on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User' ;
grant INSERT on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User' ;
grant SELECT on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User' ;
grant UPDATE on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User' ;
grant DELETE on TABLE `##DATABASENAME##`.`log` to '##DATABASENAME##User' ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
