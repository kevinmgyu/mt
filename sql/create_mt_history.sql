use mt;

DROP TABLE IF EXISTS `user_mt_pledge`;
CREATE TABLE `user_mt_pledge` (
		`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
		`time`          DOUBLE NOT NULL,
		`block_num`     BIGINT NOT NULL,
		`transaction_id` VARCHAR(61) NOT NULL,
		`from_user_id`       VARCHAR(40) NOT NULL,
		`to_user_id`       VARCHAR(40) NOT NULL,
		`asset_type`          INT NOT NULL,
		`amount`        DECIMAL(40,8) NOT NULL,
		INDEX `idx_user_id` (`from_user_id`)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `user_mt_pledge_sum`;
CREATE TABLE `user_mt_pledge_sum` (
		`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
		`time`          DOUBLE NOT NULL,
		`from_user_id`       VARCHAR(40) NOT NULL,
		`asset_type`          INT NOT NULL,
		`amount`        DECIMAL(40,8) NOT NULL,
		INDEX `idx_user_id` (`from_user_id`)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `user_mt_redeme`;
CREATE TABLE `user_mt_redeme` (
			`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
			`time`          DOUBLE NOT NULL,
			`user_id`       VARCHAR(40) NOT NULL,
			`from_id`       VARCHAR(40) NOT NULL,
			`exe_time`          DOUBLE NOT NULL,
			`asset_type`          INT NOT NULL,
			`flag`          INT NOT NULL,
			`amount`        DECIMAL(40,8) NOT NULL,
			INDEX `idx_user_id` (`user_id`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
DROP TABLE IF EXISTS `user_mt_income`;
CREATE TABLE `user_mt_income` (
			`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
			`time`          DOUBLE NOT NULL,
			`amount`        DECIMAL(40,8) NOT NULL,
			INDEX `idx_time` (`time`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			
DROP TABLE IF EXISTS `user_mt_dividend`;
CREATE TABLE `user_mt_dividend` (
			`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
			`time`          DOUBLE NOT NULL,
			`user_id`       VARCHAR(40) NOT NULL,
			`amount`        DECIMAL(40,8) NOT NULL,
			INDEX `idx_user_id` (`user_id`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
					
DROP TABLE IF EXISTS `mt_block`;
CREATE TABLE `mt_block` (
			`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
			`time`          DOUBLE NOT NULL,
			`num`           BIGINT UNSIGNED
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			
DROP TABLE IF EXISTS `mt_pledge_dividend`;
CREATE TABLE `mt_pledge_dividend` (
			`id`            BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
			`time`          DOUBLE NOT NULL,
			`pledge_amount`        DECIMAL(40,8) NOT NULL,
			`dividend_amount`        DECIMAL(40,8) NOT NULL
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			