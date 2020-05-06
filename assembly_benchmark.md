# Assembly benchmark

## 1. Raw Reads

### 1.1 Assembly

- slurm_job: `slurm_jobs/jobs_assembly/job_rawReads_rnaspades.sh`
- path on farm: `/home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/assembly/rawReads/`
- no_cores: 20
- RAM: 43.3 GB
- Wall time: 4:00:00

### 1.2 Assembly assessment

- slurm_job: `slurm_jobs/jobs_assembly/job_rawReads_rnaQuast.sh`
- software: rnaQuast
- no_cores: 32
- RAM: 12 GB
- Wall time: 5:36:28

- **Results:**
    - path: `/home/mhussien/omnigraph/data/cDBG75/singlePartitions/slurm/assembly/rawReads/rnaQuast_assessment`
    - ```
      Genes   67071
        Transcripts     203347
        Transcripts > 500 bp    69574
        Transcripts > 1000 bp   40596
        Aligned 38882
        Uniquely aligned        32443
        Multiply aligned        534
        Unaligned       164465
        Avg. aligned fraction   0.562
      ```

---

## 2. Reads Partitions (stats generated using scripts/misc/analyse_final_components/analyse_final_components.ipynb)

### 2.1 Aggressive dislinked

- Pairs count {Pairs Count : Frequency of occurrence}
    ```text
     (1, 99969),
     (2, 181155),
     (3, 67923),
     (4, 31194),
     (5, 15717),
     (6, 8919),
     (7, 5335),
     (8, 3350),
     (9, 2062),
     (10, 1518),
     (11, 951),
     (12, 657),
     (13, 451),
     (14, 327),
     (15, 255),
     (16, 191),
     (17, 117),
     (18, 115),
     (19, 88),
     (20, 59),
     (21, 42),
     (22, 44),
     (23, 44),
     (24, 36),
     (25, 34),
     (26, 20),
     (27, 13),
     (28, 15),
     (29, 9),
     (30, 17),
     (31, 12),
     (32, 11),
     (33, 10),
     (34, 11),
     (35, 6),
     (36, 12),
     (37, 5),
     (38, 7),
     (39, 4),
     (40, 9),
     (41, 3),
     (42, 5),
     (43, 4),
     (44, 3),
     (45, 4),
     (46, 4),
     (47, 3),
     (48, 3),
     (49, 6),
     (50, 2),
     (51, 4),
     (52, 1),
     (53, 1),
     (54, 1),
     (55, 1),
     (57, 1),
     (58, 2),
     (59, 3),
     (60, 1),
     (61, 1),
     (64, 3),
     (67, 2),
     (68, 3),
     (69, 2),
     (72, 1),
     (74, 1),
     (76, 2),
     (77, 2),
     (79, 2),
     (82, 1),
     (83, 1),
     (84, 1),
     (85, 1),
     (88, 1),
     (89, 1),
     (92, 1),
     (93, 1),
     (96, 1),
     (118, 1),
     (120, 2),
     (143, 1),
     (147, 1),
     (171, 1),
     (271, 1),
     (671, 1)
    ```

#### 2.1.1 count cutoff threshold = 1

- Top & bottom 10 Final Components {finalComponentID: Number of matched Reads}
    ```text
    (1, 119491792),
    (63613, 7367),
    (5553, 7200),
    (37782, 7086),
    (63937, 6995),
    (1267, 6444),
    (26759, 6168),
    (16513, 5467),
    (2198, 5244),
    (86194, 5118)
    
  Bottom:
    (1505107, 0),
    (1505109, 0),
    (1505113, 0),
    (1505114, 0),
    (1505115, 0),
    (1505116, 0),
    (1505117, 0),
    (1505118, 0),
    (1505119, 0),
    (1505120, 0)
    ```

- Number of partitions (final components): 1,505,121
- Number of empty final components: 1,184,787
- Distribution {Number of original components: Occurrence count}
    ```text
      (1, 1320134),
      (2, 148794),
      (3, 20631),
      (4, 7608),
      (5, 3390),
      (6, 1783),
      (7, 994),
      (8, 592),
      (9, 374),
      (10, 242),
      (11, 143),
      (12, 115),
      (13, 80),
      (14, 59),
      (15, 42),
      (16, 27),
      (17, 16),
      (18, 20),
      (19, 16),
      (20, 10),
      (21, 12),
      (22, 5),
      (23, 9),
      (24, 3),
      (25, 3),
      (26, 2),
      (27, 4),
      (28, 3),
      (29, 3),
      (31, 2),
      (33, 1),
      (38, 1),
      (39, 1),
      (49, 1),
      (142871, 1)
    ```


#### 2.1.2 count cutoff threshold = 2

- Top & bottom 10 Final Components {finalComponentID: Number of matched Reads}
    ```text
  
    (1, 116097945),
    (52135, 13766),
    (1271, 13116),
    (16350, 10968),
    (7870, 10759),
    (48329, 10355),
    (16298, 9946),
    (21858, 9762),
    (62915, 9578),
    (3201, 9455)
    
  Bottom:    
    (1607802, 0),
    (1607804, 0),
    (1607808, 0),
    (1607809, 0),
    (1607810, 0),
    (1607811, 0),
    (1607812, 0),
    (1607813, 0),
    (1607814, 0),
    (1607815, 0)
    ```

- number of partitions: 1,590,607
- Number of empty final components: 1,184,787
- Distribution {Number of original components: Occurrence count}
    ```text
    (1, 1402894),
    (2, 158169),
    (3, 19176),
    (4, 5699),
    (5, 2171),
    (6, 996),
    (7, 570),
    (8, 310),
    (9, 203),
    (10, 105),
    (11, 95),
    (12, 49),
    (13, 45),
    (14, 33),
    (15, 29),
    (16, 17),
    (17, 12),
    (18, 15),
    (19, 4),
    (20, 3),
    (21, 4),
    (22, 4),
    (25, 1),
    (28, 1),
    (40, 1),
    (76335, 1)
    ```
  
#### 2.1.2 count cutoff threshold = 3

- Top & bottom 10 Final Components {finalComponentID: Number of matched Reads}
    ```text
    (2, 112086830),
    (5517, 18627),
    (109329, 17979),
    (10894, 17251),
    (71415, 16376),
    (26175, 15318),
    (3677, 13859),
    (40014, 13747),
    (13838, 13551),
    (8957, 13056)
  
  Bottom:
    (1767916, 0),
    (1767918, 0),
    (1767923, 0),
    (1767926, 0),
    (1767927, 0),
    (1767932, 0),
    (1767933, 0),
    (1767934, 0),
    (1767935, 0),
    (1767937, 0)
    ```

- number of partitions: 1,767,937
- Number of empty final components: 1,184,787
- Distribution {Number of original components: Occurrence count}
    ```text
    (1, 1682288),
    (2, 73100),
    (3, 8538),
    (4, 2305),
    (5, 843),
    (6, 388),
    (7, 203),
    (8, 98),
    (9, 77),
    (10, 32),
    (11, 20),
    (12, 17),
    (13, 9),
    (14, 9),
    (15, 2),
    (16, 5),
    (17, 1),
    (20, 1),
    (32434, 1)  
    ```