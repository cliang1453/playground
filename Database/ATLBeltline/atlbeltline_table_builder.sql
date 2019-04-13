-- MySQL dump 10.13  Distrib 8.0.13, for Win64 (x86_64)
--
-- Host: localhost    Database: atlantabeltline
-- ------------------------------------------------------
-- Server version	8.0.13

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
 SET NAMES utf8mb4 ;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `administrator`
--

DROP TABLE IF EXISTS `administrator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `administrator` (
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`username`),
  CONSTRAINT `administrator_idfk_1` FOREIGN KEY (`username`) REFERENCES `employee` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `administrator`
--

LOCK TABLES `administrator` WRITE;
/*!40000 ALTER TABLE `administrator` DISABLE KEYS */;
INSERT INTO `administrator` VALUES ('james.smith');
/*!40000 ALTER TABLE `administrator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `assign_to`
--

DROP TABLE IF EXISTS `assign_to`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `assign_to` (
  `staffusername` varchar(50) NOT NULL,
  `eventname` varchar(50) NOT NULL,
  `startdate` date NOT NULL,
  `sitename` varchar(50) NOT NULL,
  PRIMARY KEY (`staffusername`,`sitename`,`startdate`,`eventname`),
  KEY `assign_to_idfk_1` (`eventname`,`startdate`,`sitename`),
  CONSTRAINT `assign_to_idfk_1` FOREIGN KEY (`eventname`, `startdate`, `sitename`) REFERENCES `event` (`name`, `startdate`, `sitename`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `assign_to_idfk_2` FOREIGN KEY (`staffusername`) REFERENCES `staff` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `assign_to`
--

LOCK TABLES `assign_to` WRITE;
/*!40000 ALTER TABLE `assign_to` DISABLE KEYS */;
INSERT INTO `assign_to` VALUES ('staff3','Arboretum Walking Tour','2019-02-08','Inman Park'),('michael.smith','Bus Tour','2019-02-01','Inman Park'),('staff2','Bus Tour','2019-02-01','Inman Park'),('michael.smith','Bus Tour','2019-02-08','Inman Park'),('robert.smith','Bus Tour','2019-02-08','Inman Park'),('robert.smith','Eastside Trail','2019-02-04','Inman Park'),('staff2','Eastside Trail','2019-02-04','Inman Park'),('michael.smith','Eastside Trail','2019-02-04','Piedmont Park'),('staff1','Eastside Trail','2019-02-04','Piedmont Park'),('michael.smith','Eastside Trail','2019-02-13','Historic Fourth Ward Park'),('staff1','Eastside Trail','2019-03-01','Inman Park'),('staff1','Official Atlanta BeltLine Bike Tour','2019-02-09','Atlanta BeltLine Center'),('robert.smith','Private Bus Tour','2019-02-01','Inman Park'),('staff1','Westside Trail','2019-02-18','Westview Cemetery'),('staff3','Westside Trail','2019-02-18','Westview Cemetery');
/*!40000 ALTER TABLE `assign_to` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `connect`
--

DROP TABLE IF EXISTS `connect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `connect` (
  `sitename` varchar(50) NOT NULL,
  `type` varchar(50) NOT NULL,
  `route` varchar(50) NOT NULL,
  PRIMARY KEY (`type`,`sitename`,`route`),
  KEY `connect_idfk_1` (`sitename`),
  KEY `connect_idfk_2` (`type`,`route`),
  CONSTRAINT `connect_idfk_1` FOREIGN KEY (`sitename`) REFERENCES `site` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `connect_idfk_2` FOREIGN KEY (`type`, `route`) REFERENCES `transit` (`type`, `route`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `connect`
--

LOCK TABLES `connect` WRITE;
/*!40000 ALTER TABLE `connect` DISABLE KEYS */;
INSERT INTO `connect` VALUES ('Historic Fourth Ward Park','Bike','Relay'),('Historic Fourth Ward Park','Bus','152'),('Historic Fourth Ward Park','MARTA','Blue'),('Inman Park','Bus','152'),('Inman Park','MARTA','Blue'),('Piedmont Park','Bike','Relay'),('Piedmont Park','Bus','152'),('Piedmont Park','MARTA','Blue'),('Westview Cemetery','MARTA','Blue');
/*!40000 ALTER TABLE `connect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `employee`
--

DROP TABLE IF EXISTS `employee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `employee` (
  `employeeid` varchar(9) NOT NULL,
  `phone` varchar(10) NOT NULL,
  `address` varchar(90) NOT NULL,
  `city` varchar(50) NOT NULL,
  `state` varchar(50) NOT NULL,
  `zipcode` varchar(5) NOT NULL,
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`username`),
  UNIQUE KEY `employeeid` (`employeeid`),
  UNIQUE KEY `phone` (`phone`),
  CONSTRAINT `employee_idfk_1` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `employee`
--

LOCK TABLES `employee` WRITE;
/*!40000 ALTER TABLE `employee` DISABLE KEYS */;
INSERT INTO `employee` VALUES ('000000005','5124776435','350 Ferst Drive','Atlanta','GA','30332','david.smith'),('000000001','4043721234','123 East Main Street','Rochester','NY','14604','james.smith'),('000000006','8045126767','123 East Main Street','Rochester','NY','14604','manager1'),('000000007','9876543210','123 East Main Street','Rochester','NY','14604','manager2'),('000000008','5432167890','350 Ferst Drive','Atlanta','GA','30332','manager3'),('000000009','8053467565','123 East Main Street','Columbus','OH','43215','manager4'),('000000010','8031446782','801 Atlantic Drive','Atlanta','GA','30332','manager5'),('000000004','7890123456','123 East Main Street','Richland','PA','17987','maria.garcia'),('000000002','4043726789','350 Ferst Drive','Atlanta','GA','30332','michael.smith'),('000000003','1234567890','123 East Main Street','Columbus','OH','43215','robert.smith'),('000000011','8024456765','266 Ferst Drive Northwest','Atlanta','GA','30332','staff1'),('000000012','8888888888','266 Ferst Drive Northwest','Atlanta','GA','30332','staff2'),('000000013','3333333333','801 Atlantic Drive','Atlanta','GA','30332','staff3');
/*!40000 ALTER TABLE `employee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `event`
--

DROP TABLE IF EXISTS `event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `event` (
  `name` varchar(50) NOT NULL,
  `startdate` date NOT NULL,
  `sitename` varchar(50) NOT NULL,
  `enddate` date NOT NULL,
  `price` decimal(2,0) NOT NULL,
  `capacity` decimal(6,0) NOT NULL,
  `minstaffreq` decimal(1,0) NOT NULL,
  `description` longtext NOT NULL,
  PRIMARY KEY (`name`,`startdate`,`sitename`),
  KEY `event_idfk_1_idx` (`sitename`),
  CONSTRAINT `event_idfk_1` FOREIGN KEY (`sitename`) REFERENCES `site` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `event`
--

LOCK TABLES `event` WRITE;
/*!40000 ALTER TABLE `event` DISABLE KEYS */;
INSERT INTO `event` VALUES ('Arboretum Walking Tour','2019-02-08','Inman Park','2019-02-11',5,5,1,'A combination of multi-use trail and linear greenspace, the Eastside Trail was the first finished section of the Atlanta BeltLine trail in the old rail corridor. The Eastside Trail, which was funded by a combination of public and private philanthropic sources, runs from the tip of Piedmont Park to Reynoldstown. More details at https://beltline.org/explore-atlanta-beltline-trails/eastside-trail/'),('Bus Tour','2019-02-01','Inman Park','2019-02-02',25,6,2,'The Atlanta BeltLine Partnership’s tour program operates with a natural gas-powered, ADA accessible tour bus funded through contributions from 10th & Monroe, LLC, SunTrust Bank Trusteed Foundations – Florence C. and Harry L. English Memorial Fund and Thomas Guy Woolford Charitable Trust, and AGL Resources'),('Bus Tour','2019-02-08','Inman Park','2019-02-10',25,6,2,'The Atlanta BeltLine Partnership’s tour program operates with a natural gas-powered, ADA accessible tour bus funded through contributions from 10th & Monroe, LLC, SunTrust Bank Trusteed Foundations – Florence C. and Harry L. English Memorial Fund and Thomas Guy Woolford Charitable Trust, and AGL Resources'),('Eastside Trail','2019-02-04','Inman Park','2019-02-05',0,99999,1,'A combination of multi-use trail and linear greenspace, the Eastside Trail was the first finished section of the Atlanta BeltLine trail in the old rail corridor. The Eastside Trail, which was funded by a combination of public and private philanthropic sources, runs from the tip of Piedmont Park to Reynoldstown. More details at https://beltline.org/explore-atlanta-beltline-trails/eastside-trail/'),('Eastside Trail','2019-02-04','Piedmont Park','2019-02-05',0,99999,1,'A combination of multi-use trail and linear greenspace, the Eastside Trail was the first finished section of the Atlanta BeltLine trail in the old rail corridor. The Eastside Trail, which was funded by a combination of public and private philanthropic sources, runs from the tip of Piedmont Park to Reynoldstown. More details at https://beltline.org/explore-atlanta-beltline-trails/eastside-trail/'),('Eastside Trail','2019-02-13','Historic Fourth Ward Park','2019-02-14',0,99999,1,'A combination of multi-use trail and linear greenspace, the Eastside Trail was the first finished section of the Atlanta BeltLine trail in the old rail corridor. The Eastside Trail, which was funded by a combination of public and private philanthropic sources, runs from the tip of Piedmont Park to Reynoldstown. More details at https://beltline.org/explore-atlanta-beltline-trails/eastside-trail/'),('Eastside Trail','2019-03-01','Inman Park','2019-03-02',0,99999,1,'A combination of multi-use trail and linear greenspace, the Eastside Trail was the first finished section of the Atlanta BeltLine trail in the old rail corridor. The Eastside Trail, which was funded by a combination of public and private philanthropic sources, runs from the tip of Piedmont Park to Reynoldstown. More details at https://beltline.org/explore-atlanta-beltline-trails/eastside-trail/'),('Official Atlanta BeltLine Bike Tour','2019-02-09','Atlanta BeltLine Center','2019-02-14',5,5,1,'Official Atlanta BeltLine Arboretum Walking Tours provide an up-close view of the Westside Trail and the Atlanta BeltLine Arboretum led by Trees Atlanta Docents. The one and a half hour tours step off at at 10am (Oct thru May), and 9am (June thru September). Departure for all tours is from Rose Circle Park near Brown Middle School. More details at: https://beltline.org/visit/atlanta-beltline-tours/#arboretum-walking'),('Private Bus Tour','2019-02-01','Inman Park','2019-02-02',40,4,1,'Private tours are available most days, pending bus and tour guide availability. Private tours can accommodate up to 4 guests per tour, and are subject to a tour fee (nonprofit rates are available). As a nonprofit organization with limited resources, we are unable to offer free private tours. We thank you for your support and your understanding as we try to provide as many private group tours as possible. The Atlanta BeltLine Partnership’s tour program operates with a natural gas-powered, ADA accessible tour bus funded through contributions from 10th & Monroe, LLC, SunTrust Bank Trusteed Foundations – Florence C. and Harry L. English Memorial Fund and Thomas Guy Woolford Charitable Trust, and AGL Resources'),('Westside Trail','2019-02-18','Westview Cemetery','2019-02-21',0,99999,1,'The Westside Trail is a free amenity that offers a bicycle and pedestrian-safe corridor with a 14-foot-wide multi-use trail surrounded by mature trees and grasses thanks to Trees Atlanta’s Arboretum. With 16 points of entry, 14 of which will be ADA-accessible with ramp and stair systems, the trail provides numerous access points for people of all abilities. More details at: https://beltline.org/explore-atlanta-beltline-trails/westside-trail/');
/*!40000 ALTER TABLE `event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `manager`
--

DROP TABLE IF EXISTS `manager`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `manager` (
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`username`),
  CONSTRAINT `manager_idfk_1` FOREIGN KEY (`username`) REFERENCES `employee` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `manager`
--

LOCK TABLES `manager` WRITE;
/*!40000 ALTER TABLE `manager` DISABLE KEYS */;
INSERT INTO `manager` VALUES ('david.smith'),('manager1'),('manager2'),('manager3'),('manager4'),('manager5'),('maria.garcia');
/*!40000 ALTER TABLE `manager` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `site`
--

DROP TABLE IF EXISTS `site`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `site` (
  `name` varchar(50) NOT NULL,
  `address` varchar(90) DEFAULT NULL,
  `zipcode` varchar(5) NOT NULL,
  `openeveryday` varchar(1) NOT NULL,
  `managerusername` varchar(50) NOT NULL,
  PRIMARY KEY (`name`),
  KEY `site_idfk_1` (`managerusername`),
  CONSTRAINT `site_idfk_1` FOREIGN KEY (`managerusername`) REFERENCES `manager` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `site`
--

LOCK TABLES `site` WRITE;
/*!40000 ALTER TABLE `site` DISABLE KEYS */;
INSERT INTO `site` VALUES ('Atlanta Beltline Center','112 Krog Street Northeast','30307','N','manager3'),('Historic Fourth Ward Park','680 Dallas Street Northeast','30308','Y','manager4'),('Inman Park','','30307','Y','david.smith'),('Piedmont Park','400 Park Drive Northeast','30306','Y','manager2'),('Westview Cemetery','1680 Westview Drive Southwest','30310','N','manager5');
/*!40000 ALTER TABLE `site` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `staff`
--

DROP TABLE IF EXISTS `staff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `staff` (
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`username`),
  CONSTRAINT `staff_idfk_1` FOREIGN KEY (`username`) REFERENCES `employee` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `staff`
--

LOCK TABLES `staff` WRITE;
/*!40000 ALTER TABLE `staff` DISABLE KEYS */;
INSERT INTO `staff` VALUES ('michael.smith'),('robert.smith'),('staff1'),('staff2'),('staff3');
/*!40000 ALTER TABLE `staff` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `take`
--

DROP TABLE IF EXISTS `take`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `take` (
  `username` varchar(50) NOT NULL,
  `type` varchar(50) NOT NULL,
  `route` varchar(50) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY (`date`,`username`,`type`,`route`),
  KEY `take_idfk_1` (`username`),
  KEY `take_idfk_2` (`type`,`route`),
  CONSTRAINT `take_idfk_1` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `take_idfk_2` FOREIGN KEY (`type`, `route`) REFERENCES `transit` (`type`, `route`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `take`
--

LOCK TABLES `take` WRITE;
/*!40000 ALTER TABLE `take` DISABLE KEYS */;
INSERT INTO `take` VALUES ('manager2','Bus','152','2019-03-20'),('manager2','MARTA','Blue','2019-03-20'),('manager2','MARTA','Blue','2019-03-21'),('manager2','MARTA','Blue','2019-03-22'),('manager3','Bike','Relay','2019-03-20'),('maria.hernandez','Bike','Relay','2019-03-20'),('maria.hernandez','Bus','152','2019-03-20'),('maria.hernandez','Bus','152','2019-03-22'),('mary.smith','Bike','Relay','2019-03-23'),('visitor1','MARTA','Blue','2019-03-21');
/*!40000 ALTER TABLE `take` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transit`
--

DROP TABLE IF EXISTS `transit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `transit` (
  `type` varchar(50) NOT NULL,
  `route` varchar(50) NOT NULL,
  `price` decimal(3,2) NOT NULL,
  PRIMARY KEY (`type`,`route`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transit`
--

LOCK TABLES `transit` WRITE;
/*!40000 ALTER TABLE `transit` DISABLE KEYS */;
INSERT INTO `transit` VALUES ('Bike','Relay',1.00),('Bus','152',2.00),('MARTA','Blue',2.00);
/*!40000 ALTER TABLE `transit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `user` (
  `username` varchar(50) NOT NULL,
  `firstname` varchar(50) NOT NULL,
  `lastname` varchar(50) NOT NULL,
  `status` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('david.smith','David','Smith','Approved','dsmith456'),('﻿james.smith','James','Smith','Approved','jsmith123'),('manager1','Manager','One','Pending','manager1'),('manager2','Manager','Two','Approved','manager2'),('manager3','Manager','Three','Approved','manager3'),('manager4','Manager','Four','Approved','manager4'),('manager5','Manager','Five','Approved','manager5'),('maria.garcia','Maria','Garcia','Approved','mgarcia123'),('maria.hernandez','Maria','Hernandez','Approved','mhernandez'),('maria.rodriguez','Maria','Rodriguez','Declined','mrodriguez'),('mary.smith','Mary','Smith','Approved','msmith789'),('michael.smith','Michael','Smith','Approved','msmith456'),('robert.smith','Robert ','Smith','Approved','rsmith789'),('staff1','Staff','One','Approved','staff1234'),('staff2','Staff','Two','Approved','staff4567'),('staff3','Staff','Three','Approved','staff7890'),('user1','User','One','Pending','user123456'),('visitor1','Visitor','One','Approved','visitor123');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `useremail`
--

DROP TABLE IF EXISTS `useremail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `useremail` (
  `emailaddress` varchar(50) NOT NULL,
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`emailaddress`,`username`),
  KEY `useremail_idfk_1` (`username`),
  CONSTRAINT `useremail_idfk_1` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `useremail`
--

LOCK TABLES `useremail` WRITE;
/*!40000 ALTER TABLE `useremail` DISABLE KEYS */;
INSERT INTO `useremail` VALUES ('dsmith@outlook.com','david.smith'),('jsmith@gatech.edu','james.smith'),('jsmith@gmail.com','james.smith'),('jsmith@hotmail.com','james.smith'),('jsmith@outlook.com','james.smith'),('m1@beltline.com','manager1'),('m2@beltline.com','manager2'),('m3@beltline.com','manager3'),('m4@beltline.com','manager4'),('m5@beltline.com','manager5'),('mgarcia@gatech.edu','maria.garcia'),('mgarcia@yahoo.com','maria.garcia'),('mh@gatech.edu','maria.hernandez'),('mh123@gmail.com','maria.hernandez'),('mrodriguez@gmail.com','maria.rodriguez'),('mary@outlook.com','mary.smith'),('msmith@gmail.com','michael.smith'),('rsmith@hotmail.com','robert.smith'),('s1@beltline.com','staff1'),('s2@beltline.com','staff2'),('s3@beltline.com','staff3'),('u1@beltline.com','user1'),('v1@beltline.com','visitor1');
/*!40000 ALTER TABLE `useremail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `visitevent`
--

DROP TABLE IF EXISTS `visitevent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `visitevent` (
  `username` varchar(50) NOT NULL,
  `ename` varchar(50) NOT NULL,
  `startdate` date NOT NULL,
  `sitename` varchar(50) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY (`date`,`username`,`ename`,`startdate`,`sitename`),
  KEY `visitevent_idfk_1` (`ename`,`startdate`,`sitename`),
  KEY `visitevent_idfk_2` (`username`),
  CONSTRAINT `visitevent_idfk_1` FOREIGN KEY (`ename`, `startdate`, `sitename`) REFERENCES `event` (`name`, `startdate`, `sitename`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `visitevent_idfk_2` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `visitevent`
--

LOCK TABLES `visitevent` WRITE;
/*!40000 ALTER TABLE `visitevent` DISABLE KEYS */;
INSERT INTO `visitevent` VALUES ('manager2','Bus Tour','2019-02-01','Inman Park','2019-02-02'),('manager4','Bus Tour','2019-02-01','Inman Park','2019-02-01'),('manager5','Bus Tour','2019-02-01','Inman Park','2019-02-02'),('maria.garcia','Bus Tour','2019-02-01','Inman Park','2019-02-02'),('mary.smith','Bus Tour','2019-02-01','Inman Park','2019-02-01'),('mary.smith','Private Bus Tour','2019-02-01','Inman Park','2019-02-01'),('mary.smith','Private Bus Tour','2019-02-01','Inman Park','2019-02-02'),('mary.smith','Eastside Trail','2019-02-04','Piedmont Park','2019-02-04'),('mary.smith','Arboretum Walking Tour','2019-02-08','Inman Park','2019-02-10'),('mary.smith','Official Atlanta BeltLine Bike Tour','2019-02-09','Atlanta BeltLine Center','2019-02-10'),('mary.smith','Eastside Trail','2019-02-13','Historic Fourth Ward Park','2019-02-13'),('mary.smith','Eastside Trail','2019-02-13','Historic Fourth Ward Park','2019-02-14'),('mary.smith','Westside Trail','2019-02-18','Westview Cemetery','2019-02-19'),('staff2','Bus Tour','2019-02-01','Inman Park','2019-02-02'),('visitor1','Official Atlanta BeltLine Bike Tour','2019-02-09','Atlanta BeltLine Center','2019-02-10'),('visitor1','Eastside Trail','2019-02-13','Historic Fourth Ward Park','2019-02-14'),('visitor1','Westside Trail','2019-02-18','Westview Cemetery','2019-02-19');
/*!40000 ALTER TABLE `visitevent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `visitor`
--

DROP TABLE IF EXISTS `visitor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `visitor` (
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`username`),
  CONSTRAINT `visitor_idfk_1` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `visitor`
--

LOCK TABLES `visitor` WRITE;
/*!40000 ALTER TABLE `visitor` DISABLE KEYS */;
INSERT INTO `visitor` VALUES ('manager2'),('manager4'),('manager5'),('maria.garcia'),('maria.rodriguez'),('mary.smith'),('michael.smith'),('staff2'),('staff3'),('visitor1');
/*!40000 ALTER TABLE `visitor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `visitsite`
--

DROP TABLE IF EXISTS `visitsite`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `visitsite` (
  `username` varchar(50) NOT NULL,
  `sitename` varchar(50) NOT NULL,
  `date` date NOT NULL,
  PRIMARY KEY (`date`,`username`,`sitename`),
  KEY `visitsite_idfk_1` (`username`),
  KEY `visitsite_idfk_2` (`sitename`),
  CONSTRAINT `visitsite_idfk_1` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `visitsite_idfk_2` FOREIGN KEY (`sitename`) REFERENCES `site` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `visitsite`
--

LOCK TABLES `visitsite` WRITE;
/*!40000 ALTER TABLE `visitsite` DISABLE KEYS */;
INSERT INTO `visitsite` VALUES ('mary.smith','Atlanta Beltline Center','2019-02-01'),('mary.smith','Inman Park','2019-02-01'),('mary.smith','Historic Fourth Ward Park','2019-02-02'),('mary.smith','Inman Park','2019-02-02'),('mary.smith','Piedmont Park','2019-02-02'),('mary.smith','Inman Park','2019-02-03'),('mary.smith','Atlanta Beltline Center','2019-02-10'),('visitor1','Inman Park','2019-02-01'),('visitor1','Piedmont Park','2019-02-01'),('visitor1','Westview Cemetery','2019-02-06'),('visitor1','Atlanta Beltline Center','2019-02-09'),('visitor1','Historic Fourth Ward Park','2019-02-11'),('visitor1','Piedmont Park','2019-02-11'),('visitor1','Atlanta Beltline Center','2019-02-13');
/*!40000 ALTER TABLE `visitsite` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-04-07 15:14:29
