系統程式作業一

2017-02-27

---

# 基本 Bash、Unix 工具操作

### apt-get
Ubuntu 的套件管理系統
#### 更新套件清單
```
sudo apt-get update
```
![](https://i.imgur.com/dhHtbFu.png)
#### 安裝套件
```
sudo apt-get install foo
```
![](https://i.imgur.com/Vplm0sk.png)
#### 移除套件
```
sudo apt-get remove bar
sudo apt-get --purge remove bar #連設定檔也刪掉
```
![](https://i.imgur.com/GwaAxAS.png)
#### 更新已安裝套件
```
sudo apt-get upgrade
```
![](https://i.imgur.com/DVa5MMG.png)
#### 清理套件
```
sudo apt-get clean
sudo apt-get autoclean
```
![](https://i.imgur.com/F6PSQYK.png)

### chgrp
更換檔案群組
```
chgrp group_name file_name
```
![](https://i.imgur.com/wN935m0.png)

### chmod
更換檔案權限設定
```
chmod permission_notation file_name
```
![](https://i.imgur.com/QUWI0A3.png)

### chown
變更檔案使用者（群組）
```
chown group:user file_name 
```
![](https://i.imgur.com/k8dKLGm.png)


### gcc
GNU C 編譯器
```
gcc file_name.c -o exec_name
gcc file_name.c -S #輸出組語
```
![](https://i.imgur.com/eBORbq6.png)
![](https://i.imgur.com/ykwjqSU.png)


### ln
連結檔案
```
ln -s source target
```
![](https://i.imgur.com/t4DUFFP.png)

### ls
列出當前目錄內容
```
ls 
ls -l #詳細
ls -a #顯示隱藏檔
ls -al #以上兩者的組合
ls file_name
#以上參數都可以組合用
```
![](https://i.imgur.com/e7LLSf4.png)
![](https://i.imgur.com/w2qFTFW.png)
![](https://i.imgur.com/mFG5BB1.png)
![](https://i.imgur.com/VrwCatx.png)


### man
叫出說明書
```
man command
man 3 command
```
![](https://i.imgur.com/ISq4778.png)
![](https://i.imgur.com/e6lNcrn.jpg)


### mount
掛載媒體裝置
```
sudo mount device mount_point
```
![](https://i.imgur.com/0Y6UMPV.png)

### mv
移動、更名檔案（目錄）
```
mv source target
```
![](https://i.imgur.com/DRmaL7S.png)


### tar
打包（或結合其他參數壓縮）檔案、目錄
```
tar [參數] 目標 來源
```
![](https://i.imgur.com/8v4m18e.png)

### vim
文件編輯器
```
vim 檔名
```
![](https://i.imgur.com/exvYEoN.png)

