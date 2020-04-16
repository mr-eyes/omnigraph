# Miscellaneous Stats

1. Stats ln,km,kc, (Before clustering only) + (original & dislinked)
2. Stats ln (After clustering only) + (original & dislinked)
3. Edge connectivity (After clustering) + (original & dislinked)
4. Connected components Bandage + My script (After clustering) + (original & dislinked)
5. Statistics of large component (After clustering only) + (original & dislinked)


> Mild dislinkage: maxDust=8.x ACTG
> Aggressive dislinkage: maxDust=4 ACTGGTCA

- [ ] Repeat the jobs on mild


1. cDBG_SRR11015356_k75.unitigs.fa (Repeat for Mild and Aggressive )

- Number of nodes: 11824622
- Number of components: 1906331
- Largest components
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    [(8, 5201966, 470491350), (555, 22477, 2008781), (3177, 19309, 1761536), (4810, 14135, 1305499), (66, 12354, 1143652), (26123, 11981, 1034014), (9452, 11535, 1066741), (14165, 11121, 960225), (2216, 10813, 979662), (4596, 10095, 867800), (8445, 9866, 865500), (5812, 9649, 816595), (3384, 9408, 827137), (6272, 9257, 795484), (14023, 9233, 808414), (3336, 9090, 807168), (22448, 9076, 776111), (6651, 8815, 800158), (23800, 8784, 781078), (6839, 8656, 759741), (20924, 8408, 723026), (15771, 7792, 689236), (2011, 7550, 681242), (11125, 7423, 708792), (19037, 7193, 625471), (2362, 7175, 707541), (2723, 7167, 619874), (5438, 7115, 643292), (13833, 7022, 597035), (7849, 6817, 609016)]
    ```
- Edges Connectivity 
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    8	82886	41162	69691	36960	81015	130942	72139	86606	48163	41567	247224	460590	376980	94777	81562	939186	382702	469180	84783	52068	0	856943	237540	135408	91892
    ```




## 1. Original cDBG75

### 1.1 Components stats

- Number of nodes: 11824622
- Number of connected components: 1906331
- Largest component number of nodes: 5201966
- Largest component length (bp): 470491350
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    [(8, 5201966, 470491350), (555, 22477, 2008781), (3177, 19309, 1761536), (4810, 14135, 1305499), (66, 12354, 1143652), (26123, 11981, 1034014), (9452, 11535, 1066741), (14165, 11121, 960225), (2216, 10813, 979662), (4596, 10095, 867800), (8445, 9866, 865500), (5812, 9649, 816595), (3384, 9408, 827137), (6272, 9257, 795484), (14023, 9233, 808414), (3336, 9090, 807168), (22448, 9076, 776111), (6651, 8815, 800158), (23800, 8784, 781078), (6839, 8656, 759741), (20924, 8408, 723026), (15771, 7792, 689236), (2011, 7550, 681242), (11125, 7423, 708792), (19037, 7193, 625471), (2362, 7175, 707541), (2723, 7167, 619874), (5438, 7115, 643292), (13833, 7022, 597035), (7849, 6817, 609016)]
    ```

### 1.2 Edges Connectivity
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    8	82886	41162	69691	36960	81015	130942	72139	86606	48163	41567	247224	460590	376980	94777	81562	939186	382702	469180	84783	52068	0	856943	237540	135408	91892
    ```
