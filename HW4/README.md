---
Date: 2017-04-06 15:15
ID number: 404410030
Name: 鄭光宇
Environment: Mac OSX 10.12.3
Simple Guide: just make critical
---
#### 環境
OS:

Mac OSX 10.12.3

Compiler:
```
GNU gcc 6.3
```

#### 編譯

`make all`

若要觀察 critical section ，

`make critical`

#### 執行

`./addGrade`

會以 Append 的方式將

人名 成績1 成績2 成績3 成績4 成績5

寫入。寫入前將檔案鎖上，之後關檔，解除鎖定。
