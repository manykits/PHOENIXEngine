/*
Navicat MySQL Data Transfer

Source Server         : aliyun
Source Server Version : 50613
Source Host           : 115.28.35.69:3306
Source Database       : accounts

Target Server Type    : MYSQL
Target Server Version : 50613
File Encoding         : 65001

Date: 2014-05-16 15:57:06
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `account_tb`
-- ----------------------------
DROP TABLE IF EXISTS `account_tb`;
CREATE TABLE `account_tb` (
  `uin` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(32) NOT NULL DEFAULT '',
  `password` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`uin`)
) ENGINE=InnoDB AUTO_INCREMENT=11562 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of account_tb
-- ----------------------------
INSERT INTO `account_tb` VALUES ('10001', 'a', '831291ea9458a771');
INSERT INTO `account_tb` VALUES ('10002', 'b', 'ac59075b964b0715');
INSERT INTO `account_tb` VALUES ('10003', 'c', 'ac59075b964b0715');

-- ----------------------------
-- Table structure for `serverlist`
-- ----------------------------
DROP TABLE IF EXISTS `serverlist_tb`;
CREATE TABLE `serverlist_tb` (
  `id` int(11) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `addr` varchar(16) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `svgroup` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of serverlist
-- ----------------------------
INSERT INTO `serverlist_tb` VALUES ('1', '本地调试服', '127.0.0.1', '7889', '1', '100', '1');
INSERT INTO `serverlist_tb` VALUES ('2', '测试服', '115.28.35.69', '7889', '0', '100', '1');
INSERT INTO `serverlist_tb` VALUES ('3', '程序服', '115.28.35.69', '7890', '2', '100', '1');