### 1.3 Kmers metadata histogram
- **Unitigs metadata histogram:**
    - [KM](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKM_histo.html)
    - [KM (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKM_yLog_histo.html)
    - [LN](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsLN_histo.html)
    - [LN (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsLN_yLog_histo.html)
    - [KC](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKC_histo.html)
    - [KC (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKC_yLog_histo.html)


---


## 2. Aggressive Dislinked cDBG75

### 2.1 Components stats | agg_dis_cDBG75

- Number of nodes: 11672056
- Number of connected components: 1899664
- Largest component number of nodes: 4587865,
- Largest component length (bp): 415963910
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    [(8, 4587865, 415963910), (561, 22282, 1992741), (3223, 19301, 1760901), (403, 17744, 1567633), (4879, 13949, 1289456), (6489, 13015, 1122490), (12584, 12570, 1100743), (67, 12339, 1142333), (26517, 11981, 1034014), (9593, 11532, 1066448), (7951, 11450, 1096283), (4633, 11139, 1003534), (14376, 11121, 960225), (2242, 10537, 953723), (8574, 9852, 864402), (5896, 9649, 816595), (3433, 9408, 827137), (14233, 9231, 808184), (6364, 9207, 788958), (3384, 9081, 806285), (22788, 9012, 770867), (6747, 8815, 800158), (7962, 8731, 765955), (24161, 8691, 773513), (6940, 8570, 752348), (26956, 8542, 754974), (21245, 8408, 723026), (4662, 8084, 697516), (15078, 8015, 705604), (6750, 7900, 692412)]
    ```

- Bandage info 
    ```
    Node count:                       11672056
    Edge count:                       0
    Smallest edge overlap (bp):       0
    Largest edge overlap (bp):        0
    Total length (bp):                1121294392
    Total length no overlaps (bp):    1121294392
    Dead ends:                        23344112
    Percentage dead ends:             100%
    Connected components:             11672056
    Largest component (bp):           1683
    Total length orphaned nodes (bp): 1683
    N50 (bp):                         88
    Shortest node (bp):               75
    Lower quartile node (bp):         75
    Median node (bp):                 81
    Upper quartile node (bp):         103
    Longest node (bp):                1683
    Median depth:                     1
    Estimated sequence length (bp):   1121294392
    ```

### 2.2 Edges Connectivity | agg_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    8	79570	39152	67554	35654	80006	122279	63174	78808	45165	40031	222248	389976	325093	86102	78670	813283	329030	397594	74480	49209	0	742039	213781	126685	88282
    ```
  
### 2.3 Kmers metadata histogram | agg_dis_cDBG75
- Unitigs metadata histogram:
    - [KM (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/aggressive_dislinked/aggressive_dislinked_cDBG_SRR11015356_k75.unitigsKM_yLog_histo.html)
    - [LN (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/aggressive_dislinked/aggressive_dislinked_cDBG_SRR11015356_k75.unitigsLN_yLog_histo.html)
    - [KC (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/aggressive_dislinked/aggressive_dislinked_cDBG_SRR11015356_k75.unitigsKC_yLog_histo.html)



---



## 3. Mild Dislinked cDBG75

### 3.1 Components stats  | mild_dis_cDBG75

- Number of nodes: 11766538
- Number of connected components: 1904390
- Largest component number of nodes: 4949770
- Largest component length (bp): 448281180
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    [(8, 4949770, 448281180), (556, 22477, 2008781), (3194, 19309, 1761536), (4833, 14135, 1305499), (6219, 13785, 1187100), (66, 12354, 1143652), (26247, 11981, 1034014), (9500, 11535, 1066741), (7867, 11489, 1099996), (14235, 11121, 960225), (2227, 10812, 979568), (8486, 9866, 865500), (4619, 9667, 832990), (5840, 9649, 816595), (3404, 9408, 827137), (6303, 9257, 795484), (14093, 9233, 808414), (3355, 9090, 807168), (22553, 9076, 776111), (6684, 8815, 800158), (23915, 8784, 781078), (6873, 8653, 759470), (26680, 8623, 762120), (21024, 8408, 723026), (15850, 7639, 676399), (2019, 7550, 681242), (11182, 7423, 708792), (19129, 7193, 625471), (2374, 7175, 707541), (22195, 7169, 616647)]

    ```
- Bandage info 
    ```
    Node count:                       11766538
    Edge count:                       0
    Smallest edge overlap (bp):       0
    Largest edge overlap (bp):        0
    Total length (bp):                1129128323
    Total length no overlaps (bp):    1129128323
    Dead ends:                        23533076
    Percentage dead ends:             100%
    Connected components:             11766538
    Largest component (bp):           1683
    Total length orphaned nodes (bp): 1683
    N50 (bp):                         88
    Shortest node (bp):               75
    Lower quartile node (bp):         75
    Median node (bp):                 81
    Upper quartile node (bp):         103
    Longest node (bp):                1683
    Median depth:                     1
    Estimated sequence length (bp):   1129128323
    ```

### 3.2 Edges Connectivity  | mild_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    8	81301	40258	68693	36260	80351	127385	68357	83344	46830	40776	237406	431752	355418	90999	80195	888820	359897	439966	80571	50833	0	810435	227997	131761	90165
    ```
### 3.3 Kmers metadata histogram  | mild_dis_cDBG75
- Unitigs metadata histogram:
    - [KM (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/mild_dislinked/mild_filtered_dislinked_cDBG_SRR11015356_k75.unitigsKM_yLog_histo.html)
    - [LN (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/mild_dislinked/mild_filtered_dislinked_cDBG_SRR11015356_k75.unitigsLN_yLog_histo.html)
    - [KC (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/mild_dislinked/mild_filtered_dislinked_cDBG_SRR11015356_k75.unitigsKC_yLog_histo.html)





---

## 4. Mild cDBG_75 (Reduced)

### 4.1 CDHIT @ 95%

#### 4.1.1 Components stats  | reduced_95_mild_dis_cDBG75

- Number of nodes: 1980172
- Number of connected components: 1848471
- Largest component number of nodes: 29
- Largest component length (bp): 8556
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    (88589, 29, 6375), (105793, 29, 8556), (163616, 27, 7868), (22525, 25, 4536), (29591, 25, 4804), (109927, 25, 3950), (140239, 25, 3875), (252535, 25, 4860), (31324, 24, 6658), (41683, 24, 5582), (187571, 24, 4807), (33248, 23, 3892), (50682, 23, 3959), (58499, 23, 4949), (55126, 22, 3112), (194312, 22, 5531), (230702, 22, 4167), (307870, 22, 4447), (31983, 21, 4627), (42880, 21, 6477), (84860, 21, 4133), (122739, 21, 4056), (164818, 21, 6390), (14317, 20, 3195), (23456, 20, 2941), (63465, 20, 4305), (131885, 20, 3224), (162239, 20, 4799), (164257, 20, 4293), (12849, 19, 3540)]

#### 4.1.2 Edges Connectivity  | reduced_95_mild_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    8556	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1	0	0	0	0
    ```


---

### 4.2 CDHIT @ 97%

#### 4.2.1 Components stats  | reduced_97_mild_dis_cDBG75

- Number of nodes: 2332606
- Number of connected components: 2160129
- Largest component number of nodes: 35
- Largest component length (bp): 8943
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    (6270, 38, 8344), (171188, 38, 5819), (48269, 35, 8943), (32451, 31, 5782), (8824, 29, 4884), (62124, 29, 6375), (239471, 29, 8556), (308419, 28, 5306), (55124, 27, 3822), (237100, 27, 5016), (260332, 26, 3953), (6046, 25, 5230), (66936, 25, 6735), (130872, 25, 4051), (305653, 25, 4804), (335583, 25, 3950), (377378, 25, 4536), (2738, 24, 4807), (16700, 24, 5582), (30649, 24, 4807), (177837, 24, 4606), (447305, 24, 4117), (1818, 23, 4487), (126752, 23, 4430), (136806, 23, 4209), (237167, 23, 3988), (910, 22, 4133), (124705, 22, 3393), (180369, 22, 2359), (252642, 22, 4509)

#### 4.2.2 Edges Connectivity  | reduced_97_mild_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    48269	0	0	0	0	0	0	0	0	0	0	0	1	7	0	0	7	9	2	0	0	0	8	1	0	0
    ```


---


### 4.3 CDHIT @ 98.5%

#### 4.3.1 Components stats  | reduced_98.5_mild_dis_cDBG75

- Number of nodes: 4839097
- Number of connected components: 4487423
- Largest component number of nodes: 41
- Largest component length (bp): 9393
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    (26514, 44, 8794), (757160, 42, 6177), (497267, 41, 9393), (825327, 41, 7221), (136267, 40, 5723), (167375, 38, 4774), (314755, 37, 9169), (183997, 36, 6117), (208319, 36, 6204), (26162, 35, 5520), (245302, 35, 5758), (554981, 35, 6442), (1150839, 35, 7190), (72906, 33, 4272), (111979, 33, 7215), (264507, 33, 4593), (1140537, 33, 6197), (26588, 32, 6022), (659062, 32, 4418), (11008, 31, 5034), (18254, 31, 5796), (79252, 31, 5782), (210260, 31, 5030), (38954, 30, 3901), (54671, 30, 4915), (156927, 30, 5715), (188533, 30, 3765), (66043, 29, 7015), (98281, 29, 3568), (114400, 29, 5104)

#### 4.3.2 Edges Connectivity  | reduced_98.5_mild_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    497267	0	0	0	0	0	0	0	0	0	0	0	1	8	0	0	9	10	3	0	0	0	9	1	0	0
    ```
