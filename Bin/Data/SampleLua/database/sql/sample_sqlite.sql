/*
create table of sample
*/

-- ----------------------------
-- table structure for `sample`
-- ----------------------------
DROP TABLE IF EXISTS `sample`;
CREATE TABLE `sample` (
  `uin` INTEGER NOT NULL DEFAULT '10000',
  `name` TEXT NOT NULL DEFAULT '',
  `passwd` TEXT DEFAULT NULL,
  `create_time` INTEGER DEFAULT NULL,
  `login_time` INTEGER DEFAULT NULL,
  `device_id` TEXT DEFAULT NULL,
  `create_ip` TEXT DEFAULT NULL,
  `login_ip` TEXT DEFAULT NULL,
  PRIMARY KEY (`uin`)
);

-- ----------------------------
-- records of sample
-- ----------------------------
INSERT INTO `sample` VALUES ('10001', 'many', '123', null, null, null, null, null);
INSERT INTO `sample` VALUES ('10002', 'manyxu', '1234', null, null, null, null, null);
INSERT INTO `sample` VALUES ('10003', 'xuduo', '12345', null, null, null, null, null);