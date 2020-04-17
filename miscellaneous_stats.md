# Miscellaneous Stats


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
- Bandage info
    ```tsv
    Node count:                       11824622
    Edge count:                       12050926
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                1133741131
    Total length no overlaps (bp):    394207035
    Dead ends:                        8969729
    Percentage dead ends:             37.9282%
    Connected components:             1906331
    Largest component (bp):           470491350
    Total length orphaned nodes (bp): 470491350
    N50 (bp):                         88
    Shortest node (bp):               75
    Lower quartile node (bp):         75
    Median node (bp):                 81
    Upper quartile node (bp):         103
    Longest node (bp):                1683
    Median depth:                     1
    Estimated sequence length (bp):   1553644566
    ```

### 1.3 Kmers metadata histogram
- **Unitigs metadata histogram:**
    - [KM](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKM_histo.html)
    - [KM (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKM_yLog_histo.html)
    - [LN](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsLN_histo.html)
    - [LN (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsLN_yLog_histo.html)
    - [KC](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKC_histo.html)
    - [KC (y log scale)](https://www.mr-eyes.com/omnigraph/cDBG75/report/original/cDBG_SRR11015356_k75.unitigsKC_yLog_histo.html)
- LN & KM Stats
    ```text
    Stats for all components: 
    
    LN Field Stats: 
    
     - Max LN: 1683
         - Frequency of unitigs with length == 75 : 2973797
         - Sum of frequencies of unitigs length [76:80] : 2849278
         - Sum of frequencies of unitigs length [81:151] : 5633912
         - Sum of frequencies of unitigs length >151 : 367635
    
    KM Field Stats: 
         - Frequency of unitigs with KM [1.0: <= 2.0] : 4255575
         - Frequency of unitigs with KM [> 2.0: <= 10.0] : 3529266
         - Frequency of unitigs with KM [> 10.0:<= 50.0] : 780929
         - Frequency of unitigs with KM [> 50.0:<= 100.0] : 425439
         - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 1916180
         - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 860851
         - Frequency of unitigs with KM [> 10000.0] : 56382
    
     ------------------------------ 
    
    Stats for component 8:
    
        LN Field Stats: 
             - Max Ln:  1640 
             - Frequency of unitigs with length == 75 : 1608339
             - Sum of frequencies of unitigs length [76:80] : 1308768
             - Sum of frequencies of unitigs length [81:151] : 2238687
             - Sum of frequencies of unitigs length >151 : 46172
    
        KM Field Stats: 
             - Frequency of unitigs with KM [1.0: <= 2.0] : 1426699
             - Frequency of unitigs with KM [> 2.0: <= 10.0] : 1739636
             - Frequency of unitigs with KM [> 10.0:<= 50.0] : 446265
             - Frequency of unitigs with KM [> 50.0:<= 100.0] : 172360
             - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 777843
             - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 585944
             - Frequency of unitigs with KM [> 10000.0] : 53219
    ```

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
    Edge count:                       11879715
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                1121294392
    Total length no overlaps (bp):    392469872
    Dead ends:                        8895489
    Percentage dead ends:             38.1059%
    Connected components:             1899664
    Largest component (bp):           415963910
    Total length orphaned nodes (bp): 415963910
    N50 (bp):                         88
    Shortest node (bp):               75
    Lower quartile node (bp):         75
    Median node (bp):                 81
    Upper quartile node (bp):         103
    Longest node (bp):                1683
    Median depth:                     1.01333
    Estimated sequence length (bp):   1532666787
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

- LN & KM Stats
    ```text
    Stats for all components: 
    
    LN Field Stats: 
    
     - Max LN: 1683
    	 - Frequency of unitigs with length == 75 : 2931500
    	 - Sum of frequencies of unitigs length [76:80] : 2791099
    	 - Sum of frequencies of unitigs length [81:151] : 5581822
    	 - Sum of frequencies of unitigs length >151 : 367635
    
    KM Field Stats: 
    	 - Frequency of unitigs with KM [1.0: <= 2.0] : 4202427
    	 - Frequency of unitigs with KM [> 2.0: <= 10.0] : 3472800
    	 - Frequency of unitigs with KM [> 10.0:<= 50.0] : 756188
    	 - Frequency of unitigs with KM [> 50.0:<= 100.0] : 419277
    	 - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 1905585
    	 - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 859420
    	 - Frequency of unitigs with KM [> 10000.0] : 56359
    
     ------------------------------ 
    
    Stats for component 8:
    
    	LN Field Stats: 
    		 - Max Ln:  1359 
    		 - Frequency of unitigs with length == 75 : 1424890
    		 - Sum of frequencies of unitigs length [76:80] : 1131388
    		 - Sum of frequencies of unitigs length [81:151] : 1992836
    		 - Sum of frequencies of unitigs length >151 : 38751
    
    	KM Field Stats: 
    		 - Frequency of unitigs with KM [1.0: <= 2.0] : 1248791
    		 - Frequency of unitigs with KM [> 2.0: <= 10.0] : 1551404
    		 - Frequency of unitigs with KM [> 10.0:<= 50.0] : 395622
    		 - Frequency of unitigs with KM [> 50.0:<= 100.0] : 147340
    		 - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 659347
    		 - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 532373
    		 - Frequency of unitigs with KM [> 10000.0] : 52988

    ```

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
    Edge count:                       11981674
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                1129128323
    Total length no overlaps (bp):    393700413
    Dead ends:                        8942960
    Percentage dead ends:             38.0017%
    Connected components:             1904390
    Largest component (bp):           448281180
    Total length orphaned nodes (bp): 448281180
    N50 (bp):                         88
    Shortest node (bp):               75
    Lower quartile node (bp):         75
    Median node (bp):                 81
    Upper quartile node (bp):         103
    Longest node (bp):                1683
    Median depth:                     1.00671
    Estimated sequence length (bp):   1543554145
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
- LN & KM Stats
    ```text
    Stats for all components: 
    
    LN Field Stats: 
    
     - Max LN: 1683
         - Frequency of unitigs with length == 75 : 2955448
         - Sum of frequencies of unitigs length [76:80] : 2825047
         - Sum of frequencies of unitigs length [81:151] : 5618408
         - Sum of frequencies of unitigs length >151 : 367635
    
    KM Field Stats: 
         - Frequency of unitigs with KM [1.0: <= 2.0] : 4236666
         - Frequency of unitigs with KM [> 2.0: <= 10.0] : 3506450
         - Frequency of unitigs with KM [> 10.0:<= 50.0] : 770661
         - Frequency of unitigs with KM [> 50.0:<= 100.0] : 423062
         - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 1912802
         - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 860526
         - Frequency of unitigs with KM [> 10000.0] : 56371
    
     ------------------------------ 
    
    Stats for component 8:
    
        LN Field Stats: 
             - Max Ln:  1425 
             - Frequency of unitigs with length == 75 : 1532054
             - Sum of frequencies of unitigs length [76:80] : 1234110
             - Sum of frequencies of unitigs length [81:151] : 2140073
             - Sum of frequencies of unitigs length >151 : 43533
    
        KM Field Stats: 
             - Frequency of unitigs with KM [1.0: <= 2.0] : 1354461
             - Frequency of unitigs with KM [> 2.0: <= 10.0] : 1659628
             - Frequency of unitigs with KM [> 10.0:<= 50.0] : 424271
             - Frequency of unitigs with KM [> 50.0:<= 100.0] : 162360
             - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 730815
             - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 565217
             - Frequency of unitigs with KM [> 10000.0] : 53018
    ```



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
    105793	0	0	0	0	0	0	0	0	0	0	1	3	4	0	0	12	6	3	0	0	0	0	0	0	0
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

#### 4.3.1 Representitives Files

```tsv
file                          format  type   num_seqs      sum_len  min_len  avg_len  max_len
reps_unitigs_cDBG75_0.985.fa  FASTA   DNA   5,939,098  650,190,787       75    109.5    1,683
```

##### 4.3.1.1 LN & KM stats

```txt
Stats for all components: 

LN Field Stats: 

 - Max LN: 1683
	 - Frequency of unitigs with length == 75 : 515842
	 - Sum of frequencies of unitigs length [76:80] : 1044177
	 - Sum of frequencies of unitigs length [81:151] : 4012080
	 - Sum of frequencies of unitigs length >151 : 366999

KM Field Stats: 
	 - Frequency of unitigs with KM [1.0: <= 2.0] : 2871415
	 - Frequency of unitigs with KM [> 2.0: <= 10.0] : 2351611
	 - Frequency of unitigs with KM [> 10.0:<= 50.0] : 353595
	 - Frequency of unitigs with KM [> 50.0:<= 100.0] : 173919
	 - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 188558
	 - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 0
	 - Frequency of unitigs with KM [> 10000.0] : 0
```
- Bandage stats
    ```
    Node count:                       5939098
    Edge count:                       1432945
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                650190787
    Total length no overlaps (bp):    498109243
    Dead ends:                        9197631
    Percentage dead ends:             77.4329%
    Connected components:             4512295
    Largest component (bp):           12203
    Total length orphaned nodes (bp): 12203
    N50 (bp):                         109
    Shortest node (bp):               75
    Lower quartile node (bp):         80
    Median node (bp):                 96
    Upper quartile node (bp):         126
    Longest node (bp):                1683
    Median depth:                     0.878788
    Estimated sequence length (bp):   1499022010
    ```



#### 4.3.2 Components stats  | reduced_98.5_mild_dis_cDBG75

- Number of nodes: 4868502
- Number of connected components: 4511879
- Largest component number of nodes: 41
- Largest component length (bp): 9393
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
    (26505, 44, 8794), (258180, 42, 6177), (2797, 41, 9393), (371504, 41, 7221), (207569, 40, 5723), (16974, 38, 6284), (818166, 38, 4774), (106127, 37, 9169), (427888, 36, 6204), (71935, 35, 6442), (229822, 35, 5520), (254432, 35, 7190), (400024, 35, 5758), (127294, 33, 4593), (195691, 33, 7215), (247591, 33, 4272), (594183, 33, 6197), (228022, 32, 4418), (435654, 32, 6022), (115508, 31, 5034), (127789, 31, 5796), (284492, 31, 5782), (367909, 31, 5050), (487857, 31, 5944), (203517, 30, 3901), (383374, 30, 4915), (385497, 30, 3765), (398468, 30, 5715), (31235, 29, 6185), (71113, 29, 4182)```
- Bandage Info
    ```text
    Node count:                       4868502
    Edge count:                       362766
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                570966694
    Total length no overlaps (bp):    535468524
    Dead ends:                        9196843
    Percentage dead ends:             94.4525%
    Connected components:             4511879
    Largest component (bp):           9393
    Total length orphaned nodes (bp): 9393
    N50 (bp):                         119
    Shortest node (bp):               75
    Lower quartile node (bp):         80
    Median node (bp):                 98
    Upper quartile node (bp):         136
    Longest node (bp):                4574
    Median depth:                     0.378151
    Estimated sequence length (bp):   494862017
    ```

#### 4.3.3 Edges Connectivity  | reduced_98.5_mild_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    2797	0	0	0	0	0	0	0	0	0	0	1	1	8	0	0	11	10	3	0	0	0	7	0	0	0
    ```


## 5. Aggressive cDBG_75 (Reduced)

### 5.1 CDHIT @ 98.5%

#### 5.1.1 Representitives Files

```tsv
file                          format  type   num_seqs      sum_len  min_len  avg_len  max_len
reps_unitigs_cDBG75_0.985.fa  FASTA   DNA   5,903,962  647,039,807       75    109.6    1,683

```

##### 5.1.1.1 LN & KM Stats

```text
Stats for all components: 

LN Field Stats: 

 - Max LN: 1683
	 - Frequency of unitigs with length == 75 : 513805
	 - Sum of frequencies of unitigs length [76:80] : 1034242
	 - Sum of frequencies of unitigs length [81:151] : 3988916
	 - Sum of frequencies of unitigs length >151 : 366999

KM Field Stats: 
	 - Frequency of unitigs with KM [1.0: <= 2.0] : 2854823
	 - Frequency of unitigs with KM [> 2.0: <= 10.0] : 2335754
	 - Frequency of unitigs with KM [> 10.0:<= 50.0] : 351250
	 - Frequency of unitigs with KM [> 50.0:<= 100.0] : 173662
	 - Frequency of unitigs with KM [> 100.0:<= 1000.0] : 188473
	 - Frequency of unitigs with KM [> 1000.0:<= 10000.0] : 0
	 - Frequency of unitigs with KM [> 10000.0] : 0
```

- Bandage stats
    ```
    Node count:                       5903962
    Edge count:                       1420478
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                647039807
    Total length no overlaps (bp):    496134493
    Dead ends:                        9149635
    Percentage dead ends:             77.4872%
    Connected components:             4489444
    Largest component (bp):           12203
    Total length orphaned nodes (bp): 12203
    N50 (bp):                         110
    Shortest node (bp):               75
    Lower quartile node (bp):         80
    Median node (bp):                 96
    Upper quartile node (bp):         127
    Longest node (bp):                1683
    Median depth:                     0.88189
    Estimated sequence length (bp):   1494092397
    ```

#### 5.1.2 Components stats  | reduced_98.5_aggr_dis_cDBG75

- Number of nodes: 4839097
- Number of connected components: 4487423
- Largest component number of nodes: 41
- Largest component length (bp): 9393
- Top 30 components: 
    ```text
    Top 30 components: (ID, nodesCount, length_BP)
        [(958179, 44, 8794), (79082, 42, 6177), (74361, 41, 9393), (242485, 41, 7221), (54799, 40, 5723), (251786, 38, 4774), (521394, 37, 9169), (16131, 36, 6117), (770505, 36, 6204), (150447, 35, 5520), (233723, 3
    5, 5758), (280225, 35, 6442), (355497, 35, 7190), (72569, 33, 4593), (396038, 33, 4272), (407453, 33, 7215), (616083, 33, 6197), (245095, 32, 4418), (1221652, 32, 6022), (165674, 31, 5034), (417455, 31, 5782
    ), (538271, 31, 5030), (904836, 31, 5796), (62769, 30, 4915), (131755, 30, 3901), (171668, 30, 5715), (291315, 30, 3765), (6937, 29, 7040), (46133, 29, 7015), (131982, 29, 4182)]```


- Bandage info
    ```text
    Node count:                       4839097
    Edge count:                       357652
    Smallest edge overlap (bp):       74
    Largest edge overlap (bp):        74
    Total length (bp):                568239807
    Total length no overlaps (bp):    533209613
    Dead ends:                        9145691
    Percentage dead ends:             94.4979%
    Connected components:             4487423
    Largest component (bp):           9393
    Total length orphaned nodes (bp): 9393
    N50 (bp):                         119
    Shortest node (bp):               75
    Lower quartile node (bp):         80
    Median node (bp):                 98
    Upper quartile node (bp):         136
    Longest node (bp):                4574
    Median depth:                     0.378151
    Estimated sequence length (bp):   493896378
    ```

#### 5.1.3 Edges Connectivity  | reduced_95_aggr_dis_cDBG75
- Edges connectivity
    ```tsv
    compID	(-4, 0)	(-4, 1)	(-4, 2)	(-4, 3)	(-4, 4)	(-3, 0)	(-3, 1)	(-3, 2)	(-3, 3)	(-3, 4)	(-2, 0)	(-2, 1)	(-2, 2)	(-2, 3)	(-2, 4)	(-1, 0)	(-1, 1)	(-1, 2)	(-1, 3)	(-1, 4)	(0, 0)	(0, 1)	(0, 2)	(0, 3)	(0, 4)
    74361	0	0	0	0	0	0	0	0	0	0	1	2	8	0	0	9	10	2	0	0	0	9	0	0	0
    ```