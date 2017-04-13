環境：
Mac OSX 10.12.4

編譯器：
gcc 6.3

如何編譯：
make

如何觀察tmpFile：
make test
然後執行 acp
和正常版本的差異只有在移動 tmpFile 前，會用 getchar() 等待使用者輸入一個字元，才會進行檔案搬移。

如何執行:
./acp source_file target_file
會複製 source_file 至 tmpFile ，然後將 tmpFile 移動至 target_file。
