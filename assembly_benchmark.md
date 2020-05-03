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
    - path: `<path on farm>` 
    - uniquely aligned: x
    - partially aligned: x
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

#### 2.1.1 count cutoff threshold = 1

- number of partitions: 184,987
