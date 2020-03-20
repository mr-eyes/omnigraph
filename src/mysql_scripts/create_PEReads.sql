CREATE TABLE `universal_transcriptome`.`PEReads` 
  ( 
     `id`                        INT NOT NULL auto_increment, 
     `pe_seq1`                   VARCHAR(160) NOT NULL, 
     `pe_seq2`                   VARCHAR(160) NOT NULL, 
     `seq1_collective_component` INT NOT NULL, 
     `seq2_collective_component` INT NOT NULL, 
     `seq1_original_component`   INT NOT NULL, 
     `seq2_original_component`   INT NOT NULL, 
     PRIMARY KEY (`id`), 
     INDEX `idxseq1collective` (`seq1_collective_component`), 
     INDEX `idxseq2collective` (`seq2_collective_component`), 
     INDEX `idxseq1original` (`seq1_original_component`), 
     INDEX `idxseq2original` (`seq2_original_component`) 
  ) 
engine = innodb; 