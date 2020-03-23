CREATE TABLE `universal_transcriptome`.`unitigs_tracker` 
  ( 
     `id`                     INT NOT NULL auto_increment, 
     `unitig_id`              INT UNSIGNED NOT NULL, 
     `originalcomponent_id`   INT UNSIGNED NOT NULL, 
     `collectivecomponent_id` INT UNSIGNED NOT NULL, 
     `finalcomponent_id`      INT UNSIGNED NOT NULL, 
     PRIMARY KEY (`id`), 
     INDEX `idx_unitigid` (`unitig_id`), 
     INDEX `idx_originalcomponentid` (`originalcomponent_id`), 
     INDEX `idx_collectivecomponentid` (`collectivecomponent_id`), 
     INDEX `idx_finalcomponentid` (`finalcomponent_id`) 
  ) 
engine = innodb;