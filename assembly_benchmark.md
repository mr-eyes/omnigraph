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

## 2. Reads Partitions

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

- number of partitions: 184,987
- Distribution {Number of original components: Occurrence count}
    ```text
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

- number of partitions: 287,682
- Distribution {Number of original components: Occurrence count}
    ```text
     (2, 258138),
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

- number of partitions: 366,773
- Distribution {Number of original components: Occurrence count}
    ```text
      (2, 354224),
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