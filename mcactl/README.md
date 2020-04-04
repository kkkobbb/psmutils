# mcactl (MemoryCardAdaptor ConTroLler)
メモリーカードアダプタ(MCA)を制御して、ps1メモリーカード(MC)の読み書きを行う

* 依存パッケージ
  * libusb-1.0


## 操作説明
実行時にMCAがUSBポートに差し込まれていれば自動で認識し、オプションで指定した動作が実行される
    * USBのベンダID等の指定は不要
    * libusbを使用


## 使用例
* 読み込み メモリーカード全て標準出力へ
  * `./mcactl -r`
* 読み込み メモリーカード全て ファイルへ
  * `./mcactl -r -o memcard.mc`
* 読み込み メモリーカード ブロック0 ファイルへ
  * `./mcactl -r -b0 -l1 -o memcard.bin`
* 書き込み メモリーカード全て 標準入力から
  * `command | ./mcactl -w`
* 書き込み メモリーカード全て ファイルから
  * `./mcactl -w -i data.mc`
* 書き込み メモリーカード ブロック3 - 8 ファイルから
  * `./mcactl -w -b3 -l6 -i data.bin`


---

## おまけ
## MCA操作時のフォーマットについて

PS1でのメモリーカード操作の情報にMCA用のヘッダ付加して通信している
(入れ子の構成になっている)


### 読み込み時バッファの内訳

* MCAへの送信時は以下のような構成になる

        +-------------+
        | MCA header  |  4 byte
        |-------------|
        | PS1 header  |  12 byte
        |             |
        |-------------|
        |    data     |  1 - 128 byte
              :
              :
        |             |
        +-------------+

  * MCA header : メモリーカードアダプタ用のヘッダ
  * PS1 header : PS1本体でのメモリーカード操作のためのヘッダ
  * data       : 読み込みに必要なサイズのバッファ (サイズはPS1 headerで指定される)

* dataはPS1 headerで指定されたサイズ分必要
  * (MCAから受信するデータが送信時と同じサイズとなるため？)

* MCAから受信するデータは送信時と同じサイズ
* data部に指定した位置のデータが格納される
* 先頭2バイトに読み込みの成否が格納される


### 書き込み時バッファの内訳

* MCAへの送信時は以下のような構成になる

        +-------------+
        | MCA header  |  4 byte
        |-------------|
        | PS1 header  |  10 byte
        |             |
        |-------------|
        |    data     |  1 - 128 byte
              :
              :
        |             |
        +-------------+

  * MCA header : メモリーカードアダプタ用のヘッダ
  * PS1 header : PS1本体でのメモリーカード操作のためのヘッダ (読み込み時とはサイズが異なることに注意)
  * data       : 書き込むデータを格納する (サイズはPS1 headrで指定される)

* dataはPS1 headerで指定されたサイズ分必要
  * (MCAから受信するデータが送信時と同じサイズとなるため？)

* MCAから受信するデータは送信時と同じサイズ
* 先頭2バイトに書き込みの成否が格納される