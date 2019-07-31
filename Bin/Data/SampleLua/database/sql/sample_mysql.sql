/*
create table of sample
*/

-- ----------------------------
-- table structure for `sample`
-- ----------------------------
DROP TABLE IF EXISTS `sample`;
CREATE TABLE `sample` (
  `uin` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL DEFAULT '',
  `passwd` varchar(16) DEFAULT NULL,
  `create_time` bigint(11) DEFAULT NULL,
  `login_time` bigint(20) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `create_ip` varchar(32) DEFAULT NULL,
  `login_ip` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`uin`)
) ENGINE=InnoDB AUTO_INCREMENT=11562 DEFAULT CHARSET=utf8;

-- ----------------------------
-- records of sample
-- ----------------------------
INSERT INTO `sample` VALUES ('10001', 'many', '123', null, null, null, null, null);
INSERT INTO `sample` VALUES ('10002', 'manyxu', '1234', null, null, null, null, null);
INSERT INTO `sample` VALUES ('10003', 'xuduo', '12345', null, null, null, null, null);
