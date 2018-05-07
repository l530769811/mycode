#ifndef __ESOP_SQLITE_SQL_H__
#define __ESOP_SQLITE_SQL_H__
#include <tchar.h>

const TCHAR open_pragma[] = {_T("PRAGMA FOREIGN_KEYS=ON;")};
const TCHAR create_user_table[]  = {_T("CREATE TABLE tlVeriryClientUser(\
									   ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
									   UserName VARCHAR(20) NOT NULL,\
									   Password VARCHAR(20) NOT NULL,\
									   Phone VARCHAR(20) NOT NULL,\
									   Describe VARCHAR(255),\
									   CreateTime DATE NOT NULL ,\
									   ExtraText TEXT,\
									   UNIQUE (UserName));")};
const TCHAR create_user_index[] = {_T("CREATE UNIQUE INDEX UserNameIndex ON tlVeriryClientUser (UserName);")};
const TCHAR insert_clientuser_data[] = {_T("INSERT INTO tlVeriryClientUser ( \
										 UserName,\
										 Password,\
										 Phone, \
										 Describe,\
										 ExtraText,\
										 CreateTime\
										 ) VALUES (\
										 '%s',\
										 '%s',\
										 '%s',\
										 '%s',\
										 '%s',\
										 DATE('NOW'));")};
const TCHAR update_user_password_data[] = {_T("UPDATE tlVeriryClientUser SET Password='%s' WHERE UserName='%s';")};
const TCHAR select_user_data[] = {_T("SELECT * FROM tlVeriryClientUser;")};
const TCHAR select_Login[] = {_T("SELECT * FROM tlVeriryClientUser WHERE UserName='%s' and Password='%s';")};

const TCHAR create_cardregister_table[]  = {_T("CREATE TABLE tlVeriryCardUser(\
									   ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
									   CardNumber VARCHAR(20) NOT NULL,\
									   CardUserPassword  VARCHAR(20) DEFAULT NULL,\
									   CardPasswordEncrypt VARCHAR(20) DEFAULT NULL,\
									   CarUserFirname  VARCHAR(10) DEFAULT NULL,\
									   Describe VARCHAR(255) DEFAULT NULL,\
									   CardRegistOwnUser VARCHAR(20) DEFAULT NULL,\
									   CardRegistUserPhone VARCHAR(20) DEFAULT NULL,\
									   CardRegistUserSex INT DEFAULT 0,\
									   CardRegistTime DATE NOT NULL DEFAULT '0000-00-00 00:00:00',\
									   ExtraText TEXT,\
									   UNIQUE (CardNumber),\
									   FOREIGN KEY (CardRegistOwnUser) REFERENCES tlVeriryClientUser (UserName));")};
const TCHAR create_cardregister_index[] = {_T("CREATE UNIQUE INDEX VeriryCardUserIndex ON tlVeriryCardUser (CardNumber);")};
const TCHAR insert_carduser_data[] = {_T("INSERT INTO tlVeriryCardUser ( \
									  CardNumber,\
									  CardUserPassword,\
									  CardPasswordEncrypt,\
									  CarUserFirname,\
									  Describe,\
									  CardRegistOwnUser,\
									  CardRegistUserPhone,\
									  CardRegistUserSex,\
									  CardRegistTime,\
									  ExtraText) VALUES (\
									  '%s',\
									  '%s',\
									  '%s',\
									  '%s',\
									  '%s',\
									  '%s',\
									  '%s',\
									  '%d',\
									  DATE('NOW'),\
									  '%s');")};
const TCHAR delete_carduser_data[] = { _T("DELETE FROM tlVeriryCardUser WHERE CardNumber='%s';")};
const TCHAR update_carduser_password_data[] = {_T("UPDATE tlVeriryCardUser SET CardUserPassword='%s',\
												  CardPasswordEncrypt='%s' WHERE CardNumber='%s';")};

const TCHAR update_carduser_firstname_data[] = {_T("UPDATE tlVeriryCardUser SET CarUserFirname='%s' WHERE CardNumber='%s';")};
const TCHAR update_carduser_sex_data[] = {_T("UPDATE tlVeriryCardUser SET CardRegistUserSex='%d' WHERE CardNumber='%s';")};

const TCHAR select_card_info[] = {_T("SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s';")};
const TCHAR select_card_info_name_pass[] = {_T("SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s' \
											   and CardUserPassword='%s';")};
const TCHAR select_card_info_name_pass_encrypt[] = {_T("SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s' \
											   and CardPasswordEncrypt='%s';")};
/*
const TCHAR create_device_table[]  = {_T("CREATE TABLE tlEsopDevice(\
									   ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
									   DeviceName VARCHAR(20) NOT NULL,\
									   AreaName VARCHAR(20) NOT NULL,\
									   MacAddress VARCHAR(20),\
									   IpAddress VARCHAR(20),\
									   PictureTime INT NOT NULL DEFAULT 5000,\
									   PictureMode INT NOT NULL DEFAULT 0,\
									   PictureShowX INT NOT NULL DEFAULT 1024,\
									   PictureShowY INT NOT NULL DEFAULT 768,\
									   PlayMode INT NOT NULL DEFAULT 0,\
									   UNIQUE (MacAddress),\
									   FOREIGN KEY (AreaName) REFERENCES tlEsopArea (AreaName));")};
const TCHAR create_device_index[] = {_T("CREATE UNIQUE INDEX MacAddressIndex ON tlEsopDevice (MacAddress);")};

const TCHAR create_company_table[]  = {_T("CREATE TABLE tlEsopCompany(\
										 ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
										 UserName VARCHAR(50),\
										 CompanyName VARCHAR(20),\
										 CompanyAddress TEXT,\
										 CompanyTel VARCHAR(20),\
										 CompanyFax VARCHAR(20),\
										 CompanyQQ VARCHAR(20),\
										 CompanyContact VARCHAR(20),\
										 CompanyEmail VARCHAR(20),\
										 CompanyEmployeCount INT,\
										 CompanyIndustry VARCHAR(20),\
										 UNIQUE (CompanyName),\
										 FOREIGN KEY (UserName) REFERENCES tlEsopUser (UserName));")};
const TCHAR create_company_index[] = {_T("CREATE UNIQUE INDEX ForUserNameIndex ON tlEsopCompany (UserName);")};

const TCHAR select_user_data[] = {_T("SELECT * FROM tlEsopUser;")};
const TCHAR select_lastLogin[] = {_T("SELECT * FROM tlEsopUser WHERE UserName='%s';")};


const TCHAR update_company[] = {_T("UPDATE tlEsopCompany SET CompanyName='%s', CompanyAddress='%s' ,CompanyTel='%s',\
								   CompanyFax='%s', CompanyQQ='%s' ,CompanyContact='%s',\
								   CompanyEmail='%s', CompanyEmployeCount='%s' ,CompanyIndustry='%s' \
								   WHERE UserName='%s';")};

const TCHAR create_area_file_table[] = {_T("CREATE TABLE tlEsopAreaFile (\
										   ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
										   FileName VARCHAR(20) NOT NULL DEFAULT NULL,\
										   FileMd5 VARCHAR(20),\
										   FileType VARCHAR(20),\
										   FileSize INT NOT NULL DEFAULT 0,\
										   AreaName VARCHAR(20) NOT NULL,\
										   PdfTotalPage INT NOT NULL DEFAULT 0,\
										   CONSTRAINT Md5_AreaName UNIQUE (FileMd5, AreaName),\
										   FOREIGN KEY (AreaName) REFERENCES tlEsopArea (AreaName));")};
const TCHAR create_area_file_index[] = {_T("CREATE INDEX AreaFileAreaNameIndex ON tlEsopAreaFile (AreaName);")};
const TCHAR	select_area_file[] = {_T("SELECT * FROM tlEsopAreaFile WHERE AreaName='%s';")};
const TCHAR	insert_area_file[] = {_T("INSERT INTO tlEsopAreaFile (FileName, FileMd5, FileType, AreaName, FileSize, PdfTotalPage) VALUES ('%s', '%s', '%s', '%s', %lld, %d);")};
const TCHAR delete_area_file[] = {_T("DELETE FROM tlEsopAreaFile WHERE  AreaName='%s' AND FileMd5='%s';")};
const TCHAR delete_area_file_area_name[] = {_T("DELETE FROM tlEsopAreaFile WHERE  AreaName='%s';")};
const TCHAR change_area_file_area_name[] = {_T("UPDATE tlEsopAreaFile SET AreaName = '%s' WHERE AreaName = '%s';")};
//const TCHAR updata_area_file[] = {_T("WHERE  AreaName='%s' AND FileMd5='%s';")};
*/
#endif //__ESOP_SQLITE_SQL_H__